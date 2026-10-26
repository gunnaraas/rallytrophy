# Archive global initializer

`FUN_004a4c60` is a `__fastcall` initializer that copies the most significant
byte of its register argument into `DAT_0056bba8`, then clears the three
adjacent 32-bit fields at `DAT_0056bbac`, `DAT_0056bbb0`, and `DAT_0056bbb4`.
The same fields are read by `FUN_004a4cb0`, which treats the latter three as
begin, end, and capacity pointers for a vector of archive-cache pointers.
This interpretation follows the observed pointer arithmetic in its binary
body (Evidence `ev_145f9623f741c1a865180898e7e444b5f4d7be465a934338ebf696b7bb4cb77c`);
the first byte's role is not yet established (initializer behavior is observed
in Evidence `ev_fb20da71412974de83b32424d714d2f045a323ad51ea08f99c9c92c1b3318400`).

The initializer is implemented at `0x4a4c60` in `src/file_archive.cpp` and
matches the 29-byte shipped function. Evidence bundle:
`evidence-2026-10-22-archive-init.json`.
