#!/usr/bin/env python3
"""Aligned instruction diff of one NONMATCHING function: original vs build/obj (run match.py on SOURCE first).

usage: insdiff.py [--norm] [SOURCE] ADDRESS   (SOURCE defaults to the triage table entry)
  --norm  mask stack offsets, jump targets and immediates first, so a frame-size
          shift does not count every [esp+X] operand as a difference
"""
import re
import difflib
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import TARGET_EXE, marked_functions  # noqa: E402
from objcmp import PE, relocations  # noqa: E402
from triage import compare, original_size  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def norm(lines):
    return [re.sub(r"0x[0-9a-f]+", "N", l) for l in lines]


def main(source, address, normalise=False):
    address = int(address, 16)
    pe = PE(TARGET_EXE)
    symbol = next(n for a, n, _ in marked_functions(source) if a == address)

    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    orig, built, *_ = compare(obj, symbol, pe, relocations(pe), address, original_size(obj, symbol, address, pe), None)
    if normalise:
        orig, built = norm(orig), norm(built)
    for line in difflib.unified_diff(orig, built, "original", "built", n=2, lineterm=""):
        print(line)


if __name__ == "__main__":
    args = [a for a in sys.argv[1:] if a != "--norm"]
    if len(args) == 1:  # address only: find the source file by its marker
        import glob
        a = int(args[0], 16)
        args = [next(f for f in sorted(glob.glob(os.path.join(ROOT, "src", "*.c*")))
                     if any(x == a for x, _, _ in marked_functions(f))), args[0]]
    main(*args[:2], normalise="--norm" in sys.argv)
