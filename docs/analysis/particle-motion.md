# Particle motion (1.01)

## CarEmitterC update (`0x47b560`)

The 463-byte routine detaches its particle list, initializes its bounds from
the first particle (or zeros them for an empty list), then ages and moves
live particles. It updates alpha from remaining lifetime with x87 integer
rounding, scales velocity by the frame duration, and expands the bounds.
Live particles are linked back onto the emitter's list and receive a y-velocity
increment of `dt * -6.81f`; expired particles join the shared pool at
`0x56ae90`. Assembly and pseudocode evidence:
`ev_27a2d8ad32636283d00b95ea2b308f322150e36a81c01426369e98981315993d`.

The lifetime comparison uses the integer views of age and lifetime. The
reconstruction retains that comparison rather than replacing it with a float
comparison. For motion, `0x40b0c0` returns the scaled vector address in EAX;
ESI still holds the current particle. Instructions `0x47b60f`–`0x47b629`
load position.x before adding displacement.x, then load displacement.y/z
before adding the corresponding position components.

Source/matcher observation: the previous C++ helper reversed the x operands.
A local nine-instruction x87 block now reproduces all three updates in the
observed order. The existing EBP frame, helper call, bounds branches, and
recycling code remain byte-exact. This block relies on the verified VC6
register assignments at the vector-scaling call boundary; it is intended for
the project's fixed compiler settings. The focused file check passes 6/6
marked entries, with three other entries still NONMATCHING. Relocation
operands are masked by the existing matcher; runtime effects are untested.

## Emission frame (`0x47bf80`)

The 540-byte routine converts absolute direction components to integers
before choosing a coordinate axis. It forms a perpendicular vector with a
cross product, normalizes that vector, and forms the other perpendicular
axis with a second cross product. It stores the origin, both axes, and the
input direction, sets the emission budget to `0.1f`, then calls `0x47c1a0`
twice with zero cost and the supplied scale and age. Evidence:
`ev_6cd376f8f02dfcb4c7be8fbd57457638b3627275747285be1ae683443bd1619c`.

The intermediate integer for the absolute y component is spilled at
`[ebp-0x10]` (`0x47bfbd`) and reloaded from there (`0x47bfc5`). The same
slot is subsequently used for the cross-product destination pointer.
The reconstruction uses a named local union to express those nonoverlapping
integer and pointer lifetimes. It assigns the pointer member before either
cross product and never reads the integer afterward.

Matcher/source observation: that explicit stack-slot reuse resolves the two
remaining differing bytes. The complete routine and all seven marked entries
in its source file match; two other entries remain NONMATCHING. The existing
cross-product instruction sequences and exception frame are unchanged.
