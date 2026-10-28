# Stage constructor (`0x40c9b0`)

## Vector insert boundary

Plain disassembly of the shipped PE shows calls to `0x4bc680` at `0x40d751`
and `0x40d766`. In both sequences the Stage vector is in `ECX`, the count is
one, and the value is passed by reference. The receivers are Stage+`0x68` and
Stage+`0x28`. The helper at `0x4bc680` is the out-of-line
`std::vector<void*>::insert(pos, count, value)` instance, as identified by
`src/unk_4bc680.cpp`.

The current `Stage::Stage` body had `push_back` at both sites. Replacing the
first Stage+`0x68` insertion with `StageLightVector::Insert` and aliasing that
method to the real vector helper produces 8,609 bytes with 5,755 differing
bytes (target: 8,656); the previous candidate was 8,487 bytes / 5,808 differing
bytes. The focused matcher reports one NONMATCHING function and no compile
failures.

Rejected isolated probes:

- Calling `std::vector<void*>::insert` directly at the first Stage+`0x68` site:
  8,789 bytes / 5,890 differences.
- Rewriting all 18 pointer pushes to stand-in `Insert` calls: 7,856 bytes /
  6,231 differences.
- Applying a generic stand-in to the first Stage+`0x28` light push as well:
  no additional reduction from the 8,609 / 5,755 result.
- Reordering the 21 start-position assignments to the target PE's observed
  store sequence: 8,609 bytes / 5,762 differences.
- Reusing the now-dead constructor parameter `b` as the inserted pointer's
  reference slot: 8,611 bytes / 5,764 differences.

These are source-shape results, not evidence that the whole constructor is
correct. The candidate still differs in most of the function. The disassembly
observations above are from the local official PE; no REA Evidence ID is
claimed for them.

## Collision-mesh errors and logging

Target disassembly calls the empty logger at `0x402c90` for degenerate
triangles at `0x40e418`. For the missing-vertices path, it logs the fatal
message at `0x40e4d8`, initializes a string-like local at EBP-`0x1c`, and calls
`0x414550` at `0x40e532`; it then copies the result into an exception string
through `0x4024f0` before the throw helper at `0x4c13ca`. The missing-triangles
path repeats this structure from `0x40e5b6`. PE disassembly of `0x414550`
shows a string range-replacement body: it derives lengths from two pointer
pairs, then grows, moves, and copies string data. The caller pushes the local
`begin`/`end` pair followed by a format literal and the object's field at
`+0x118`; that source pair's meaning remains unresolved. Therefore the existing
`StMsg::Format` interpretation is provisional and these caller observations do
not establish printf-style formatting. These are plain PE disassembly
observations, not REA Evidence IDs.

The current whole-function candidate is 8,451 bytes versus 8,656 target bytes,
with 5,770 differing bytes. Focused matching confirms it remains
NONMATCHING. The error-path source mapping is supported, but the constructor
still needs byte-level work elsewhere.

## Body setup locals

The target's FuncInfo at `0x4e5320` has states through 80; the current object
has states through 76. Plain PE disassembly associates states 63 and 64 with
empty-destructor locals at EBP-`0xe0` and EBP-`0x18`. At EBP-`0xe0`, the target
writes the extent `{250, 200, 250}` and passes its address to `StBody` at
`0x40db9f`. At EBP-`0x18`, it copies the source point's X and Z words with
integer moves, then computes and stores Y at EBP-`0x14`. The empty destructor
target `0x402c90` is a one-byte `ret`.

Replacing these raw arrays with `StVec3` locals reproduced two additional
unwind states. A trial using integer word copies for X/Z reached 8,447 bytes / 5,781
differences, versus the retained candidate's 8,609 / 5,755, and reserved a
0x350-byte frame instead of the target's 0x258. Its generated stores also
landed at EBP-`0xe4` and EBP-`0x2c`/`0x24`, rather than the target offsets.
Single-local `StVec3` trials were worse (8,381 / 5,849 and 8,455 / 5,834).
The substitutions were discarded. This maps likely lifetime types but does
not yet explain their target offsets or the source expression that schedules
the X/Z copies before the Y calculation.
