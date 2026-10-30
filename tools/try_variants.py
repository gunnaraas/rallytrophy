#!/usr/bin/env python3
"""Try alternative bodies for one marked function without editing the real file.

Each variant file holds a complete replacement definition (marker line included).
The function's current definition in SOURCE is swapped out for each variant, the
file is compiled in its own build/try/<n>/ directory (variants compile in
parallel) and the function is compared like objcmp.py does. Variants are ranked
by differing bytes; the source is untouched.

usage: try_variants.py [-j N] [-k] SOURCE ADDRESS VARIANT...
  -j N   parallel compiles (default: number of CPUs, at most 8)
  -k     keep build/try/<n>/ (n = variant index) for objcmp.py inspection
A variant may also be a directory: every *.cpp in it is tried.
"""
import glob
import os
import re
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from match import CFLAGS, CXXFLAGS, TARGET_EXE, marked_functions  # noqa: E402
from objcmp import PE, relocations  # noqa: E402
from triage import compare, sizes_table  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def split_function(text, address):
    """Return (start, end) of the marked definition for `address` in `text`."""
    m = re.search(rf"^//[^\n]*(?:FUNCTION|NONMATCHING):\s*RALLYTROPHY\s+0x{address:x}\b.*?$", text, re.M | re.I)
    if not m:
        sys.exit(f"no marker for 0x{address:x}")
    brace = text.index("{", m.end())
    depth, i = 0, brace
    while True:
        depth += {"{": 1, "}": -1}.get(text[i], 0)
        i += 1
        if depth == 0:
            return m.start(), i


def main(argv):
    jobs = min(os.cpu_count() or 4, 8)
    if argv and argv[0] == "-j":
        jobs, argv = int(argv[1]), argv[2:]
    keep = argv and argv[0] == "-k"
    argv = argv[1:] if keep else argv
    source, address, *variants = argv
    address = int(address, 16)
    source = os.path.abspath(source)
    variants = [v for item in variants for v in
                (sorted(glob.glob(os.path.join(item, "*.cpp"))) if os.path.isdir(item) else [item])]
    text = open(source).read()
    start, end = split_function(text, address)
    sizes = sizes_table()
    later = [e for e in sorted(sizes) if e > address]
    limit = later[0] - address if later else None
    pe = PE(TARGET_EXE)
    original_relocs = relocations(pe)
    base = os.environ.get("RT_TRY_DIR") or os.path.join(ROOT, "build", "try")
    shutil.rmtree(base, ignore_errors=True)

    def off(t):
        return re.sub(r"(FUNCTION|NONMATCHING):\s*RALLYTROPHY", "OFF:", t)

    def run(item):
        n, variant = item
        outdir = os.path.join(base, str(n))
        os.makedirs(outdir, exist_ok=True)
        body = open(variant).read().strip().replace("NONMATCHING:", "FUNCTION:")
        trial = os.path.join(outdir, os.path.basename(source))
        open(trial, "w").write(off(text[:start]) + body + "\n" + off(text[end:]))
        include = os.path.relpath(os.path.join(ROOT, "include"), outdir).replace("/", "\\")
        flags = [include if f == "..\\..\\include" else f for f in (CXXFLAGS if trial.endswith(".cpp") else CFLAGS)]
        cc = subprocess.run([os.path.join(ROOT, "tools", "vc6.sh"), "cl", *flags, os.path.basename(trial)],
                            capture_output=True, text=True, cwd=outdir)
        log = "\n".join(l for l in cc.stdout.replace("\r", "").splitlines()
                        if l.strip() and l.strip() != os.path.basename(trial) and " warning " not in l)
        if cc.returncode != 0:
            return (10 ** 9, 0, 0, variant, "COMPILE FAILED\n" + log)
        obj = os.path.splitext(trial)[0] + ".obj"
        name = next((f[1] for f in marked_functions(trial) if f[0] == address), None)
        try:
            _, _, bad, bl, ol = compare(obj, name, pe, original_relocs, address, sizes[address], limit)
        except SystemExit as e:
            return (10 ** 9, 0, 0, variant, str(e)[:200])
        return (bad, bl, ol, variant, "")

    with ThreadPoolExecutor(jobs) as pool:
        results = list(pool.map(run, enumerate(variants)))
    for bad, bl, ol, variant, note in sorted(results, key=lambda r: (r[0], abs(r[1] - r[2]))):
        if note:
            print(f"{os.path.basename(variant)}: {note}")
        elif bad == 0 and bl == ol:
            print(f"{os.path.basename(variant)}: MATCH ({bl} bytes)")
        else:
            print(f"{os.path.basename(variant)}: DIFF {bad} bytes differ, built {bl} vs original {ol}"
                  + (f"  [build/try/{variants.index(variant)}]" if keep else ""))
    if not keep:
        shutil.rmtree(base, ignore_errors=True)


if __name__ == "__main__":
    main(sys.argv[1:])
