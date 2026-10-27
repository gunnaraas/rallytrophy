# Car reset and place on track (`0x45cc30`)

REA Evidence `ev_dd5399404fe4d12b8d49a8529f09c470d3a5a926f29c4ce866384c642263bd97`
identifies a contiguous 3,623-byte `FUN_0045cc30` body. Its fourth wheel
initialization loop advances the pointer for the hit normal by 12 bytes each
iteration and passes the start of each `Vec3` to the ground probe. The broader
behavior map is in [`../../analysis/range-459890.md`](../../analysis/range-459890.md).

## Matching probe

The previous source indexed the normal as `hitNormal[i]`; the target's loop
keeps a pointer to the `z` component and backs up two floats for the `Vec3`
argument. Using a `normalZ = &hitNormal[0].z` pointer, passing `normalZ - 2`,
addressing components relative to that pointer, and advancing it by three
floats produced a 3,644-byte candidate with 2,193 differing bytes outside
relocations. Making the three `v1408` zero writes volatile in target component
order reduced the count to 2,189. Marking `normalZ` itself volatile then
produced a 3,629-byte candidate with 2,123 differences and aligned the stack
frame with the target's `0xc4` bytes. Finally, moving the `k1414` assignments
before the volatile zero writes reduced the count to 2,114. These improve on
the 3,581-byte baseline with 2,573 differences. The current body remains
`NONMATCHING`.

Other differences remain in frame-slot assignment for transform operands and
`Vec3` locals, the x87 schedule of the first direction scaling, zero stores
around the repeated-store setup, query-box stores, and settle-loop register
allocation. The focused matcher reports 9 exact functions and 5
`NONMATCHING`, with no compile failures.

Two attempts to address the direction x87 schedule by calculating components
in `z/y/x` order were worse: assigning from the body-up vector into an
uninitialized local gave 3,606 / 2,932 differences, and retaining the vector
copy before multiplying in `z/y/x` order gave 3,614 / 2,914. Both were
reverted in favor of the 3,629 / 2,114 candidate. In the second wheel-query
loop, trying only `z` before `x/y` also regressed to 3,620 / 2,854 differences.
Preloading `0.02f` to a scalar local before the volatile zero writes and using
it for the three step fields regressed from 2,114 to 2,123 differences, so the
literal assignments remain.

Reordering the first transform block's `point`, `matrix`, and `result` local
declarations to try to shift the inline-assembly operand homes produced 2,134
differences; the original declaration order was restored.

Swapping the local declarations from `float h[4]; BYTE hit[4];` to
`BYTE hit[4]; float h[4];` had no effect on the compiled body (3,629/3,623
bytes, 2,114 differences). The original order remains. Target-home comparison
was identified in `ev_dd5399404fe4d12b8d49a8529f09c470d3a5a926f29c4ce866384c642263bd97`.

Making only `hit` volatile improved the candidate to 3,635 bytes / 1,958
differences. The target writes hit bytes through `[ebp + index + 0xc]` and
reuses `[ebp + 0x8]` for the loop counter (`0x45d221`–`0x45d263`). The first
volatile-array candidate improved the overall schedule but placed hit at
`[ebp + 0x8]`; a volatile loop-index probe regressed to 3,664 / 2,348 and was
reverted. Making the function-wide counter `i` volatile regressed to 3,842 /
3,373; it was also reverted. Reordering the float-height and volatile hit declarations emitted
identical code, as did changing the flag type from `BYTE` to `bool`. Evidence:
`ev_117a96552f47b03869326925a65ba9656366ff6cfee1be33265c4dd19d6fc6f1`. Accessing
the volatile array through a volatile base pointer worsened the candidate to
3,671 bytes / 2,380 differences; direct indexing remains for that form.

Target instructions at `0x45d221`–`0x45d263` also show that hit bytes use the
second-argument home `[ebp + index + 0xc]`. Evidence:
`ev_94c4b49df374c54afa3ccb2d12c5551b367a918fb3e2702ae31379d071864a29`.
Aliasing the volatile hit pointer to the now-dead `world` parameter (`&world`)
makes the candidate emit those stores and later byte reads, reducing the
mismatch by three bytes to 3,635 / 1,955. Keep this alias form.

The target then reads the first two hit bytes as one DWORD and tests `AL` and
`AH` at `0x45d272`–`0x45d27b` (same Evidence
`ev_94c4b49df374c54afa3ccb2d12c5551b367a918fb3e2702ae31379d071864a29`).
Loading `*(volatile DWORD*)hit` and testing its low two bytes in C produced
3,633 bytes / 2,003 differences, worse than the byte-wise checks. Reverted;
that expression did not preserve the target's test schedule.

Loading the pair as a volatile `WORD` and testing it as one value produced
3,628 bytes / 2,078 differences. This is also worse than the byte-wise check
and was reverted.

Inlining the target's DWORD load and `AL`/`AH` tests, then storing the result
to a C local before the existing gate, produced 3,653 bytes / 2,149
differences. The extra C-visible result disrupts the surrounding loop and was
reverted. Evidence for the target schedule:
`ev_94c4b49df374c54afa3ccb2d12c5551b367a918fb3e2702ae31379d071864a29`.

Using the target's `AL`/`AH` tests to branch directly around the pose block,
while leaving the second hit-pair check in C, caused VC6 to emit a 6,210-byte
body with 5,760 differences. Reverted. The inline branch crossing the C++
block does not produce a usable function layout.

Replacing the start-position `Vec3` copy and `+=` with explicit x/y/z scalar
assignments worsened the placement routine to 3,605 bytes with 2,634 differing
bytes. The vector copy and addition were restored.
