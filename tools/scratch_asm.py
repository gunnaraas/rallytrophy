#!/usr/bin/env python3
"""Emit decomp.me-ready target assembly (GNU as, Intel syntax) for one function.

Absolute operands are found through the PE base relocations and replaced by
symbols, so the scratch diff lines up with what the compiler emits:
  functions -> _FUN_xxxxxxxx (or the Ghidra name from functions.csv)
  data      -> _DAT_xxxxxxxx
  imports   -> __imp__Name@N (decoration from IMPORT_ARG_BYTES)
Branches inside the function become local labels.

usage: scratch_asm.py ANALYSIS_EXE FUNCTIONS_CSV ADDRESS
"""
import csv
import os
import re
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, __file__.rsplit("/", 1)[0])
from securom_api_map import IMAGE_BASE, PE  # noqa: E402

# stdcall argument bytes for import decoration; extend as new APIs show up.
IMPORT_ARG_BYTES = {
    "GetModuleFileNameA": 12,
    "MessageBoxA": 16,
    "LoadIconA": 8,
    "LoadCursorA": 8,
    "timeGetTime": 0,
    "CoInitializeEx": 8,
    "CoUninitialize": 0,
    "RegCreateKeyExA": 36,
    "RegSetValueExA": 24,
    "RegQueryValueExA": 24,
    "RegCloseKey": 4,
}

INSN = re.compile(r"^\s*([0-9a-f]+):\s*(.*)$")
REG = r"(?:e?[abcd]x|[abcd][lh]|e?[sd]i|e?[sb]p)"
REG_REG = re.compile(rf"^\w+\s+{REG},{REG}$")
# Opcodes of the "reg <- r/m" direction. MSVC encodes reg,reg ALU ops this way
# while GNU as defaults to the "r/m <- reg" form, so those need `{load}`.
LOAD_FORM_OPCODES = {0x02, 0x03, 0x0A, 0x0B, 0x12, 0x13, 0x1A, 0x1B, 0x22, 0x23,
                     0x2A, 0x2B, 0x32, 0x33, 0x3A, 0x3B, 0x8A, 0x8B}


def relocations(pe):
    """Return the set of VAs holding absolute addresses (IMAGE_REL_BASED_HIGHLOW)."""
    b = pe.b
    peoff = struct.unpack_from("<I", b, 0x3C)[0]
    rva, size = struct.unpack_from("<II", b, peoff + 24 + 96 + 5 * 8)
    out, off, end = set(), pe.rva_to_off(rva), pe.rva_to_off(rva) + size
    while off < end:
        page, block = struct.unpack_from("<II", b, off)
        if not block:
            break
        for k in range(8, block, 2):
            entry = struct.unpack_from("<H", b, off + k)[0]
            if entry >> 12 == 3:
                out.add(IMAGE_BASE + page + (entry & 0xFFF))
        off += block
    return out


