# Range 0x4b5140-0x4b6abf module map

Observed from Ghidra pseudocode, inventory and existing declarations/callers; names are reconstructions or local header aliases, not recovered original types. Eighteen unmarked functions. The NONMATCHING prose in unk_41b9e0.cpp mentions 0x4b5140 as a callee, not its own function marker; retain it. Neighbours: decompiled bounding-sphere geometry before this range; keyboard/mouse hooks start at 0x4b6ac0.

| Range | Role | Vtable |
|---|---|---|
| 0x4b5140-0x4b535f | Quaternion/matrix conversion | None referenced |
| 0x4b5360-0x4b591f | 4x4 transform construction, multiplication and vector transform; identity global initialization | None referenced |
| 0x4b5920-0x4b69ef | Tri3 (0x34 bytes): triangle overlap, coplanar tests and segment intersection | None referenced |
| 0x4b69f0-0x4b6abf | Win32 message pump / quit-state service | None referenced |

## Functions

| Address | Bytes | Role |
|---|---:|---|
| 0x4b5140 | 250 | Rotation matrix to quaternion (x,y,z,w): trace-positive and largest-diagonal branches; Mat4Src::Init alias in unk_41b9e0.h. |
| 0x4b5240 | 215 | Quaternion to matrix rotation block; writes only nine 3x3 entries, leaving other matrix fields untouched. |
| 0x4b5360 | 149 | Static identity-transform initializer for DAT_0057b980; calls 0x4b5430 with unit basis vectors. |
| 0x4b5430 | 94 | Construct homogeneous 4x4 matrix from three basis vectors, zero translation and set bottom-right 1. |
| 0x4b5490 | 173 | In-place full 4x4 matrix multiplication (Mat4::Mul); all four components per row. |
| 0x4b5540 | 145 | Affine composition into this from two matrices: rotate all four rows, add second translation; homogeneous column untouched. |
| 0x4b55e0 | 137 | In-place affine composition, translation included; known SceneTransform::Multiply / ViewMat::Concat aliases. |
| 0x4b5670 | 234 | Construct homogeneous rotation matrix from three Euler angles (fsin/fcos). |
| 0x4b5760 | 230 | Construct homogeneous axis-angle rotation matrix (Rodrigues form); known RotMatrix constructor alias. |
| 0x4b5850 | 149 | Transform Vec3 by matrix including translation, returning via hidden output pointer; Ghidra expands matrix-by-value into scalar parameters. |
| 0x4b5920 | 143 | Initialize Tri3 with three vertex pointers, two cached edges, normal and negated plane constant. |
| 0x4b59b0 | 1538 | Triangle/triangle overlap: plane-side rejection, intersection intervals along dominant axis, coplanar fallback. |
| 0x4b5fc0 | 377 | Coplanar overlap: select projection axes from normal, test edges then mutual point containment. |
| 0x4b6140 | 926 | Projected segment versus three edges of the second triangle; uses shared projection/vertex globals. |
| 0x4b64e0 | 226 | Projected point-in-triangle test using consistent edge-side products and tolerance. |
| 0x4b65d0 | 550 | Tri3 point containment using three oriented edge/normal cross products and float sign bits. |
| 0x4b6800 | 447 | Intersect finite segment with triangle plane and interior; output hit plus endpoint on positive plane side; return segment parameter, or 1 on failure. |
| 0x4b69f0 | 150 | Pump/translate/dispatch messages; optional wait/service callback; WM_QUIT clears DAT_0050243c and sets quit flag; widely called by UI/race loops. |

Skip candidates: no confirmed CRT/third-party code. 0x4b59b0/0x4b5fc0/0x4b6140/0x4b64e0 resemble the Moller triangle-overlap algorithm family, but attribution is unproven; retain pending provenance review. 0x4b5360 is an engine static initializer, not a generic CRT stub. No identifying in-range strings; unrelated executable copyright strings do not establish ownership.

Drafting notes: Tri3 coplanar helpers share DAT_0057b9c0/c4 projection axes and DAT_0057b9c8/cc/d0 vertices, so preserve their shared state. Local matrix aliases describe the same operation but do not prove a single original class. Preserve float evaluation order, low-byte booleans and MSVC6 calling conventions.

## Work groups

1. Quaternion conversion: 0x4b5140, 0x4b5240.
2. Identity/basis construction: 0x4b5360, 0x4b5430.
3. Matrix composition: 0x4b5490, 0x4b5540, 0x4b55e0.
4. Rotation/vector transforms: 0x4b5670, 0x4b5760, 0x4b5850.
5. Triangle initialization and main overlap: 0x4b5920, 0x4b59b0.
6. Coplanar overlap helpers (shared globals): 0x4b5fc0, 0x4b6140, 0x4b64e0.
7. Triangle containment and segment hit: 0x4b65d0, 0x4b6800.
8. Message pump: 0x4b69f0.
