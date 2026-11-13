#!/usr/bin/env python3
"""Which source form produces an instruction idiom? Ask the byte-exact corpus.

Every matched FUNCTION is a worked example of what VC6 emits for a given source form. This tool
indexes them: a /FAcs listing per source file gives the source line(s) behind every instruction
of every byte-exact function, keyed by the original's objdump text. A NONMATCHING function's
differing hunks (shape-aligned like sizegap.py) are then looked up in that index, so instead of
guessing a form, read the forms that provably produced the original's instructions elsewhere.

usage: idiom.py --build [FILE...]         (re)index the matched functions of FILE... (default: all)
       idiom.py ADDRESS [-n HUNKS] [-k HITS]   look up every differing hunk of a NONMATCHING function
                                          (needs its object in build/obj, i.e. match.py on the file)
       idiom.py -e REGEX [-c CONTEXT] [-k HITS]  grep original instructions (objdump intel syntax);
                                          `a;b` = consecutive instructions matching a then b;
                                          -w N: one regex over N joined instructions (groups tie registers)
The index lives in build/idiom/corpus.json; --build takes ~5 s per source file.
"""
import difflib
import glob
import json
import os
import re
import subprocess
import sys
from collections import Counter

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import CFLAGS, CXXFLAGS, TARGET_EXE, marked_functions  # noqa: E402
from objcmp import PE, coff_function, disasm  # noqa: E402
from sizegap import code_only, shape  # noqa: E402
from triage import original_size, sizes_table  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IMAGE_BASE = 0x400000
OUT = os.path.join(ROOT, "build", "idiom")
CORPUS = os.path.join(OUT, "corpus.json")


def compile_cod(src):
    """Compile with a machine-code listing (/FAcs); return the .cod path."""
    os.makedirs(OUT, exist_ok=True)
    base = os.path.splitext(os.path.basename(src))[0]
    flags = [f for f in (CXXFLAGS if src.endswith(".cpp") else CFLAGS) if f != "/YX"]
    rel = os.path.relpath(src, OUT).replace("/", "\\")
    cc = subprocess.run([os.path.join(ROOT, "tools", "vc6.sh"), "cl", *flags, "/FAcs", f"/Fa{base}.cod", rel],
                        capture_output=True, text=True, cwd=OUT)
    if cc.returncode:
        sys.exit(cc.stdout)
    return os.path.join(OUT, base + ".cod")


def listing_offsets(cod, symbol):
    """{instruction offset: (first, last source line)} and {line: text} for `symbol`'s PROC in a /FAcs listing.
    Instructions after a run of source-line comments belong to that whole run (VC6 schedules across it)."""
    lines = open(cod, errors="replace").read().splitlines()
    start = next((i for i, l in enumerate(lines) if " PROC" in l and (
        re.search(rf";\s*{re.escape(symbol)}(,|$)", l) or l.split()[0] in (symbol, "_" + symbol))), None)
    if start is None:
        return None, None
    mangled = lines[start].split()[0]
    offs, text, run, after_ins = {}, {}, [], False
    for l in lines[start + 1:]:
        if l.startswith(mangled) and "ENDP" in l:
            break
        m = re.match(r";\s*(\d+)\s*:\s?(.*)", l)
        if m:
            if after_ins:
                run, after_ins = [], False
            run.append(int(m.group(1)))
            text[run[-1]] = m.group(2).strip()
            continue
        m = re.match(r"  ([0-9a-f]{5})\t", l)
        if m:
            offs[int(m.group(1), 16)] = (run[0], run[-1]) if run else (0, 0)
            after_ins = True
    return offs, text


