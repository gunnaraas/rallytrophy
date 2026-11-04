#!/bin/sh
# Make a scratch workspace for one agent: tracked files from HEAD plus links to the
# untracked inputs, with its own build/ so parallel agents never share build/obj.
# usage: tools/mkws.sh DIR
set -e
ROOT=$(cd "$(dirname "$0")/.." && pwd)
mkdir -p "$1"
git -C "$ROOT" archive HEAD | tar -x -C "$1"
for d in orig toolchain extracted; do ln -sfn "$ROOT/$d" "$1/$d"; done
mkdir -p "$1/build"
echo "$1"
