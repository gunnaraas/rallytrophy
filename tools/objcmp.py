#!/usr/bin/env python3
"""Compare a function compiled into a COFF .obj against the original exe.

Bytes covered by a relocation (in the .obj) or a base relocation (in the exe)
are masked, since the linker fills them in. On mismatch, prints a side-by-side
disassembly diff.

Overloads must be selected with an exact COFF symbol.

usage: objcmp.py OBJ SYMBOL EXE ADDRESS SIZE [LIMIT]
  LIMIT: bytes up to the next function (bounds catch handlers/jump tables)
exit status: 0 on exact match, 1 otherwise
"""
import difflib
import os
import re
import struct
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from scratch_asm import relocations  # noqa: E402
from securom_api_map import IMAGE_BASE, PE  # noqa: E402


def coff_function(path, symbol):
    """Return (bytes, set of relocated offsets, {offset: target symbol}) for `symbol`."""
    b = open(path, "rb").read()
    _, nsec, _, symptr, nsym, optsz, _ = struct.unpack_from("<HHIIIHH", b, 0)
    strtab = symptr + 18 * nsym

    def name_at(off):
        raw = b[off:off + 8]
        if raw[:4] == b"\0\0\0\0":
            s = strtab + struct.unpack_from("<I", raw, 4)[0]
            return b[s:b.index(b"\0", s)].decode()
        return raw.rstrip(b"\0").decode()

    sections = [struct.unpack_from("<8sIIIIIIHHI", b, 20 + optsz + 40 * i) for i in range(nsec)]
    symbols, i = [], 0
    while i < nsym:
        off = symptr + 18 * i
        value, secnum, _, _, naux = struct.unpack_from("<IhHBB", b, off + 8)
        symbols.append((i, name_at(off), value, secnum))
        i += 1 + naux
    by_index = {idx: name for idx, name, _, _ in symbols}

    # Accept an exact symbol, a C name (_name), a C++ function (?name@...) or a
    # qualified member: Class::Class -> ??0Class@, Class::~Class -> ??1Class@,
    # Class::Method -> ?Method@Class@.
    if "::" in symbol:
        cls, member = symbol.rsplit("::", 1)
        cls = "@".join(reversed(cls.split("::")))
        prefixes = [f"??0{cls}@" if member == cls.split("@")[0] else
                    f"??1{cls}@" if member == "~" + cls.split("@")[0] else f"?{member}@{cls}@"]
        plain = None
    else:
        plain = symbol.lstrip("_")
        prefixes = [f"?{plain}@", f"_{plain}@"]  # C++ name, or C __stdcall (_name@N)
    matches = [s for s in symbols if s[3] > 0 and (
        s[1] == symbol or (plain and s[1] == "_" + plain) or s[1].startswith(tuple(prefixes)))]
    if not matches:
        sys.exit(f"{symbol} not found in {path}; defined: "
                 f"{[s[1] for s in symbols if s[3] > 0 and not s[1].startswith(('.', '$', '@'))]}")
    if len(matches) > 1:
        sys.exit(f"ambiguous symbol {symbol}: {[s[1] for s in matches]}; "
                 "use an exact COFF symbol (// SYMBOL: in the source marker)")
    _, _, start, secnum = matches[0]
    sec = sections[secnum - 1]
    rawsize, rawptr, relptr, nrel = sec[3], sec[4], sec[5], sec[7]
    ends = [v for _, n, v, s in symbols if s == secnum and v > start and not n.startswith("$")]
    end = min(ends + [rawsize])
    code = b[rawptr + start:rawptr + end]

    masked, targets = set(), {}
    for k in range(nrel):
        va, symidx, _ = struct.unpack_from("<IIH", b, relptr + 10 * k)
        if start <= va < end:
            masked.update(range(va - start, va - start + 4))
            targets[va - start] = by_index.get(symidx, f"#{symidx}")
    return code, masked, targets


def disassemble(code, vma):
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(code)
        f.flush()
        out = subprocess.run(
            ["objdump", "-D", "-b", "binary", "-m", "i386", "-M", "intel", "--no-show-raw-insn",
             f"--adjust-vma=0x{vma:x}", f.name], check=True, capture_output=True, text=True).stdout
    return [line.split(":", 1)[1].strip() for line in out.splitlines()
            if line.startswith("  ") and ":" in line]


def aligned_relocation_table(built, image, size, limit, targets, original_relocs, address):
    """Recognize an aligned pointer table after the analyzed instruction body.

    Require matching padding, complete corresponding relocations, and a known
    next-function boundary. This does not validate relocation target identities.
    """
    if limit is None or not size < len(built) <= min(limit, len(image)):
        return False
    first = min((off for off in targets if off >= size), default=len(built))
    if not 0 <= first - size <= 15 or first + 4 > len(built):
        return False
    padding = built[size:first]
    if padding != image[size:first] or any(c not in (0x90, 0xCC) for c in padding):
        return False
    if (len(built) - first) % 4:
        return False
    return all(off in targets and address + off in original_relocs
               for off in range(first, len(built), 4))


def main(obj, symbol, exe, address, size, limit=None):
    address, size = int(address, 16), int(size)
    limit = int(limit) if limit else None
    built, masked, targets = coff_function(obj, symbol)
    pe = PE(exe)
    # Drop the compiler's trailing padding. Anything left beyond Ghidra's size is
    # code the compiler emits after the body under the same symbol (e.g. catch
    # handlers), which the exe also has right after the function: compare it too.
    built = built.rstrip(b"\x90\xcc")
    image = pe.b[pe.rva_to_off(address - IMAGE_BASE):]
    original_relocs = relocations(pe)
    if len(built) > size and size < len(image) and (
            image[size] not in (0x90, 0xCC) or
            aligned_relocation_table(built, image, size, limit, targets,
                                     original_relocs, address)):
        # The original continues past Ghidra's size with code or data that
        # belongs to this function (catch handlers, switch jump tables): take
        # everything up to the next function, minus the alignment padding.
        size = len(image[:limit].rstrip(b"\x90\xcc")) if limit else len(built)
    original = image[:size]
    for r in original_relocs:
        if address <= r < address + size:
            masked.update(range(r - address, r - address + 4))
    # For display, zero each side's own linker-filled fields so addresses don't
    # show up as differences (instruction boundaries may differ between sides).
    original_fields = {i for r in original_relocs if address <= r < address + size
                       for i in range(r - address, r - address + 4)}
    built_fields = {i for off in targets for i in range(off, off + 4)}

    bad = [i for i in range(max(len(built), len(original)))
           if i not in masked and built[i:i + 1] != original[i:i + 1]]
    if not bad and len(built) == len(original):
        print(f"MATCH {symbol} 0x{address:x} ({size} bytes, {len(targets)} relocations)")
        return 0

    print(f"DIFF {symbol} 0x{address:x}: built {len(built)} bytes, original {len(original)}; "
          f"{len(bad)} differing bytes outside relocations")
    left = disassemble(bytes(0 if i in original_fields else c for i, c in enumerate(original)), address)
    right = disassemble(bytes(0 if i in built_fields else c for i, c in enumerate(built)), address)
    # Relative call targets differ between an unlinked .obj and the exe; hide them.
    left = [re.sub(r"^call\s+0x[0-9a-f]+$", "call <rel>", l) for l in left]
    right = [re.sub(r"^call\s+0x[0-9a-f]+$", "call <rel>", l) for l in right]
    for line in difflib.unified_diff(left, right, "original", "built", n=2, lineterm=""):
        print(line)
    return 1


if __name__ == "__main__":
    sys.exit(main(*sys.argv[1:7]))
