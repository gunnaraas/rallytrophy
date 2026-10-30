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
  one function and report which variants match. `RT_TRY_DIR` moves its
  scratch directory so parallel runs do not collide.
- [`permute.py`](permute.py): random hill-climbing permuter over
  `try_variants.py` (statement swaps, commuted operands, if/else flips,
  inlined temporaries); never edits the source, writes the best body to
  `build/permute/<addr>/best.cpp`. Apply it only on MATCH: mutations can
  reorder dependent statements.
- [`flag_sweep.py`](flag_sweep.py): score every NONMATCHING function in the
  given files under alternative compiler flags.
- [`asm_helpers.py`](asm_helpers.py): list NONMATCHING functions whose
  original contains inlined x87 `__asm` helpers the build lacks.
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
