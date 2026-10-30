#!/usr/bin/env python3
"""Random source permuter for one marked function (hill climbing over try_variants.py).

Each round mutates the current best body N ways (statement swaps, commuted
comparisons, temporaries inlined/hoisted, if/else flips, loop forms), scores every
variant with try_variants.py and keeps the best. A byte-exact match validates
itself, so mutations only need to compile; nothing here edits SOURCE. Mutations may reorder
dependent statements, so only a MATCH is safe to apply; a closer non-matching best.cpp is a hint,
not a change to copy. The best
body is written to build/permute/<addr>/best.cpp.

usage: permute.py [-r ROUNDS] [-n PER_ROUND] [-s SEED] SOURCE ADDRESS
"""
import os
import random
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from try_variants import split_function  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
STMT = re.compile(r"^(\s*)([^{}\n]*;)[ \t]*$")
KEYWORD = re.compile(r"^\s*(return|break|continue|goto|case|default|if|else|for|while|do|switch)\b")
IDENT = re.compile(r"[A-Za-z_]\w*")


def simple_lines(lines):
    return [i for i, l in enumerate(lines) if STMT.match(l) and not KEYWORD.match(l) and "//" not in l]


def swap_adjacent(lines, rng):
    """Swap two neighbouring simple statements at the same indent."""
    cand = [i for i in simple_lines(lines) if i + 1 < len(lines) and STMT.match(lines[i + 1])
            and not KEYWORD.match(lines[i + 1]) and STMT.match(lines[i]).group(1) == STMT.match(lines[i + 1]).group(1)]
    if not cand:
        return None
    i = rng.choice(cand)
    lines[i], lines[i + 1] = lines[i + 1], lines[i]
    return lines


def move_decl(lines, rng):
    """Move a declaration (`T name;` / `T name = x;`) up or down past one statement."""
    decl = re.compile(r"^\s*(?:const\s+|unsigned\s+)*[A-Za-z_][\w:<>]*[\s*&]+[A-Za-z_]\w*(\s*=.*)?;\s*$")
    cand = [i for i in simple_lines(lines) if decl.match(lines[i])]
    if not cand:
        return None
    i = rng.choice(cand)
    j = i + rng.choice((-1, 1))
    if not (0 <= j < len(lines)) or not STMT.match(lines[j]) or KEYWORD.match(lines[j]):
        return None
    lines[i], lines[j] = lines[j], lines[i]
    return lines


OPERAND = r"[\w.\->\[\]]+"
COMMUTE = re.compile(rf"\b({OPERAND})\s*(==|!=|\+|\*|&|\||\^)\s*({OPERAND})")
SWAP_CMP = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}
ORDER = re.compile(rf"\b({OPERAND})\s*(<=|>=|<|>)\s*({OPERAND})(?=\s*[);&|?])")


def commute(lines, rng):
    """a==b -> b==a, a+b -> b+a, a<b -> b>a on one line."""
    cand = [(i, m) for i, l in enumerate(lines) for m in list(COMMUTE.finditer(l)) + list(ORDER.finditer(l))
            if "->" not in m.group(2) and not l.lstrip().startswith(("#", "//"))]
    if not cand:
        return None
    i, m = rng.choice(cand)
    op = SWAP_CMP.get(m.group(2), m.group(2))
    lines[i] = lines[i][:m.start()] + f"{m.group(3)} {op} {m.group(1)}" + lines[i][m.end():]
    return lines


def flip_if(lines, rng):
    """`if (c) {A} else {B}` -> `if (!(c)) {B} else {A}` for braced single-line-header ifs."""
    text = "\n".join(lines)
    starts = [m for m in re.finditer(r"\bif \((.*)\) \{\n", text)]
    rng.shuffle(starts)
    for m in starts:
        a_end = block_end(text, m.end() - 2)
        tail = re.match(r"\s*else \{\n", text[a_end:])
        if not tail:
            continue
        b_open = a_end + tail.end() - 2
        b_end = block_end(text, b_open)
        a = text[m.end():a_end - 1]
        b = text[b_open + 2:b_end - 1]
        new = f"if (!({m.group(1)})) {{\n{b}}} else {{\n{a}}}"
        return (text[:m.start()] + new + text[b_end:]).split("\n")
    return None


def block_end(text, brace):
    depth, i = 0, brace
    while True:
        depth += {"{": 1, "}": -1}.get(text[i], 0)
        i += 1
        if depth == 0:
            return i


