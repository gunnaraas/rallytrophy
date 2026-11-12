#!/usr/bin/env python3
"""Stack-slot map of one function: the original's [esp+X] slots next to our named locals.

Compiles SOURCE with /FAs into build/slotmap/, reads every local's frame offset and every
`_name$[esp+N]` access from the VC6 listing, tracks esp depth through both disassemblies
and aligns them, so each original slot is labelled with the source variable that lands
on it in our build. Slots are offsets from the entry esp (more negative = deeper).
VC6 orders plain locals by reference count, highest count at the lowest offset
(analysis/NOTES.md), so the refs columns say which variable needs one more or one fewer
source reference.

Check the "frames anchored" line first: on an exact match nearly all accesses agree, and the
only flagged rows are objects initialised in the prologue. Below ~70% the table is unreliable.

usage: slotmap.py SOURCE ADDRESS [--no-compile]
"""
import difflib
import os
import re
import subprocess
import sys
from collections import Counter, defaultdict

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import CXXFLAGS, CFLAGS, TARGET_EXE, marked_functions  # noqa: E402
from objcmp import PE, coff_function, disasm  # noqa: E402
from triage import original_size, sizes_table  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
IMAGE_BASE = 0x400000
OUT = os.path.join(ROOT, "build", "slotmap")
ESP = re.compile(r"\[esp(?:\+0x([0-9a-f]+))?\]")


def compile_listing(src):
    os.makedirs(OUT, exist_ok=True)
    base = os.path.splitext(os.path.basename(src))[0]
    flags = CXXFLAGS if src.endswith(".cpp") else CFLAGS
    flags = [f for f in flags if f != "/YX"]  # no pch: the listing must come from a full compile
    rel = os.path.relpath(src, OUT).replace("/", "\\")
    cc = subprocess.run([os.path.join(ROOT, "tools", "vc6.sh"), "cl", *flags, "/FAs", f"/Fa{base}.asm", rel],
                        capture_output=True, text=True, cwd=OUT)
    if cc.returncode:
        sys.exit(cc.stdout)
    return os.path.join(OUT, base + ".obj"), os.path.join(OUT, base + ".asm")


def listing(asm, symbol):
    """({name: frame offset}, [names referenced per instruction, in order])."""
    lines = open(asm, errors="replace").read().splitlines()
    # PROC lines carry the demangled name as a comment: `?X@Y@@QAEXXZ PROC NEAR ; Y::X, COMDAT`
    start = next(i for i, l in enumerate(lines) if " PROC" in l and (
        re.search(rf";\s*{re.escape(symbol)}(,|$)", l) or l.split()[0] in (symbol, "_" + symbol)))
    symbol = lines[start].split()[0]
    offs = {}
    j = start - 1
    while j > 0 and re.match(r"^[_$]\S*\s*=\s*-?\d+", lines[j]):
        name, val = re.match(r"^(\S+)\s*=\s*(-?\d+)", lines[j]).groups()
        offs[name] = int(val)
        j -= 1
    ins = []
    for l in lines[start + 1:]:
        if l.startswith(symbol + "\tENDP") or l.startswith(symbol + " ENDP"):
            break
        if not l.startswith("\t") or l.startswith("\t;") or not l.strip():
            continue
        op = l.split()[0]
        if op in ("npad", "ORG", "ALIGN", "DD", "DB", "DW") or op.endswith(":"):
            continue
        ins.append([n for n in re.findall(r"([_$][\w$@?]*)\[esp", l) if n in offs])
    return offs, ins


def depths(ins, popped=lambda target: None):
    """esp depth (bytes below the entry esp) before each instruction. popped(target) gives the bytes a
    direct callee removes (its `ret N`); unknown callees are assumed to pop what was pushed since the last call."""
    d, at, out, pend, barrier = 0, {}, [], [], False
    for i, (addr, t) in enumerate(ins):
        if barrier or addr in at:
            d = at.get(addr, d)
        barrier = False
        out.append(d)
        op = t.split()[0]
        if op == "push":
            d += 4
            pend.append(4)
        elif op == "pop":
            d -= 4
        elif m := re.match(r"sub\s+esp,(0x[0-9a-f]+|\d+)$", t):
            d += int(m.group(1), 0)
        elif m := re.match(r"add\s+esp,(0x[0-9a-f]+|\d+)$", t):
            d -= int(m.group(1), 0)
            pend = []
        elif op == "call":
            nxt = ins[i + 1][1] if i + 1 < len(ins) else ""
            if not nxt.startswith("add    esp"):
                m = re.match(r"call\s+0x([0-9a-f]+)$", t)
                n = popped(int(m.group(1), 16)) if m else None
                d -= sum(pend) if n is None else n
                pend = []
        if op.startswith("j") and (m := re.search(r"0x([0-9a-f]+)$", t)):
            at.setdefault(int(m.group(1), 16), d)
        if op in ("jmp", "ret"):
            barrier = True
    return out