def build(files):
    corpus = json.load(open(CORPUS)) if os.path.exists(CORPUS) else {}
    pe = PE(TARGET_EXE)
    sizes = sizes_table()
    for src in files:
        src = os.path.abspath(src)
        marked = [(a, s) for a, s, nm in marked_functions(src) if not nm and a in sizes]
        if not marked:
            continue
        cod = compile_cod(src)
        obj = cod[:-4] + ".obj"
        for address, symbol in marked:
            offs, text = listing_offsets(cod, symbol)
            if offs is None:
                print(f"{src}: {symbol} not in listing, skipped", file=sys.stderr)
                continue
            size = original_size(obj, symbol, address, pe)
            image = pe.b[pe.rva_to_off(address - IMAGE_BASE):][:size]
            ins, _ = code_only(disasm(image, address), address + size)
            rows = []
            for a, t in ins:
                lo, hi = offs.get(a - address, (0, 0))
                rows.append([a, t, lo, hi])
            lines = {str(k): v for k, v in text.items()}
            asm_lines = sum(bool(re.match(r"(mov|lea|push|pop|call|f\w+|cmp|jmp|j\w\w?)\s", v.strip())) for v in text.values())
            if text and asm_lines > len(text) // 2:  # inline-asm body: not a C++ form, leave it out
                corpus.pop(hex(address), None)
                continue
            corpus[hex(address)] = {"name": symbol, "file": os.path.relpath(src, ROOT), "ins": rows, "src": lines}
        print(f"{os.path.relpath(src, ROOT)}: {len(marked)} functions indexed", flush=True)
    json.dump(corpus, open(CORPUS, "w"))
    print(f"{len(corpus)} functions in {os.path.relpath(CORPUS, ROOT)}")


# Three looseness levels: exact text (calls/absolute data masked), registers masked, shape only.
def exact(t):
    t = re.sub(r"\b(call|j\w+)\s+0x[0-9a-f]+", r"\1 T", t)
    return re.sub(r"ds:0x[0-9a-f]+", "ds:A", t)


def regs(t):
    return re.sub(r"\b(e?[abcd]x|e?[sd]i|e?[sb]p|[abcd][lh])\b", "R", exact(t))


LEVELS = [("exact", exact), ("registers masked", regs), ("shape only", shape)]


def load():
    if not os.path.exists(CORPUS):
        sys.exit("no corpus yet: run idiom.py --build first")
    return json.load(open(CORPUS))


_hay = {}


def search(corpus, seq, level):
    """[(address, index)] where the original of a matched function contains `seq` (normalised at `level`)."""
    _, f = LEVELS[level]
    hits = []
    for addr, fn in corpus.items():
        hay = _hay.get((addr, level))
        if hay is None:
            hay = _hay[(addr, level)] = [f(t) for _, t, _, _ in fn["ins"]]
        for i in range(len(hay) - len(seq) + 1):
            if hay[i] == seq[0] and hay[i:i + len(seq)] == seq:
                hits.append((addr, i))
    return hits


def show_hits(corpus, hits, length, k):
    """Group hits by the source text that produced the run, most common form first."""
    forms = Counter()
    where = {}
    for addr, idx in hits:
        fn = corpus[addr]
        lo = min(r[2] for r in fn["ins"][idx:idx + length] if r[2]) if any(r[2] for r in fn["ins"][idx:idx + length]) else 0
        hi = max(r[3] for r in fn["ins"][idx:idx + length])
        src = " | ".join(fn["src"].get(str(n), "") for n in range(lo, hi + 1) if fn["src"].get(str(n), "").strip())
        key = src[:160] or "(no source line)"
        forms[key] += 1
        where.setdefault(key, []).append(f"{fn['name']} {addr} {fn['file']}:{lo}")
    for src, n in forms.most_common(k):
        print(f"    x{n}  {src}")
        for w in where[src][:2]:
            print(f"          {w}")


