# Mouse position helpers (1.01)

Target: extracted Patch 1.01 reference executable, SHA-256
`cc1718e21a83b7422cfd2b304a9fe43e4f6a6e7550389d9f20bf30530f19ec8d`.

- `FUN_004b6d00` (48 bytes) takes a pointer to two adjacent floats, converts
  y then x to integers for the cursor-position call, and copies the original
  float values into `0x57ba00` and `0x57ba04` afterwards. Assembly evidence:
  `ev_693a5388f8989722f0a50188c733416beab39d2e10d19f1e70bafc8e310022e5`.
  Identifying the indirect call as `SetCursorPos` is a plain observation from
  `tools/scratch_asm.py` import resolution against this reference executable.
- `FUN_004b6d40` (21 bytes) stores zero to both globals. Assembly evidence:
  `ev_7076c5e78db82c1472302bf49f13774a3617560461cd120842acce95b56f94b7`.
  Its role as the position-global initializer is an inference from those stores
  and its location among the module's initialization routines.

Implementation: `src/input_mouse.cpp`, using a module-local layout view
like the existing race-mode callers. Plain validation observation: both
functions compile byte-exactly against the official executable under the
repository's VC6 matcher (69 bytes total). This checks generated code;
cursor movement has not been exercised at runtime.
