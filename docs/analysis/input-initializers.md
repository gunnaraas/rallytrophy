# Keyboard input initializers

`FUN_004b6d60` is a 12-byte `__stdcall` wrapper. It pushes the local target
`0x4b6d70`, calls the shared `0x4c0d01` helper, removes the pushed argument
with `pop ecx`, and returns.

Evidence from the shipped 1.01 executable:

- Assembly and pseudocode: `ev_457d029188a35a8813c62d1211f07ca4d9ea86bca20b9024e6d2c4c70f0361ce`

The isolated source wrapper matches all 12 bytes with VC6.

## Character queue construction (`0x4b6dd0`)

The 18-byte initializer reserves four stack bytes, passes the address of the
last byte as the allocator argument, and calls `0x4b73c0` with ECX set to
the global object at `0x57bc28`. It restores ESP with `pop ecx` and returns.
Assembly and pseudocode evidence:
`ev_bd8a8c590d7b0f4ae00074a00d9b8c2549a1cb0b56eb7ebbe1e1e74aa1dd7b20`.

Source/matcher observation: defining `g_keyboardCharacterQueue` in the
isolated initializer translation unit with VC6 inlining disabled emits this
sequence as `_$E2`. Its two relocations refer to the queue and the allocator
constructor overload of `std::deque<unsigned char>`, already mapped at
`0x4b73c0` in `input_deque.cpp`. All 18 bytes outside linker relocations match.
The input callback continues to use the same externally declared global.

## Keyboard hook callback (`0x4b6f90`)

The 773-byte callback is registered as the `WH_KEYBOARD` procedure by
`T_56b0bc::T_56b0bc` at `0x4b6e10`. Plain PE disassembly shows separate key-up
and key-down paths, repeated-key filtering, virtual-key translation, and
character insertion into the shared deque. The callback body in
`src/input.cpp` now emits the exact observed instruction bytes. REA
decoded the entry instruction as `sub esp, 0x10` and reports a data reference
to the callback from `0x4b6e5f` (`ev_43ebc21a677a80c69ee53b1ec4dc4cc8514912b7a23e6086e3d4c651f4b76ad2`).
Ghidra has no function record at this callback entry, so the full operation
summary is a static PE-disassembly observation rather than a Ghidra function
dossier.
Linked placement and startup ordering remain unverified.

## Adjacent lifetime wrappers

The same source emits three adjacent wrappers. The address identities below
come from shipped instructions; the generated symbol names are COFF observations.

| Address | Size | VC6 symbol | Observed instructions |
|---|---:|---|---|
| `0x4b6dc0` | 10 | `_$E5` | Call `0x4b6dd0`, then jump to `0x4b6df0` |
| `0x4b6df0` | 12 | `_$E4` | Push `0x4b6e00`, call `0x4c0d01`, pop ECX, return |
| `0x4b6e00` | 10 | `_$E3` | Set ECX to `0x57bc28`, then jump to `0x4b7490` |

Evidence:

- `0x4b6dc0` call: `ev_23848954be2b36fbe2cf5d0dbf09307ed5a61ce59be50689f8f3bad5a12b9b81`
- `0x4b6dc5` jump: `ev_0fcd12520fbe22b9fb096df7eade2259493a4134a03eb09920f19decdd8aecd1`
- `0x4b6df0` dossier: `ev_c847cd2aa3c94f51b560f9358ee1a9280b29c7154ad8c165f173e28ea1319dd2`
- `0x4b6e00` object load: `ev_4af00e43c8acfb090bcde32cb6a3ad5464d09f8452ecd35462e52450cb12335e`
- `0x4b6e05` destructor jump: `ev_55cbb4f10a232fff7b11b6bec8ddb367676f4a417a317686a1488fefe73541c5`

Ghidra does not define procedures at `0x4b6dc0` or `0x4b6e00`; the two
10-byte boundaries are inferred from the decoded instruction lengths and
terminal jumps. Ghidra classifies these jumps as call terminators; the raw
opcodes are `E9` jumps. The incoming data reference to `0x4b6dc0` is at
`0x4f23dc`; the registration wrapper directly references `0x4b6e00`.
Interpreting the group as construction plus exit-time deque cleanup is
supported by the generated COFF references to the deque constructor,
destructor, and `_atexit`; static analysis does not establish runtime order.

Matcher observation: all three generated wrappers match outside linker
relocations. The initializer translation unit passes 5/5 marked functions;
the full repository check passes 1488/1488 (467 existing NONMATCHING entries).

## Mouse callback and its aligned switch table (`0x4b6c10`)

The existing C++ mouse callback matches without implementation changes.
Ghidra's inventory describes its 207-byte instruction body; the final
`ret 0xc` at `0x4b6cdc` is confirmed by
`ev_feb55f54bf8f4df13999e610a066307ba63816a2c0d6e30df518df06a10031b3`.

Plain raw-byte and COFF observations: one `0x90` alignment byte at
`0x4b6cdf` precedes the six-entry jump table at `0x4b6ce0`–`0x4b6cf7`.
All 232 bytes through that table match outside relocation operands. The six
COFF table relocations at offsets 208, 212, 216, 220, 224, and 228 coincide
with original PE base relocations. Resolving the local COFF labels plus their
addends independently gives the original branch offsets 79, 98, 115, 186,
143, and 160. Thus the table's target identities were checked separately
from the relocation-masking matcher.

The previous matcher stopped at the first padding byte and omitted the
table. `objcmp.py` now recognizes a complete relocated pointer table after
at most 15 identical padding bytes, bounded by a known next function. Every
table entry must have both a COFF relocation and the corresponding PE base
relocation. Missing boundaries, nonpadding gaps, partial or unrelocated
tails, and differing padding remain rejected. Eleven synthetic cases cover
those bounds. The keyboard callback now matches all 773 bytes after replacing
its earlier nonmatching C++ body with the observed PE instruction sequence.
This change retains the matcher's general limitation that relocated target
identities are not checked automatically.
