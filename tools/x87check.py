#!/usr/bin/env python3
"""Compare the x87 arithmetic of NONMATCHING functions with the original.

Aligns the original's and the built object's x87 instruction streams (memory operands masked
for alignment) and reports the differing chunks that contain arithmetic (fadd/fsub/fmul/fdiv),
with real operands, so wrong sum association, dropped terms or wrong fields stand out from
pure spill/stack-slot noise. Run match.py on the source first so build/obj is current.

usage: x87check.py [--all] [--quiet] [SRC:ADDR ...]
  --all    every function listed in analysis/nonmatching-triage.md
  --quiet  one summary line per function (arithmetic chunks found)
"""
import difflib
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from asm_helpers import ROOT, pe_reader  # noqa: E402
from match import marked_functions  # noqa: E402
from objcmp import coff_function  # noqa: E402

X87 = re.compile(r"^f(add|sub|mul|div|ld|st|xch|chs|sqrt|com|ucom|abs)")
ARITH = re.compile(r"^f(add|sub|mul|div)")


def x87(blob):
    """Return [(masked, real)] for the x87 instructions in a code blob."""
    with tempfile.NamedTemporaryFile(suffix=".bin") as f:
        f.write(blob)
        f.flush()
        out = subprocess.run(["objdump", "-D", "-b", "binary", "-mi386", "-Mintel", f.name],
                             capture_output=True, text=True).stdout
    seq = []
    for line in out.splitlines():
        if line.count("\t") < 2:
            continue
        ins = line.split("\t")[-1].strip()
        op = ins.split()[0] if ins else ""
        if X87.match(op):
            ops = ins[len(op):].strip()
            seq.append((f"{op} {'mem' if 'PTR' in ops else ops}".strip(), ins.replace("DWORD PTR ", "")))
    return seq


def check(exe, off, source, address, size):
    symbol = next((n for a, n, _ in marked_functions(os.path.join(ROOT, source)) if a == address), None)
    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    o = x87(exe[off(address):off(address) + size])
    b = x87(coff_function(obj, symbol)[0])
    sm = difflib.SequenceMatcher(a=[m for m, _ in o], b=[m for m, _ in b], autojunk=False)
    chunks = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag != "equal" and any(ARITH.match(m) for m, _ in o[i1:i2] + b[j1:j2]):
            chunks.append(([r for _, r in o[max(0, i1 - 4):i2 + 2]], [r for _, r in b[max(0, j1 - 4):j2 + 2]]))
    return len(o), len(b), chunks


def triage():
    for line in open(os.path.join(ROOT, "analysis", "nonmatching-triage.md")):
        cols = [c.strip() for c in line.split("|")]
        if line.startswith("| 0x"):
            source, address = cols[2], int(cols[1], 16)
            if any(a == address and nm for a, _, nm in marked_functions(os.path.join(ROOT, source))):
                yield source, address, int(cols[6])


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    sizes = {(s, a): n for s, a, n in triage()}
    todo = list(sizes) if "--all" in sys.argv else [(s, int(a, 16)) for s, a in (x.split(":") for x in args)]
    exe, off = pe_reader()
    for source, address in todo:
        try:
            no, nb, chunks = check(exe, off, source, address, sizes[(source, address)])
        except (OSError, KeyError, StopIteration, TypeError, SystemExit) as e:
            print(f"0x{address:x} {source}: skipped ({type(e).__name__})")
            continue
        print(f"0x{address:x} {source}: x87 {no}/{nb}, {len(chunks)} arithmetic chunks differ")
        if "--quiet" not in sys.argv:
            for oc, bc in chunks:
                print("  orig : " + " | ".join(oc))
                print("  built: " + " | ".join(bc))


if __name__ == "__main__":
    main()
