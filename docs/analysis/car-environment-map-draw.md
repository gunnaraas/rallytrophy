# Car environment-map drawing

`Car::Draw459270` at `0x459270` renders environment-map faces using a local
`MirrorView`. The current source is in `src/unk_459230.cpp`; the broader
function map is in [`analysis/range-4563e0.md`](../../analysis/range-4563e0.md).

## Instruction-order evidence

REA assembly evidence `ev_aef70530775bc57200598a290fd7beea54403b7e1323f7fc92a39a154061b2ac`
shows the target loads the camera argument, computes and spills `camera + 0x20`,
then writes the FOV, near-plane, and far-plane constants. It copies that matrix,
copies `camera + 0x60`, and then loads `Car + 0x24` as the body pointer before
reading the body fields. These orders are visible in the listing around
`0x4592d5`–`0x459338`.

The source now computes the frame pointer before the projection constants. This
behavior-preserving order probe reduced the focused matcher’s differing bytes
from 206 to 203; the function remains NONMATCHING. The source declares the body
pointer after both matrix copies, but VC6 hoists its load before the second copy.
Moving that declaration between the copies lowered the raw difference count
further, but made the emitted load occur before both copies, contrary to the
target, so it was reverted. Moving the face-matrix initialization after the
cross-product call and broadly reordering its identity stores both worsened the
match and were reverted. A narrower reorder of the identity matrix’s final three zero
stores to elements 14, 13, then 12 matches the target order at `0x4594dd` and
reduces the focused difference from 203 to 201 bytes. Assembly evidence for the
function is in `ev_aef70530775bc57200598a290fd7beea54403b7e1323f7fc92a39a154061b2ac`.
The exact instruction schedule remains unresolved.

An additional fast-path instruction read, `ev_4096faa87f0df77b86ead2ff2a6bc66c5ef17ce0a69547ab54573917888d6152`, confirms the body pointer load at `0x459320`, after the second `rep movsd` at `0x45931e`. A volatile local pointer probe kept that read after the copy but forced stack stores and reloads, expanding the candidate to 1532 bytes with 938 differing bytes; it was reverted. Reading `car->body` through a volatile pointer into an ordinary local instead gives a 1516-byte candidate with 132 differing bytes and places the pointer load after the second copy. Making the `unkac` correction a volatile read reduces the matcher count to 131; it also changes the x87 operand order, so the p.x instruction sequence remains open.

The remaining source schedules matrix initialization by leaving `EnvMat`'s constructor empty and invoking `SetIdentity` as the left operand of the cross-product call expression. This puts the nine identity stores after the three argument pushes and before the call; their assignment order follows target stores at `0x459603`–`0x459661` (`ev_4096faa87f0df77b86ead2ff2a6bc66c5ef17ce0a69547ab54573917888d6152`). The current 1516-byte candidate has 54 differing bytes. Reading both `p.x` and `body->unkac` through volatile lvalues reduced the previous 59-byte diff, but VC6 emits `fld [body+0xac]; fadd [p.x]`, opposite the target's `fld [p.x]; fadd [body+0xac]`. Swapping the C++ operand order did not change that assembly. A volatile read of `p.x` alone preserved the target x87 operand order but had 60 differing bytes; a volatile `Vec3` lvalue compound assignment restored the operand order but had 62 differences. The candidate remains NONMATCHING; the x87 operand order and unrelated register-load order remain unresolved.

Two follow-up scheduling probes were rejected: moving only the `EnvMat` declaration after the temporary vectors had no effect, while making the matrix pointer local volatile expanded output to 1524 bytes with 882 differing bytes. A narrowly scoped inline-assembly correction of `p.x` shifted the compiler's local layout and expanded output to 1520 bytes with 974 differing bytes. A hardcoded EBP-slot inline-assembly probe also shifted vector locals and yielded 1516 bytes with 169 differing bytes. A `register float oldX` temporary expanded to 1520 bytes with 991 differences. The current candidate is 54 differing bytes.
