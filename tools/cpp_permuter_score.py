#!/usr/bin/env python3
"""Adapt objcmp.py's exact byte verdict to cpp_permuter's score-cmd protocol."""
import argparse
import os
import re
import subprocess
import sys


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--obj", required=True, help="candidate VC6 COFF object")
    parser.add_argument("--symbol", required=True, help="COFF symbol or qualified C++ name")
    parser.add_argument("--exe", required=True, help="official target executable")
    parser.add_argument("--address", required=True, help="original function address")
    parser.add_argument("--size", required=True, type=int, help="original function size")
    parser.add_argument("--limit", help="optional byte limit to the next function")
    args = parser.parse_args()

    objcmp = os.path.join(os.path.dirname(os.path.abspath(__file__)), "objcmp.py")
    command = [sys.executable, "-I", objcmp, args.obj, args.symbol, args.exe,
               args.address, str(args.size)]
    if args.limit is not None:
        command.append(args.limit)
    result = subprocess.run(command, capture_output=True, text=True)
    if result.stdout:
        sys.stdout.write(result.stdout)
    if result.stderr:
        sys.stderr.write(result.stderr)

    if result.returncode == 0 and result.stdout.startswith("MATCH "):
        score = 0
    elif result.returncode == 1 and result.stdout.startswith("DIFF "):
        first_line = result.stdout.splitlines()[0]
        mismatch = re.search(r";\s*(\d+)\s+differing bytes outside relocations$", first_line)
        if not mismatch:
            return 1
        score = int(mismatch.group(1))
    else:
        return result.returncode or 1

    print(f"cpp_permuter score: {score}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
