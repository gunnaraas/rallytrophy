# Menu cell drawing primitives

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
