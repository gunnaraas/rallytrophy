# GameScreen11A4::Frame matching notes

`GameScreen11A4::Frame` at `0x427c10` is in
[`src/unk_427600.cpp`](../../src/unk_427600.cpp), mapped by
[`analysis/range-4266c0.md`](../../analysis/range-4266c0.md).

REA evidence `ev_5ecd4ccbf7351685e154ad32c40f8558f8ee73c5edb4110a4ae220598957f11b`
records one contiguous 2,745-byte body from `0x427c10` through `0x4286c8`.
The evidence includes the complete instruction sequence and Ghidra pseudocode;
the pseudocode is a behavior reference, not reconstructed source.

## Current candidate

The source candidate compiles to 2,747 bytes. `tools/objcmp.py` reports 582
differing bytes outside relocations. The diff is largely a small set of
register and stack-slot changes repeated in branch targets, plus the two-byte
size difference. The source comment lists the previously tested statement,
switch, and local-variable variants.

The target pseudocode models the click flag as `bool`, so that type was tested.
It generated a one-byte stack flag, grew the function to 2,748 bytes, and
increased the differing-byte count to 1,756. Grouping the click flag and `sy`
in a local struct kept the size at 2,747 but increased differences to 626.
Both trials were reverted; the current `int clicked` and separate `float sy`
candidate remains the best observed result.
Separating their declarations from initialization, while preserving the target's
initialization order, and moving the title color assignment after `scaleY` each
left the 2,747-byte/582-difference result unchanged.

The current focused matcher reports 10/10 marked functions checked in this
source file, with this function still `NONMATCHING`. No exact match or commit
has been made for this function.
