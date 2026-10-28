# Skid-mark constructor (`0x4741a0`)

Status: **NONMATCHING**. The constructor initializes one wheel's skid-mark
buffer, its render state, and the `SkidMark` object name.

Evidence: REA instruction listing
`ev_aa9c2695b578a5bc27939c13a62cc93386a8eb785705979066595e27c3c4b1f7`
(`0x4741a0`–`0x474457`, 696 bytes). Comparison results below are local
VC6 object/reference-PE observations.

## Vertex initialization

The target uses an induction pointer at `seg + 0x1c`, the first vertex's
position.y. Expressing that cursor explicitly reproduces the scalar stores,
register allocation, and complete 128-segment initialization loop. Each segment
contains four vertices, with white colors, zero positions and normals, and
paired texture coordinates. All original initialized fields remain present.

The ordinary `std::string` assignment can remain: a string-layout stand-in
produced no additional improvement. The retained cursor change moves the
constructor from 561 bytes / 396 differing bytes to 594 / 198. The first
unmasked byte difference is the branch displacement at `0x474369`; the
preceding 457 bytes agree after relocation masking. This is a prefix result,
not a claim that the entire function matches or that its relocations have
received a final audit.

## Remaining string boundaries

The original expands zero-length string cleanup into three separate
constructor exits. The current compiler instead emits a cleanup call and
shares return paths. Positive-length growth calls the canonical string copy
helper; trimming also calls the canonical tidy helper.

Isolated probes tested nested string helpers, direct constructor branches,
member versus static refcount access, delayed reference-count writes, distinct
adjusted-pointer forms, forced cleanup inlining, and terminator-before-length
stores. Direct branches reached 635 bytes but retained merged exits; changing
reference-count write order alone did not recover the target. These probes
were not installed in source. Further work should resolve the cleanup/return
boundaries before pursuing final scheduling differences.

Validation: focused matcher reports **10/10 exact, 2 NONMATCHING**, with no
compile failures. The constructor remains NONMATCHING.
