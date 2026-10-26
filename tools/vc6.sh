#!/bin/sh
# Run a VC6 SP5 tool (cl, link, dumpbin, ...) under Wine in the rt-vc6 distrobox.
# usage: tools/vc6.sh cl /c /O2 foo.c
#        Works from the host or from inside the container; the cwd is preserved.
set -e
TOOLCHAIN=$(cd "$(dirname "$0")/../toolchain" && pwd)

if [ -z "$CONTAINER_ID" ] && [ ! -f /run/.containerenv ]; then
	exec distrobox enter rt-vc6 -- "$0" "$@"
fi

tool=$1
shift
win() { echo "Z:$1" | tr / '\\'; }
export WINEPREFIX="$HOME/.local/share/wineprefixes/rt-vc6"
export WINEDEBUG=-all
export WINEPATH="$(win "$TOOLCHAIN/vc98/bin");$(win "$TOOLCHAIN/msdev98/bin")"
export INCLUDE="$(win "$TOOLCHAIN/vc98/include")"
export LIB="$(win "$TOOLCHAIN/vc98/lib")"
exec wine "$TOOLCHAIN/vc98/bin/$tool.exe" "$@"
