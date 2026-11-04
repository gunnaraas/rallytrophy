#!/usr/bin/env python3
"""Rank NONMATCHING functions by structural lead strength (after match.py + triage.py).

Per function: frame sizes, the original's EH unwind map (states, how many are
siblings of -1, destructors) against the build's stored states, misaligned
call-sequence rows (first one shown) and how often PROGRESS.md mentions it.
Score: calls misaligned 3, EH differs 3, ebp-frame differs 2, frame size 1;
inst-delta-0 functions with no structural tag score 0 (register-only).

usage: dossier.py [--md FILE] [ADDRESS...]
"""
import os
import re
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from calls import names, sequence  # noqa: E402
from ehmap import unwind  # noqa: E402
from match import TARGET_EXE  # noqa: E402
from objcmp import PE  # noqa: E402
from structural import ROOT, rows  # noqa: E402


def main(argv):
    out = open(argv[argv.index("--md") + 1], "w") if "--md" in argv else sys.stdout
    only = {int(a, 16) for a in argv if a.startswith("0x")}
    pe, known = PE(TARGET_EXE), names()
    progress = open(os.path.join(ROOT, "analysis", "PROGRESS.md")).read()
    triage = {int(l.split("|")[1], 16): [c.strip() for c in l.split("|")]
              for l in open(os.path.join(ROOT, "analysis", "nonmatching-triage.md")) if l.startswith("| 0x")}
    table = []
    for address, source, name, size, dn, tags, o, b, extra in rows():
        if only and address not in only:
            continue
        cols = triage[address]
        delta, ehnote = int(cols[8]), cols[11]
        eh = unwind(address, pe)
        if eh:
            max_state, states, _ = eh
            dtors = sorted({re.sub(r".*(jmp|call) ", "", t) for _, _, t in states if "(no" not in t})
            ehtxt = f"{max_state} states ({sum(1 for _, p, _ in states if p == -1)} top-level): {', '.join(dtors)[:60]}"
        else:
            ehtxt = "none"
        try:
            left, right, ops = sequence(os.path.join(ROOT, source), address, pe, known)
            bad = [(l, r) for tag, i1, i2, j1, j2 in ops if tag != "equal"
                   for l, r in [(left[i1 + k] if i1 + k < i2 else "", right[j1 + k] if j1 + k < j2 else "")
                                for k in range(max(i2 - i1, j2 - j1))]]
        except Exception as e:  # noqa: BLE001
            bad = [(f"calls.py failed: {e}", "")]
        first = (bad[0][0][7:] + " | " + bad[0][1][7:]) if bad else ""
        ehdiff = ehnote.startswith("eh") and eh is not None
        score = 3 * bool(bad) + 3 * ehdiff + 2 * ("ebp-frame" in tags) + (any(t.startswith("frame") for t in tags))
        if delta == 0 and not tags and not bad:
            score = 0
        tries = len(re.findall(rf"0x{address:x}", progress))
        table.append((score, address, source, name, size, delta, f"{o['frame']:#x}/{b['frame']:#x}", ehtxt,
                      ehnote.replace("eh ", ""), len(bad), first[:70], tries))
    table.sort(key=lambda r: (-r[0], r[4]))
    print("| score | address | file | function | orig size | inst delta | frame o/b | original EH map | states stored o vs b | calls off | first misaligned call | log mentions |", file=out)
    print("|---|---|---|---|---|---|---|---|---|---|---|---|", file=out)
    for r in table:
        print("| " + " | ".join(str(x) for x in (r[0], f"0x{r[1]:x}", *r[2:])) + " |", file=out)


if __name__ == "__main__":
    main(sys.argv[1:])
