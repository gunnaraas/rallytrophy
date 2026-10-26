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

## `MenuScreen::DrawSelector` (`0x438dc0`)

The target body is 381 bytes, returns with `ret 0x24`, and installs handler
`0x4d83c0`. Its prologue sets EH state 1 before storing the white color.
Evidence: `ev_73b97d230353bead1ead5e92b7252d501ec9446d64872a642c76816f2cc2ac83`.

Default-constructing both `Color` locals before assigning `white.value = -1`
reproduces the original instruction schedule. The focused matcher reports all
59 functions in this module exact, with no remaining `NONMATCHING` markers.
The selector itself matches all 381 bytes with 21 COFF relocations.

Local verification limitation: the relocation-copy helper cannot resolve the
compiler-local EH handler label in `.text$x` because that section has no
enclosing external function symbol. It therefore does not produce a copy for
this function. The byte match remains an object comparison with relocation
fields masked; runtime behavior has not been tested.

Independent COFF/PE audit: all 13 absolute relocations correspond to PE base
relocations, all five direct calls name the target functions, and the three
FS references use `__except_list` with zero operands. The generated EH handler
sets `eax` to its EHInfo and jumps to `__CxxFrameHandler`, matching the original
handler's structure. Both EHInfo records have magic `0x19930520`, two unwind
states, and no try/catch entries. The two cleanup actions address the same
frame slots (`ebp-0x18`, `ebp-0x24`) and invoke `Color::~Color` (`0x402c90`).
Thus the helper's unresolved label is a local-label resolution limitation;
the generated unwind semantics are present in the object. This paragraph
records local object/PE observations.
