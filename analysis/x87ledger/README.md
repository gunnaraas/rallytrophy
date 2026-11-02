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
Batch 3: 181 agree, 3 disagree - rounding-point-only fixes in 0x49e3d0 (identical at 24-bit precision),
reclassified as NOISE. Integer bug found by the batch-3 reviewer outside the x87 scope and fixed: 0x43ffd0 case 2
of both switches read option slot DAT_00501300[6] where the original reads 0x50131c = [7] (orig 44052d push
0x50131c, 440725 mov edx,ds:0x50131c; build now push/mov DAT_00501300+0x1c).

Orchestrator line-by-line verification of the two functions most often questioned (all events read from
both instruction streams, current HEAD):
- 0x46c790 Wheel::UpdateForces, 24 events: identical sequences up to relocated constants (cc9d, cd0a, cd10,
  cd2b, cf14), same commutative products/quotients with slot renames (cc7f, ccb9, cd6d, cd73 - orig st(1) is
  [esi+0x2ac] loaded at 46cd4d; slot map orig -0x64/-0x54/-0x48/-0x50/-0x80 = built -0x2c/-0x4c/-0x54/-0x58/-0x3c),
  the 0x2f4 relaxation/blend (cd99, cda2, cdd3) and 0x314 product (d060) with the same operands, the 9.81 grip
  compare (d0eb: |(-0x80)*[0x509418]| vs (-0x64)*9.81 = built |(-0x3c)*DAT_00509418| vs (-0x2c)*9.81), int pushes
  (built 96f), and two-term force/torque adds of a call result (d443..d4dc, built cfe/d10).
- 0x41ef60 RespBody::Respond, 40 events: same dot fields with a 0x20-shifted base (f08c), two-factor h[9..14]
  stores in a different order (f0d6..f0ed), accumulator adds into renamed slots (f106, f11f), the t2 sum and
  normalisation paired one-off (f50c..f557), cap scaling with renamed slots (f586..f623), two-term force/torque
  adds of call results (f6a2..f742, built 797, fb73..fc1a), compare with swapped operands (f866), two-term sum
  into 0x258 (f909), the K/s divides unrolled in the original vs a loop (f979..f9d5), compare forms (fac6, fad6).