def ret_bytes(pe, cache={}):
    """popped(target) for the original: the N of the first `ret N` in the callee (0 for a plain ret)."""
    def popped(target):
        if target not in cache:
            try:
                code = pe.b[pe.rva_to_off(target - IMAGE_BASE):][:0x2000]
            except Exception:
                code = b""
            cache[target] = None
            for _, t in disasm(code, target):
                if t.startswith("ret"):
                    m = re.match(r"ret\s+(0x[0-9a-f]+)", t)
                    cache[target] = int(m.group(1), 16) if m else 0
                    break
        return cache[target]
    return popped


def esp_slot(text, depth):
    m = ESP.search(text)
    return None if not m else int(m.group(1) or "0", 16) - depth


def main(argv):
    compile_ = "--no-compile" not in argv
    source, address = [a for a in argv if not a.startswith("--")][:2]
    source, address = os.path.abspath(source), int(address, 16)
    symbol = next(n for a, n, _ in marked_functions(source) if a == address)
    base = os.path.splitext(os.path.basename(source))[0]
    obj, asm = compile_listing(source) if compile_ else (os.path.join(OUT, base + ".obj"),
                                                         os.path.join(OUT, base + ".asm"))
    built_code = coff_function(obj, symbol)[0].rstrip(b"\x90\xcc")
    pe = PE(TARGET_EXE)
    size = original_size(obj, symbol, address, pe) if address in sizes_table() else len(built_code)
    orig = disasm(pe.b[pe.rva_to_off(address - IMAGE_BASE):][:size], address)
    built = disasm(built_code, 0)
    offs, names = listing(asm, symbol)
    if len(names) < len(built):  # longer is fine: EH funclets follow the body in the listing
        print(f"warning: listing has {len(names)} instructions, object {len(built)}; names may be misaligned")
    od = depths(orig, ret_bytes(pe))
    brefs = Counter(n for ns in names for n in ns)
    # extent of each of our locals: up to the next local's offset (arrays and objects span several dwords)
    order = sorted(set(offs.values()))
    end = {o: (order[k + 1] if k + 1 < len(order) else o + 4) for k, o in enumerate(order)}
    span = lambda n: (offs[n], end[offs[n]])

    # align the two instruction streams; every aligned pair where our side names a local is a vote
    key = lambda t: re.sub(r"0x[0-9a-f]+", "N", t)
    sm = difflib.SequenceMatcher(None, [key(t) for _, t in orig], [key(t) for _, t in built], autojunk=False)
    votes = []  # (original slot, our name)
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag in ("equal", "replace") and i2 - i1 == j2 - j1:
            for i, j in zip(range(i1, i2), range(j1, j2)):
                s = esp_slot(orig[i][1], od[i])
                if s is not None and j < len(names) and names[j]:
                    votes.append((s, names[j][0]))
    # the two frames differ by a constant: the most common (original slot - our offset)
    shift = Counter(s - offs[n] for s, n in votes).most_common(1)
    shift = shift[0][0] if shift else 0
    agree = sum(1 for s, n in votes if span(n)[0] <= s - shift < span(n)[1])
    print(f"{symbol}: frames anchored by {agree}/{len(votes)} aligned accesses "
          f"(low agreement = esp tracking or alignment is off; trust the table less)")

    orefs = Counter(s - shift for (_, t), d in zip(orig, od) if (s := esp_slot(t, d)) is not None)
    oname = defaultdict(Counter)
    for s, n in votes:
        oname[s - shift][n] += 1
    print(f"{'orig':>6} {'refs':>4}  {'our local there (aligned hits)':34} {'ours at':>7} {'refs':>4}")
    used = set()
    for s in sorted(set(orefs) | set(oname)):
        if s >= 0:  # return address and incoming arguments: not locals
            continue
        cands = oname.get(s)
        if cands:
            n, hits = cands.most_common(1)[0]
            used.add(n)
            lo, hi = span(n)
            # $T scratch bytes and the EH record are compiler bookkeeping, not source variables
            quirk = n.startswith("$T") or n == "__$EHRec$"
            flag = "" if lo <= s < hi or quirk else "  <- different slot"
            print(f"{s:6d} {orefs[s]:4d}  {n + f' ({hits})':34} {lo:7d} {brefs[n]:4d}{flag}")
        else:
            owner = next((n for n in offs if span(n)[0] <= s < span(n)[1]), None)
            print(f"{s:6d} {orefs[s]:4d}  {'?' + (f'  (ours: inside {owner})' if owner else ''):34}")
    rest = [n for n in offs if n not in used and brefs[n]]
    if rest:
        print("our locals no original access aligned to:",
              ", ".join(f"{n}@{offs[n]}({brefs[n]})" for n in sorted(rest, key=offs.get)))



if __name__ == "__main__":
    main(sys.argv[1:])
