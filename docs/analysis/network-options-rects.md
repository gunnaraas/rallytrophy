# Network-options hit rectangles (`0x43a080`)

REA instruction evidence
`ev_c1794121daa12bf7f05e2f5fde0af8ab6acfbedffb9966193402df63b753539a`
reports a contiguous 727-byte body. The geometry below was separately checked
against instructions and float values in the official PE. These are static
observations, not runtime interaction results.

The old 1,469-byte draft added four fixed boxes after its four-row loop and
used placeholder coordinates. The target adds only three fixed boxes:

| Box | Upper left | Lower right |
|---|---|---|
| Row i, i=0..3 | (54, 252 + 42*i) | (1014, 252 + 42*(i+1)) |
| First fixed | (54, 607) | (474, 25494) |
| Second fixed | (500, 262) | (550, 483) |
| Third fixed | (900, 262) | (950, 483) |

The unusual first fixed lower Y is retained as observed: the code multiplies
42 (`0x4e1708`) by 607 (`0x4e1738`). No behavioral interpretation is asserted.
The row base is a runtime 262-10. Two point locals persist across the loop
and are assigned before allocation. Point copy, rectangle construction,
and every vector insertion stay calls.

The final body matches all 727 bytes with 38 relocations. All three aliases
resolve. All 16 direct call destinations and all 18 float operands agree with
the original PE; named float globals retain their original identities and
literal values match the referenced original bytes. Existing exact functions
remain passing. These checks cover function/object code, not a linked game.

An intermediate compound-arithmetic version was 727 bytes / 25 differences
at four scheduling sites. Direct expressions using stand-in globals passed
the relocation-masked comparison but reversed several commutative operand
references. Replacing the constant operands with literals preserves both the
original instruction schedule and operand identities. Earlier expression/
declaration-order, scalar-helper, qualifier, member-read, temporary and setter
probes did not resolve both checks and were discarded.

Focused validation passes 16 exact functions across the two changed modules,
with five NONMATCHING. Full validation passes 1,804/1,804 marked functions,
with 280 NONMATCHING, zero differences, and zero compile failures.