def lookup_hunks(address, n, k):
    corpus = load()
    pe = PE(TARGET_EXE)
    source = next(f for f in sorted(glob.glob(os.path.join(ROOT, "src", "*.c*")))
                  if any(a == address for a, _, _ in marked_functions(f)))
    symbol = next(s for a, s, _ in marked_functions(source) if a == address)
    obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(source))[0] + ".obj")
    built_code = coff_function(obj, symbol)[0].rstrip(b"\x90\xcc")
    size = original_size(obj, symbol, address, pe)
    image = pe.b[pe.rva_to_off(address - IMAGE_BASE):]
    orig, _ = code_only(disasm(image[:size], address), address + size)
    built, _ = code_only(disasm(built_code, 0), len(built_code))
    sm = difflib.SequenceMatcher(None, [shape(t) for _, t in orig], [shape(t) for _, t in built], autojunk=False)
    hunks = [(i1, i2, j1, j2) for tag, i1, i2, j1, j2 in sm.get_opcodes() if tag != "equal"]
    print(f"{symbol} {hex(address)}: {len(hunks)} differing hunks; corpus of {len(corpus)} matched functions")
    for i1, i2, j1, j2 in hunks[:n]:
        lo, hi = max(i1 - 1, 0), min(i2 + 1, len(orig))  # one instruction of context each side
        print(f"\n== orig 0x{orig[lo][0]:x} ({i2 - i1} original / {j2 - j1} built instructions)")
        for x in range(lo, hi):
            mark = " " if x < i1 or x >= i2 else "*"
            print(f"  {mark} {orig[x][1]}")
        if i2 == i1:
            print("  (nothing on the original side; our extra instructions:)")
            for y in range(j1, j2):
                print(f"  + {built[y][1]}")
        windows = [("with context", lo, hi), ("hunk + next", i1, hi), ("previous + hunk", lo, i2), ("hunk alone", i1, i2)]
        if i2 == i1:
            windows = [("with context", lo, hi)]
        found = False
        for level, (label, f) in enumerate(LEVELS):
            for wlabel, a, b in windows:
                hits = search(corpus, [f(t) for _, t in orig[a:b]], level)
                if hits:
                    print(f"  {len(hits)} hits ({label}, {wlabel}):")
                    show_hits(corpus, hits, b - a, k)
                    found = True
                    break
            if found:
                break
        if not found:
            print("  no hits in the corpus")


def grep(pattern, context, k, window):
    """Instructions matching PATTERN; `a;b;c` means three consecutive instructions matching a, b and c.
    With -w N the pattern is one regex over N consecutive instructions joined by newlines, so a group
    can tie registers across instructions: `lea\s+(e..),.*\nmov\s+e..,\1$`."""
    corpus = load()
    rxs = [re.compile(p) for p in pattern.split(";")] if not window else [re.compile(pattern, re.M)]
    width = window or len(rxs)
    total = 0
    forms = Counter()
    for addr, fn in corpus.items():
        ins = fn["ins"]
        for idx in range(len(ins) - width + 1):
            if window:
                if not rxs[0].search("\n".join(r[1] for r in ins[idx:idx + width])):
                    continue
            elif not all(rx.search(ins[idx + j][1]) for j, rx in enumerate(rxs)):
                continue
            total += 1
            lo = min(r[2] for r in ins[idx:idx + width] if r[2]) if any(r[2] for r in ins[idx:idx + width]) else 0
            hi = max(r[3] for r in ins[idx:idx + width])
            src = " | ".join(fn["src"].get(str(x), "") for x in range(lo, hi + 1) if fn["src"].get(str(x), "").strip())
            forms[src[:160]] += 1
            if total <= k:
                print(f"{fn['name']} {addr}  {fn['file']}:{lo}")
                for j, r in enumerate(ins[max(idx - context, 0):idx + width + context]):
                    hit = idx <= max(idx - context, 0) + j < idx + width
                    print(f"   {'>' if hit else ' '} {r[1]}")
                print(f"     {src[:200]}")
    print(f"\n{total} occurrences in {len(corpus)} matched functions; most common source forms:")
    for src, n in forms.most_common(k):
        print(f"  x{n}  {src}")


if __name__ == "__main__":
    argv = sys.argv[1:]

    def opt(name, default):
        return type(default)(argv[argv.index(name) + 1]) if name in argv else default

    if "--build" in argv:
        files = [a for a in argv if a != "--build"] or sorted(glob.glob(os.path.join(ROOT, "src", "**", "*.c*"),
                                                                    recursive=True))
        build(files)
    elif "-e" in argv:
        grep(opt("-e", ""), opt("-c", 2), opt("-k", 10), opt("-w", 0))
    elif argv:
        lookup_hunks(int(argv[0], 16), opt("-n", 8), opt("-k", 6))
    else:
        print(__doc__)
