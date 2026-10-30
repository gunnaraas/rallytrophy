# CarLight::Render matching notes

Target function: `CarLight::Render` at `0x46ea20` (1124 bytes). The function
renders a light glow quad after camera, facing, alpha, and depth checks. REA
behavior evidence: `ev_7eecfed6d4d5d346d067eaafd9b55bf5a74e442547caa69d7c58fdb88e6ea0ea`;
full target instruction evidence: `ev_e1ce979294f864047c7e0db8364ab17034d93f7b7e3c58e7bc90161e067a1596`.

## Compiler-order probes

Plain matcher observations from 2026-10-27:

| Source form | Built bytes | Differing bytes |
|---|---:|---:|
| Baseline before local-order probes | 1091 | 690 |
| Load near plane before the forward dot product | 1097 | 483 |
| `Dot(-v, Forward())` | 1097 | 483 |
| `Dot(Forward(), -v)` | 1097 | 477 |
| Constructor-based camera-relative vector assignment | 1113 | 709 |
| Local `lightSize` for all six axis components | 1088 | 491 |
| `volatile` alpha | 1097 | 477 |
| Named volatile `alphaScaled` temporary before `FloatToInt` | 1101 | 614 |
| Local stack copies of depth near/far bits | 1097 | 477 |
| Local `Vec3` construction for quad corner 0 | 1097 | 483 |
| `!(forwardDistance > nearPlane)` guard | 1095 | 626 |
| `!(nearPlane < forwardDistance)` guard | 1097 | 477 |
| Positive branch `if (forwardDistance > nearPlane) { ... } else return` | 1095 | 626 |
| Scale `Right().x` before copying `Position()` | 1089 | 634 |
| Positive depth interval block (`near < depth && depth < far`) | 1095 | 626 |

The retained facing expression is `Dot(Forward(), -v)`. In the target
disassembly, the facing-vector load precedes its corresponding multiply; this
source order reduces the focused `objcmp` difference by six bytes. It remains
nonmatching. The current focused matcher reports 10/10 marked functions in
`src/unk_46e880.cpp`, with `CarLight::Render` the sole `NONMATCHING` function.

The current near-plane source uses a positive success branch,
`if (forwardDistance > nearPlane) { ... } else return;`. It compiles to the
target's `TEST AH, 0x41`, which rejects less-than, equal, and unordered results.
The shorter `!(nearPlane < forwardDistance)` candidate emitted `TEST AH, 1`,
which does not reject unordered results. The target-compatible status-mask
sequence is currently 1095 bytes with 626 differing bytes; matching the guard
alone does not resolve the larger stack, alpha, and quad-construction gaps.

The current candidate is 1,101 bytes with 614 differing bytes after a volatile
`alphaScaled` local forced the target-like float spill before integer
conversion (the prior inline expression was 1,095 bytes with 626 differences).
Wrapping the converted result in a volatile integer `rawAlpha` instead adds
stores and loads around the inline `FISTP`, worsening the comparison to 1,107
bytes / 638 differences; that probe was reverted. The target's `FISTP` writes
directly to its local stack home (REA instruction evidence
`ev_dd1aeecb4a4dd2083e5c24e9edfde14f91d90fa80ac91297dfaf6d57b3530c9a`).
The remaining diff includes stack-frame and local-slot differences, FPU
comparison/branch differences, alpha storage, and quad-corner construction.
These are open matching work; this note does not claim semantic similarity is
byte-exact completion.

The depth range is now written as the positive conjunction shown in Ghidra's
behavior reference: `nearBits < depthBits && depthBits < farBits`
(`ev_9ef21be9f3cea54b065dd44ff3f09d749dfb8c3a8c96e423cb24687e1114394c`).
Wrapping quad construction in that success branch, with and without an explicit
`else return`, leaves the focused result unchanged at 1095 bytes and 626
differences; the target's near/depth argument-slot reuse is still unresolved.

Ghidra's behavior reference `ev_9ef21be9f3cea54b065dd44ff3f09d749dfb8c3a8c96e423cb24687e1114394c`
places the scaled `Right().x` component before the Position components in its
local setup. Swapping only the `Right` and `Position` declarations did not
change emitted bytes; moving the `Right().x` multiply before the Position copy
produced 1089 bytes with 634 differences, so that scheduling probe was
reverted.

Using `Dot(v, camera->forward) > nearPlane` directly for the near-plane test,
instead of storing the expanded component sum in `forwardDistance`, left the
candidate at 1,095 bytes with 626 differences. This source-shape probe was
reverted.
