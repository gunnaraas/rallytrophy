# Optional `cpp_permuter` setup

Keep the optional build outside this checkout. Set `PERMUTER` to the path of
the `cpp_permuter` executable. The local pilot used upstream commit
`59a4fea7d20aaae4d58786aa1a0425e9251199ef` and was built with CMake and g++.

Prerequisite recheck (2026-11-24): WSL has CMake 4.2.3 and g++ 15.2.0, and
`g++ -std=c++17 -x c++ -fsyntax-only /dev/null` succeeds. LLVM 20's disassembler
is installed as `/usr/bin/llvm-objdump-20`; the unversioned `llvm-objdump`
name is absent. The project's `--score-cmd` path bypasses cpp_permuter's native
objdump scorer, so the Rally Trophy PE adapter needs no extra package. For a
COFF target using the native scorer, pass
`--objdump /usr/bin/llvm-objdump-20`.

The upstream scorer normally disassembles a target COFF object with
`llvm-objdump`. Rally Trophy's oracle is a PE executable, so the optional
`tools/cpp_permuter_score.py` adapter invokes the existing `objcmp.py` against
that EXE. `objcmp.py` returns status 1 for a byte difference, while
`cpp_permuter` discards scores from any nonzero scorer status. The adapter
preserves the disassembly output, turns the count of differing bytes outside
relocations into the final numeric score, and returns status 0 for both a
valid match and a valid difference. Score 0 still means `objcmp.py` reported
an exact match. This is a byte-count ranking, not the permuter's instruction
edit-distance ranking.

The optional `tools/cpp_permuter_compile.sh` compiles in the mirrored source
directory, so Wine/VC6 receives relative source and object paths. It uses the
project's `/O2 /Oa /G6 /GX /YX` flags and `../include`; when cpp_permuter sets
`PERMUTER_NO_PCH=1` for a multi-file edit, it omits `/YX`.

## Invocation template

Run from WSL at the repository root, set an isolated output directory outside
the Rally Trophy and cpp_permuter checkouts, and select only the passes that
apply. Replace the placeholders with the chosen function's symbol/address/size.
This template is documentation; it does not copy candidates into the project
sources.

```sh
ROOT=/path/to/rallytrophy
PERMUTER=/path/to/cpp_permuter/build/cpp_permuter
OUT=/tmp/rt_cpp_permuter_runs/<function>

"$PERMUTER" \
  --source "$ROOT/src/<source>.cpp" \
  --function '<Class>::<Method>' \
  --mirror-root "$ROOT" \
  --output-dir "$OUT" \
  --compile "sh $ROOT/tools/cpp_permuter_compile.sh {src} {obj}" \
  --score-cmd "python3 -I $ROOT/tools/cpp_permuter_score.py --obj {obj} --symbol '<Class>::<Method>' --exe $ROOT/orig/RallyTrophy.exe --address 0x<address> --size <size>" \
  --mode random --iterations 100 --jobs 1
```

Review candidates from `$OUT/output-<score>-<n>/source.cpp` manually. Copy a
candidate into a project source only as a separate step, then run
`python3 -I tools/match.py --quiet src/<source>.cpp`. Only that matcher can
confirm a project function as matching. The parser's handling of each target
function is still unknown until a future, explicitly selected run.

The compile adapter and score wrapper were smoke-tested with a synthetic C++
fixture in `/tmp`: VC6 produced an i386 COFF object and the wrapper accepted an
`objcmp.py` difference as a numeric score with status 0. A later isolated
`Wheel::Wheel` run confirmed that the parser selects the function and that the
PE scorer reports the baseline 8-byte difference. Bounded `dead_wrap` produced
no candidates because this constructor has no loop; `scope_block` compiled two
candidates, neither better than baseline. No candidate was copied into the
project source. This does not validate parser handling for other functions.
