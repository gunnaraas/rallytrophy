# Service-menu page backgrounds

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
matcher reports all 15 exact-marked functions in the module passing, with
seven other functions still `NONMATCHING`.

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
