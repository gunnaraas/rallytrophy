#!/usr/bin/env python3
"""Search the declaration order of locals that feed an inline-asm macro.

VC6 gives locals read by `__asm` stack slots in declaration order within their block (analysis/NOTES.md, "Locals read
by inline asm"), so a group of consecutive declarations ahead of a `..._TRANSFORM...` macro use has one right order.
For each NONMATCHING function this finds those groups and tries every permutation, greedily per group, scoring with
try_variants.py. It prints the best order; the body goes to build/declorder/<addr>.cpp. Behaviour is unchanged as
long as the initialisers of a group do not depend on each other (the tool does not check that; read the result).

usage: declorder.py [SOURCE ADDRESS]...   (default: every NONMATCHING function with such a group)
"""
import itertools
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import marked_functions  # noqa: E402
from try_variants import split_function  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DECL = re.compile(r"^\s+(?:const\s+)?[A-Za-z_][\w:]*\s*\*?\s*[A-Za-z_]\w*(?:\s*=\s*[^;{}]+)?;\s*$")
USE = re.compile(r"\b[A-Z0-9_]*TRANSFORM[A-Z0-9_]*\s*\(")


def groups(lines):
    """[(first, last)] line ranges of 2..5 consecutive declarations directly above a transform macro use."""
    out = []
    for i, l in enumerate(lines):
        if not USE.search(l) or l.lstrip().startswith(("#", "//")):
            continue
        j = i
        while j > 0 and DECL.match(lines[j - 1]):
            j -= 1
        if 2 <= i - j <= 5:
            out.append((j, i))
    return out


def score(source, address, bodies, work):
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work)
    for n, b in enumerate(bodies):
        open(os.path.join(work, f"v{n:04}.cpp"), "w").write(b)
    out = subprocess.run([sys.executable, "-I", os.path.join(ROOT, "tools", "try_variants.py"), "-j", "10", source,
                          f"0x{address:x}", work], capture_output=True, text=True, cwd=ROOT,
                         env={**os.environ, "RT_TRY_DIR": os.path.join(work, "try")}).stdout
    res = {}
    for line in out.splitlines():
        m = re.match(r"v(\d+)\.cpp: (MATCH|DIFF (\d+) bytes differ, built (\d+) vs original (\d+), (\d+) insns)", line)
        if m:
            res[int(m.group(1))] = (0, 0) if m.group(2) == "MATCH" else (int(m.group(6)), int(m.group(3)))
    return res


def search(source, address):
    text = open(source).read()
    a, b = split_function(text, address)
    lines = text[a:b].split("\n")
    gs = groups(lines)
    if not gs:
        return None
    work = os.path.join(ROOT, "build", "declorder", f"work_{address:x}")

    def build(orders):
        out = list(lines)
        for (j, i), o in zip(gs, orders):
            out[j:i] = [lines[j + k] for k in o]
        return "\n".join(out)

    cur = [tuple(range(i - j)) for j, i in gs]
    base = score(source, address, [build(cur)], work).get(0)
    if base is None:
        return None
    best, changed = base, True
    while changed and best != (0, 0):
        changed = False
        for k, (j, i) in enumerate(gs):
            perms = list(itertools.permutations(range(i - j)))
            r = score(source, address, [build(cur[:k] + [p] + cur[k + 1:]) for p in perms], work)
            if r:
                bi = min(r, key=r.get)
                if r[bi] < best:
                    best, cur, changed = r[bi], cur[:k] + [perms[bi]] + cur[k + 1:], True
    shutil.rmtree(work, ignore_errors=True)
    if best < base:
        open(os.path.join(ROOT, "build", "declorder", f"{address:x}.cpp"), "w").write(build(cur))
    return len(gs), base, best


def main(argv):
    os.makedirs(os.path.join(ROOT, "build", "declorder"), exist_ok=True)
    if argv:
        todo = [(os.path.abspath(argv[i]), int(argv[i + 1], 16)) for i in range(0, len(argv), 2)]
    else:
        todo = []
        for name in sorted(os.listdir(os.path.join(ROOT, "src"))):
            path = os.path.join(ROOT, "src", name)
            if not name.endswith(".cpp"):
                continue
            text = open(path).read()
            for m in re.finditer(r"// NONMATCHING: RALLYTROPHY (0x[0-9a-f]+)", text):
                todo.append((path, int(m.group(1), 16)))
    for path, address in todo:
        r = search(path, address)
        if r:
            n, base, best = r
            tag = "MATCH" if best == (0, 0) else ("improved" if best < base else "same")
            print(f"0x{address:x} {os.path.relpath(path, ROOT)}: {n} groups, {base} -> {best} {tag}", flush=True)


if __name__ == "__main__":
    main(sys.argv[1:])
