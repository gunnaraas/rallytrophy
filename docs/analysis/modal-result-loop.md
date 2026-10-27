# Modal result loop (`0x436250`)

## Binary observations

The shipped function occupies `0x436250`–`0x436894` inclusive and is 1,605
bytes. At entry, EDI is initialized to 1, ESI receives `this`, and EBX is zeroed.
The target later keeps the text renderer in EBP and reloads the horizontal
scale before centering the second row. The mouse-warp temporaries use exception
states 1 and 2, while the inline string uses state 3. These are observations
from the shipped executable's instruction listing:
`ev_73c2dac610c7620c901786acf194f63a917eee40521190f8ff865ebf6a414d9f`.

## Source and matcher status

The source body is `GameObj::FUN_00436250` in
[`src/unk_4303e0.cpp`](../../src/unk_4303e0.cpp). Reordering the trivial local
declarations to `first`, `choice`, `wait`, then `HudPoint mouse` reduced the
temporary candidate's out-of-relocation mismatch count from 731 to 713. Adding
a local alias of the existing `text` member for the drawing section reduced it
again to 510. Recomputing the row center from the horizontal scale, instead of
keeping `cx` live, reduced it to 464. The current candidate is 1,599 bytes
versus the 1,605-byte target. Ordering the integer locals as `wait`, `choice`,
`first` reduced it to 461. The candidate has the target's 0x3c-byte frame,
renderer in EBP, and EBX=0/EDI=1 constants. Remaining differences include
local stack slots and string spills/inline expansion. The source-shape
explanation is an inference; the byte counts are matcher observations.

The normal focused file check reports 42 marked functions, 33 exact and 9
`NONMATCHING`, with no hard diffs or compile failures. A temporary probe with
this function enabled for comparison measured the 461-byte difference; the
function remains marked `NONMATCHING` in the working source.
