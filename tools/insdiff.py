#!/usr/bin/env python3
"""Aligned instruction diff of one NONMATCHING function: original vs build/obj (run match.py on SOURCE first).

usage: insdiff.py SOURCE ADDRESS
"""
import difflib
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import TARGET_EXE, marked_functions  # noqa: E402
from objcmp import PE, relocations  # noqa: E402
from triage import compare  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def main(source, address):
    address = int(address, 16)
    pe = PE(TARGET_EXE)
    symbol = next(n for a, n, _ in marked_functions(source) if a == address)
    size = next(int(l.split("|")[6]) for l in open(os.path.join(ROOT, "analysis", "nonmatching-triage.md"))
                if l.startswith(f"| 0x{address:x} "))
    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    orig, built, *_ = compare(obj, symbol, pe, relocations(pe), address, size, None)
    for line in difflib.unified_diff(orig, built, "original", "built", n=2, lineterm=""):
        print(line)


if __name__ == "__main__":
    main(*sys.argv[1:3])
