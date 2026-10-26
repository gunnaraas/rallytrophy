# Exhaust smoke emission

`ExhaustSmoke::Emit` at `0x4786a0` spans 405 bytes. Evidence
`ev_645f0c500e94a3a1dd5ead843a64dde8d1dbb78f7bd7634c30af2623f8e75716`
covers its complete inclusive range `0x4786a0`–`0x478834` in the official
executable, SHA-256
`2d5b2632a6467edb31eb43e8decf237d47e11d62e9b6f37bf8d528e4ad0d83b7`.
The observations below follow assembly; particle field names and the emission
interpretation follow the reconstructed layout in `unk_47c460.h`.

## State updates

The object arrives in ECX. Five four-byte stack arguments provide position,
velocity, throttle, step, and age. The function preserves EBX/ESI/EDI,
uses 24 bytes of local stack space, and returns with `ret 0x14`.

When the free-list head at `0x56ae90` is null, call `ParticlePool::Grow`
at `0x478960`. Remove one particle from that list using its `+0x40` link.
Three calls to `rand` produce values scaled by the float reciprocal of 32767.
Denote them `r1`, `r2`, and `r3`. The particle receives:

| Field | Offset | Value |
|---|---|---|
| age | `+0x08` | supplied age |
| lifetime | `+0x0c` | 1.0 |
| position | `+0x10` | supplied position, then adjusted below |
| velocity | `+0x1c` | supplied velocity, then damped below |
| rotation | `+0x28` | float conversion of `rand() & 0x3ff` |
| spin | `+0x2c` | `(1.0 - (r3 + r3)) * 64.0` |
| size | `+0x30` | `(r1 + 1.0) * 0.05`, then adjusted below |
| gravity | `+0x34` | `r2 * 0.25 + 0.6` |
| color | `+0x38` | `0xffffffff` |
| alpha scale | `+0x3c` | `throttle * 0.15 + 0.1` |
| frame | `+0x04` | a separate random mask, `(rand() & 3) | 0x100` |

There are five random calls total; frame is generated before rotation.
Velocity is multiplied by `pow(0.1f, age)`. The x87 result is retained across
the three component multiplies, with each component stored as a float.
Position is reduced by the damped velocity multiplied by step. The particle
is inserted at the object's list head (`+0x128`), and size is reduced by
`step * gravity`. These are static state-update observations; rendering and
random seeding behavior were not executed here.

## Constants and reconstruction

Local PE extraction observation: the referenced float bits are
`0x38000100` (reciprocal of 32767), `0x3f800000` (1),
`0x3d4ccccd` (0.05), `0x3e800000` (0.25), `0x3f19999a` (0.6),
`0x3e19999a` (0.15), `0x3dcccccd` (0.1), and `0x42800000` (64).
The transcription uses named local constants with those float values.
In particular, the damping base is the float value `0.1f`, promoted to x87;
replacing it with a double literal would change its input value.

The pool-growth alias resolves to `?Grow@ParticlePool@@QAEXXZ`.
The vector-scale alias resolves to the existing exact `ScenePoint`
implementation at `0x40b0c0`, `??DScenePoint@@QBE?AU0@M@Z`.
The power helper resolves to `__CIpow`, matching the preceding compiled
C++ body's CRT relocation. These names are local source/COFF observations.

Independent local object/PE inspection checked all 12 float-reference sites:
each named constant's stored bits equal the original referenced float bits.
The `ScenePoint` scale implementation and the reconstructed `Vec3` both
use three floats. The scale helper receives its receiver in ECX, hidden
destination at entry ESP+4, and scalar at ESP+8; it returns the destination
in EAX with `ret 8`. The emitter's two pushes preserve this convention.
The three alias targets also exist as strong symbols in their reconstructed
objects or the local VC6 CRT library. These are local inspection observations,
not a claim that a full executable has linked.

Ordinary statement reordering had left a same-size build with 41 differing
bytes. Isolated copy-assignment and compound-subtraction trials did not
remove the scheduling differences; a volatile lifetime store increased them.
The instruction transcription preserves the x87 intermediates, velocity
copy/store order, scale-call argument preparation, and register restoration.

Focused matcher: 11/11 marked functions exact, one existing NONMATCHING,
no differences or compile failures. The new function matches 405 bytes
with 24 object relocations. This establishes object-code matching; final
linking and runtime smoke rendering remain separate verification work.

Full matcher after this change: 1,754/1,754 marked functions exact,
330 NONMATCHING, no differences or compile failures.
