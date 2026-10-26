# `Drivetrain::SelectGear` (`0x46b020`)

Ghidra assembly evidence: `ev_5d15d3de1a5c28329d00bef133ea87bbcdd511aa9f06b6b13b5612fa2f4e3555`.

The downshift loop computes the candidate lower gear's speed using `ratio[g]`, so the corresponding upshift threshold is `shiftUp[g]`. The class initializes gear 1's threshold at index 2; when the current gear is 2 (`g == 2`), this candidate is gear 1 and must be checked against index 2. The original instruction reads `[edx+0x80]`, resolving to `shiftUp[g]`; the prior source used `shiftUp[g + 1]` and emitted `[edx+0x84]`. Changed that one source index.

Focused matcher: `python3 -I tools/match.py src/unk_46a8a0.cpp` reports 22/22 marked functions matched except `SelectGear`; its body remains 376 bytes with 25 differing bytes outside relocations (previously 26). The index correction fixes the semantic offset mismatch. Remaining differences are localized to the initial velocity/dot setup: VC6 moves the `velocity[1]` stack store and the forward-vector `lea` around the x87 load/duplicate sequence compared with the target.

Tried a volatile intermediate for the y component, a volatile store directly to `v.y`, and the `Vec3` three-argument constructor. These produced 384/306, 376/29, and 404/315 (built bytes/differing bytes), respectively, or failed to compile for aggregate initialization. All trials were discarded; the source is restored to the 376-byte/25-difference baseline. No further source-only lifetime adjustment was justified by these results.
