# HUD point initializer

`HudPoint_Default` at `0x439080` writes the default x/y floats from
`DAT_00579bd0` and `DAT_00579bd4` into the supplied point. Ghidra identifies a
32-byte `__cdecl` function with one pointer parameter. Its observed sequence
loads x through x87, transfers y through ECX, writes zero into the saved-ECX
stack slot, then pops that zero back into ECX before returning.

Evidence from the shipped 1.01 executable:

- Assembly and pseudocode: `ev_1c6681c51a0e8928003868b8f4dd18ed9b27f612f657bf5dd18d4854b99dec08`

The source uses a naked body to preserve the observed register and stack
sequence. VC6 canonicalizes `mov dword ptr [esp+0], 0` to the shorter `[esp]`
encoding, so the eight-byte store is emitted explicitly to retain its zero
displacement. The focused VC6 matcher reports the 32-byte function exact.
