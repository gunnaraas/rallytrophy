# Main-menu layout loader (`0x434e00`)

Local PE disassembly and COFF comparison observations: this loader builds the
seven rectangles in `data/extras/mainmenu.ini`. Its original draft was 1,553
bytes with 302 differences against the 1,572-byte target.

The target calls insertion for the first six rectangles and expands the
seventh insertion. In that expansion only the allocation-prefix copy is
inlined; fill and allocation-tail copy remain calls. `G10_RectVector` pins
those boundaries while preserving the real allocator and vector layout.
A compile-time check compares its size to the actual vector member.

| Helper | Target | Calls |
|---|---|---:|
| insert | `0x4bc680` | 6 |
| size | `0x414460` | 1 |
| uninitialized copy | `0x424320` | 3 |
| uninitialized fill | `0x424350` | 2 |
| destroy | `0x403130` | 1 |
| construct pointer | `0x403140` | 1 |

The six vector and five string/geometry aliases resolve to existing symbols.
All 31 calls through them were checked against the original PE destinations.
The result is an exact 1,572-byte C++ body with 81 relocations. The focused
module check passes 43 exact-marked functions, with eight still NONMATCHING.
Full matcher: 1,798 exact markers, 286 NONMATCHING, no differences or compile
failures.
The shared `DAT_004f7618` config-key table remains external; this verifies the
function/object rather than a linked game.
