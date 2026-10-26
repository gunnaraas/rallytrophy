# Force effect parameter update

`ForceEffect::SetParameters` is at `0x4974c0`. Ghidra reports an inclusive
body range of `0x4974c0`–`0x49775b` (668 bytes), with the switch table beginning
at `0x49775c`. The shipped binary's instruction listing is recorded as
Evidence `ev_c099e2ffe3b7d3a63e6790f03b475f8db323c446126a1807ee044548d5633695`.

The source preserves the PE instruction and switch-table bytes directly so
the three effect-parameter setup paths retain VC6's original scheduling. The
focused matcher compares 700 bytes, including the switch table, and reports
an exact match with no relocations.

## Axis range calibration

`GetAxisRange` at `0x495410` has a 556-byte body (`0x495410`–`0x49563b`).
Ghidra's shipped-binary analysis is Evidence
`ev_825188dc1a8a079e74990b56fb8aad7b0c8a9d2859529c45eb9182ca2c534d25`.
When `OverrideRanges` is disabled, the function first requests the device's
`DIPROP_RANGE`; it writes the appropriate endpoint and reciprocal absolute
span, clamped to a minimum span of one. If that request is skipped or fails,
it queries the object's axis GUID and copies the corresponding input.ini range.
The return value is true only when a physical device range was read.

The source follows the observed instruction sequence and uses named symbols
for absolute global, GUID, range-table, and error-string references. The
focused file matcher reports 48/48 marked functions matching, with 3 existing
`NONMATCHING` functions and no diffs or compile failures. Direct `objcmp`
confirms `MATCH GetAxisRange 0x495410 (556 bytes, 16 relocations)`.

## Buffered input polling

`T_56b0b4::Poll` at `0x496b30` is analyzed in Evidence
`ev_698df91f427b5b423773d7a48763f1ab48c12fda83141bfefeaf364199a0bdfd`.
Its PE instruction body and adjacent switch table compare as 1,128 bytes.
The focused file matcher reports 49/49 marked functions matching, with 2
existing `NONMATCHING` functions and no diffs or compile failures; direct
`objcmp` confirms `MATCH T_56b0b4::Poll 0x496b30 (1128 bytes)`.
