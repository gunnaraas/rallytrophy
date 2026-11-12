#!/usr/bin/env python3
"""Where does a NONMATCHING function's size difference come from?

Aligns original and built instruction streams by instruction shape (mnemonic plus operand
kinds; registers, offsets and constants masked), groups the differences into hunks and
ranks them by byte delta. Each hunk is labelled with our source lines (from a /FAs listing),
so "40 bytes extra at lines 312-318" points at the statement to rewrite.

usage: sizegap.py SOURCE ADDRESS [--no-compile] [-n HUNKS] [-v]
  -v  print the instructions of each hunk side by side
"""
import difflib
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import TARGET_EXE, marked_functions  # noqa: E402
from objcmp import PE, coff_function  # noqa: E402
from slotmap import IMAGE_BASE, OUT, compile_listing, disasm  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
SKIP = ("npad", "ORG", "ALIGN", "DD", "DB", "DW")


def shape(t):
    t = re.sub(r"\b(e?[abcd]x|e?[sd]i|e?[sb]p|[abcd][lh])\b", "R", t)
    t = re.sub(r"st\(\d\)", "ST", t)
    t = re.sub(r"0x[0-9a-f]+|\b\d+\b", "N", t)
    return re.sub(r"\s+", " ", t)


def source_lines(asm, symbol):
    """Source line of each instruction in the listing's body of `symbol`, plus the source text per line."""
    lines = open(asm, errors="replace").read().splitlines()
    start = next(i for i, l in enumerate(lines) if " PROC" in l and (
        re.search(rf";\s*{re.escape(symbol)}(,|$)", l) or l.split()[0] in (symbol, "_" + symbol)))
    mangled = lines[start].split()[0]
    cur, out, text = 0, [], {}
    for l in lines[start + 1:]:
        if l.startswith(mangled) and "ENDP" in l:
            break
        m = re.match(r";\s*(\d+)\s*:\s?(.*)", l)
        if m:
            cur = int(m.group(1))
            text[cur] = m.group(2).strip()
            continue
        if l.startswith("\t") and not l.startswith("\t;") and l.strip() and l.split()[0] not in SKIP \
                and not l.split()[0].endswith(":"):
            out.append(cur)
    return out, text


def main(argv):
    verbose = "-v" in argv
    n = int(argv[argv.index("-n") + 1]) if "-n" in argv else 8
    pos = [a for i, a in enumerate(argv) if not a.startswith("-") and (i == 0 or argv[i - 1] != "-n")]
    source, address = os.path.abspath(pos[0]), int(pos[1], 16)
    symbol = next(s for a, s, _ in marked_functions(source) if a == address)
    base = os.path.splitext(os.path.basename(source))[0]
    obj, asm = (compile_listing(source) if "--no-compile" not in argv else
                (os.path.join(OUT, base + ".obj"), os.path.join(OUT, base + ".asm")))
    built_code = coff_function(obj, symbol)[0].rstrip(b"\x90\xcc")
    pe = PE(TARGET_EXE)
    size = next((int(l.split("|")[6]) for l in open(os.path.join(ROOT, "analysis", "nonmatching-triage.md"))
                 if l.startswith(f"| 0x{address:x} ")), len(built_code))
    image = pe.b[pe.rva_to_off(address - IMAGE_BASE):]
    orig = disasm(image[:size], address)
    built = disasm(built_code, 0)
    olen = [b - a for (a, _), (b, _) in zip(orig, orig[1:] + [(address + size, "")])]
    blen = [b - a for (a, _), (b, _) in zip(built, built[1:] + [(len(built_code), "")])]
    lines, text = source_lines(asm, symbol)

    def src(j1, j2):
        ls = sorted({lines[j] for j in range(max(j1, 0), min(j2, len(lines))) if lines[j]})
        if not ls and 0 < j1 <= len(lines):
            ls = [lines[j1 - 1]]
        return ls

    sm = difflib.SequenceMatcher(None, [shape(t) for _, t in orig], [shape(t) for _, t in built], autojunk=False)
    hunks = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            continue
        ob, bb = sum(olen[i1:i2]), sum(blen[j1:j2])
        hunks.append((bb - ob, i1, i2, j1, j2))
    # a block only on one side whose instructions appear in a one-sided block on the other side moved, it is not extra
    oshape = [shape(t) for _, t in orig]
    bshape = [shape(t) for _, t in built]
    dels = {k: h for k, h in enumerate(hunks) if h[3] == h[4]}
    moved = set()
    for k, h in enumerate(hunks):
        if h[1] != h[2] or k in moved:
            continue
        ins = sorted(bshape[h[3]:h[4]])
        for kd, d in dels.items():
            if kd not in moved and sorted(oshape[d[1]:d[2]]) == ins:
                moved |= {k, kd}
                break
    if moved:
        print(f"moved blocks (same instructions, other position): "
              + ", ".join(f"ours +0x{built[hunks[k][3]][0]:x} ({hunks[k][4] - hunks[k][3]} insns, "
                          f"line {src(hunks[k][3], hunks[k][4])[0] if src(hunks[k][3], hunks[k][4]) else '?'})"
                          for k in sorted(moved) if hunks[k][1] == hunks[k][2]))
    hunks = [h for k, h in enumerate(hunks) if k not in moved]
    print(f"{symbol}: built {len(built_code)} vs original {size} bytes ({len(built_code) - size:+d}); "
          f"{len(hunks)} differing hunks, {sum(i2 - i1 for _, i1, i2, _, _ in hunks)} original / "
          f"{sum(j2 - j1 for *_, j1, j2 in hunks)} built instructions in them")
    for delta, i1, i2, j1, j2 in sorted(hunks, key=lambda h: (-abs(h[0]), -(h[2] - h[1] + h[4] - h[3])))[:n]:
        oa = orig[i1][0] if i1 < len(orig) else address + size
        ls = src(j1, j2)
        where = f"lines {ls[0]}-{ls[-1]}" if len(ls) > 1 else f"line {ls[0]}" if ls else "line ?"
        print(f"\n{delta:+5d} bytes  orig 0x{oa:x} ({i2 - i1} insns)  ours +0x{built[j1][0] if j1 < len(built) else len(built_code):x}"
              f" ({j2 - j1} insns)  {where}")
        for l in ls[:4]:
            print(f"        {l:5d}: {text.get(l, '')[:100]}")
        if verbose:
            for k in range(max(i2 - i1, j2 - j1)):
                a = orig[i1 + k][1] if i1 + k < i2 else ""
                b = built[j1 + k][1] if j1 + k < j2 else ""
                print(f"          {a:42.42} | {b:.42}")


if __name__ == "__main__":
    main(sys.argv[1:])
