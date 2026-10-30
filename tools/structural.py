#!/usr/bin/env python3
"""Classify NONMATCHING functions by structural difference from the original.

Per function (original vs build/obj) it reports: stack frame size, ebp frame,
EH state stores (count and highest state), calls, double-precision FPU operands, and the instruction
mnemonics the original uses more often than the build. Rows are grouped into
clusters by which of those differ, largest cluster first.

usage: structural.py [--rows]
"""
import collections
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from asm_helpers import ROOT, pe_reader  # noqa: E402
from match import marked_functions  # noqa: E402
from objcmp import coff_function  # noqa: E402

STATE = re.compile(r"mov\s+(?:DWORD|BYTE) PTR \[ebp-0x4\],(0x[0-9a-f]+|e[a-d]x|e[sd]i|[a-d]l)$")  # state may come from a zeroed register


def disasm(blob):
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(blob)
        f.flush()
        out = subprocess.run(["objdump", "-D", "-b", "binary", "-mi386", "-Mintel", f.name],
                             capture_output=True, text=True).stdout
    return [l.split("\t")[-1].strip() for l in out.splitlines() if re.match(r"\s+[0-9a-f]+:\t", l) and l.count("\t") >= 2]


def shape(blob, eh):
    ins = disasm(blob)
    frame = next((int(m.group(1), 16) for i in ins[:6] if (m := re.match(r"sub\s+esp,(0x[0-9a-f]+)", i))), 0)
    # EH functions store the state index in [ebp-4]; others hit [esp+x] too, so only count those when EH is on
    states = [m.group(1) for i in ins if eh and (m := STATE.search(i))]
    ops = collections.Counter(i.split()[0] for i in ins if i)
    return {"frame": frame, "ebp": ins[:2] == ["push   ebp", "mov    ebp,esp"],
            "states": len(states), "calls": ops["call"], "ops": ops, "n": len(ins),
            "double": sum(1 for i in ins if re.match(r"f\w+\s+QWORD PTR", i))}


def rows():
    exe, off = pe_reader()
    for line in open(os.path.join(ROOT, "analysis", "nonmatching-triage.md")):
        cols = [c.strip() for c in line.split("|")]
        if not line.startswith("| 0x"):
            continue
        address, source, name, size = int(cols[1], 16), cols[2], cols[3], int(cols[6])
        obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
        symbol = next((n for a, n, _ in marked_functions(os.path.join(ROOT, source)) if a == address), None)
        try:
            built = coff_function(obj, symbol)[0]
        except (OSError, SystemExit, StopIteration, TypeError):
            continue
        orig = exe[off(address):off(address) + size]
        eh = b"\x64\xa1\x00\x00\x00\x00" in orig[:32]  # mov eax, fs:[0]
        o, b = shape(orig, eh), shape(built, eh)
        tags = []
        if o["ebp"] != b["ebp"]:
            tags.append("ebp-frame")
        elif o["frame"] != b["frame"]:
            tags.append(f"frame{o['frame'] - b['frame']:+d}")
        if o["states"] != b["states"]:
            tags.append("eh-states")
        if o["calls"] != b["calls"]:
            tags.append("calls")
        if o["double"] != b["double"]:
            tags.append("double")  # float/double literal or variable type differs
        extra = {k: v - b["ops"][k] for k, v in o["ops"].items() if v - b["ops"][k] > 0 and k != "call"}
        yield address, source, name, size, o["n"] - b["n"], tags, o, b, extra


def main():
    clusters = collections.defaultdict(list)
    for r in rows():
        address, source, name, size, dn, tags, o, b, extra = r
        if "--rows" in sys.argv:
            print(f"0x{address:x} {source} {name} {size} dn={dn:+d} {' '.join(tags) or '-'} "
                  f"frame {o['frame']}/{b['frame']} states {o['states']}/{b['states']} calls {o['calls']}/{b['calls']} "
                  f"orig+ {dict(sorted(extra.items(), key=lambda kv: -kv[1])[:5])}")
        for t in tags or ["none"]:
            clusters[re.sub(r"[+-]\d+$", "", t) if t.startswith("frame") else t].append(r)
    for t, rs in sorted(clusters.items(), key=lambda kv: -len(kv[1])):
        print(f"{t}: {len(rs)} functions, {sum(r[3] for r in rs)} bytes")


if __name__ == "__main__":
    main()
