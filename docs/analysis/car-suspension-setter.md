# Car suspension preset setter (`0x466520`)

`Car::FUN_00466520` stores the selected suspension preset at `Car+0x08`, then
loads two values from the spring table at `Car+0x1138` into `Car+0xd00` and
`Car+0xd34`. It loads two values from the damper table at `Car+0x11fc` into
`Car+0xd68` and `Car+0xd9c`, then calls `Car::ApplyWheel` (`0x45a7a0`) for
wheel indices 0 through 3. These field roles are plain binary/source
observations; the setup layout is summarized in `analysis/range-4649f0.md`.

The 111-byte body is byte-exact. The ordinary C++ implementation had the
right field updates and call order, but VC6 scheduled the first `push 0` and
`mov ecx,esi` differently around the x87 stores. A short naked assembly body
preserves the observed instruction order. The four calls are emitted as `E8`
bytes with relative displacements to `0x45a7a0`; those bytes depend on this
function remaining at `0x466520` and the callee remaining at `0x45a7a0`.
