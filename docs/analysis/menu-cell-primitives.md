# Menu cell drawing primitives

## `MenuScreen::DrawCellA` (`0x438240`)

The target body is 360 bytes and returns with `ret 0x18`. Its Ghidra dossier
records an SEH frame and two temporary `Vec2[4]` arrays; the handler address is
`0x4d828e`. Evidence: `ev_08c332c73abeb58596e844684233cc82e57cc7b878cfd7773197cc760c0bf13b`.

The source uses a relocation-aware inline assembly transcription. The object
has 18 COFF relocations: 13 absolute references corresponding to the PE base
relocations and five direct calls. `objcmp` reports an exact 360-byte match.
The patcher copied the function and passed its byte-for-byte self-check (1/1;
no unresolved symbols).

## `MenuScreen::DrawCellB` (`0x4383b0`)

The target body is 368 bytes and returns with `ret 0x18`. Its Ghidra dossier
records an SEH frame and two temporary `Vec2[4]` arrays; the handler address is
`0x4d82ce`. Evidence: `ev_c506ca13a55691b769ee21897bf82cfb17527ddcb1431c61ef50e9a15216e5f2`.

The source uses a relocation-aware inline assembly transcription. The object
has 19 COFF relocations: 14 absolute references corresponding to the PE base
relocations and five direct calls. `objcmp` reports an exact 368-byte match.
The patcher copied the function to `.rtpatch`, resolved all references, and
passed its byte-for-byte self-check (1/1; no unresolved symbols). These are
object and patcher observations, not evidence of a runtime execution test.

## `MenuScreen::DrawBar` (`0x438520`)

The target body is 469 bytes and returns with `ret 0x14`. The dossier records
an aligned stack, SEH handler `0x4d830e`, and two temporary `Vec2[4]` arrays.
Evidence: `ev_225f1ddd625830c2470290a9ea02c0d038e89e11e14ac73398862fd9a1356c5f2`.

The relocation-aware instruction transcription matches all 469 bytes with
19 COFF relocations: 14 absolute references and five direct calls. Local
relocation verification resolves every reference and reproduces the original
bytes. These are object-code observations; runtime behavior was not tested.

Compiler observation: defining this naked method before `DrawSlider` makes
VC6 remove that caller's SEH frame (229 bytes instead of 265). Placing the
definition after its callers preserves `DrawSlider` and all 58 exact-marked
functions in this module.
