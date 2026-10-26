# Range 0x4b7a40-0x4bbdef module map (observation from pseudocode + header grep; names are reconstructions)

| Range | Role | Vtable |
|---|---|---|
| 0x4b7a40-0x4b8340 | DrawContext (0x194 bytes, session +0x0c): ctor 7a40 (string "Scene"), dtor 7bf0, stubs 7bd0/7c50/7c60, FUN_4b7c80 (1219B, draw/setup w/ RaceDeviceSlots+ViewManager), 8160, 82f0, 8310, 8340 (reset +0x10 of entries) | 0x4e29ec |
| 0x4b83a0-0x4b8b10 | Scene entry list / nodes: 83a0 (476B), 8580, 8640, 8680 (871B), 89f0; call 4bd000 (CRT-ish alloc helper) and 4baf90 | none seen |
| 0x4b8b10-0x4b8f70 | Float helpers: 8b10 (matrix/vec store), 8c30 (529B, sqrt, 6-float box/sphere), 8e50/8ee0/8f70 small | none |
| 0x4b90b0-0x4b9b50 | Mesh container / deformation model: 9170, 91c0 (880B, throws logic_error "deformation_and_visual_model_b..." / "deformation_model_has_invalid_..."), 9530 Repair, 95b0 Refresh (479B), 9790 Find, 9830, 99c0 Save, 9a60, 9b20 ~MeshHandleList, 9b50 (469B) | std::logic_error 0x4e2c78 (thrown) |
| 0x4b9d30 | RenderBody::Update (1750B, fpatan; one large function) | - |
| 0x4ba440-0x4ba600 | Shadow/poly object hierarchy: ctor 4ba440 (vtbl 4e2a28 base), 4ba4a0, 4ba580 (vtbl 4e2a4c), 4ba5a0 ShadowCaster ctor (0x178B, vtbl 4e2a70), 4ba5f0 dtor/reset, 4ba600 | 0x4e2a28, 0x4e2a4c, 0x4e2a70 |
| 0x4ba620-0x4bad30 | Frustum/skid: Init(RenderDevice) 4ba620 (523B), TestSphere 4ba830, 4ba9d0, 4bab80, 4bad30 (475B) | none |
| 0x4baf40-0x4bb130 | DynamicMesh (0x38B, 0x2c-byte verts): ctor baf40, baf70, dtor baf90, bafe0 (286B) | none |
| 0x4bb130 | StageAnimator base ctor | 0x4e1fb4 (slot0 dtor 0x46fe90, slot1 pure) |
| 0x4bb170-0x4bb260 | Small: bb170 (167B vec math; exact PE body preserves original x87 operand order), bb230/bb240 stubs (call 490ad0 / 4c0d01 = CRT) | none |
| 0x4bb260-0x4bbdef | SkyWorld ray tracing (cdecl): TraceRay bb260, bb3a0 (1635B), bba10, bbb30 (646B); callees 4b3470, 4a6800 | none |

Skip candidates: none clearly third-party; 4bd000/4c1116/4c11f4/4c1276/4c0d01 are outside range (CRT/new/delete). Math: 4b9d30 fpatan, 4b8c30 sqrt are game code.
Static-init/atexit stubs: not found in range.

## Work groups
1. 0x4b7a40-0x4b7c60 DrawContext ctor/dtor/stubs (6 fns)
2. 0x4b7c80 (1219B) + 0x4b8160 (396B)
3. 0x4b82f0-0x4b8640 (8 small/medium fns incl 83a0, 8580)
4. 0x4b8680 (871B) + 0x4b89f0
5. 0x4b8b10-0x4b9170 (8 fns floats helpers)
6. 0x4b91c0-0x4b9830 (mesh container, 91c0/9530/95b0/9790/9830)
7. 0x4b99c0-0x4b9b50 (4 fns)
8. 0x4b9d30 (1750B single)
9. 0x4ba440-0x4ba620 (ShadowCaster hierarchy, 7 fns)
10. 0x4ba620-0x4bad30 (Frustum/skid, 5 fns)
11. 0x4baf40-0x4bb260 (DynamicMesh, StageAnimator, small, 9 fns)
12. 0x4bb3a0 (1635B single)
13. 0x4bb260, 0x4bba10, 0x4bbb30 (ray trace)
