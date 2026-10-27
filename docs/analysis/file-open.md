# File open path (Patch 1.01)

Target: `File::Open`, original `FUN_004a4f50` at `0x4a4f50` (1247 bytes).
REA pseudocode evidence:
`ev_99c9e6265233a208ee2d60d17289d27174e62b68f151a2d9bd0b01833ecdd22b`.
These addresses belong to Patch 1.01.

The function copies the requested name into File's string at +0x4, sets the
mode flag at +0x16 when bit `0x100` is present, clears the byte at +0x14, and
tries the archive path through `FUN_004a5430` first. If that helper does not
open the entry, Open constructs a legacy `fstream` and stores its pointer at
+0. A stream whose fail/bad bits (`flags & 6`) are clear is accepted. Unless
mode bit `2` is present, it obtains the file size and stores it in both the
logical-size field +0x18 and raw-size field +0x24. With bit `2`, both sizes
are set to zero.

If opening the disk file fails, Open closes the partially initialized stream
and throws `file_error` with message `Cannot open file` and the requested
filename. After either the archive or disk path succeeds, it sets +0x14,
reads `DAT_0056bbb8` bytes into a 256-byte local buffer, and compares them to
the copyright prefix at `0x501ce4`. A matching prefix returns success. A
mismatch throws `file_error` with message `Invalid file` and the original
filename. These messages and branches are observed in the pseudocode record.

The archive helper's return convention is observed at the call site: a
nonzero result skips disk stream construction and proceeds to the shared
prefix check. The role of bit `0x100` is only partly established here; the
function sets +0x16 when present and does not visibly clear it when absent.
Do not simplify that state transition without inspecting other callers or
the helper.

The current reconstructed `File` offsets are in `include/rallytrophy.h`.
The pseudocode has extensive stack and exception artifacts, so this note is a
behavior guide rather than source to copy. Object-code verification must still
be done against the 1247-byte function.
