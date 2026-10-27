# Car physics step (`0x45bc20`)

## Target evidence

REA Evidence `ev_c92c59602c67fd58248c61d5b8be6532014a74b970e4fb2582dd1e42d5e74d32`
identifies `FUN_0045bc20` as a contiguous 2,002-byte thiscall body at
`0x45bc20`–`0x45c3f1`. Its assembly shows the physics update's stack frame,
exception handler, floating-point schedule, calls, and object-field accesses.
The detailed behavior map is in [`../../analysis/range-459890.md`](../../analysis/range-459890.md).

## Matching probes

The source-form baseline compiled to 1,944 bytes, with 1,687 differing bytes
outside relocations. Making the matrix pointer object volatile
(`float* volatile rotp`) compiled to 1,965 bytes and reduced the difference to
1,648 bytes. Replacing the post-collision `Vec3` centre copy with three direct
component updates brought the candidate to 1,944 bytes / 1,641 differences.
This supports the hypothesis that pointer and update source shape affect the
target's frame and instruction schedule, but the function remains far from
exact.

Reordering the `speed2` expression to `y² + z² + x²` makes VC6 emit the target's
`x²`, `z²`, `y²` x87 accumulation order. This changes the disassembly locally
but leaves the total at 1,641 differing bytes; the current source retains the
ordering because it aligns that operation sequence with the target evidence.

Other isolated trials were worse than the `rotp` variant: making `posp`
volatile produced 1,962 bytes / 1,679 differences; making both pointers
volatile produced 1,983 / 1,649; a volatile loop counter produced 1,990 /
1,698; and radius-local or right-associated sum forms produced 1,958 / 1,694
and 1,944 / 1,690 respectively. The scale-assignment inline-assembly trial
produced 1,947 / 1,657. These are compiler-output observations, not evidence of
the original source form. The current source preserves the best volatile
matrix-pointer and direct-centre-update forms plus the `speed2` ordering that
matches the target's x87 accumulation order.

The best current candidate is still marked `NONMATCHING`. Remaining source
shape differences include stack-slot and register allocation, the radius-sum
x87 schedule, exception-state lifetimes, transform argument slots, and the
post-collision centre update order. Continue from the current candidate and
compare the compiled body after each small change.

A balanced radius sum, `(r0 + r1) + (r2 + r3)`, compiled to 1,946 bytes with
1,661 differing bytes, worse than the current 1,944 / 1,641 candidate. It was
reverted. The existing left-associated sum remains the best tested form.

Removing the local `Vec3& torque` alias and writing both accumulations through
`b->torque` emitted identical object bytes: 1,944/2,002 bytes with 1,641
differences. The alias was restored.

Replacing the stored-square form with the explicit expression `(s * s) *
(s * s)` emitted identical object bytes: 1,944/2,002 bytes and 1,641
differences. The compiler already schedules this calculation identically; the
original source form remains.

Hoisting `Vec3 wp2` from the `speed2 > 1.0f` branch to function scope produced
1,946 bytes / 1,529 differences, improving the prior best by 112 differing
bytes. This is consistent with the target's persistent `Vec3` lifetime/EH
states; keep the hoisted declaration and branch-local use. Evidence:
`ev_c92c59602c67fd58248c61d5b8be6532014a74b970e4fb2582dd1e42d5e74d32`.

Hoisting the earlier drag-point `Vec3 wp` to the same scope increased the
difference count from 1,529 to 1,552; its block-local declaration remains.
Moving the `down` scalar to function scope emitted identical object bytes; its
branch-local declaration remains.
Hoisting the inverse-speed scalar likewise emitted identical bytes; it remains
branch-local.
Hoisting the collision-path `Vec3 delta` increased the body to 1,962 bytes /
1,541 differences, worse than the `wp2`-only form, so `delta` remains local to
the airborne branch.

Hoisting the downforce arm `Vec3 arm2` to function scope and assigning it in
the branch increased the candidate to 1,979 bytes / 1,618 differences. It also
changed the frame to `0x90`; the best `wp2`-only candidate remains at 1,946 /
1,529 with frame `0x84`. Keep `arm2` branch-local.

Moving the function-scope `wp2` declaration from immediately before `speed2`
to immediately after it emitted an identical `PhysicsStep` disassembly and
retained 1,946 bytes / 1,529 differences. Declaration order in this pair does
not account for the velocity/speed stack-home mismatch.

Target assembly Evidence `ev_273e39f372c3db10a3f6ed15c69c287fb86e0c30d1c8e69d7a44255b722e0674`
shows the inactive-wheel count and later `speed2` both using `[ebp-0x10]`,
while velocity components use `[ebp-0x1c]`, `-0x18`, and `-0x14`. Declaring
`speed2` before `vel` while assigning it at the original point emitted the
same disassembly as the current form; the candidate homes remain shifted and
the mismatch remains 1,529 bytes. The lexical declaration order alone does
not move these homes.

Moving `Vec3 vel`'s declaration beside `gravity` but assigning it only after
the inactive-wheel loop produced 1,942 bytes / 1,620 differences, worse than
the best 1,946 / 1,529 form. Reverted. Evidence for the target homes:
`ev_273e39f372c3db10a3f6ed15c69c287fb86e0c30d1c8e69d7a44255b722e0674`.
