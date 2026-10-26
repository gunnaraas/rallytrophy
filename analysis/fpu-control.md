# FPU control-word normalizer

`FUN_004a7d30` reads the x87 control word. Unless the exception masks are all
set, precision control is 24-bit, and rounding is round-to-nearest, it writes a
normalized control word with those settings. On the first call it saves the
original word at `DAT_00577bc0` and sets the initialized flag at
`DAT_00577bc4`. These behaviors follow the observed instructions and
pseudocode (Evidence
`ev_f22d7a46ba6f3c0281c6d613e487e8d21196dca987b25d7a15e3a34a856e5e1f`).

The implementation in `src/winmain.cpp` uses a narrow inline-assembly body to
preserve the observed x87 instruction order and matches all 87 bytes.
