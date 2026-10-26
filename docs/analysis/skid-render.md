# Skid-mark rendering

## `SkidMarks::Render` (`0x474a80`)

The observed body covers `0x474a80`–`0x474c10` (401 bytes), accepts `this`
in `ecx`, and returns with `ret 8`. It saves the previous `fs:[0]` value and
installs SEH handler `0x4dc562`. Evidence:
`ev_891367692d4806af0183bd305b094926435ef0d1cedc9f3f8ca0eb406ea0a6a6`.

The function skips rendering when quality is zero. Otherwise it bounds the
segment count by capacity, fetches the device transform at vtable offset
`0xe4`, constructs two arrays of six 16-byte plane objects, and initializes
the frustum helper (`0x4ba620`). Each segment advances by `0xc8` bytes.
Visibility testing (`0x4ba9d0`) precedes submission through device vtable
offset `0x114`. Evidence: the same function dossier.

Depth is accumulated as `m[6]*y + m[2]*x + m[10]*z + m[14]`, in that
x87 evaluation order. The original C++ draft compiled to 401 bytes but
evaluated y, z, x, leaving six differing bytes. Local experiments with
sequential accumulation and volatile coefficient reads retained or expanded
the mismatch. These are compiler observations.

The source now transcribes the bounded instruction body with symbolic
references, preserving the SEH frame, unwind states, array cleanup calls,
and x87 order. Local checks match 401 bytes and 13 COFF relocations; all
10 exact-marked functions in the module still match. The separate relocation
copy check reproduces the original bytes (1/1), with zero unresolved symbols.
Runtime execution was not tested.

Independent COFF/PE audit confirms seven absolute relocations (one handler,
four destructor references, two constructor references) and six direct-call
relocations (two array constructors, frustum initialization, visibility test,
and two array destructors). All seven absolute operand sites correspond to
PE base relocations and name the original targets. The original handler's
EHInfo (`0x4eba88`) and four-state unwind map (`0x4ebaa8`) remain applicable
to the identical stack frame and state transitions. Both quality paths
balance register saves and restore `fs:[0]`. These are local artifact
observations from the independent audit.
