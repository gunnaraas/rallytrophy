# Decomp tools

The tools here compile C/C++ source with the project's local VC6 toolchain,
compare object code with the reference executable, and generate progress views.
They expect locally supplied game files and toolchain components; see
[`analysis/NOTES.md`](../analysis/NOTES.md) and the root [README](../README.md).

## Matching

- [`match.py`](match.py): compile source files and compare every marked
  function with the locally supplied PE. Pass source paths for a focused check;
  use `--quiet` for concise output.
- [`objcmp.py`](objcmp.py): compare one compiled COFF function against a PE
  range; normally called by `match.py`.
- [`scratch_asm.py`](scratch_asm.py): extract and inspect target instruction
  bytes while analyzing a function.
- [`try_variants.py`](try_variants.py): compile alternative source forms for
  one function and report which variants match, by positional bytes and by
  aligned instructions (`insns differ`). `RT_TRY_DIR` moves its
  scratch directory so parallel runs do not collide.
- [`permute.py`](permute.py): random hill-climbing permuter over
  `try_variants.py` (statement swaps, commuted operands, if/else flips,
  inlined temporaries); never edits the source, writes the best body to
  `build/permute/<addr>/best.cpp`, ranking by aligned instructions first.
  Apply it only on MATCH: mutations can
  reorder dependent statements.
- [`flag_sweep.py`](flag_sweep.py): score every NONMATCHING function in the
  given files under alternative compiler flags.
- [`asm_helpers.py`](asm_helpers.py): list NONMATCHING functions whose
  original contains inlined x87 `__asm` helpers the build lacks.
- [`numdiff.py`](numdiff.py): list instruction pairs of NONMATCHING functions that differ only in one number;
  finds wrong field offsets, struct sizes and constants in the source (many hits are only a different pointer anchor).
- [`x87check.py`](x87check.py): list x87 arithmetic chunks that differ between
  the original and the build (real operands), to spot wrong sum order, dropped
  terms or wrong fields; `--all --quiet` summarises every NONMATCHING function.
  `--sym` evaluates the x87 stack symbolically and prints only float stores,
  call arguments and compares whose expression trees differ (commutative order
  normalised, association not), which also catches `(a+b)+c` vs `a+(b+c)`.
  Events carry their address (original VA / object offset); float constants
  are compared by value (exe `.rdata`, `__real@` literals, read-only object
  data, `DAT_xxxxxxxx` externs). `--sym --list` prints one line per differing
  event: `orig_addr | built_off | orig tree | built tree` (`-` = unpaired).
- [`insdiff.py`](insdiff.py): aligned instruction diff of one NONMATCHING
  function against the original (after `match.py` on its source).
- [`calls.py`](calls.py): side-by-side call sequence of one NONMATCHING
  function (original vs build); the first `!` row is usually the structural
  difference (missing/extra inline expansion, wrong operand type, moved block).
- [`ehmap.py`](ehmap.py): the original's C++ EH FuncInfo for a function:
  unwind map (state, parent state, cleanup funclet with its destructor and
  `[ebp-X]` slot) and try blocks, i.e. every destructible local/temporary.
- [`dossier.py`](dossier.py): rank every NONMATCHING function by structural
  lead strength (EH map vs stored states, misaligned calls, frame deltas, prior
  log mentions) into `analysis/dossier.md`; pick agent targets from the top.
- [`mkws.sh`](mkws.sh): make a scratch workspace (tracked files from HEAD plus
  links to `orig/`, `toolchain/`, `extracted/` and a private `build/`) so
  parallel agents never share `build/obj`.
- [`structural.py`](structural.py): cluster NONMATCHING functions by stack
  frame, EH-state and call-count differences from the original (`--rows` for
  per-function detail).
- [`vc6.sh`](vc6.sh): run a VC6 tool through the local Wine/Distrobox setup.

## Inventory and reports

- [`progress.py`](progress.py): regenerate the progress summary, progress SVG,
  and function-size/complexity map after a successful full matcher pass.
- [`patch_exe.py`](patch_exe.py): place exact matched bodies into a local PE
  image for experiments; it does not produce a verified full rebuild.
- [`securom_api_map.py`](securom_api_map.py): support analysis of the protected
  executable's import-call dispatcher.
- [`ghidra/`](ghidra/): scripts for exporting function inventories and
  decompilation ranges from a local Ghidra project.
