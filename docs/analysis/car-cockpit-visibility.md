# Car cockpit visibility switch

## Observed function

`Car::FUN_004691e0` at `0x4691e0` is 442 bytes. It receives the Car
in ECX and a four-byte requested mode on the stack, preserves ESI/EDI,
and returns with `ret 4`. Equal current and requested modes return before
any updates. Otherwise it performs the following sequence:

1. Attach the nonnull nodes at `+0x2018` and `+0x201c` to the body at `+0x24`.
2. Detach the current nodes at `+0x2048` and `+0x200c` when nonnull.
3. Choose replacements for `+0x200c` and `+0x2048`. A nonzero mode prefers
   `+0x2014` and `+0x2050` when available; otherwise use `+0x2010` and
   `+0x204c`. A null selection leaves the corresponding current slot intact.
   Attach the resulting nonnull nodes to the body and store the mode at
   `+0x1eb4`.
4. For a nonzero mode, set bit 0 in bytes at `+0x210c`, `+0x2238`,
   `+0x2364`, `+0x2494`, and `+0x25f4`; copy the three 32-bit values at
   `+0x2084/+0x208c/+0x2094` to `+0x2074/+0x2078/+0x207c`.
5. For zero mode, first set bit 0 at node offset `+0xc` for the nonnull
   slots `+0x2054/+0x2058`. Clear bit 0 in the five bytes above, and copy
   the alternate values at `+0x2080/+0x2088/+0x2090` to the same destinations.
6. Set bit 1 for a nonzero mode, or clear it for zero, in four bytes starting
   at Car `+0x34` with stride `0x330`.

Evidence: `ev_1d991568c7a83c95b1d5c586c6965d6bdba8cbe772e61940c40ab59c58b2061c`,
Ghidra assembly for the complete inclusive range `0x4691e0`–`0x469399`
in official executable SHA-256
`2d5b2632a6467edb31eb43e8decf237d47e11d62e9b6f37bf8d528e4ad0d83b7`.
The cockpit, body, and wheel labels are interpretations from the existing
Car layout; the field accesses and control flow are binary observations.
This is static evidence, not observed rendering behavior.

## Reconstruction and linkage

The instruction transcription preserves the six direct byte-memory OR/AND
operations at `0x4692b6/0x4692bc/0x4692c2` and
`0x46933e/0x469344/0x46934a`. Ordinary byte accesses, volatile accesses,
typed node views, and bitfield accesses produced separate load/modify/store
instructions in isolated VC6 trials. Small inline assembly regions introduced
an EBP frame, leaving four extra bytes even when the region itself matched.

The two call aliases point to the reconstructed `SceneNode::AttachTo`
(`0x4b2b30`) and `SceneNode::Detach` (`0x4b2b90`) definitions. Local source
and COFF symbol inspection establishes these spellings:
`?AttachTo@SceneNode@@QAEXPAV1@@Z` and `?Detach@SceneNode@@QAEXXZ`.
The earlier local `ModelPart::SetParent` / `Show` names described these same
call targets. See [scene-object.md](scene-object.md) for their reconstruction.

Focused matcher: 5/5 marked functions exact, two existing NONMATCHING,
no differences or compile failures. The new body matches 442 bytes with
six object relocations. A full linked image remains outside this object
comparison result.

Full matcher after this change: 1,753/1,753 marked functions exact,
331 NONMATCHING, no differences or compile failures.

## Remaining neighboring visibility-list mismatch

`Car::FUN_00468c70` remains NONMATCHING. Its 1,392-byte target is covered
by Evidence
`ev_83c63b2fb79a891b5a49387768b9cd42f88d03de950a0fe92f4ca2dbc114cf14`.
Local object comparison finds a 1,390-byte build: the target tests
`+0x1c738` with `jne` followed by an unconditional jump, whereas the build
uses a single inverse conditional jump. Near the array-growth tail, the
target pushes the old allocation after `rep movsd`; the build pushes it
before. These differences shift later branch operands.

Isolated source trials replacing the nested condition with a one-case
switch, or deleting through `list->data` instead of its cached `old`
pointer, reproduced the same mismatches. Neither trial changed the
repository source. A different branch/scheduling constraint is still needed.
