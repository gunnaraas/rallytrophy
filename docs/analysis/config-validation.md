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

## `file_error` constructor (`0x409390`)

The target constructor is 357 bytes (Evidence
`ev_cefb6dea84ae9597d334b6aadf28dca48eb0b204db7439ff6c719ac0424627e5`). It
builds the base `runtime_error` message through the three string concatenation
calls at `0x4093c1`, `0x4093d9`, and `0x4093ee`, then copies the filename and
message into the two member strings at offsets `+0x1c` and `+0x2c`. The target
calls `0x4c12db` with the empty-string global to initialize its `std::exception`
base, then builds the `std::runtime_error` message string at `this+0xc`. The
callee dossier identifies `0x4c12db` as the `std::exception(const char*)`
constructor (Evidence `ev_4e8fffa6ca883708aa5fb814dfafb55e5bd361730270e2638442e9c68ac842b1`).
The target sets one cleanup state before its three concatenation-temporary
destructor calls (`0x409462`), without changing it between those calls.

The current inline constructor in `include/rallytrophy.h` emits 372 bytes and
differs in 112 bytes outside relocations. In particular, its compiler-generated
exception-state updates around those temporary destructors differ from the
target. This is still open; the matching behavior and message construction are
not evidence that the constructor is byte-exact.

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
