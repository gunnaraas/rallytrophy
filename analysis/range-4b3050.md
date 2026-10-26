# Range 0x4b3050-0x4b44df module map

Observed from Ghidra pseudocode, inventory and existing declarations/callers; descriptive names are reconstructions. Six unmarked functions. Neighbours: scene-object base ends at 0x4b304f; decompiled bounding-sphere geometry starts at 0x4b44e0.

| Range | Role | Vtable |
|---|---|---|
| 0x4b3050-0x4b346f | Scene-object light-influence mask query (inferred): DrawContext list, layer masks, sphere/cone tests | None referenced |
| 0x4b3470-0x4b44df | Triangle/segment geometry and swept-sphere collision helpers | None referenced |

## Functions

| Address | Bytes | Role |
|---|---:|---|
| 0x4b3050 | 997 | Build per-object influence bitmask from DrawContext (`DAT_00581de4`) list +0x160..+0x164; filter +8 masks, type +0x124 and sphere/cone bounds; object flag +0xc selects bounds representation. |
| 0x4b3470 | 902 | Compute three triangle interpolation/barycentric weights for a point; used by SkyWorld ray tracing (0x4bb260) and sun geometry (0x45dac0). |
| 0x4b3800 | 1473 | Closest point on triangle represented by origin and two edge vectors; return squared distance (absolute value guards roundoff); collision callers in unk_418570.cpp. |
| 0x4b3dd0 | 267 | Closest point on finite segment `a + t*dir`, clamping t to [0,1]; collision callers in unk_49b8e0.cpp. |
| 0x4b3ee0 | 429 | Point-in-triangle test by dominant-normal-axis projection; boolean result in low byte; used by ray tracing and swept collision. |
| 0x4b4090 | 1029 | Swept sphere versus finite edge, including endpoint fallback; solve quadratics, update earliest time and contact point; callers in unk_418b90.cpp. |

Skip candidates: no confirmed CRT/third-party functions or static-init stubs. 0x4b3800 resembles a published region-based point/triangle distance algorithm; provenance unconfirmed, retain for drafting. MSVC EH scaffolding and SQRT do not make these CRT routines. No identifying in-range string references.

## Work groups

1. Scene influence query: 0x4b3050.
2. Triangle weights and containment: 0x4b3470, 0x4b3ee0.
3. Closest-point helpers: 0x4b3800, 0x4b3dd0.
4. Swept edge collision: 0x4b4090.
