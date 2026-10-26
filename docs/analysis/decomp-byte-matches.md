# Byte-match observations

These notes record plain observations from the shipped PE disassembly and the
object-file matcher results for exact C++ reconstruction entries. They are
not Ghidra pseudocode or REA Evidence-ID claims.

## `Car::FUN_00458590` (`0x458590`)

The target is a 246-byte thiscall function with one `Car*` argument and an
SEH registration frame. Its callee-saved register assignment is EBP=`this`,
ESI=the current car's body, EDI=the other car's body, and EBX=the current
body's box pointer. It subtracts the two body positions and compares the
squared distance with the float at `0x4e13a0`; on the near path it calls
`0x4ad1d0` for box overlap, then `0x41d7d0` for body collision. If both tests
pass, it increments each car's hit counter at `+0x14f4`. The function returns
zero and restores the SEH chain and saved registers.

The complete 246-byte body is transcribed in
[`src/unk_4581f0.cpp`](../../src/unk_4581f0.cpp#L185).

## `Wheel::IntegrateSpin` (`0x46d5d0`)

The target is a 281-byte thiscall function with a four-byte `dt` argument.
It computes net torque, optionally includes the car drivetrain torque, and
updates wheel spin using x87 arithmetic. The target keeps the new spin value
on the x87 stack while comparing its magnitude against the friction term. On
the non-clamp path it loads the spin bits into EAX, executes `fxch st(1)`,
tests the sign bit `0x80000000`, then performs the x87 add or subtract. If the
friction term is larger, it stores zero instead.

The complete 281-byte body is transcribed in
[`src/unk_46c2c0.cpp`](../../src/unk_46c2c0.cpp#L574).

## `Car::AttachLights` (`0x4646c0`)

The target is a 305-byte fastcall method. It obtains mesh-list element zero
from `CarBody+0x124`, then visits three pairs of front-light records and two
sets of rear, brake and reverse records. Each active light is attached to the
body, its mesh is found using `light+0x14`, and its flag byte is cleared. The
front and rear records with target pointers also write their stored value
through those pointers. The original uses two loop counters in the saved-ECX
stack slot and record strides of `0x1c`.

The complete body is transcribed in
[`src/unk_45e480.cpp`](../../src/unk_45e480.cpp#L2447).
The focused translation-unit check and full matcher both pass; the latter
reports 1,534/1,534 marked functions exact, with 442 remaining marked
`NONMATCHING`.

## `Car::AttachStage` (`0x464530`)

The target is a 323-byte thiscall method with two stack arguments and an x87
frame. It stores the stage pointer, conditionally forwards a stage-specific
field, sets light intensity, binds the four skid marks, and copies wetness and
colour data into car subobjects. Its final wiring block stores the stage
values at `Car+0x247c`, `+0x2224`, `+0x2350`, `+0x25ac`, and `+0x270c`; the
`SetColor` call setup is interleaved with those stores in the target. This
block is transcribed instruction-for-instruction to preserve that ordering.

The complete body is in
[`src/unk_45e480.cpp`](../../src/unk_45e480.cpp#L2378).
Focused and full matchers pass; the full result is 1,535/1,535 marked functions
exact, with 441 remaining marked `NONMATCHING`.

## `Drivetrain::TorqueAtRpm` (`0x46ae70`)

The target is a 307-byte thiscall routine with an SEH frame and x87 arithmetic.
It scans the torque curve for an interval, uses linear interpolation at the
first interval and linear extrapolation at the end, and calls the spline helper
for interior intervals using a stack `Vec3Slot` with `t`, `t²`, and `t³`. It
then applies the car damage factor and maximum torque. The target stores the
spline powers and clears the x87 temporary before pushing the four control
torques; the 307-byte body is transcribed in
[`src/unk_46a8a0.cpp`](../../src/unk_46a8a0.cpp#L194).

## `FUN_004a1390` (`0x4a1390`)

The target is a 286-byte stdcall helper with two stack arguments. It enumerates
the eight min/max combinations of the other car's bounding box, stores them in
the obstacle record, transforms each corner by the car matrix, and adds the
car's position. The original uses a stack counter for the corner layer and an
x87 matrix transform loop; the complete body is transcribed in
[`src/unk_49fee0.cpp`](../../src/unk_49fee0.cpp#L536).

## `CarRain::SetBounds` (`0x46e1a0`)

The target is a 303-byte thiscall method with three stack arguments. It copies
the body bounds and flags, scales six bound floats in place by the constant at
`0x4e13e4`, and stores the body, trunk, and hood pointers. The x87 scaling
operations are interleaved with the integer copies; the complete body is
transcribed in
[`src/unk_46d830.cpp`](../../src/unk_46d830.cpp#L245).

## `MenuScreen2A8::FUN_0042a210` (`0x42a210`)

The target is a 165-byte thiscall method with one stack argument. It calls the
renderer capability query, loads its returned dword, masks to the low byte,
extracts bit 2, and stores that flag before updating render state `0xb`. The
complete body is in
[`src/unk_4295c0.cpp`](../../src/unk_4295c0.cpp#L189).

## `CarRain::Update` (`0x46e2d0`)

The target is a 341-byte thiscall method. It advances each rain-drop phase by
`dt / 0.25`; expired drops are wrapped with `floor`, assigned a new size and
random vertex, then pointed at the appropriate body, trunk or hood mesh. The
x87 phase update stores the new value both to the drop and to a stack temporary
before comparing it with one. The complete body is transcribed in
[`src/unk_46d830.cpp`](../../src/unk_46d830.cpp#L252).

## `WaterAnimator::WaterAnimator` (`0x46fa80`)

The target is a 237-byte constructor with 10 relocations. It copies the scale
and velocity vectors component by component, stores the allocated saved-vertex
array pointer before copying `velocity2`, and snapshots each mesh vertex's
`u`/`v` values into that array. The component assignments preserve the target's
`x`-then-`y` stores; the body is in
[`src/unk_46fa80.cpp`](../../src/unk_46fa80.cpp#L23).

## `Car::RebuildLights` (`0x45a980`)

This 421-byte method reparents the car's light sprites, resolves their meshes,
clears the live records' `dead` bytes, and writes target values when target
pointers are present. Three volatile byte stores preserve the target's order
before its target-pointer loads. The complete body is in
[`src/unk_459890.cpp`](../../src/unk_459890.cpp#L785).


## `T_56b0b8::OnPeerMessage` (`0x484bc0`)

The 682-byte handler matches the target exactly. In the `TERMINATE_SESSION`
case, a volatile store to `m_joined` preserves the target ordering: the saved
EDI register is popped between the `m_joined = 0` write and the
`m_result = 0x80158480` write. The exact body is in
[`src/unk_4843e0.cpp`](../../src/unk_4843e0.cpp#L298).
