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

## Query member initialization

The target stores `Stage+0x13e8 = 0x11` at `0x40cc56`, before it pushes the
arguments for the first `StSample` array constructor (`0x40cc60`–`0x40cc79`).
Making `StQuery::mode` volatile in `include/stage.h` forces this store ahead of
those pushes, but VC6 also moves the next EH-state store ahead of it. The target
keeps that state store after the pushes, so the volatile field worsens the
whole-function byte difference from 5,770 to 5,772. The field is not otherwise
read or written through `StQuery` in this source tree; this probe was reverted.
The instruction ordering is observed in the shipped PE / Ghidra Listing
Evidence `ev_9b0c2cfce9d6f6e519dcc89206830b384da1777059ef694ae11eba0755bbff82`.

## Collision-mesh errors and logging

Target disassembly calls the empty logger at `0x402c90` for degenerate
triangles at `0x40e418`. For the missing-vertices path, it logs the fatal
message at `0x40e4d8`, initializes a string-like local at EBP-`0x1c`, and calls
`0x414550` at `0x40e532`; it then copies the result into an exception string
through `0x4024f0` before the throw helper at `0x4c13ca`. The missing-triangles
path repeats this structure from `0x40e5b6`. Ghidra assembly shows
`0x414550` derives both destination and source lengths from pointer pairs,
then grows, moves, and copies string data (Evidence
`ev_8fc3033cf819131f3072d2bc90493acc15e52b1d8fd4037574eb6e2404f6ce32`). Its
only resolved caller is `Stage::Stage` (Evidence
`ev_3c12754d57c4c5f54d774a140138db69cb203e0d1b272b6077cd418674a5b640`). At
`0x40e532`, that caller passes the local `begin`/`end` pair, then the format
literal and the object's field at `+0x118` (assembly Evidence
`ev_9b0c2cfce9d6f6e519dcc89206830b384da1777059ef694ae11eba0755bbff82`). The
source pair's runtime meaning remains unresolved, so the current
`StMsg::Format` interpretation is provisional; these observations do not
establish printf-style formatting.

A count-taking `replace(first, last, source, count)` probe using `s2 - s1` as
the count emitted 501 bytes with 430 differing bytes, versus 443 bytes / 400
differences for the existing range-overload candidate. It was reverted. This
overload does not reproduce the target's code despite the target's observed
pointer-pair length calculation; the caller's source-pair semantics remain
unresolved. Declaring the destination iterators `const char*` does not compile
with this VC6 header: its available four-argument `replace` overload requires
mutable destination iterators. That signature probe was reverted. The function
inventory classifies `0x414550` as `__thiscall`; recompiling the same operation
as a layout-compatible `std::string`-derived member also leaves the candidate
at 443 bytes / 400 differences. The calling-convention wrapper alone does not
explain the mismatch, so that probe was reverted too. The active source now
defines the actual `StMsg::Format` member in a separate translation unit and
shares its unchanged layout through `include/unk_414330.h`. This produces the
same 443-byte / 400-difference body, but the Stage caller and helper now use
the same member symbol (`?Format@StMsg@@QAEAAU1@PAD0PBD1@Z`); the 8,451-byte
Stage constructor candidate is unchanged. Both affected source files compile
in the focused matcher; checking all three TUs that include the header reports
eight exact functions, three NONMATCHING functions, and no compile failures.
Returning the underlying `replace` reference rather than `*this` also leaves
the helper at 443/400, so the clearer `return *this` form is retained.
Hoisting destination position, erase count, and source count in the target's
observed order and calling the count overload instead regresses to 450 bytes /
445 differences; it was reverted.

The range-replace reconstruction now defines `StMsg::Format` directly on the
shared `StMsg` layout in `include/unk_414330.h`, with the helper calls linked
to the corresponding VC6 `basic_string` methods. The shipped VC6 header at
`toolchain/vc98/include/xstring` confirms that the iterator overload computes
the source range, invokes the numeric replace overload, then copies the source
range. Moving the shrinking suffix move before `_Grow`, as that overload does,
improved the earlier local-layout candidate from 443 bytes / 400 differences
to 505 bytes / 351 differences. The current direct-member form uses the
header's `npos - sourceLen <= len - eraseLen` overflow check and a
`last == 0 ? 0 : last - first` erase count. Rewriting the null `first` case
to store the pointer value back into its stack argument slot, then computing
the suffix length and moving the shrinking suffix before `newLen`, improves
the candidate to 505 bytes / 324 differing bytes. It remains NONMATCHING. A
volatile
`last`-guarded erase count regressed in the earlier local-layout form. The
caller's source pair still has unresolved runtime meaning, so this remains
provisional and does not establish the intended error text.

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
