#!/usr/bin/env python3
"""Random source permuter for one marked function (hill climbing over try_variants.py).

Each round mutates the current best body N ways (statement swaps, commuted
comparisons, temporaries inlined/hoisted, if/else flips, loop forms, counted-loop
rewrites, int-typed copies, split declarations, expanded compound assignments, ternaries), scores every
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


def _block_close(lines, i):
    """Index of the line holding the `}` that closes the block opened on line i (or None)."""
    depth = 0
    for j in range(i, len(lines)):
        depth += lines[j].count("{") - lines[j].count("}")
        if depth == 0 and j > i:
            return j
        if depth == 0 and "{" in lines[j] and "}" in lines[j]:
            return None
    return None


FOR_COUNT = re.compile(r"^(\s*)for \((?:(?:unsigned )?int )?(\w+) = 0; \2 < ([^;]+); (?:\2\+\+|\+\+\2)\) \{\s*$")


def counter_loop(lines, rng):
    """Rewrite a counted for loop whose index is unused in the body (measured lever, NOTES "Loop counter forms")."""
    cand = []
    for i, l in enumerate(lines):
        m = FOR_COUNT.match(l)
        if not m:
            continue
        end = _block_close(lines, i)
        if end is None or any(re.search(rf"\b{m.group(2)}\b", k) for k in lines[i + 1:end]):
            continue
        cand.append((i, end, m))
    if not cand:
        return None
    i, end, m = rng.choice(cand)
    ind, n = m.group(1), m.group(3)
    form = rng.randrange(5)
    if form == 0:
        lines[i] = f"{ind}{{ int cnt__ = {n}; while (cnt__--) {{"
        lines[end] = lines[end] + " }"
    elif form == 1:
        lines[i] = f"{ind}for (int cnt__ = {n}; cnt__; cnt__--) {{"
    elif form == 2:
        lines[i] = f"{ind}if ({n} > 0) {{ int cnt__ = {n}; while (cnt__--) {{"
        lines[end] = lines[end] + " }"
    elif form == 3:
        lines[i] = f"{ind}if ({n} > 0) {{ int cnt__ = {n}; do {{"
        lines[end] = lines[end].replace("}", "} while (--cnt__); }", 1)
    else:
        lines[i] = f"{ind}if ({n} > 0) for (int cnt__ = {n}; cnt__; cnt__--) {{"
    return lines


ASSIGN = re.compile(r"^(\s*)([A-Za-z_][\w\.\->\[\]]*) = ([A-Za-z_][\w\.\->\[\]]*);\s*$")


def typed_copy(lines, rng):
    """Copy a 4-byte value through an int local (NOTES "Typed copies and indexing"); wrong sizes cannot MATCH."""
    cand = [i for i, l in enumerate(lines) if ASSIGN.match(l)]
    if not cand:
        return None
    i = rng.choice(cand)
    ind, dst, src = ASSIGN.match(lines[i]).groups()
    k = rng.randrange(10 ** 6)
    lines[i:i + 1] = [f"{ind}int cp{k}__ = *(int*)&({src});", f"{ind}*(int*)&({dst}) = cp{k}__;"]
    return lines


DECL_INIT = re.compile(r"^(\s*)((?:const |unsigned )?[A-Za-z_][\w:]*(?:\s*\*+\s*|\s+))([A-Za-z_]\w*) = ([^;]+);\s*$")


def split_decl(lines, rng):
    """`T x = e;` -> `T x;` + `x = e;` (changes where the slot/register is claimed)."""
    cand = [i for i, l in enumerate(lines) if DECL_INIT.match(l) and "const " not in l and "&" not in DECL_INIT.match(l).group(2)]
    if not cand:
        return None
    i = rng.choice(cand)
    ind, ty, name, expr = DECL_INIT.match(lines[i]).groups()
    lines[i:i + 1] = [f"{ind}{ty}{name};", f"{ind}{name} = {expr};"]
    return lines


COMPOUND = re.compile(r"^(\s*)([A-Za-z_][\w\.\->\[\]]*) ([-+*])= ([^;]+);\s*$")


def expand_compound(lines, rng):
    """`a += b;` -> `a = a + b;` or `a = b + a;`."""
    cand = [i for i, l in enumerate(lines) if COMPOUND.match(l)]
    if not cand:
        return None
    i = rng.choice(cand)
    ind, a, op, b = COMPOUND.match(lines[i]).groups()
    if op == "-" or rng.random() < 0.5:
        lines[i] = f"{ind}{a} = {a} {op} ({b});"
    else:
        lines[i] = f"{ind}{a} = ({b}) {op} {a};"
    return lines


IF_ASSIGN = re.compile(r"^(\s*)if \(([^;{}]+)\) ([A-Za-z_][\w\.\->\[\]]*) = ([^;]+);\s*$")


def ternary_sub(lines, rng):
    """`x = a; if (c) x = b;` on two lines -> `x = c ? b : a;` (and the `x -= c ? d : 0` family via expand first)."""
    cand = []
    for i in range(1, len(lines)):
        m = IF_ASSIGN.match(lines[i])
        p = ASSIGN.match(lines[i - 1]) or DECL_INIT.match(lines[i - 1])
        if m and p and p.groups()[-2].split()[-1].lstrip("*") == m.group(3):
            cand.append(i)
    if not cand:
        return None
    i = rng.choice(cand)
    m = IF_ASSIGN.match(lines[i])
    prev = lines[i - 1].rstrip()
    head, a = prev[:-1].rsplit(" = ", 1)
    lines[i - 1:i + 1] = [f"{head} = ({m.group(2)}) ? ({m.group(4)}) : ({a});"]
    return lines


MUTATORS = [swap_adjacent, swap_adjacent, move_decl, commute, flip_if, inline_temp, loop_form,
            counter_loop, counter_loop, typed_copy, split_decl, expand_compound, ternary_sub]


def mutate(body, rng, steps):
    lines = body.split("\n")
    head, rest = lines[:1], lines[1:]  # keep the marker line fixed
    for _ in range(steps):
        out = rng.choice(MUTATORS)(list(rest), rng)
        if out is not None:
            rest = out
    return "\n".join(head + rest)


def score(source, address, bodies, workdir, jobs):
    """Return [((differing instructions, differing bytes + |size delta|), body)] via try_variants.py.

    Aligned instructions rank first: the positional byte count punishes any size change upstream.
    """
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
        m = re.match(r"(v\d+)\.cpp: (MATCH|DIFF (\d+) bytes differ, built (\d+) vs original (\d+), (\d+) insns)", line)
        if m:
            s = (0, 0) if m.group(2) == "MATCH" else (int(m.group(6)), int(m.group(3)) + abs(int(m.group(4)) - int(m.group(5))))
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
        if best_score == (0, 0):
            print(f"MATCH: {os.path.join(out, 'best.cpp')}")
            return
    print(f"best {best_score}: {os.path.join(out, 'best.cpp')}")


if __name__ == "__main__":
    main(sys.argv[1:])
