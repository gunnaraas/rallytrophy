# Parallel shadow rendering

This note records current matching work on the software shadow renderer in
`src/unk_46fdf0.cpp`. The broader function map and subsystem layout are in
[`analysis/range-46fdf0.md`](../../analysis/range-46fdf0.md).

## `ParallelShadow::Create` (`0x470610`)

REA assembly evidence `ev_22c39413bccfaabbe1f4aa8d9d76264a62c41ae7fe9a4baad8d680895bbc5e32`
shows the target interleaving call-setup pushes with sprite initialization
stores on both the render-target and system-texture paths. The 715-byte
candidate initially differed in 168 bytes. Making the `unk60` stores
volatile reduced that to 162. Also making the `unk70` stores volatile reduces
it to 131; adding volatile `unk40` stores to that form regresses to 155.
Volatile `unk40` alone gave 165, and making `unk18`/`unk24` volatile gave
182. The current 715-byte candidate keeps `unk60` and `unk70` volatile, but
call-setup instruction scheduling still differs and the function remains
NONMATCHING.

## `ParallelShadow::Prepare` (`0x470940`)

The first light/forward dot product is written in the target's observed z/y/x
accumulator order. Reversing the C++ term order left the candidate unchanged
at 3,490 bytes and 1,483 differing bytes within the 3,336-byte target range;
the focused matcher still reports all 62 marked functions passing. This local
`objcmp` observation did not improve code generation. The existing frame,
inlined transform, and register mismatches remain the main gap.

## `ParallelShadow::ProjectMeshes` (`0x471ab0`)

REA assembly evidence `ev_378c13001f5c265115cca9d40c27f7d342ab1bd4c7ce252692de74fc458d597c`
shows the target's center-offset stores in z/y/x order and records the x87
schedule around the face radius and plane tests. Expressing the offsets in
that order improved the VC6 candidate from 1,759/1,788 bytes with 1,136
differing bytes to 1,763/1,788 bytes with 1,121 differing bytes. Reordering
the last two dot-product terms, both directly and through a separate sum,
did not change generated code. The function remains NONMATCHING; mesh-loop
frame and register allocation still differ.

## `ParallelShadow::ParallelShadow` (`0x4703c0`)

REA evidence `ev_e2d18152e9c91241b4226ef784a2de4febf5e28195db3b37cc938a94cbefa4fb`
shows the constructor's byte-offset vertex loop, stride `0x2c`, end offset
`0xb000`, and four caster clears. Declaring the loop offset before the vertex
pointer locals improves the 378-byte candidate from 15 to 14 differing bytes.
Moving it to function entry regressed to 392 bytes with 219 differences; moving
it between the pointer declarations and removing the `register` hint had no
effect. Replacing the captured pointer with direct `mesh->vertices` accesses
regressed to 393 bytes with 125 differences. The retained candidate still
differs in register/SIB selection. Reversing the address-addition operands and
expressing the addresses as integer sums both emitted identical candidate
bytes; these local compiler probes were reverted. Naming
the vertex-colour sentinel before the pointer locals also left the candidate
unchanged and was reverted. Naming
the index-allocation result and moving the vertex-pointer capture around its
assignment both regressed to 379 bytes with 164 differing bytes; those probes
were reverted. An inline-assembly loop using the captured registers forced an
EBP frame and regressed to 368 bytes with 276 differences; it was reverted.

## `ParallelShadow::RasteriseObject` (`0x4721b0`)

REA assembly evidence `ev_29d97e716110d7023b37b541d681a71602455c2e54fc525bd1455f0e8cda5ddd`
shows the target converting the half-size integer to float, storing it at
`[ebp-0x10]`, and subtracting the constant at `0x4e1b44`. Making source local
`h` volatile forces that stack home. The candidate is 1,354/1,372 bytes with
634 differing bytes, compared with 1,347/1,372 bytes and 689 differences
before the probe. The volatile form is retained because its instruction
sequence and byte comparison are closer to the target; the function remains
NONMATCHING. Reversing the x-direction dot-product operands did not change
codegen. Replacing the volatile local with a separate volatile spill worsened
the candidate to 1,350 bytes with 656 differing bytes, so that probe was
reverted. A separate `m13Scaled = -(b * m[13])` temporary, evaluated before
the `m[12]` update, follows the target's retained x87 intermediate in
evidence `ev_97dc54e79fef93c284c6e010b9015776b75175379b7307954374c1592c15c85e`
and improves the current candidate to 1,362/1,372 bytes with 550 differing
bytes. Explicit `m[12] = m[12] + h` and `m[12] = h + m[12]` forms emit the same
code as `m[12] += h`, as does accessing the matrix through its global name.
An inline-assembly store emitted the requested `m[12]` load/add/store, but
forced `m13Scaled` out of the x87 register stack and regressed to 648 differing
bytes; it was reverted.

## `FillTriangle` (`0x472710`)

REA assembly evidence `ev_183891f7dd3fcb066f1d31b4cd436092089ebcd199d92facdf9f5b616af31588`
confirms the target row-A decrement sequence as `dec`, `test`, then store,
with `jg` on the positive-row path. The current nonvolatile source emits the
same decrement/test/store instructions. Rewriting the branch as a positive
condition or an explicit forward `goto` increased the candidate from 92 to
93 differing bytes; both probes were reverted. The function remains
NONMATCHING.
