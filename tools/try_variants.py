#!/usr/bin/env python3
"""Try alternative bodies for one marked function without editing the real file.

Each variant file holds a complete replacement definition (marker line included).
The function's current definition in SOURCE is swapped out for each variant in
turn, compiled in build/try/, and checked with match.py; the source is untouched.

usage: try_variants.py SOURCE ADDRESS VARIANT...
"""
import os
import re
import shutil
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def split_function(text, address):
    """Return (start, end) of the marked definition for `address` in `text`."""
    m = re.search(rf"^//[^\n]*(?:FUNCTION|NONMATCHING):\s*RALLYTROPHY\s+0x{address:x}\b.*?$", text, re.M | re.I)
    if not m:
        sys.exit(f"no marker for 0x{address:x}")
    brace = text.index("{", m.end())
    depth, i = 0, brace
    while True:
        depth += {"{": 1, "}": -1}.get(text[i], 0)
        i += 1
        if depth == 0:
            return m.start(), i


def main(source, address, *variants):
    address = int(address, 16)
    text = open(source).read()
    start, end = split_function(text, address)
    outdir = os.path.join(ROOT, "build", "try")
    os.makedirs(outdir, exist_ok=True)
    for variant in variants:
        body = open(variant).read().strip().replace("NONMATCHING:", "FUNCTION:")  # judge strictly
        trial = os.path.join(outdir, os.path.basename(source))
        def off(t):
            return re.sub(r"(FUNCTION|NONMATCHING):\s*RALLYTROPHY", "OFF:", t)
        open(trial, "w").write(off(text[:start]) + body + off(text[end:]))
        r = subprocess.run([sys.executable, "-I", os.path.join(ROOT, "tools", "match.py"), trial],
                           capture_output=True, text=True)
        verdict = next((l for l in r.stdout.splitlines() if l.startswith(("MATCH", "DIFF", "COMPILE"))),
                       r.stdout.strip().splitlines()[-1] if r.stdout.strip() else r.stderr.strip())
        print(f"{os.path.basename(variant)}: {verdict}")
    shutil.rmtree(outdir, ignore_errors=True)


if __name__ == "__main__":
    main(*sys.argv[1:])
