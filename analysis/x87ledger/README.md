# x87 behaviour ledger, 2026-11-01

`tools/x87check.py --all --sym --list` on every NONMATCHING function (event addresses, branch-aware
memory, constants compared by value) printed 738 differing events in 87 functions; the other 133
functions had none. Every event has one line in `batch0.md`..`batch3.md`:
`orig_addr | built_off | verdict | reason`, checked against both instruction streams.

- REAL-FIXED: association, product grouping, field read, dropped term or constant value differed;
  fixed in the source in the same pass (original and built instructions quoted on the line).
- NOISE: same computation; the tool's linear walk, slot/call-result naming, int vs float copies,
  branch layout or event pairing differ. Lines marked "24-bit precision control" are rounding-point
  differences that cannot change a value because FUN_004a7d30 runs the x87 at 24-bit precision
  (see `analysis/fpu-control.md`).

Orchestrator citation check (scripted against both binaries): every `address mnemonic` citation in the
0x417540 (146) and 0x46c790 (68) lines names the instruction actually at that address (two apparent
mismatches were address ranges whose end instruction is the cited fadd); all 53 original addresses cited
in the 0x41ef60 lines are real instruction starts. The 0x411600 pointer base and the 0x45cc30 Cast/rideHeight
verdicts were verified by the orchestrator directly (see the notes at the end of batch0.md and batch1.md).

Independent cross-review: a second agent re-derived every verdict from both instruction streams before
reading the ledger's reason (build/orch/xreview0..3.txt). Batch 0: 185/185 agree. Batch 1: 185/185 agree.
Batch 2: 166 agree, 18 disagree - all REAL-FIXED lines whose values were already identical (read-only
constants of the same value, a cached float equal to the field re-read); the orchestrator confirmed every
reference to those .data/.rdata addresses is an fld read, and reclassified them as NOISE (value-identical).
Side notes checked by the orchestrator: CityLight::Render's sign clamp is present in both builds (same
test/jge/mov 0 after the sign-bit exit); AIDriver::FUN_0049ffa0's lea [this+0x1b0] vs [this+0x1a8] is the
same obstacle record (the original's cursor points 8 bytes in: its [ebx+0xc] = 0 is the build's [ecx+0x14] = 0).
