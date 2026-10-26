# WinMain startup helper (1.01)

`0x4a17b0` returns immediately. Ghidra pseudocode and the assembly evidence
show no instructions beyond `RET`; no additional purpose is inferred.
Evidence: `ev_93ffd39fa5c0ebd0a128034a848fcebb9b7215e7f951ceb45c0bc24f5055dc68`,
`ev_49873fac4d43975b3306764fba9078e2d2cc7113517defca195fd7daf764fbb8`.
Plain validation observation: `objcmp.py` matches the single byte against the
official executable.

## Adapter array initializer

`0x4a1a70` initializes the adapter array at `0x56b968`: it sets the count to
zero, capacity and growth step to 64, allocates `0x500` bytes for 64
20-byte `AdapterEntry` records, then sets the end pointer to the data pointer.
Evidence: `ev_942d31924723ccebe99c315685ab76c54926e31a209a6ac640640239518e1612`,
`ev_1580b65671a28a86c28db6309476bb806a6e948ebfc0ea3dabb5b619ec91ccf5`.
Plain validation observation: `objcmp.py` matches all 49 bytes.

## ECX setup thunk (`0x4a1770`)

`0x4a1770` loads `0x56aeb0` into ECX and tail-jumps to `0x403190`. Ghidra's
pseudocode represents this as a call with the address of `DAT_0056aeb0` and
return; the instruction listing reports the jump as a terminal call, so that
control-flow annotation does not establish why the thunk has no fallthrough.
Evidence: `ev_ce9a72bdb9005f0600c2c8e45de95da01123c4ffb8eeca6e92b95ad536dcfcca`,
`ev_4ff223d8abae268bbf4058693c949ce035fb8d4470a754ee5f4f8a08535c5f9a`.
Plain validation observation: `objcmp.py` matches all 10 bytes.

## Startup helper wrapper (`0x4a1780`)

`0x4a1780` pushes the address `0x4a1790`, calls `0x4c0d01`, removes the
argument with `POP ECX`, and returns. Ghidra pseudocode also represents the
address as the helper's argument. Evidence:
`ev_d6a7fb444704d9308c93132252549d7484e7f875afb1b586416a48398986c94e`,
`ev_a56e032efc69a58664dcd214f68d5baa791ab428211af44b8f485a40d41c4778`.
Plain validation observation: `objcmp.py` matches all 12 bytes.

## ECX setup thunk (`0x4a19f0`)

`0x4a19f0` loads `0x56b97d` into ECX and tail-jumps to `0x4bd4cf`.
Ghidra's pseudocode presents this as a call followed by return. Evidence:
`ev_72a3dba5fa4ae295e8ed32781d3ea6e19cd4119d87d6002bd05111d0d30f1e70`,
`ev_805f92c66f91fd54715a5b880d87673f512b3e91dcadffbc4ac8243ed97c5969`.
Plain validation observation: `objcmp.py` matches all 10 bytes.

## Startup helper wrappers

`0x4a1a00` pushes the address `0x4a1a10`, calls `0x4c0d01`, removes the
argument with `POP ECX`, and returns. Ghidra represents the call as passing
that address to the helper; no further purpose is inferred here. Evidence:
`ev_24d8b77539eb130902acfd20cb22d8158d7da7e1314600c2001300b57b7d8c63`,
`ev_0e511a2dd9c9280ccd66d0a56baf01de79b79b14d129763526c6cafbb2d009fe`.
Plain validation observation: `objcmp.py` matches all 12 bytes.

`0x4a1a40` repeats the wrapper pattern with address `0x4a1a50`:
it pushes that address, calls `0x4c0d01`, removes the argument with `POP ECX`,
and returns. Evidence:
`ev_84aadd4a7abd210b6fd4fd7c7a54f12dd3450b448c449a8165da35dd3cb45260`,
`ev_16705da0c2208ed102c5ae766913bd5d06591a41e96c1fe20ad3336e44d76681`.
Plain validation observation: `objcmp.py` matches all 12 bytes.

`0x4a1ab0` uses the same wrapper shape with address `0x4a1ac0`:
it pushes the address, calls `0x4c0d01`, removes the argument with `POP ECX`,
and returns. Evidence:
`ev_13460f0f2eb5712e94aeab3e133e903a2739bf0e36be7bace3a06f8679ffeb3d`,
`ev_475454ae40515b66f3518d69b96ed2ec9ccdad6f0d1ad3436c9accb00c38ad73`.
Plain validation observation: `objcmp.py` matches all 12 bytes.

## ECX setup thunk (`0x4a1a30`)

`0x4a1a30` loads `0x56b97c` into ECX and tail-jumps to `0x4be3a2`. Evidence:
`ev_042ff4a73f6f3a7b1243fa22fcadce826d3671a9b05b5e0a46a0048980af5e5c`,
`ev_9be796bcbce2ce3fad25d935448b308f9dc8f8e15cb38b600fbb975268e76737`.
Plain validation observation: `objcmp.py` matches all 10 bytes.

## WinMain static initializers (`0x4a1760`–`0x4a1810`)

Plain binary observations from the 1.01 executable identify three static
objects in this block. The first, at `0x56aeb0`, uses `0x4a1760` to call its
constructor thunk `0x4a1770` and then register its destructor thunk `0x4a1790`;
that thunk sets ECX and tail-jumps to `0x48e660`. The second is a trivial
static: `0x4a17a0` calls the one-byte constructor `0x4a17b0`, then registers
the one-byte destructor `0x4a17d0`. The third is the `ParticlePool` global at
`0x56ae90`: `0x4a17e0` calls constructor thunk `0x4a17f0`, which tail-jumps
to `0x478870`, then jumps to registration wrapper `0x4a1800`; that wrapper
registers destructor thunk `0x4a1810`, which tail-jumps to `0x4788c0`.

All inventoried functions in the block match exactly. The fixed relative
`E9` displacements in `0x4a1760`, `0x4a1790`, `0x4a17a0`, `0x4a17e0`,
`0x4a17f0`, and `0x4a1810` depend on their original addresses and target
functions. `0x4a1760`, `0x4a1790`, `0x4a17a0`, `0x4a17d0`, `0x4a17e0`,
and `0x4a1810` are listed in `functions_extra.csv` because they were absent
from Ghidra's function inventory but are reached by the initializer/atexit
references.
