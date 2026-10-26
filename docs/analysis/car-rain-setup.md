# Car rain setup (`0x4560a0`)

The observed body is 769 bytes and returns with `ret 4`. It installs SEH
handler `0x4da6a2`, checks the car's rain-enable field, appends
`rain_drop.bmf` to the supplied directory, and tests for that file.
Evidence: `ev_046b6a95d3842d3fb060ae2ee4e5601ea23699ece8452983e9576767a3153def`.

When the file exists, the function constructs the model-file context, creates
the window-rain object (`0x4766d0`, count `0x80`) and body-rain object
(`0x46d830`, count `0x20`), initializes them, attaches them to the car body,
and assigns their geometry. It destroys the model-file child objects and
cleans up the path before returning. Evidence: the same function dossier.
The source names describe reconstructed roles.

## Exact source form

The inline path append now names its copy destination before `memcpy`:

```cpp
char* dest = ptr + len;
memcpy(dest, s, m);
```

This temporary reproduces the target pointer/count register allocation and
the later directory-string register choice. The focused matcher confirms
769 bytes and 31 COFF relocations, with all seven exact-marked functions in
the module passing (five other bodies remain `NONMATCHING`). Reversing the
pointer addition or naming the directory string alone retained the original
11-byte mismatch. These are compiler and matcher observations.

## Exception cleanup

The path owns its cleanup through a real destructor. `Free` is forced inline
to retain the target normal-exit instruction layout; explicit `path.Free()`
calls were removed. A draft with an empty destructor matched the main body
but leaked the path during exceptions, so it was rejected before committing.

Independent COFF/PE inspection confirms all 31 relocation identities and the
four-state unwind table. Its path cleanup now calls a destructor matching
the original string destructor (`0x430720`): 61 bytes and one relocation to
operator delete (`0x4bd000`). Model-file destruction and allocation-failure
cleanup also match. Normal returns and exception unwinding each destroy the
path exactly once. These are local artifact observations.

Local verification limitation: the relocation-copy helper stops on an
unresolved `__except_list` symbol, so it does not produce a copy for this
function. Object comparison masks relocation operands; the independent
COFF/PE audit checks their identities and exception-cleanup semantics.
Runtime execution was not tested.
