#!/usr/bin/env python3
"""Compile every source file under src/ with VC6 and check each marked function.

A function is marked by a comment on the line(s) right before its definition:
    // FUNCTION: RALLYTROPHY 0x4a1ad0
Functions that are known not to match yet are marked NONMATCHING instead; they
are compiled and compared but don't fail the run.
An optional // SYMBOL: <exact COFF name> before the definition disambiguates overloads.
The function's size comes from analysis/functions.csv or analysis/functions_extra.csv.

usage: match.py [--quiet] [--changed] [FILE...]   (default: every .c/.cpp under src/)
  --quiet    don't print MATCH or NONMATCHING lines; compiler output, COMPILE FAILED,
             DIFF blocks of FUNCTION-marked mismatches and the summary are still printed,
             plus one extra line counting MATCH / NONMATCHING / DIFF / COMPILE FAILED.
  --changed  check only src/ files that are modified, staged or untracked relative to
             HEAD (git); if any file under include/ changed, check every file instead.
Without flags the behaviour and output are unchanged.
exit status: 0 if every marked function matches
"""
import csv
import glob
import os
import re
import subprocess
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
TARGET_EXE = os.path.join(ROOT, "orig", "RallyTrophy.exe")
CFLAGS = ["/nologo", "/c", "/O2", "/Oa", "/G6", "/W3", "/I", "..\\..\\include"]
CXXFLAGS = CFLAGS + ["/GX"]
# Extra flags for experiments, e.g. RT_EXTRA_FLAGS="/Oa" (not for normal use).
CFLAGS += os.environ.get("RT_EXTRA_FLAGS", "").split()
CXXFLAGS += os.environ.get("RT_EXTRA_FLAGS", "").split()

MARKER = re.compile(r"\b(FUNCTION|NONMATCHING):\s*RALLYTROPHY\s+0x([0-9a-fA-F]+)")
DEFINITION = re.compile(r"^[\w\s\*&<>,:]*?((?:\w+::)*(?:operator\s*[^\s(]+|~?\w+))\s*\(")


def marked_functions(path):
    """Yield (address, name or explicit COFF selector, nonmatching) per definition."""
    lines = open(path).read().splitlines()
    for i, line in enumerate(lines):
        m = MARKER.search(line)
        if not m:
            continue
        symbol = None
        for follow in lines[i + 1:i + 16]:  # skip the comment block after the marker
            override = re.match(r"\s*//\s*SYMBOL:\s*(\S+)\s*$", follow)
            if override:
                symbol = override.group(1)
            d = DEFINITION.match(follow)
            if d and not follow.lstrip().startswith(("/*", "//", "*")):
                yield int(m.group(2), 16), symbol or d.group(1), m.group(1) == "NONMATCHING"
                break


def changed_sources():
    """src/ files differing from HEAD (modified, staged or untracked); None if include/ changed."""
    def git(*args):
        return subprocess.run(["git", *args], capture_output=True, text=True, cwd=ROOT, check=True).stdout.split()
    paths = set(git("diff", "--name-only", "--relative", "HEAD", "--", "src", "include"))
    paths |= set(git("ls-files", "--others", "--exclude-standard", "--", "src", "include"))
    if any(p.startswith("include/") for p in paths):
        return None
    return sorted(os.path.join(ROOT, p) for p in paths
                  if p.startswith("src/") and p.endswith((".c", ".cpp")) and os.path.exists(os.path.join(ROOT, p)))


def main(argv):
    quiet = "--quiet" in argv
    changed = "--changed" in argv
    files = [a for a in argv if a not in ("--quiet", "--changed")]
    if changed:
        found = changed_sources()
        if found is None:
            print("note: a file under include/ changed; checking every file")
            files = []
        elif not found and not files:
            print("no changed files under src/")
            return 0
        else:
            files = sorted(set(files) | set(found))
    sizes = {}
    entries = []
    # functions_extra.csv adds functions Ghidra didn't find (e.g. only referenced as data).
    for name in ("functions.csv", "functions_extra.csv"):
        for r in csv.DictReader(open(os.path.join(ROOT, "analysis", name))):
            sizes[int(r["entry"], 16)] = int(r["size"])
    entries = sorted(sizes)
    def limit(address):  # bytes to the next known function
        later = [e for e in entries if e > address]
        return str(later[0] - address) if later else ""
    files = files or sorted(glob.glob(os.path.join(ROOT, "src", "**", "*.c*"), recursive=True))
    outdir = os.path.join(ROOT, "build", "obj")
    os.makedirs(outdir, exist_ok=True)
    total = matched = near = 0
    failed = 0
    for src in files:
        src = os.path.abspath(src)
        cplusplus = src.endswith(".cpp")
        obj = os.path.join(outdir, os.path.splitext(os.path.basename(src))[0] + ".obj")
        flags = CXXFLAGS if cplusplus else CFLAGS
        rel = os.path.relpath(src, outdir).replace("/", "\\")
        cc = subprocess.run([os.path.join(ROOT, "tools", "vc6.sh"), "cl", *flags, rel],
                            capture_output=True, text=True, cwd=outdir)
        log = "\n".join(l for l in cc.stdout.replace("\r", "").splitlines()
                        if l.strip() and l.strip() != os.path.basename(src))
        if log:
            print(log)
        if cc.returncode != 0:
            print(f"COMPILE FAILED {src}")
            failed += 1
            total += sum(1 for f in marked_functions(src) if not f[2])
            continue
        for address, name, nonmatching in marked_functions(src):
            if nonmatching:
                r = subprocess.run([sys.executable, "-I", os.path.join(ROOT, "tools", "objcmp.py"),
                                    obj, name, TARGET_EXE, hex(address), str(sizes[address]), limit(address)],
                                   capture_output=True, text=True)
                if not quiet:
                    print("NONMATCHING " + (r.stdout.splitlines() or [r.stderr.strip()])[0])
                near += 1
                continue
            total += 1
            r = subprocess.run([sys.executable, "-I", os.path.join(ROOT, "tools", "objcmp.py"),
                                obj, name, TARGET_EXE, hex(address), str(sizes[address]), limit(address)],
                               capture_output=True, text=True)
            if not (quiet and r.returncode == 0):
                print(r.stdout.rstrip() or r.stderr.rstrip())
            matched += r.returncode == 0
    print(f"\n{matched}/{total} marked functions match" + (f" ({near} NONMATCHING)" if near else ""))
    if quiet:
        print(f"MATCH {matched} / NONMATCHING {near} / DIFF {total - matched} / COMPILE FAILED {failed}")
    return 0 if matched == total else 1


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
