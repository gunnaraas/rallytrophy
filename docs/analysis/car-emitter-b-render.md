# CarEmitterB::Render (`0x479530`)

`CarEmitterB::Render` at `0x479530` is in
[`src/unk_479170.cpp`](../../src/unk_479170.cpp), in the particle module mapped
by [`analysis/range-4741a0.md`](../../analysis/range-4741a0.md).

REA evidence `ev_5b4d48f9ab52efad57ec064c262bdb82f9becfd1de97d3481bb7a5c1be9901b1`
records the 1,216-byte target body from `0x479530` through `0x4799ef`. For the
second and third quad vectors, the target loads particle size from
`[ESI+0x30]` before multiplying by the sine-table value at `0x4796fc` and
`0x479752`; the products are then passed to `Vec3::operator*(float)` at
`0x40b0c0`.

## Match result

The focused matcher reports an exact 1,216-byte match with 27 relocations.
The source uses a separate local copy of `p->size` for each sine product:
`sineSizeA` for the second quad vector and `sineSizeB` for the third. This
reproduces the target's `FLD [size]; FMUL [sin]` order at both sites.

An initial `sineSizeA` local reduced the residual from seven bytes to four; the
second local closed the remaining hunk. Earlier shared-temporary, volatile
size, and product-temporary probes had been discarded because they increased
the diff substantially.
