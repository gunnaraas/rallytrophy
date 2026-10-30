# TrailQueue::Draw matching notes

`TrailQueue::Draw` at `0x40b600` updates the four trail alpha bytes, totals their
fade contribution, and draws live quads. The source is in
[`src/unk_40b560.cpp`](../../src/unk_40b560.cpp), with the surrounding address
map in [`analysis/range-409690.md`](../../analysis/range-409690.md).

## Target evidence and source corrections

REA instruction evidence
`ev_27f3e3b77b70ee090cd651245b8f5428adea6c2b52559c857427e2dcaf010004`
records the shipped 663-byte function. At `0x40b641`–`0x40b65b`, it checks
`time >= now - 5000` before computing age. The source now follows that order and
loads the timestamp once. The target uses `cmp age, 0x1194; jle` for the full
alpha plateau; the source expresses the fade-out branch as `age > 0x1194`.

The target converts each scaled age with x87 `FILD`, two `FMUL`s, `FSTP`,
`FLD`, and `FISTP`, rather than calling `_ftol`. An inline helper now preserves
that conversion sequence. The target's fade-out branch stores
`(BYTE)(0xff - delta)` and adds `0x1fe - stored_alpha` to the running total;
this differs from adding the stored alpha itself. The Ghidra-generated
behavior reference records this formula in `ev_087957f01ff011e02c0eb1cc2c762b1cac2275e4b5cfed70e382e184baea438a`.

At `0x40b74f`–`0x40b790`, REA shows the dimensions at device offsets `+4` and
`+8` loaded as integers and converted with `FILD`. The source now reads them
through integer pointers and converts them to floats for the height scale and
UV scale; the earlier float-pointer reads interpreted the integer bit patterns
as floats.

## Current match state

The current candidate is 658 bytes against the 663-byte target, with 403
differing bytes outside relocations. Naming the fade contribution separately
before combining it with the base y offset reduced the diff by four bytes;
then naming the scaled offset before subtracting it from the initial y matched
the target's fade, offset, y load order and reduced the diff by one more byte.
A pointer-based `do` loop reduced the
focused mismatch from 433 to 408 bytes and matches the target's pointer walk
more closely. Reusing one local for both integer screen dimensions was tested
and worsened the result to 649 bytes with 410 differing bytes, so separate
height and width locals remain. Remaining differences include local stack
slots, floating-point scheduling, the loop counter calculation, and quad
setup. This function is still `NONMATCHING`; the candidate is not recorded as
complete.

Materializing the loop bound as `usedOffset = -0xa0 - (int)this` produced 666
bytes with 426 differences; making it volatile produced 664 bytes with 428
differences. Moving the nonvolatile declaration before the other locals left
the 666/426 result unchanged. The inline expression remains in the source.
