# Car wheel contact (`0x45c700`)

## Target evidence

REA Evidence `ev_af5f5586f35ae18eb2ec962f2895369e53df17277d2c787e5bd8b75f92d812f0`
identifies the 1,144-byte `FUN_0045c700` body. The function updates suspension
contact for four wheels, queries ground height and surface data, and writes
per-wheel contact and surface fields. The working field map is in
[`../../analysis/range-459890.md`](../../analysis/range-459890.md).

## Matching probes

The baseline source compiled to 1,134 bytes with 919 differing bytes outside
relocations. Reading the target's `1.0f` constant at `0x4e1c1c` through a
volatile pointer changed the emitted order to `fld [constant]; fadd rideTop`
and improved the result to 1,130 bytes / 913 differences. This aligns the
local operation with the target disassembly; it does not make the function
exact. The same-valued constant at `0x4e1348` was also tested and emitted the
same object bytes; the source now names the address observed at `0x45c796`.

Removing the cached `rideBase` local and reading `c->rideHeight` at the
subtraction site reduced the frame and yielded 1,128 bytes / 868 differences.
Constructing the local vector with its three-argument constructor instead of
assigning its components compiled to the earlier 1,134 / 919 result, so the
component assignments remain. Initializing the contact-distance local from
`dist` before the comparison made the body 1,125 bytes / 926 differences, so
the original branch form remains. The current focused check reports 9 exact,
5 `NONMATCHING`, and no compile failures in this source file.

Remaining differences include spring-minimum common-subexpression handling,
the local-y x87 stack schedule, contact-flag zero setup, and general register
and stack allocation. The current source is still marked `NONMATCHING`; keep
testing small source-shape changes against the compiled body.

Explicitly caching `ly - r[-3]` in a `springMin` local before the branch
produced 1,118 bytes / 877 differences, worse than the current 1,128 / 868
candidate, so that form was reverted.

Writing the contact flag through a shared `int contact` local and selecting
`dist` versus `rideTop` with a conditional expression compiled to 1,125 bytes
with 935 differing bytes, worse than the branch form; it was reverted. The
target sets the flag on separate branches at `0x45c8cb` and `0x45c8da`.
Evidence: `ev_4d4150b5ac59aa584b622bcc8bd010d4f4d44cb72e3ab3ee76a66d239aaa3348`.

Reordering the temporary vector assignments to x/z/y, matching the target's
component-load schedule, had no effect on the compiled body (1,128/1,144 bytes,
868 differing bytes). The source remains x/y/z.

Reordering the `hitNormal` and `wheelSurface` pointer declarations to match the
target prologue also emitted identical bytes; the original declaration order
remains.

Splitting the local-y expression into `local.y = wp[-1]; local.y += radius;`
produced 1,124 bytes / 870 differences, two more differences than the 1,128 /
868 candidate. The original single expression remains.

The target's ride-height home is visible at `0x45c785` (load from
`[edi+0x10fc]`), `0x45c793` (store to `[ebp-0x28]`), and `0x45c8e4` (subtract
from that stack slot), Evidence `ev_af5f5586f35ae18eb2ec962f2895369e53df17277d2c787e5bd8b75f92d812f0`.
Moving only the declaration to function scope while preserving the per-wheel
assignment emitted a 1,130-byte candidate with 913 differing bytes, versus
1,128 / 868 for the current direct-load form. Reverted; the alternate scope
does not reproduce the target home.

Hoisting `int noContact = 0` before the wheel loop and using it at the four
existing contact-clear stores produced 1,128 / 871, three more differing bytes
than the current candidate. Reverted. The target's distinct branch stores
remain documented in Evidence `ev_4d4150b5ac59aa584b622bcc8bd010d4f4d44cb72e3ab3ee76a66d239aaa3348`.
