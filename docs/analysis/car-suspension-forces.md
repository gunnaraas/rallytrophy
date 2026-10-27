# Car suspension forces (`0x45c400`)

REA Evidence `ev_0e4f83e9e6d1f5123614bc85583566b0385db1b9b9ac3667fa4c27aeb1e847b0`
identifies a contiguous 756-byte `FUN_0045c400` body. The opening x87 sequence
computes the front anti-roll difference with two loads, `fxch`, and a non-popping
`fsub`; the source's ordinary subtraction compiles to a direct memory `fsub`.
The function then applies spring, damper, and anti-roll forces and their torque
to the body. The wider behavior map is in
[`../../analysis/range-459890.md`](../../analysis/range-459890.md).

## Matching probes

The current candidate is 739 bytes with 656 differing bytes outside
relocations. Reordering only the four source assignments to `arb[0]` through
`arb[3]`, matching the target's observed local-array write order, yielded 657
differing bytes. An aggregate initializer in the same order yielded the same
657 differences. Both probes were reverted. Remaining documented differences
include x87 operand scheduling, pointer and `Vec3` stack slots, and torque-add
operand order. The focused source-file check reports 9 exact functions and 5
`NONMATCHING`, with no compile failures.

Materializing the cross-product result as a local `Vec3` made the torque add
emit the target's `result + torque` operand order but worsened the overall
candidate to 740 bytes / 657 differences. Binding that return value to a
`const Vec3&` altered two unrelated static-initializer matches in the same
source file. Both forms were reverted.

Using a local cross-product result and a volatile pointer to body torque emitted
the target's result-first component additions, but yielded 740 bytes / 658
differences, worse than the 739 / 656 best candidate. The probe was reverted.
Target assembly at `0x45c64b`–`0x45c67c` confirms the result component is loaded
before each torque accumulator; Evidence: `ev_cd864be1d01c15008106b9a0b6d86b3922e9911c3b9afaad23f7bfc80db1a547`.

Volatile local homes for the two spring-difference operands did not reproduce
the target's x87 two-load schedule and regressed the candidate to 763 bytes /
661 differences. The original `W6_Sub` expressions remain.

Rewriting `W6_Sub` with inline x87 loads, `fxch`, non-popping `fsub`, then
storing and clearing the remaining stack value produced 793 bytes / 709
differences. That helper closes the x87 stack before the rate multiply, unlike
the target's schedule, and regresses the function. It was reverted; matching
the observed opening instructions requires reproducing their surrounding
floating-point lifetime, not just the subtraction itself. Evidence:
`ev_0e4f83e9e6d1f5123614bc85583566b0385db1b9b9ac3667fa4c27aeb1e847b0`.
