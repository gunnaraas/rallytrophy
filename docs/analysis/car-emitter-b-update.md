# CarEmitterB::Update matching notes

`CarEmitterB::Update` at `0x479250` is in
[`src/unk_479170.cpp`](../../src/unk_479170.cpp), within the particle module
mapped by [`analysis/range-4741a0.md`](../../analysis/range-4741a0.md).

REA evidence `ev_800fbf6b258e5b69ddabeac3f9e79bd350789cf724f68c1239eec27da32c2333`
records the complete 564-byte body from `0x479250` through `0x479483`. At the
end of the live-particle path, the target clears `EBX` at `0x47945b`, then
stores the updated age from x87 at `0x47945d`. The source expresses that age
update as `(dt + dt) + p->age` and then advances to the saved next particle.

## Current match state

The exact source expression is `p->age += dt + dt;`. It compiles to the target's
564-byte body, including the target order of `XOR EBX,EBX` before the final
`FSTP [particle+8]`. `tools/objcmp.py` reports a byte-exact match with ten
relocations. The equivalent-looking explicit assignment
`p->age = (dt + dt) + p->age;` reversed those two independent instructions and
left five differing bytes. The `for` increment, named-age temporary, and
guarded `do while` probes also retained that mismatch.
