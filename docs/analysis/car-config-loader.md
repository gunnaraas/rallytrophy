# Car configuration loader (`0x462350`)

Status: NONMATCHING; tracked body unchanged by the axis-temporary probe.

REA instruction evidence
`ev_0ce566d01f60510d418fce5fc71a04d892081d17fc3943977f83829eab8823b6`
covers the 4,484-byte function in the current reference executable (SHA-256
`2d5b2632a6467edb31eb43e8decf237d47e11d62e9b6f37bf8d528e4ad0d83b7`).

## Axis temporaries

The target allocates a 0x88-byte frame. Local unwind-map inspection shows state
8 calls the folded empty destructor for the object at steady `ESP+0x5c`, and
state 9 does so for the object at `ESP+0x50`; state 9 is installed at
`0x4632c5`. These observations establish two nested destructible local
lifetimes, but do not identify their types by themselves. Hood/trunk axes use
cosine and sine with `(0, cos, sin)` and `(0, -sin, cos)` components. Local
spills at `0x463319`–`0x46333b` retain cosine, sine and negated sine. Inference:
the two locals are persistent `Vec3` axes reused for both members.

Isolated two-local probes reproduce the target frame size, both unwind states,
and the local slots. The straightforward copy form emits 4,504 bytes versus
4,484 (baseline 4,439); the best tested ordering variant emits 4,508 bytes.
Neither matches. The remaining local axis discrepancy includes the target's
`fst [esp+0x30]; push key; fcos` order, while the probe reloads the angle before
`fcos`. A three-float-constructor variant emitted 4,516 bytes and was rejected.
Exception-string boundaries, earlier configuration copies, mass handling,
and wiper setup also differ, so whole-function byte-position counts do not
isolate the axis result. These are local PE/unwind observations; the `Vec3`
identification remains an inference.

Probes: ignored `build/probes/car_ini_axis_locals.cpp` and
`build/probes/car_ini/`. Focused validation: 19 exact, five NONMATCHING, no
failures. Next work: reconcile the exception-string call boundaries, the
target's FPU/store order, mass handling, and wiper setup; preserve actual
`std::runtime_error` behavior when tuning its string helpers.
