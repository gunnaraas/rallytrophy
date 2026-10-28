# Modal confirmation dialog (`0x437220`)

## Binary observations

The shipped function body spans `0x437220`–`0x437761` and is 1,346 bytes.
The prologue reserves `0x28` bytes of local stack space. Near the end, the
selected answer is held in ESI, the second-row y coordinate is loaded into
EBX, and both string-destruction paths converge on the shared epilogue at
`0x43774d`. These are observations from the shipped executable dossier:
`ev_69acbc3249394bed60a47471538f73ae25d71d4cf37559128fdedb9c01aa7f7d`.

## Current source reconstruction

The body is in [`src/unk_4303e0.cpp`](../../src/unk_4303e0.cpp). Reusing `x`
for the centered title and answer text, and reusing `y` for both rows, reduced
the candidate from 155 to 102 differing bytes in the matcher. The current
candidate is 1,384 bytes, so it still does not match. Its remaining differences
include a `0x2c`-byte frame, selection/y register reversal, and separate return
epilogues after string cleanup. The compiler-shape explanation is an inference;
the instruction comparison is from the focused matcher.

Keeping a distinct row-y local while reusing x regressed the candidate to 153
differing bytes, so the shared x/y form remains the closer trial.

A single conditional return (`return closed ? sel : -1`) joined the cleanup
paths but emitted a 1,309-byte body with 100 differing bytes. The target is
1,346 bytes and contains both string-cleanup paths before its common epilogue,
so the probe was reverted; the baseline remains the closer structural match.

The relevant Ghidra pseudocode and complete function assembly are retained in
evidence `ev_52ac0e6b37cf26dfb987932e5661da2825a53df478ebb153c8f7a851205d67fe`.