def inline_temp(lines, rng):
    """Replace a single-use `T x = expr;` temporary with its expression."""
    decl = re.compile(r"^\s*(?:const\s+)?[A-Za-z_][\w:]*[\s*]+([A-Za-z_]\w*)\s*=\s*([^;]+);\s*$")
    cand = []
    for i, l in enumerate(lines):
        m = decl.match(l)
        if not m:
            continue
        name = m.group(1)
        uses = [j for j, k in enumerate(lines) if j != i and re.search(rf"\b{name}\b", k)]
        if len(uses) == 1 and uses[0] > i and not re.search(rf"\b{name}\s*(=[^=]|\+\+|--|\+=|-=)", lines[uses[0]]):
            cand.append((i, uses[0], name, m.group(2)))
    if not cand:
        return None
    i, j, name, expr = rng.choice(cand)
    lines[j] = re.sub(rf"\b{name}\b", lambda _: f"({expr})", lines[j], count=1)
    del lines[i]
    return lines


def loop_form(lines, rng):
    """Flip i++ <-> ++i."""
    cand = [i for i, l in enumerate(lines) if re.search(r"\b\w+\+\+|\+\+\w+\b", l)]
    if not cand:
        return None
    i = rng.choice(cand)
    l = lines[i]
    if re.search(r"\b(\w+)\+\+", l):
        lines[i] = re.sub(r"\b(\w+)\+\+", r"++\1", l, count=1)
    else:
        lines[i] = re.sub(r"\+\+(\w+)\b", r"\1++", l, count=1)
    return lines


MUTATORS = [swap_adjacent, swap_adjacent, move_decl, commute, flip_if, inline_temp, loop_form]


def mutate(body, rng, steps):
    lines = body.split("\n")
    head, rest = lines[:1], lines[1:]  # keep the marker line fixed
    for _ in range(steps):
        out = rng.choice(MUTATORS)(list(rest), rng)
        if out is not None:
            rest = out
    return "\n".join(head + rest)


def score(source, address, bodies, workdir, jobs):
    """Return [(differing_bytes + |size delta|, body)] via try_variants.py."""
    shutil.rmtree(workdir, ignore_errors=True)
    os.makedirs(workdir)
    paths = []
    for n, b in enumerate(bodies):
        p = os.path.join(workdir, f"v{n:04}.cpp")
        open(p, "w").write(b)
        paths.append(p)
    out = subprocess.run([sys.executable, "-I", os.path.join(ROOT, "tools", "try_variants.py"), "-j", str(jobs),
                          source, f"0x{address:x}", *paths], stdin=subprocess.DEVNULL, capture_output=True, text=True, cwd=ROOT, env={**os.environ, "RT_TRY_DIR": os.path.join(workdir, "try")}).stdout
    result = []
    for line in out.splitlines():
        m = re.match(r"(v\d+)\.cpp: (MATCH|DIFF (\d+) bytes differ, built (\d+) vs original (\d+))", line)
        if m:
            s = 0 if m.group(2) == "MATCH" else int(m.group(3)) + abs(int(m.group(4)) - int(m.group(5)))
            result.append((s, bodies[int(m.group(1)[1:])]))
    return result


def main(argv):
    rounds, per, seed, jobs = 30, 24, None, min(os.cpu_count() or 4, 12)
    while argv and argv[0].startswith("-"):
        flag, val, argv = argv[0], argv[1], argv[2:]
        rounds, per, seed, jobs = {"-r": (int(val), per, seed, jobs), "-n": (rounds, int(val), seed, jobs),
                                   "-s": (rounds, per, int(val), jobs), "-j": (rounds, per, seed, int(val))}[flag]
    source, address = os.path.abspath(argv[0]), int(argv[1], 16)
    rng = random.Random(seed)
    text = open(source).read()
    start, end = split_function(text, address)
    best = text[start:end]
    out = os.path.join(ROOT, "build", "permute", f"{address:x}")
    os.makedirs(out, exist_ok=True)
    base = score(source, address, [best], os.path.join(out, "work"), jobs)
    if not base:
        sys.exit("baseline did not compile or compare")
    best_score = base[0][0]
    print(f"baseline {best_score}", flush=True)
    for r in range(rounds):
        bodies = {mutate(best, rng, rng.choice((1, 1, 2, 3))) for _ in range(per)} - {best}
        results = score(source, address, sorted(bodies), os.path.join(out, "work"), jobs)
        print(f"round {r}: {len(results)}/{len(bodies)} compiled, best {min([x[0] for x in results] or [None])}", flush=True)
        if results:
            s, body = min(results, key=lambda x: x[0])
            if s < best_score or (s == best_score and rng.random() < 0.3):
                if s < best_score:
                    print(f"round {r}: {best_score} -> {s}", flush=True)
                best, best_score = body, s
                open(os.path.join(out, "best.cpp"), "w").write(best + "\n")
        if best_score == 0:
            print(f"MATCH: {os.path.join(out, 'best.cpp')}")
            return
    print(f"best {best_score}: {os.path.join(out, 'best.cpp')}")


if __name__ == "__main__":
    main(sys.argv[1:])
