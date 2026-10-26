# Bounding-sphere construction (1.01)

Implementation: `src/scene_sphere.cpp`, with the `SceneSphere` and
`ScenePoint` layout views in `include/scene_object.h`. Plain validation
observation: the fourteen completed entries below compile byte-exactly with
the configured VC6 compiler against the official executable (2585 bytes total).

| Address | Bytes | Operation |
|---|---:|---|
| `0x4b44e0` | 29 | static vector constructor |
| `0x4b4500` | 12 | register vector destructor with `atexit` |
| `0x4b4510` | 32 | static vector destructor callback |
| `0x4b4530` | 677 | collect vertex position pointers and optionally build |
| `0x4b47e0` | 64 | clear shared point-pointer list |
| `0x4b4820` | 73 | dispatch by point count |
| `0x4b4870` | 648 | randomized sphere construction and fallback |
| `0x4b4b00` | 87 | rebuild with one fixed support point |
| `0x4b4b60` | 97 | rebuild with two fixed support points |
| `0x4b4bd0` | 98 | rebuild with three fixed support points |
| `0x4b4c40` | 30 | one-point sphere |
| `0x4b4c60` | 102 | two-point sphere |
| `0x4b4e80` | 563 | four-point sphere |
| `0x4b50c0` | 73 | containment test |

## Static scratch-list lifetime

The compiler-generated group for the file-static point vector has a constructor,
atexit-registration thunk, and destructor callback in this claimed range:
`0x4b44e0`, `0x4b4500`, and `0x4b4510`. The adjacent CRT initializer wrapper
at `0x4b44d0` calls the constructor then the registration thunk. The thunk
registers the callback.
Evidence for constructor and registration behavior:
`ev_79503608bb03d166d09b826a8f1960a41bc0e6dcc7c064af97b948e18c52fb8d`,
`ev_4e4dc4a2d8ebacedc046896718c86c3f20ca42d86a8ee51ded32f65fa218433b`.
The constructor copies the high byte of ECX to `0x57b960` and zeros the three
vector pointers at `0x57b964`–`0x57b96c`. The destructor callback calls the
allocator's delete routine on the begin pointer, then zeros those three
pointers; the first four-byte field is left alone. Assembly and bytes confirm
the generated wrapper, callback registration and adjacent destructor body:
`ev_8cdf1a3ebb0ff34d1c142cb118150384dc1640864a78b81b132d9cdea454acc1`,
`ev_54472eadfbb41c550ddaf98169131edb6f403be9097c54ef11cdaedd9101c856`,
`ev_b63a6401306c15dd646a034ca041851910e264a1d2b6149e22f1fe3a67b178a8`.
Plain validation observation: the three in-range compiler-generated entries match exactly.

## Shared scratch list and dispatch

`0x4b47e0` clears the logical length of the point-pointer vector whose begin
and end pointers are at `0x57b964` and `0x57b968`. It retains capacity and does
not destroy the pointed-to geometry. Evidence:
`ev_126e32c08b4a316865ed36a6c82dec2cb5cee03503dab51442cc583e661010b6`.
Plain compilation observation: VC6 `std::vector<ScenePoint*>::resize(0)` emits
the exact 64-byte entry, including unreachable template erase code omitted
from Ghidra's pseudocode.

`0x4b4820` zeroes center/radius for an empty list, calls the one-point setter
for one point, and calls `0x4b4870` for multiple points. Evidence:
`ev_b29e8831f5cbacdb1546cf861240bae9674ed62cf79989798d0b36849e198fc9`.
These helpers are members in the reconstruction to preserve the callers'
ECX convention; the reset helper itself does not read ECX.

## Randomized builder

