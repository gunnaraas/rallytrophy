# `Drivetrain::SelectGear` (`0x46b020`)

## Binary behavior

The 376-byte body installs SEH handler `0x4dbc48`, copies the body velocity,
projects away a negative forward component, computes wheel RPM, and walks
the shift thresholds to choose a gear. Evidence:
`ev_38a455eebb45bb71b016327f3427b840f47c2138b5826dc4d3e93599a7c8e104`.

The downshift loop computes the candidate lower gear's speed using `ratio[g]`,
so its corresponding upshift threshold is `shiftUp[g]`. The class initializes
gear 1's threshold at index 2; when the current gear is 2 (`g == 2`), the
candidate is gear 1 and is checked against index 2. The original instruction
reads `[edx+0x80]`, resolving to `shiftUp[g]`; the prior source used
`shiftUp[g + 1]` and emitted `[edx+0x84]`. Evidence:
`ev_5d15d3de1a5c28329d00bef133ea87bbcdd511aa9f06b6b13b5612fa2f4e3555`.

## Exact source form

Copy-constructing `Vec3` from the velocity and writing the dot product in
x/y/z source order reproduces the target velocity-copy and EH-state
scheduling. VC6 emits the target z/y/x x87 product order from that expression.
The focused matcher now reports all 23 functions in the module exact.
`SelectGear` matches 376 bytes with seven COFF relocations. These are compiler
and matcher observations, not a claim about the original source spelling.

Earlier isolated volatile-y and three-argument-constructor trials produced
384/306, 376/29, and 404/315 (built bytes/differing bytes), respectively.
Copy construction alone reduced the mismatch to 17 bytes; combining it with
the dot-product source order produced the exact match.

## Relocation and cleanup audit

Independent COFF/PE inspection confirms the generated handler corresponds
to `0x4dbc48`, the direct vector-scaling call targets `0x40b0c0`, the constants
are `60.0f` (`0x4e1ecc`) and `6.2831855f` (`0x4e1ec8`), and three FS references
use `__except_list`. The handler and two constants correspond to PE base
relocations. Generated EHInfo agrees with original `0x4eb188`: magic
`0x19930520`, one unwind state, no catch/try entries. The cleanup addresses
`ebp-0x24` and invokes the empty Vec3 destructor, matching original `0x4dbc40`.
These are local artifact observations.

The relocation-copy helper cannot resolve this compiler-local EH code label
without an enclosing external function symbol. It therefore does not produce
a copy for this function; object comparison and the independent relocation/
unwind audit provide the verification here. Runtime execution was not tested.
