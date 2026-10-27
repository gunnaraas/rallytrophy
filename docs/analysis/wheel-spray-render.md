# WheelSpray render (`0x477690`)

## Evidence

REA evidence `ev_6e2034557ea54db46de9b42321c77c445cd8efd3f0dbae384e3a4b07d011fb93`
identifies the 1,227-byte `WheelSpray::Render` body. Target disassembly shows
the two atlas-step sums load `0.125` before adding the stored UV coordinate:
`fld 0.125; fadd [v.u]`. Initial direct additions reversed those operands,
leaving eight differing bytes. Separate named accumulators initialized to
`kAtlasStep` and updated with `+=` reproduce both target additions and retain
the 1,227-byte size. Four differing bytes remain because the depth-argument
load is scheduled before the first `fadd` instead of after it (local `objcmp`
observation).

## Probes

- Reversing each C++ addition to put `kAtlasStep` on the left did not change
  object code.
- Volatile source reads produced a 1,218-byte candidate with 121 differing
  bytes; making the constant volatile produced 72 differing bytes.
- Inline assembly with raw target stack offsets produced 135 differing bytes;
  naming the local vertex fields in inline assembly produced 116.
- Replacing the complete atlas lookup, UV arithmetic, argument setup, and draw
  call with the target instruction sequence made the body 1,214 bytes with 947
  differing bytes. The compiler changed surrounding register and stack
  scheduling when the block clobbered `EAX`, `ECX`, and `EDX`; this was reverted.
- Replacing both additions with inline x87 blocks that store directly into the
  vertex fields made the body 1,224 bytes with 116 differing bytes. This
  reproduced the constant-first operand order but changed the surrounding
  schedule and local layout; it was reverted.
- Replacing only the `u` addition with inline x87 (`fld` constant, `fadd` the
  local vertex field, `fstp` the result) kept the body at 1,227 bytes but
  increased the diff from 8 to 69 bytes; it was reverted.

- Embedding the U accumulator update in its chained assignment or reusing one
  accumulator for both coordinates produced 28 differing bytes. Declaring both
  accumulators together and splitting the U stores retained four differences.
  A late `drawDepth` argument local regressed to 1,230 bytes / 122 differences.
- Declaring the U accumulator before the atlas lookup, initializing it there,
  or adding a `register` hint each retained the four-difference candidate.
  Accumulating directly into `v[1].u` instead produced eight differing bytes,
  reversing the U operands and the two U stores. These probes were reverted.
- Passing depth through an integer bit-copy left the four-byte diff unchanged;
  forcing a volatile depth read at the call site expanded it to 100 differing
  bytes. Both probes were reverted.
- Splitting the base V assignment into a store and copy retained four
  differences. Repeating the lookup expression for each store instead
  regressed to 1,221 bytes / 123 differences. Both probes were reverted.

The separate named accumulators are retained; the other probes were reverted.
The focused matcher reports 12/12 marked functions
matching in `src/unk_477370.cpp`, with this `NONMATCHING` function exempted.
