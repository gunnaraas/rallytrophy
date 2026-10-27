# Track-menu layout loader (`0x4451c0`)

Local PE disassembly and COFF comparison observations: `BuildRects` constructs
the eight rectangles described by `data/menu/rallymenu.ini`. Ordinary vector
inlining produced 1,704 bytes and 331 differences against the 1,700-byte target.

`TSG_RectVector` fixes the same helper boundaries as the matched SoundMenu
loader: seven out-of-line insertions, followed by one expanded insertion.
The important difference is that this loader leaves the allocation-tail copy
out of line. Its helper counts are:

| Helper | Target | Calls |
|---|---|---:|
| insert | `0x4bc680` | 7 |
| size | `0x414460` | 1 |
| uninitialized copy | `0x424320` | 3 |
| uninitialized fill | `0x424350` | 1 |
| destroy | `0x403130` | 1 |
| construct pointer | `0x403140` | 2 |

The twin uses the actual allocator type and three-pointer layout, guarded by
a compile-time size check against the real vector member. Six vector aliases
and five string/geometry aliases resolve to existing definitions. All 34 calls
through those aliases were checked against the original PE destinations.

The result is an exact 1,700-byte C++ match with 89 relocations. The focused
module check passes 22 exact-marked functions, with three still NONMATCHING.
Full matcher: 1,797 exact markers, 287 NONMATCHING, no differences or compile
failures.
The shared `DAT_004f94d8` key table remains an external data dependency;
this is function/object verification rather than a linked-game test.
