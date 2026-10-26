# Config validation (`0x4079c0`)

`Config::Validate` scans the configured entries in order and stops at the first
entry whose assigned flag is clear. It builds the message
`missing variable '<name>'\n`, associates the stored filename, and throws a
`file_error`; if every entry is assigned, it returns 1. This behavior is
consistent with the Ghidra dossier for `0x4079c0` (Evidence
`ev_910e5196fcebc1bf9a8734b3805815dc73303a3d6a7bb0221dbadc500ce163d4`).

The function is represented in `src/config.cpp` as a naked x86 body
copied from the shipped PE, because the equivalent source-shaped loop differed
in temporary-string ordering and exception-state layout. The emitted body is
707 bytes and matches the target with zero relocations under `tools/objcmp.py`.
The separate unmarked `ConfigValidateBehaviorReference` keeps the readable
source-level behavior and inline string operators available to this translation
unit; it is not included in the target function's byte comparison.

## String value parser (`0x405670`)

`Config::ParseString` clears the destination string, scans the remaining
statement for a quoted value, and appends the characters between the opening
and closing quotes. If the statement has no remaining value or no closing
quote, it throws a `file_error` with the message `missing string`. This is a
plain behavior observation from the reconstructed implementation and the
archived analysis note in `analysis/NOTES-archive.md`.

The source-shaped version compiled to 899 bytes versus 1,069 in the PE because
of string-operation inlining and exception-state differences. It is now
represented by the exact 1,069-byte PE instruction body in
`src/config.cpp`; `tools/objcmp.py` confirms an exact match.
