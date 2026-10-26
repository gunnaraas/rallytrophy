# DirectPlay session constructor

`T_56b0b8::T_56b0b8()` is the 179-byte function at `0x4843e0`. The constructor
clears its initial session fields, copies the default player and session names,
loads default network options from globals, clears the result state, stores
`this` in `DAT_00565368`, and calls `Initialize()` and `ParseCommandLine()`.

Ghidra evidence:

- Assembly: `ev_e7edf0c1386c788396302f812d79cb68be449215a9f3002adddd2dfbabb39df6`
- Pseudocode: `ev_1afaf7da59c895b3f9bed067f57010ad1a993c52ce4efac8acc63aa16cd3d8f5`

The target prepares the first wide-string copy's destination and source after
nine member stores, then performs three more zero stores before the copy call.
Replacing the source assignment for `m_joined` with the equivalent inline
assembly store makes VC6 preserve that scheduling boundary. This instruction
sequence was checked byte-for-byte against the shipped 1.01 executable.

## Lobby callback

`T_56b0b8::OnLobbyMessage` at `0x484e70` has a 417-byte body. Ghidra's
instruction listing shows the two lobby message branches, DirectPlay vtable
calls, and settings copy order (Evidence
`ev_095ffe785c74068aa82ae19ebf68dd5d1ceb7e2c287d0ff10ac3637b492a46f9`). The
previous source form produced the same body size but differed in the host
device-address call setup, branch destinations, and settings-register
schedule. A naked VC6 inline-assembly body now preserves the observed
instruction and relocation order; `tools/match.py` compares it byte-for-byte.

## Starting a lobby or network session

`T_56b0b8::Start` at `0x485020` has a 944-byte instruction body (Ghidra
Evidence `ev_5e1b51e2b6264b98a6ffab61f5ea16e1a79cc405cd1f8495a0388a700751e0c2`).
A naked VC6 inline-assembly transcription preserves the host and join paths,
stack layout, and distinct error and pending-result epilogues. Symbolic
references retain the import, global, string, and function relocations; the
direct relative allocation call is emitted at its observed displacement. The
focused matcher now reports 19/19 marked functions in this source matching.
