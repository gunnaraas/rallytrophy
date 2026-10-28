# Lobby panel drawing (`0x43ad80`)

REA instruction evidence
`ev_1406548d14ed74b0062a8ee7a3250e8d1b794e5cfd902f1527877041f51b01e8`
reports a contiguous 532-byte body. Local PE disassembly observations show two
textured-quad draws and no rectangle allocation or vector insertion. The
tracked function is named `Screen1378::BuildRects`, and its previous draft
incorrectly inserted two fullscreen hit boxes. The working draft now draws
the two observed panels. The actual hit-box builder is
`Screen1378::Layout` at `0x43afa0`, now matched separately.

The target constructs two arrays of four eight-byte points, UVs then positions,
and destroys them in reverse order. Full UVs are assigned once. A Color local
retains the target's by-value argument lifetime. Each draw selects the material
through vtable offset 0x8c, then draws the quad through offset 0x108 with positions,
UVs, white, and scales (1,1). The existing RaceDeviceSlots::DrawQuad declaration
preserves the Color lifetime; the DWORD-taking vf66 declaration does not.

| Panel | Origin | Dimensions | Material |
|---|---|---|---|
| First | (47*sx, 355*sy) | (896*sx, 256*sy) | unk24 |
| Second | (432*sx, 205*sy) | (512*sx, 256*sy) | unk28 |

The second panel computes width/height before x/y. Separate named second-panel
coordinates and one white assignment recover the target's 532-byte size and
stack frame. Reassigning white before the second draw produced a needless
store and changed temporary reuse; reusing x/y variables also worsened matching.

The corrected working draft remains NONMATCHING: 532 bytes / 15 differences
around second-panel coordinate stores and renderer/material loads. Explicit
device/material locals worsened this to 28 differences; separate x stores and
commuting the final coordinate sums left 15. Scalar product literals preserve
scale-load/constant-multiply identities, unlike stand-in mutable float globals.
The corrected drawing body is retained in the source and marked
NONMATCHING. Focused validation preserves eight exact functions with four
NONMATCHING entries and no compile failures. An interleaved x/y-store probe
regressed to 530 bytes / 119 differences and was rejected.

Local PE reads independently verified constants 47, 355, 896, 256, 512, 432,
and 205 at `0x4e1784`, `0x4e1780`, `0x4e177c`, `0x4e14e0`, `0x4e14e8`,
`0x4e1778`, and `0x4e1774`, respectively. Array constructor/destructor
relocations resolve to `0x490ad0` / `0x402c90`, with vector construction
and destruction at `0x4c11f4` / `0x4c1116`. The differing instruction region
shifts the second material-selection global relocation, so a final relocation
audit must follow an exact instruction match.

Further isolated helper probes: existing `SetRect` produced 554 bytes / 137
differences; an inline geometry helper returning the material produced 532 / 24,
and passing the material by const reference produced 532 / 25. All were
rejected; the direct 532 / 15 reconstruction remains installed.

Full matcher after retaining the drawing reconstruction: **1,807 exact, 277
NONMATCHING, zero DIFF and zero compile failures**. Generated progress refreshed.