def main(exe, functions_csv, address):
    start = int(address, 16)
    funcs = {int(r["entry"], 16): r for r in csv.DictReader(open(functions_csv))}
    func = funcs[start]
    end = start + int(func["size"])
    pe = PE(exe)
    relocs = relocations(pe)
    imports = pe.iat()
    warn_if_patched(pe, relocs, start, end)

    def va_bytes(va, n):
        return pe.b[pe.rva_to_off(va - IMAGE_BASE):][:n]

    def symbol(value):
        if value in imports:
            name = imports[value].split("!")[1]
            n = IMPORT_ARG_BYTES.get(name)
            if n is None:
                print(f"warning: no decoration for {name}", file=sys.stderr)
                return f'"__imp__{name}"' if "@" in name else f"__imp__{name}"
            return f'"__imp__{name}@{n}"'  # GNU as needs quotes around '@'
        if value in funcs:
            name = "_" + funcs[value]["name"]
            return f'"{name}"' if "@" in name else name  # GNU as needs quotes around '@'
        return f"_DAT_{value:08x}"

    dis = subprocess.run(
        # -D -j .text: the analysis exe flags .text as DATA, so -d would skip it.
        ["objdump", "-D", "-j", ".text", "-M", "intel", "--no-show-raw-insn", "--wide",
         f"--start-address=0x{start:x}", f"--stop-address=0x{end:x}", exe],
        check=True, capture_output=True, text=True).stdout

    insns = []
    for line in dis.splitlines():
        m = INSN.match(line)
        if m and m.group(2).strip():
            insns.append((int(m.group(1), 16), m.group(2).strip()))
    labels = set()
    for i, (va, text) in enumerate(insns):
        nxt = insns[i + 1][0] if i + 1 < len(insns) else end
        for r in [r for r in relocs if va <= r < nxt]:
            value = struct.unpack("<I", va_bytes(r, 4))[0]
            sym, const = symbol(value), rf"0x0*{value:x}\b"
            # GNU as Intel syntax reads a bare symbol as memory, so spell out
            # memory operands as [sym] and immediates as `offset sym`.
            text = re.sub(rf"ds:{const}", f"[{sym}]", text)
            text = re.sub(rf"(\[[^\]]*?){const}", rf"\g<1>{sym}", text)
            text = re.sub(const, f"offset {sym}", text)
        m = re.match(r"^(j\w+|call|loop\w*)\s+0x([0-9a-f]+)\b.*$", text)
        if m:
            target = int(m.group(2), 16)
            if start <= target < end:
                labels.add(target)
                text = f"{m.group(1)} .L{target:x}"
            else:
                text = f"{m.group(1)} {symbol(target)}"
        text = re.sub(r"\s*<[^>]*>", "", text).replace("ds:", "")
        opcode = va_bytes(va, 2)
        opcode = opcode[1] if opcode[0] == 0x66 else opcode[0]
        if REG_REG.match(re.sub(r"\s*,\s*", ",", text)) and opcode in LOAD_FORM_OPCODES:
            text = "{load} " + text
        insns[i] = (va, text)

    name = "_" + func["name"]
    lines = [".intel_syntax noprefix", f".global {name}", f"{name}:"]
    for va, text in insns:
        if va in labels:
            lines.append(f".L{va:x}:")
        lines.append(f"    {text}")
    asm = "\n".join(lines) + "\n"
    verify(asm, va_bytes(start, end - start))
    sys.stdout.write(asm)


OFFICIAL_EXE = os.path.join(os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "orig", "RallyTrophy.exe")


def warn_if_patched(pe, relocs, start, end):
    """Warn when the analysis exe differs from the official one inside this function.

    The analysis exe has the same code except for relocated operands; always
    decompile the official behaviour."""
    official = PE(OFFICIAL_EXE)
    masked = {r + k for r in relocs if start - 3 <= r < end for k in range(4)}
    a = pe.b[pe.rva_to_off(start - IMAGE_BASE):][:end - start]
    b = official.b[official.rva_to_off(start - IMAGE_BASE):][:end - start]
    for i in range(end - start):
        if a[i] != b[i] and start + i not in masked:
            print(f"WARNING: analysis exe differs from official at 0x{start + i:x} "
                  f"({a[i]:02x} vs official {b[i]:02x})", file=sys.stderr)


def verify(asm, original):
    """Assemble `asm` and check it reproduces `original` outside relocation fields."""
    with tempfile.TemporaryDirectory() as tmp:
        src, obj, raw = (os.path.join(tmp, n) for n in ("f.s", "f.o", "f.bin"))
        open(src, "w").write(asm)
        subprocess.run(["as", "--32", src, "-o", obj], check=True)
        subprocess.run(["objcopy", "-O", "binary", "-j", ".text", obj, raw], check=True)
        relocs = subprocess.run(["objdump", "-r", obj], check=True,
                                capture_output=True, text=True).stdout
        built = open(raw, "rb").read()
    fields = set()
    for line in relocs.splitlines():
        if "R_386" in line:
            r = int(line.split()[0], 16)
            fields.update(range(r, r + 4))
    bad = [i for i in range(max(len(built), len(original)))
           if i not in fields and built[i:i + 1] != original[i:i + 1]]
    if bad or len(built) != len(original):
        print(f"verify: MISMATCH at offsets {[hex(i) for i in bad[:10]]} "
              f"(built {len(built)} bytes, original {len(original)})", file=sys.stderr)
    else:
        print(f"verify: ok ({len(original)} bytes, {len(fields) // 4} relocations)",
              file=sys.stderr)


if __name__ == "__main__":
    main(*sys.argv[1:4])
