# Keyboard hook lifetime

The first `T_56b0bc` instance clears two 256-byte key-state arrays, installs a
thread-local `WH_KEYBOARD` hook, and resets the adjacent hook state. Every
instance increments `DAT_0057bc60`; the last destructor unhooks and clears
`DAT_0057bc64`. These behaviors and the failure text are observed in Evidence
`ev_7aa9067319519edcc15870c8abb12a16d5699c1b9ebbd804f2d1d1f75cf3a532` and
`ev_beba8be5952c3005da00f2f5046a6b59524086df944e4b57299888583317aa65`.

`T_56b0bc::~T_56b0bc` at `0x4b6f60` matches all 35 bytes. The constructor at
`0x4b6e10` is a behavior reconstruction in `src/input.cpp` and remains
`NONMATCHING` (127 vs. 332 bytes). The hook callback at `0x4b6f90` and the
original exception/string construction are still unresolved. Evidence bundle:
`evidence-2026-10-22-input-lifetime.json`.
