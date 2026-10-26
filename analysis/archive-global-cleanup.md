# Archive global cleanup registration

`FUN_004a4c80` registers `FUN_004a4c90` through `FUN_004c0d01`; the wrapper
matches the shipped 12-byte function (Evidence
`ev_5c8367c42ae0a40962262a0e6b4f6d238353e312a563791c20de5ebbde6d799f`).
`FUN_004c0d01` delegates to the registration helper at `0x4c0c83` and
normalizes its result (Evidence
`ev_a8f476ac63b2d8c8fa45f9d7b326a9636287f9edde0b7b4300e1db91e83eae70`).

Plain byte observation of the callback at `0x4a4c90`: it passes the vector's
begin pointer to `operator delete`, then clears the begin, end, and capacity
fields. This callback body is represented in `src/file_archive.cpp` and
`analysis/functions_extra.csv`; its 32-byte body matches. Evidence bundle:
`evidence-2026-10-22-archive-cleanup.json`.
