#!/usr/bin/env python3
"""Score NONMATCHING functions under alternative compiler flags.

For every `// NONMATCHING` function in the given sources, the current body is
compiled once per flag set via try_variants.py (RT_EXTRA_FLAGS) and the
differing-byte score is printed next to the baseline. A flag set that improves
many functions in one file hints the original object was built differently.

usage: flag_sweep.py SOURCE...
"""
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from try_variants import split_function  # noqa: E402

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
FLAGSETS = ["", "/Ob1", "/Ob2", "/Op", "/Oy-", "/Gy", "/Ox", "/Og /Oi /Ot /Oy /Ob1 /Gs /GF", "/G5", "/Zp4"]


def scores(source, address, body, tmp):
    path = os.path.join(tmp, "v.cpp")
    open(path, "w").write(body)
    out = {}
    for flags in FLAGSETS:
        env = {**os.environ, "RT_EXTRA_FLAGS": flags, "RT_TRY_DIR": os.path.join(tmp, "try")}
        r = subprocess.run([sys.executable, "-I", os.path.join(ROOT, "tools", "try_variants.py"), "-j", "1",
                            source, f"0x{address:x}", path], capture_output=True, text=True, env=env).stdout
        m = re.search(r"(MATCH|DIFF (\d+) bytes differ, built (\d+) vs original (\d+))", r)
        out[flags] = None if not m else 0 if m.group(1) == "MATCH" else \
            int(m.group(2)) + abs(int(m.group(3)) - int(m.group(4)))
    return out


def main(sources):
    print("address source " + " | ".join(f or "base" for f in FLAGSETS))
    for source in sources:
        source = os.path.abspath(source)
        text = open(source).read()
        for m in re.finditer(r"NONMATCHING:\s*RALLYTROPHY\s+0x([0-9a-f]+)", text, re.I):
            address = int(m.group(1), 16)
            start, end = split_function(text, address)
            with tempfile.TemporaryDirectory() as tmp:
                s = scores(source, address, text[start:end], tmp)
            base = s[""]
            better = {f: v for f, v in s.items() if f and v is not None and base is not None and v < base}
            print(f"0x{address:x} {os.path.relpath(source, ROOT)} base={base} "
                  + (" ".join(f"[{f}]={v}" for f, v in better.items()) or "-"), flush=True)


if __name__ == "__main__":
    main(sys.argv[1:])
