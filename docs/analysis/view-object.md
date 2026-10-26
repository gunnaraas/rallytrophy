# View object (1.01, `0x4b77b0`–`0x4b79ce`)

Implementation: `src/view_object.cpp`, with a module-local layout in
`include/view_object.h`. This is the view base used by the rear-view
mirror and the chase/replay camera; that relationship is a plain source
observation from `MirrorView` in `include/unk_481390.h` and the callers in
`src/unk_464ab0.cpp` and `src/unk_491fb0.cpp`.

## Constructor and lifetime

`FUN_004b77b0` calls the scene-object base constructor `0x4b2830`, sets the
type at +4 to 3, and installs vtable `0x4e29c8`. It initializes identity
matrices at +0x144 and +0x184, projection values near=1, far=1000,
field-of-view=`0x3fc90fdb` (approximately pi/2), zeroes +0x130/+0x134,
and sets the byte at +0x1c8 to 1. It does not initialize the intervening
saved-position fields or +0x1c4. Evidence:
`ev_da7144f06878c68e08b0b7ccfab9aa9d8259aca712a133d9e0360d379ef44396`.

`FUN_004b78e0` restores the view vtable and calls the base destructor at
`0x4b29c0`. Evidence:
`ev_bf30901ca6370466122b3cf9bdfc12ab7d8b4a14a0f00295b7d1e03b5c552257`.
The scalar deleting destructor at `0x4b78c0` calls it and conditionally
frees the object when argument bit 0 is set (plain assembly observation from
`tools/scratch_asm.py`).

Plain data observation: the nine entries at `0x4e29c8` are `0x4b78c0`,
`0x4b2c10`, `0x48e660`, `0x4b2d50`, `0x4b2fc0`, `0x4b3000`, `0x4031e0`,
`0x4b3010`, `0x4890c0`. The local base declares those nine slots; its remaining
layout is opaque. Compile-time size checks require 0x124-byte base and
0x1cc-byte derived object. Complete base behavior remains in another module.

## Horizontal basis update

`FUN_004b78f0` projects the negated row at +0x80 onto the horizontal plane,
storing x/z at +0x1a4/+0x1ac and y=0 at +0x1a8. It sets the up vector at
+0x194 to (0,1,0). If the projected vector's squared length is less than
the float threshold at `0x4e1398`, it uses the negated row at +0x70 instead.
Finally it computes up cross forward into +0x184. It does not normalize the
vectors or rewrite the matrix's fourth components. Evidence:
`ev_3c32666593e9016216c9b8ac2d2fef01a5fad174f4a0d6265b52b5252b3e751c`.

The cross product uses an inline x87 sequence matching other vector helpers
in this repository; its precise instruction order and frame are confirmed by
`ev_34d6dd3527509c12dc7550a1dab2d2ffa8a91b01612c4a2d5188b40762640c29`.
Plain byte observation: threshold bits are `0x3a83126f` (0.001f), read from
the reference executable. No square root or division occurs in this routine.

## Verification and limits

Plain validation observation: all four entries match the official executable
with `python3 -I tools/match.py --quiet src/view_object.cpp`: constructor
269 bytes, deleting destructor 30, destructor body 11, basis update 223
(533 bytes total). Source-order experiments changed only independent stores;
the final source preserves the observed order. Relocation fields are masked
by the existing matcher. This does not validate a linked executable, vtable
integration across layout views, or runtime camera behavior.

## Following module startup wrappers

`FUN_004b7a10` is a ten-byte wrapper that loads ECX with `0x57bda0` and
tail-jumps to `0x4bc970`. Assembly and pseudocode evidence:
`ev_ea2e1dc8aed1e15089456eb645c2459ff3fddc687b034b748ccd47e19d91fa29`.
Source observation: the target is the draw-queue constructor represented as
`DrawQueueX::DrawQueueX` in `src/unk_4bc680.cpp`; it is separate from the
preceding view-object methods.

`FUN_004b7a20` pushes `0x4b7a30`, calls `0x4c0d01`, pops ECX, and returns
(12 bytes). Assembly and pseudocode evidence:
`ev_d5cf5e337c7c8a5ecff88b2388e9a4388a6818b5dd6a809de71d71ba150ea8a3`.
The callback at `0x4b7a30` loads ECX with `0x57bda0` and jumps to
`0x4b85c0`. Its two decoded five-byte instructions establish a ten-byte
body even though Ghidra does not define a procedure there. Instruction evidence:
`ev_b8c93741b5c1abe41fc2a07e22fd84a6c1789651e5150ee789fefa105aa3bf62`
and `ev_10a86d08564873e47d56fa7ede088e0b4df10ced3b66ac61e3b8769e80b856db`.
The registration wrapper's data reference identifies this entry as the exit
callback. Its role as draw-queue cleanup is inferred from the shared object
address and its registration; the target's full cleanup behavior is not
established by these two instructions.

Matcher observation: all three wrappers in `src/view_static_init.cpp` match their
complete instruction sequences outside relocations. Their fixed original
addresses and address-named external entry points follow the other startup
wrappers; linked address placement and symbol resolution remain unverified.
