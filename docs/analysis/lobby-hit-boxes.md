# Lobby hit-box layout (`0x43afa0`)

REA instruction evidence
`ev_5c43dba7d92a8ba8996af5af43f538cbefba56c8c3c6eb1f75addbc96c5acd3b`
reports a contiguous 557-byte body. Local official-PE disassembly and scalar
reads establish three fixed, unscaled hit boxes:

| Box | Upper left | Lower right |
|---|---|---|
| First | (54, 262-20) | (54+420, 262-20+42) |
| Second | (54, 262-20+42) | (54+420, 262-20+42+42) |
| Back | (54, 610-10) | (54+200, 610+42) |

The prior 604-byte draft instead created three boxes in a scaled loop with
provisional dimensions. The revised body assigns two persistent point locals
before each allocation. Point copy, rectangle construction, and vector
insertion remain calls. The second lower Y doubles the row step before adding
the base, preserving the observed x87 operation order.

The result matches all 557 bytes with 34 relocations. All three aliases
resolve, all 12 direct call targets agree with the PE, and all 18 float
operands were checked with values and named-global identities retained.
The vector twin preserves the VC6 allocator/first/last/end layout, with a
compile-time size check. Verification covers function/object code, not a
linked game.

Compound field arithmetic initially left two scheduling sites (eight differing
bytes). A scoped volatile read shifted floating stores and worsened that to
15 differences; explicit bit copying left eight. Literal width and step
operands reproduce the target schedule and avoid commutative operand reversal.

Focused validation passes 16 exact functions across the two changed modules,
with five NONMATCHING. Full validation passes 1,804/1,804 marked functions,
with 280 NONMATCHING, zero differences, and zero compile failures.
