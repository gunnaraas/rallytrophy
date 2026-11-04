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
    return out


def short(symbol):
    """Class::Member form of a COFF name, so both sides align by name."""
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


def main(source, address):
    address = int(address, 16)
    pe = PE(TARGET_EXE)
    symbol = next(n for a, n, _ in marked_functions(source) if a == address)
    size = sizes_table()[address]
    image = pe.b[pe.rva_to_off(address - IMAGE_BASE):][:size]
    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    built, _, targets = coff_function(obj, symbol)
    known = names()

    def orig_name(text):
        m = re.match(r"call\s+0x([0-9a-f]+)$", text)
        if not m:
            return text
        t = int(m.group(1), 16)
        return f"call {known.get(t, '0x%x' % t)}"

    left = [f"{off:5x}  {orig_name(t)}" for off, t in calls(image, address)]
    right = [f"{off:5x}  call {short(targets.get(off + 1, t))}" if t.startswith("call   0x") else f"{off:5x}  {t}"
             for off, t in calls(built, address)]
    key = lambda ls: [l[7:].rsplit("::", 1)[-1] for l in ls]  # align by member name; FUN_ names may differ
    sm = difflib.SequenceMatcher(None, key(left), key(right), autojunk=False)
    print(f"{'original':40s} | built")
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        for k in range(max(i2 - i1, j2 - j1)):
            l = left[i1 + k] if i1 + k < i2 else ""
            r = right[j1 + k] if j1 + k < j2 else ""
            print(f"{'  ' if tag == 'equal' else '! '}{l:40s} | {r}")


if __name__ == "__main__":
    main(*sys.argv[1:3])
