# `LoadScreen::Init` (`0x4033f0`)

`PathText` reproduces the target's string helper boundaries in the two texture
paths. The remaining four differing bytes are in the final quad initialization
sequence. The PE stores `colorA`, pops `edi`, stores `wrapV`, then stores
`colorB`. With ordinary field assignments, VC6 moved both wrap stores before
the color stores. Making only the `wrapV` write volatile preserves the target
ordering through `colorA` and reduces the focused diff from 31 to four bytes;
the candidate still emits the `wrapV` store immediately before `pop edi`, while
the target pops `edi` first. Making both `wrapU` and `wrapV` volatile improved
only to 29 differing bytes, so `wrapU` remains ordinary.
Removing the local `SpriteA* q` alias and accessing `quad` directly worsened
the diff to 38 bytes; restoring the alias returned it to four. Accessing only
`quad->wrapV` directly emitted the same code as `q->wrapV`.

Verification: `python3 -I tools/match.py --quiet src/unk_403270.cpp` reports
7 exact and 2 NONMATCHING functions, with no DIFF or compile failure. The
remaining `LoadScreen::Init` difference is a compiler scheduling permutation.
Disassembly observations are from the local PE/object files, without REA
Evidence IDs.
