# Service-menu page backgrounds and hit rectangles

| Reconstructed method | Original entry | Body | SEH handler |
|---|---|---|---|
| `DrawSavePage` | `0x44acf0` | 366 bytes | `0x4d9c5e` |
| `DrawContinuePage` | `0x44ae60` | 366 bytes | `0x4d9c9e` |

Each body aligns its EBP-based frame to eight bytes, constructs two arrays
of four eight-byte points, and submits a textured quad through device vtable
slots `0x8c` and `0x108`. The normal path destroys both arrays and restores
`fs:[0]` before returning. Evidence:
`ev_6fc0b140bd6335fa62960afcee5a4cf1fbfee81964a056e964e6bfc884dfecfb`
(Save) and
`ev_a8f6e0372fbee2ac1dccb990627abad0c70a0937029c31261834cc13f03597b4`
(Continue).

The exact C++ form default-constructs Color after the arrays and assigns
white at its later use. UV stores follow x-zero, x-one, y-zero, y-one source
order. Default construction alone reduced the mismatch to 12 bytes; moving
the x-one assignments before the y-zero assignments produced exact bodies.
These are compiler observations, not original-source claims.

Each object body matches 366 bytes with 24 COFF relocations. The focused
matcher now reports all 16 exact-marked functions in the module passing, with
six other functions still `NONMATCHING`. The rectangle builder is covered below.

Local verification limitation: the relocation-copy helper cannot resolve
the compiler-local EH labels, so it does not produce copies of these
functions. Object comparison and an independent relocation/unwind audit
provide the checks here; runtime execution was not tested.

Independent COFF/PE audit confirms 17 absolute relocations (all matching PE
base-relocation sites), four direct calls, and three `__except_list` references
per function. Scale/device globals and the rectangle constants 684, 943,
266, and 522 match the original data bytes. The element callbacks name
`0x490ad0`/`0x402c90`, and the array helpers name `0x4c11f4`/`0x4c1116`.

Both generated EH tables have three unwind states chaining
`2 -> 1 -> 0 -> -1`, with no try/catch entries. The cleanup actions match
the two four-element, eight-byte arrays at `ebp-0x2c`/`ebp-0x4c` and Color
at `ebp-0x50`. Save uses original EHInfo `0x4e96f0`; Continue uses `0x4e9728`.
These are local artifact observations from the independent audit.

## Rectangle construction (`0x44afd0`)

The observed body covers `0x44afd0`–`0x44b502` (1,331 bytes) and installs
SEH handler `0x4d9d0c`. It creates tab, back, row, and tuning hit rectangles,
sets their enabled flags, and inserts them into the screen's rectangle
vector. Evidence: `ev_8d4506e624020115e9343d63b221610b94babd90b3c05083705beb2b24128ca2`.

Reusing one loop counter across the three loops reproduces the target stack
slots. Resetting that counter before `n += 7` reproduces the remaining
register allocation and store order. Counter reuse alone reduced the
mismatch from 46 to 15 bytes; the earlier reset produced the exact match.
These are compiler observations.

Independent audit confirms 1,331 bytes and all 61 relocation identities:
30 absolute references at matching PE base-relocation sites, 28 direct
calls, and three FS references. All geometry constants and allocation,
point/rectangle construction, vector insertion/copy/fill/destruction, and
deallocation calls agree with the original targets.

Generated EHInfo agrees with original `0x4e9760` and its map at `0x4e9780`.
Its ten states have predecessor values `[-1, 0, 1, 2, 1, 4, 1, 6, 1, 8]`;
all cleanup stack offsets and targets match. The point destructors are
empty as in the original, while failed allocations invoke operator delete.
These are local artifact observations. The relocation-copy helper cannot
resolve the compiler-local EH label, so runtime execution remains untested.