`0x4b4870` finds coordinate-wise minima and maxima, takes their midpoint,
and measures the greatest squared distance from that midpoint to the input
points. Its square root supplies a fallback radius. Each attempt shuffles
the shared pointer list in place using `rand() % (k + 1)` with decreasing k,
starts with the first point's sphere, and processes the remaining points.
An outside point triggers the one-fixed-support helper. If the resulting
radius is greater than the fallback, another shuffle is tried, up to five
attempts. After five oversized results, it uses the midpoint and fallback
radius. Evidence:
`ev_cf8fd6b6ad56faea2d2103765825439c61b07989b984e18d5fc09814df924c0c`.

Plain assembly observation: the stop test also exits on an unordered float
comparison, preserving a NaN result rather than forcing another attempt.
This is expressed as `!(radius > fallbackRadius)` in the source. The global
at `0x57b970` is assigned 1 before the attempts and at the end of each attempt;
its broader role is not established here. The zero constant at `0x4e134c`
has float bits `0x00000000` (plain PE-byte observation).

The three recursive helpers walk only the preceding prefix of points. An
outside point adds another fixed support: one to two, two to three, then
three to the four-point setter. Evidence:
`ev_bcd5e0fb706cc2a65247f06ea484e3ff055ee5d5d19cc4b77b244565fc23a375`,
`ev_64651e31d782931b1f70a9891b79b736cd53efcd7964ad23687c99372f858193`,
`ev_f765cbb2bf222fa00599342680b7d10b28e758a6667b08621fd2ea75ec39c76f`.
Calling this a randomized support-point sphere algorithm is an inference
from that control flow; no global optimality guarantee is asserted.

## Primitive spheres and containment

The four-point setter computes a circumsphere using a double-precision
cofactor inverse. It leaves the existing sphere unchanged when the determinant
does not exceed the observed `1e-6` threshold. Evidence:
`ev_d923ff000d776c774cff1bfa7e1ab76c21a33548d01036e0ea072233a6191ef8`.
Plain validation observation: the implementation matches all 563 bytes.

The one-point setter copies the point to the center and sets radius=0.
Evidence: `ev_0e2816be8c2f811ccf7279d235c619972beffef4a246bacd457a40c919fca7a2`.
The two-point setter uses the midpoint and half the distance between points.
Evidence: `ev_1d3b684ef2f4f2dd033c3940dc7b8b0ae1c58a9bf0667dafe6e21e86f7a540bc`.
Plain instruction observation: the half constant at `0x4e2958` is used as a
double operand. A separate float length local preserves the original x87
stack cleanup before the final multiplication.

Containment compares squared distance with squared radius, including points
on the boundary and rejecting unordered comparisons. Evidence:
`ev_1d476c0064742b1101b5d318c1ed9523ca8d0efedf71d614ab8e8ac5314fca1d`.

## Vertex collection and limits

The three-point primitive `0x4b4cd0` already has an exact entry in
`src/mesh_triangle_cache.cpp`; this module only declares its geometry
view. `AppendVertices` (`0x4b4530`) appends each vertex position from a 44-byte
record and optionally calls `Build`. Evidence:
`ev_2343f33c663188d591c7c35d5e9569fb12837be3612580767ba8f78161acc184`.
The current complete dossier is
`ev_44ce6879226232bef8d09d13cf61fca678c7b7fff2086db9282a1d70f6dcbe48`.
After reserving space for a nonzero count, it skips the append loop unless
the count is positive. On the positive path it copies the vertex cursor
through its stack slot (`0x4b45fe`/`0x4b4602`), appends position pointers,
and advances the cursor by `0x2c`. The source keeps a separate cursor inside
an explicit positive-count guard and uses a countdown do/while loop. The
optional `Build` call still occurs after collection, including for zero count.
The original negative-count reserve behavior is preserved; no validation
has been added.

Matcher observation: this source form reproduces all 677 bytes, including
the formerly missing eight-byte stack copy, without inline assembly.
The focused sphere and scene checks pass 28/28 marked entries. These checks
do not establish runtime geometric correctness. Relocation fields use the
existing mask.
