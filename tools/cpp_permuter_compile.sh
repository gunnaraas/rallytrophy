#!/bin/sh
# Compile one cpp_permuter mirror candidate with the project's VC6 SP5 setup.
set -eu

if [ "$#" -ne 2 ]; then
	echo "usage: cpp_permuter_compile.sh SOURCE OBJECT" >&2
	exit 2
fi

src=$(realpath -- "$1")
obj=$(realpath -m -- "$2")
src_dir=$(dirname -- "$src")
src_name=$(basename -- "$src")
obj_rel=$(realpath --relative-to="$src_dir" "$obj")
repo_root=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)

cd "$src_dir"
if [ "${PERMUTER_NO_PCH:-0}" = 1 ]; then
	exec "$repo_root/tools/vc6.sh" cl /nologo /c /O2 /Oa /G6 /W3 /GX /I ../include /Fo"$obj_rel" "$src_name"
fi

pch=${src_name%.*}.pch
exec "$repo_root/tools/vc6.sh" cl /nologo /c /O2 /Oa /G6 /W3 /GX /YX /Fp"$pch" /I ../include /Fo"$obj_rel" "$src_name"
