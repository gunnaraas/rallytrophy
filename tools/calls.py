#!/usr/bin/env python3
"""Side-by-side call sequence of one NONMATCHING function: original vs build/obj (run match.py on SOURCE first).

The first misaligned call usually marks the structural difference (missing or extra
inline expansion, moved block) far faster than the byte diff. Direct calls are named
from the source markers (then analysis/functions.csv); indirect calls keep their operand.

usage: calls.py SOURCE ADDRESS
"""
import csv
import difflib
import glob
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import TARGET_EXE, marked_functions  # noqa: E402
from objcmp import IMAGE_BASE, PE, coff_function  # noqa: E402
from triage import sizes_table  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def names():
    out = {}
    with open(os.path.join(ROOT, "analysis", "functions.csv")) as f:
        for row in csv.DictReader(f):
            out[int(row["entry"], 16)] = row["name"]
    for path in glob.glob(os.path.join(ROOT, "src", "**", "*.c*"), recursive=True):
        for address, name, _ in marked_functions(path):
            out[address] = name
    out.update(ORIG_SPECIAL)
    return out


SPECIAL = {"??2@YAPAXI@Z": "operator_new", "??3@YAXPAX@Z": "operator_delete",
           "??_L@YGXPAXIHP6EX0@Z1@Z": "ehvec_ctor", "??_M@YGXPAXIHP6EX0@Z@Z": "ehvec_dtor",
           "??0exception@@QAE@ABQBD@Z": "exception::exception", "_qsort": "qsort", "__chkstk": "__chkstk",
           "__CIpow": "__CIpow", "__itoa": "_itoa", "__ftol": "__ftol", "_memset": "memset", "_memcpy": "memcpy"}
ORIG_SPECIAL = {0x4c11f4: "ehvec_ctor", 0x4c1116: "ehvec_dtor", 0x4bd000: "operator_delete", 0x4bd006: "operator_new",
                0x4c12db: "exception::exception", 0x4c2221: "qsort", 0x4c1290: "__chkstk", 0x4c19f0: "__CIpow",
                0x4c1902: "_itoa"}


def coff_prefixes(name):
    """COFF name prefixes objcmp.py accepts for a marker name (Class::Member, C name or exact symbol)."""
    if name.startswith(("?", "_", "@")):
        return [name]
    if "::" in name:
        cls, member = name.rsplit("::", 1)
        cls = "@".join(reversed(cls.split("::")))
        return [f"??0{cls}@" if member == cls.split("@")[0] else
                f"??1{cls}@" if member == "~" + cls.split("@")[0] else f"?{member}@{cls}@"]
    return [f"?{name}@", f"_{name}@", f"_{name}"]


def resolve(symbol, known):
    """Canonical name of a built call target: the marker name of the function it links to, else a short form."""
    if symbol in SPECIAL:
        return SPECIAL[symbol]
    for address, name in known.items():
        if any(symbol.startswith(p) or symbol == p for p in coff_prefixes(name)):
            return known[address]
    return short(symbol)


def short(symbol):
    """Class::Member form of a COFF name, so both sides align by name."""
    if symbol in SPECIAL:
        return SPECIAL[symbol]
    m = re.match(r"\?\?([01])([A-Za-z_0-9]+)@", symbol)
    if m:
        return f"{m.group(2)}::{'~' if m.group(1) == '1' else ''}{m.group(2)}"
    m = re.match(r"\?([A-Za-z_0-9]+)@([A-Za-z_0-9]+)@@", symbol)
    if m:
        return f"{m.group(2)}::{m.group(1)}"
    m = re.match(r"\?([A-Za-z_0-9]+)@@", symbol)
    return m.group(1) if m else symbol


def calls(code, vma):
    """[(offset, text)] for every call/jmp-to-function instruction in `code`."""
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(code)
        f.flush()
        out = subprocess.run(["objdump", "-D", "-b", "binary", "-m", "i386", "-M", "intel", "--no-show-raw-insn",
                              f"--adjust-vma=0x{vma:x}", f.name], check=True, capture_output=True, text=True).stdout
    res = []
    for line in out.splitlines():
        m = re.match(r"\s*([0-9a-f]+):\s+(call\s+.*)$", line)
        if m:
            res.append((int(m.group(1), 16) - vma, m.group(2).strip()))
    return res


def sequence(source, address, pe=None, known=None):
    """(original rows, built rows, difflib opcodes) of the call sequences."""
    pe = pe or PE(TARGET_EXE)
    symbol = next(n for a, n, _ in marked_functions(source) if a == address)
    size = sizes_table()[address]
    image = pe.b[pe.rva_to_off(address - IMAGE_BASE):][:size]
    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    built, _, targets = coff_function(obj, symbol)
    known = known or names()

    def orig_name(text):
        m = re.match(r"call\s+0x([0-9a-f]+)$", text)
        if not m:
            return text
        t = int(m.group(1), 16)
        return f"call {known.get(t, '0x%x' % t)}"

    left = [f"{off:5x}  {orig_name(t)}" for off, t in calls(image, address)]
    right = [f"{off:5x}  call {resolve(targets.get(off + 1, t), known)}" if t.startswith("call   0x") else f"{off:5x}  {t}"
             for off, t in calls(built, address)]
    key = lambda ls: [re.sub(r"ds:0x[0-9a-f]+", "ds:G", l[7:]) for l in ls]  # indirect calls: the global's address is masked on the build side
    sm = difflib.SequenceMatcher(None, key(left), key(right), autojunk=False)
    return left, right, sm.get_opcodes()


def main(source, address):
    left, right, opcodes = sequence(source, int(address, 16))
    print(f"{'original':40s} | built")
    for tag, i1, i2, j1, j2 in opcodes:
        for k in range(max(i2 - i1, j2 - j1)):
            l = left[i1 + k] if i1 + k < i2 else ""
            r = right[j1 + k] if j1 + k < j2 else ""
            print(f"{'  ' if tag == 'equal' else '! '}{l:40s} | {r}")


if __name__ == "__main__":
    main(*sys.argv[1:3])
