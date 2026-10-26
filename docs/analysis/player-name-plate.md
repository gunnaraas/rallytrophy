# Player-name plate drawing

`PlayerNameMenu::DrawPlate` (`0x43f740`) covers 368 bytes, aligns its EBP-based
stack to eight bytes, installs SEH handler `0x4d8c0e`, and returns with `ret 8`.
It draws the texture at `this+0x950` on a 512-by-256 quad scaled by the global
x/y factors. Evidence:
`ev_458b690eef42d1cb1d8756b8afed275ba2a1f4a75d7b04d9eb77dc29442b42b0`.

The exact C++ form default-constructs Color immediately after the two
`HudPoint[4]` arrays, assigns its value after setting the first corner, and
writes `uv[1].x = uv[2].x = 1.0f`. Separating construction from assignment
reproduces the target EH/store scheduling; reversing that one assignment
chain fixes the final two differing bytes. These are compiler observations;
the source spelling is reconstructed. The focused object matches 368 bytes
and 22 relocations.

Independent COFF/PE audit confirms 15 absolute relocations, four direct
calls, and three `__except_list` references. Every absolute operand site
corresponds to a PE base relocation. Array constructor/destructor calls
reference `0x4c11f4`/`0x4c1116`, their element callbacks name `0x490ad0`/
`0x402c90`, and the scale/device globals and 512/256 constants agree.

Generated EHInfo agrees with original `0x4e8a80`: magic `0x19930520`, three
unwind states (`0 -> -1`, `1 -> 0`, `2 -> 1`). Array cleanups address
`ebp-0x2c` and `ebp-0x4c` (four elements of eight bytes), then the Color cleanup
addresses `ebp-0x50`. This matches uv/pos/color construction and the aligned
frame. These are local artifact observations.

The relocation-copy helper cannot resolve the compiler-local EH label in
its own code section, so it does not produce a copy for this function.
Object comparison and the independent relocation/unwind audit provide the
verification here. Runtime execution was not tested.
