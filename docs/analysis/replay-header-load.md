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
