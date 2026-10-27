# Sound-menu layout loader (`0x451c10`)

Local PE disassembly and COFF comparison observations: the target body is
1,848 bytes. It constructs nine rectangles, calls vector insertion for the
first eight, and expands the ninth insertion with selected helpers left as
calls. Ordinary `std::vector::push_back` chose different inline boundaries,
producing 2,133 bytes and 1,578 differing bytes.

`C_RectVector` uses the same allocator type and three pointers as the existing
VC6 vector layout. A compile-time size check compares it to the actual member
type. The first eight sites use an inline `PushBack` wrapper around an
out-of-line `Insert`. The ninth uses an expanded insertion with inline copy
and fill loops. Its remaining helper calls match the target policy:

| Helper | Target | Calls in loader |
|---|---|---:|
| insert | `0x4bc680` | 8 |
| size | `0x414460` | 1 |
| uninitialized copy | `0x424320` | 2 |
| uninitialized fill | `0x424350` | 1 |
| destroy | `0x403130` | 1 |
| construct pointer | `0x403140` | 3 |

All six vector aliases were checked against defined symbols in the existing
`std::vector<void*>` object. The underlying member remains the original
`std::vector<UIRect*>`; the twin controls compilation at these insertion sites.

The final body is an exact 1,848-byte match with 97 relocations. An additional
inline `InsertOne` wrapper reproduced the target's pointer-store timing,
reducing 169 differences to 15. Reversing the backward-copy comparison reduced
that to 14; using the allocator's `allocate`/`deallocate` methods instead of
direct operator calls removed the remaining register differences. The module
now has seven exact-marked functions and two remaining NONMATCHING functions.

Five additional aliases bind the pre-existing string and geometry twins:
`C_String` tidy/grow/end operations to `0x402700`/`0x402980`/`0x402960`, the
point copy to the call-compatible `Pair::Assign` at `0x424300`, and rectangle
construction to `A_Rect` at `0x4390a0`. All 11 alias targets were verified in
existing objects, and all 37 call operands through those aliases were checked
against the original PE destinations. Full matcher: 1,796 exact markers,
288 NONMATCHING, no differences or compile failures.
The shared `DAT_004fa330` config-key table remains an external
data dependency; this is function/object verification, not a linked-game test.

Measured intermediate candidates were 1,878 bytes / 586 differences with
direct calls at the first eight sites, 1,861 / 426 with the wrapper, and
1,819 / 440 with the ninth copy loops written directly. Separate inline copy
and fill helpers produced the intermediate 1,848 / 169 candidate. Passing the
wrapper argument by value regressed to 1,880 / 1,185. A volatile rectangle
store gave 1,848 / 154 but disturbed an earlier matching block and EH-state
ordering, so it was reverted.
