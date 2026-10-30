#!/usr/bin/env python3
"""List NONMATCHING functions whose original contains inlined x87 __asm helpers.

The original inlined hand-written __asm math helpers (FloatToInt, Xform/Rot);
any __asm forces an ebp frame and fixed registers, so plain C++ in their place
cannot match. A function is flagged when the original holds a helper's fixed
byte pattern more often than our built object (build/obj) does.

usage: asm_helpers.py
"""
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import marked_functions  # noqa: E402
from objcmp import coff_function  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
PATTERNS = {
    "Xform/Rot": re.compile(re.escape(bytes.fromhex("d901d80ad901d84a04")), re.S),  # fld [ecx]; fmul [edx]; fld [ecx]; fmul [edx+4]
    "FloatToInt": re.compile(rb"\xd9\x45.\xdb\x5d.", re.S),  # fld [ebp+d8]; fistp [ebp+d8]
}


def main():
    exe = open(os.path.join(ROOT, "orig", "RallyTrophy.exe"), "rb").read()
    pe = struct.unpack_from("<I", exe, 0x3c)[0]
    count, opt = struct.unpack_from("<H", exe, pe + 6)[0], struct.unpack_from("<H", exe, pe + 20)[0]
    base = struct.unpack_from("<I", exe, pe + 52)[0]
    secs = [struct.unpack_from("<IIII", exe, pe + 24 + opt + 40 * i + 8) for i in range(count)]

    def off(va):
        return next(ro + va - base - rva for vs, rva, rs, ro in secs if 0 <= va - base - rva < max(vs, rs))

    for line in open(os.path.join(ROOT, "analysis", "nonmatching-triage.md")):
        cols = [c.strip() for c in line.split("|")]
        if not line.startswith("| 0x"):
            continue
        address, source, name, size = int(cols[1], 16), cols[2], cols[3], int(cols[6])
        blob = exe[off(address):off(address) + size]
        hits = {k: len(p.findall(blob)) for k, p in PATTERNS.items() if p.search(blob)}
        if not hits:
            continue
        obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
        symbol = next((n for a, n, _ in marked_functions(os.path.join(ROOT, source)) if a == address), None)
        try:
            built = coff_function(obj, symbol)[0]
        except (OSError, SystemExit, StopIteration, TypeError):
            continue
        missing = {k: v - len(PATTERNS[k].findall(built)) for k, v in hits.items()}
        missing = {k: v for k, v in missing.items() if v > 0}
        if missing:
            print(f"0x{address:x} {source} {name} {size} " + " ".join(f"{k}x{v}" for k, v in missing.items()))


if __name__ == "__main__":
    main()
