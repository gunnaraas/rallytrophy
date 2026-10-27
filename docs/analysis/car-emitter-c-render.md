# CarEmitterC render (`0x47b730`)

## Evidence

REA function dossier `ev_3eb318ced99d1b1d1e4e4844379dccfa8d2eac04d73d234fe05faf75831f7041`
identifies a single contiguous 2,113-byte body at `0x47b730`–`0x47bf70`.
The function is a `__thiscall` method with two stack arguments and an SEH
frame. The disassembly saves the device and camera pointers in `EDI` and `ESI`
before the matrix calls. The source body is in `src/unk_47b4c0.cpp`.

## Current comparison

Focused matcher: 8 marked functions, 7 exact, 1 `NONMATCHING`, no hard diffs
or compile failures. `objcmp.py` reports equal 2,113-byte bodies with 891
different bytes outside relocation fields. The target reserves `0x204` bytes
for locals; the source candidate reserves `0x208`. Stack offsets and register
allocation diverge through much of the function, alongside operand-order
differences in floating-point expressions.

Raw `objdump` comparison observation: at relative offset `0x5de`, both bodies
begin the same position copy, transform, and cross-product instruction
sequence. The operations continue in the same order through the two scalar-
vector helper calls, but stack locations and some register choices differ.
This narrows the useful next probes to frame layout and register allocation in
the earlier camera/matrix setup rather than changing the later quad math.

The early raw disassembly also shows the target saving the view getter result
once at `[ebp-0x7c]`, with the device in `EDI` and camera in `ESI`; the current
candidate copies the getter result into two slots, starts with the device in
`ESI`, then reloads the device argument for `DrawQuad`. A ViewMat-only source
form reduced the frame to `0x204` and moved the quad array to the target's
`[ebp-0x1ac]`, but still increased the byte residual to 1,638. Frame-size
alignment by itself therefore does not reproduce the target instruction flow.
Marking the ViewMat pointer itself volatile also forced it into stack storage,
but produced a 2,111-byte body with 1,815 differing bytes and did not preserve
the target's device-register allocation.
Adding a `register RenderDevice*` alias for the device and using it for the two
matrix calls and draw call kept the body at 2,113 bytes but increased the
residual from 891 to 895; the compiler still assigned the device to `ESI`.
Using ordinary `RenderDevice*` and `Camera*` aliases together also produced
2,113 bytes with 895 differences, so naming both parameters does not change
the allocation enough.
Changing the `view` alias from `const float*` to `const float (&)[16]` produced
identical object code and left the duplicated pointer slots unchanged.
Replacing the two matrix getter expressions with inline vtable calls through
`EDI` moved both calls to that register but produced a 2,111-byte body with
1,594 differing bytes. It also made the compiler save `ESI`/`EDI` before the
early-return checks, unlike the target, so the source-level probe was reverted.
Marking only the `float* view` alias volatile produced a 2,123-byte body with
1,667 differing bytes. This also failed to retain the device pointer in `EDI`.

## Rejected probes

- Replacing the `ViewMat*` plus `float*` aliases with a direct `float*` result
  shortened the candidate to 2,107 bytes and increased the residual to 1,638
  differing bytes.
- Introducing a `Camera*` alias used throughout the function retained the
  2,113-byte body but increased the residual to 899 bytes.
- Caching `right`, `up`, and `forward` pointers at the top of the function,
  following sibling renderers, grew the candidate to 2,150 bytes and increased
  the residual to 1,738 bytes.
- Moving the `scale`, `nearZ`, and `farZ` declarations ahead of the device
  matrix calls, while keeping their assignments in place, produced no byte
  change.
- Moving all but the initial `u` assignment and the remaining vertex
  initialization until after `GetViewMatrix()` and `GetMatrixB()` shortened the
  candidate to 2,107 bytes and increased the residual to 1,627 bytes.
- Replacing the `device` parameter's uses with an initialized
  `RenderDevice*` alias retained the 2,113-byte candidate and increased the
  residual from 891 to 895 bytes.
- Grouping the first lens-translation sum in a named `float tx` intermediate
  grew the body to 2,115 bytes and increased the residual to 1,748 bytes. The
  target's x87 product order is now documented as a source-shape clue, but this
  local-based encoding does not reproduce its schedule.
- Keeping only a `ViewMat*`, indexing through `vm->m`, and passing `vm` to the
  transform macro (the matrix member is at offset zero) produced a 2,107-byte
  candidate with a 1,638-byte residual. This removed the extra four frame
  bytes and aligned the quad array with the target, but the broader instruction
  sequence diverged more; passing `vm->m` itself to the macro is rejected by
  VC6's assembler.
- Ending `pt`'s `Vec3` lifetime immediately after the second transform kept the
  body at 2,113 bytes but increased the residual from 891 to 917 bytes.
- Initializing the copied particle position as `Vec3 pc(*pos)` instead of
  default-constructing `pc` and assigning its components kept the body at 2,113
  bytes but increased the residual from 891 to 906 bytes.
- Replacing the function-wide `ViewMat cam` object with `float cam[16]` while
  casting the storage at the inverse/copy operations kept the body at 2,113
  bytes but increased the residual from 891 to 895 bytes.
- Reversing the first two `lensRot` assignments to match the target's first
  two load order produced identical object code; VC6 scheduled the loads the
  same way.
- Reversing the squared terms in `len2` from `dy * dy + dx * dx` to
  `dx * dx + dy * dy` also produced identical object code.
- Caching only the camera `right` and `up` pointers before the matrix calls
  grew the body to 2,115 bytes and increased the residual to 1,704 bytes.

All source probes were reverted. The current source remains the best tested
candidate; these results do not establish an exact match.
