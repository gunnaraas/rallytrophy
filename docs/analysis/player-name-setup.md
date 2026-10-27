# Player-name menu setup

`PlayerNameMenu::Setup` at `0x43dc70` is a 416-byte thiscall function with an
SEH frame and 12 x87 instructions. REA evidence
`ev_876a2734e4c2d619b1baad3e35e80609fe8494878e8d3049195e7dbfe5e97284`
confirms the target body and its 64-cell initialization loop. Its per-cell
coordinates use `col`, `col + 1`, `row`, and `row + 1`; each cell advances by
`0x1c` bytes. Ghidra pseudocode is a behavior reference, not original source.

The tracked candidate is 414 bytes and differs in 316 unrelocated bytes. The
target reuses a stack slot for integer-to-float conversions of `col` and
`row + 1`; the candidate keeps separate slots and reserves eight bytes in its
prologue instead of reusing the saved-ECX slot. This is an observed compiler
difference from the target assembly and matcher output.

Temporary source probes that introduced a named coordinate temporary,
scoped coordinate temporaries, or separate row counters did not improve the
candidate. One split-row-counter variant changed it to 413 bytes with 306
unrelocated differences. The tracked function body remains unchanged. Focused
matching of `src/unk_43d770.cpp` reports 14 exact functions, two
`NONMATCHING` entries (`Setup` and `Run`), and no compile failures.
