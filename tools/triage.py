#!/usr/bin/env python3
"""Classify every NONMATCHING function by the shape of its first differences.

Reuses the objects match.py left in build/obj (run match.py first). For each
`// NONMATCHING: RALLYTROPHY 0x<addr>` marker, compares the built function with
the original like objcmp.py does and records the size difference, the number of
differing bytes and the first differing instruction pair, then guesses a cause:

  argcount     `ret` vs `ret N` (wrong signature / calling convention)
  eh-state     the first difference is a cleanup-state store or EH frame setup
  registers    same instruction stream after renaming registers
  order        same instructions, different order (store / scheduling order)
  size         built and original differ in size (inlining / layout)
  other        same size, differs in more than registers

The "inst delta" column counts instructions present on only one side after
renaming registers and masking addresses: 0 means a pure register / scheduling
permutation, a handful means one missing or extra statement, hundreds mean a
structural difference. The notes column flags `ret N` and cleanup-state
sequence mismatches.

usage: triage.py [--md FILE] [FILE.cpp...]   (default: every file under src/)
"""
import csv
import glob
import os
import re
import sys
from collections import Counter

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import marked_functions  # noqa: E402
from objcmp import IMAGE_BASE, PE, aligned_relocation_table, coff_function, disassemble, relocations  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGET_EXE = os.path.join(ROOT, "orig", "RallyTrophy.exe")
REG = re.compile(r"\b(e?[abcd]x|[abcd][lh]|e?[sd]i|e?[bs]p)\b")
NUM = re.compile(r"0x[0-9a-f]+")


def shape(line):
    """An instruction with registers and addresses abstracted away."""
    line = NUM.sub("N", line)
    line = re.sub(r"^(j\w+|call|jmp)\s+.*", r"\1 X", line)
    return REG.sub("R", line)


def inst_delta(left, right):
    l, r = Counter(map(shape, left)), Counter(map(shape, right))
    return sum((l - r).values()) + sum((r - l).values())


def sizes_table():
    sizes = {}
    for name in ("functions.csv", "functions_extra.csv"):
        for r in csv.DictReader(open(os.path.join(ROOT, "analysis", name))):
            sizes[int(r["entry"], 16)] = int(r["size"])
    return sizes


def compare(obj, symbol, pe, original_relocs, address, size, limit):
    """Return (built_disasm, original_disasm, differing_bytes) with linker fields zeroed."""
    built, masked, targets = coff_function(obj, symbol)
    built = built.rstrip(b"\x90\xcc")
    image = pe.b[pe.rva_to_off(address - IMAGE_BASE):]
    if len(built) > size and size < len(image) and (
            image[size] not in (0x90, 0xCC) or
            aligned_relocation_table(built, image, size, limit, targets, original_relocs, address)):
        size = len(image[:limit].rstrip(b"\x90\xcc")) if limit else len(built)
    original = image[:size]
    for r in original_relocs:
        if address <= r < address + size:
            masked.update(range(r - address, r - address + 4))
    bad = sum(1 for i in range(max(len(built), len(original)))
              if i not in masked and built[i:i + 1] != original[i:i + 1])
    ofields = {i for r in original_relocs if address <= r < address + size for i in range(r - address, r - address + 4)}
    bfields = {i for off in targets for i in range(off, off + 4)}
    left = disassemble(bytes(0 if i in ofields else c for i, c in enumerate(original)), address)
    right = disassemble(bytes(0 if i in bfields else c for i, c in enumerate(built)), address)
    norm = lambda ls: [re.sub(r"^call\s+0x[0-9a-f]+$", "call <rel>", l) for l in ls]
    return norm(left), norm(right), bad, len(built), len(original)


STATE = re.compile(r"^mov\s+(?:DWORD|BYTE) PTR \[e[bs]p[-+]0x[0-9a-f]+\],(0x[0-9a-f]+|-?\d+)$")


def eh_states(lines):
    """Cleanup-state constants stored in order (ebp-4 / esp-slot immediates), as a compact string."""
    out = []
    for l in lines:
        m = STATE.match(l)
        if m and ("[ebp-0x4]" in l or "BYTE PTR" in l):
            v = int(m.group(1), 0)
            out.append("-1" if v == 0xffffffff else str(v))
    return ",".join(out)


