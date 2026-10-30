# `ReplayHeader::Load` (`0x403ca0`)

The target builds the path with the `PathText` stand-in. Its successful return
uses inline string cleanup at `0x404084`; the missing-file and invalid-header
paths converge at `0x4040ca`, store cleanup state `-1`, and call `_Tidy(true)`
at `0x4040db`. These are plain local PE observations.

An explicit `name.Tidy(true)` before the candidate's final `return false`
introduced the target helper call but retained the automatic inline destructor
after it. The focused candidate grew from 1,133 to 1,144 bytes, while
differing bytes changed from 589 to 588 and instruction delta from 24 to 23.
That source edit was reverted. The remaining work is to model the target's
branch-specific cleanup and EH state without adding a second cleanup path.

A follow-up used a same-layout `PathTextManual` twin without an automatic
destructor, with explicit inline `Free(true)` on success and `Tidy(true)` on
failure. This avoided duplicate cleanup but produced a 1,115-byte candidate
with 749 differing bytes (baseline: 1,133 bytes / 589 differences); the frame
remained four bytes smaller than the target. The experiment was reverted.
Focused matching after restoration reports 7 marked functions matched, 2
NONMATCHING, and no DIFF or compile failures. This cleanup split alone does
not explain the target's frame and register allocation.

The current source writes the two race fields through `DAT_00501450` before
creating the reusable `race` alias. This direct-global form changes the
compiler's register schedule and improves the body from 1,133 bytes / 589
differing bytes to 1,132 bytes / 158 differing bytes (local `objcmp.py`
comparison). Moving the car-count load ahead of the final raw-header byte
store regressed to 1,134 bytes / 592 differences and was reverted. The direct
field-store form is retained; the cleanup/frame mismatch remains.

Making only those two member stores volatile to preserve their source order
kept the 1,132-byte body but increased the diff from 158 to 162 bytes. That
probe was reverted; it did not improve the remaining register/store schedule.