def rets(lines):
    return ",".join(sorted({l.replace("ret", "ret").strip() for l in lines if l.startswith("ret")}))


def classify(left, right, built_len, orig_len):
    first = next(((i, l, r) for i, (l, r) in enumerate(zip(left, right)) if l != r), None)
    if first is None:
        first = (min(len(left), len(right)), left[len(right)] if len(left) > len(right) else "",
                 right[len(left)] if len(right) > len(left) else "")
    i, l, r = first
    l0, r0 = l.split()[0] if l else "", r.split()[0] if r else ""
    if l0 == "ret" and r0 == "ret":
        return "argcount", first
    if "fs:0" in l or "fs:0" in r or re.search(r"mov\s+DWORD PTR \[ebp-0x4\],", l) or re.search(r"mov\s+DWORD PTR \[ebp-0x4\],", r):
        return "eh-state", first
    if built_len != orig_len:
        return "size", first
    if [REG.sub("R", x) for x in left] == [REG.sub("R", x) for x in right]:
        return "registers", first
    if Counter(left) == Counter(right):
        return "order", first
    # registers + order mixed
    if Counter(REG.sub("R", x) for x in left) == Counter(REG.sub("R", x) for x in right):
        return "order+registers", first
    return "other", first


def main(argv):
    md = None
    if "--md" in argv:
        md = argv[argv.index("--md") + 1]
        argv = [a for a in argv if a not in ("--md", md)]
    files = argv or sorted(glob.glob(os.path.join(ROOT, "src", "**", "*.c*"), recursive=True))
    sizes = sizes_table()
    entries = sorted(sizes)
    pe = PE(TARGET_EXE)
    original_relocs = relocations(pe)
    rows = []
    for src in files:
        obj = os.path.join(ROOT, "build", "obj", os.path.splitext(os.path.basename(src))[0] + ".obj")
        if not os.path.exists(obj):
            continue
        for address, name, nonmatching in marked_functions(src):
            if not nonmatching:
                continue
            later = [e for e in entries if e > address]
            limit = later[0] - address if later else None
            try:
                left, right, bad, bl, ol = compare(obj, name, pe, original_relocs, address, sizes[address], limit)
            except SystemExit as e:
                rows.append((address, os.path.relpath(src, ROOT), name, "nosym", 0, 0, 0, 0, str(e)[:60], "", ""))
                continue
            cause, (i, l, r) = classify(left, right, bl, ol)
            notes = []
            if rets(left) != rets(right):
                notes.append(f"ret {rets(left)} vs {rets(right)}")
            if eh_states(left) != eh_states(right):
                notes.append(f"eh {eh_states(left)} vs {eh_states(right)}")
            rows.append((address, os.path.relpath(src, ROOT), name, cause, bl, ol, bad, inst_delta(left, right), l, r, "; ".join(notes)))
    rows.sort()
    counts = Counter(r[3] for r in rows)
    out = ["# NONMATCHING triage", "", "Generated by `tools/triage.py` from the objects in `build/obj`.", "",
           "| Cause | Functions |", "|---|---|"]
    out += [f"| {c} | {n} |" for c, n in counts.most_common()]
    perm = sum(1 for r in rows if r[7] == 0)
    near = sum(1 for r in rows if 0 < r[7] <= 4)
    out += ["", f"Instruction-multiset view: {perm} functions are pure register/scheduling permutations (inst delta 0), "
            f"{near} differ by at most four instructions.", "",
            "| Address | File | Function | Cause | Built | Orig | Diff bytes | Inst delta | First original | First built | ret / EH-state notes |",
            "|---|---|---|---|---|---|---|---|---|---|---|"]
    for a, f, n, c, bl, ol, bad, d, l, r, notes in rows:
        out.append(f"| 0x{a:x} | {f} | {n} | {c} | {bl} | {ol} | {bad} | {d} | `{l}` | `{r}` | {notes} |")
    text = "\n".join(out) + "\n"
    if md:
        open(md, "w").write(text)
    print(text if not md else "\n".join(out[:4 + len(counts) + 2]))
    print(f"{len(rows)} NONMATCHING functions")


if __name__ == "__main__":
    main(sys.argv[1:])
