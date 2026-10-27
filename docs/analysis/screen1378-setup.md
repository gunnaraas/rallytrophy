# Screen1378::Setup matching notes

`Screen1378::Setup` at `0x43a490` is in
[`src/unk_43a390.cpp`](../../src/unk_43a390.cpp); its surrounding module map
is [`analysis/range-439080.md`](../../analysis/range-439080.md).

## Target behavior

REA evidence `ev_de6cfb5ab749b59747d904de8e8d93af0a057075ecfe5bfa2a96e87251d41a8c`
records the complete 432-byte function body (`0x43a490`–`0x43a63f`). The target
returns early when the base screen state at `this+4` is nonzero. Otherwise it
allocates two `0xd4`-byte sprites, attaches textures created from
`data/menu/network_box1.tga` and `data/menu/network_box2.tga` with flags
`0x841, 1`, initializes their layout/color fields, uploads both textures using
`DAT_0056b0ac`, then sets the state and stores the `game` argument at `this+0xc`.

## Matching experiments

The original candidate used local texture pointers and chained array
assignments. It compiled to 457 bytes with 351 differing bytes outside
relocations. Assigning newly allocated sprites directly to the screen members,
then accessing them through local sprite pointers, reduced that to 426 bytes
and 148 differing bytes. Making those pointees volatile reduced the count to
139; spelling out the array element stores in ascending order changed it to
140, so the current source retains the explicit element stores because their
order follows the target's observed stores. Removing `volatile` after that
change independently regressed to 148 differences. The current candidate is
426 bytes with 140 differing bytes and remains `NONMATCHING`.
Moving the second `unk24` write after the first upload call while using
nonvolatile pointers produced 143 differences, so the current source keeps both
`unk24` writes before the uploads.
Replacing only those two writes with nonvolatile casts produced the same 140-byte
diff; that cast variation was discarded.
Direct member-expression casts for both `unk9c` writes produced 142 differences;
casting only the second produced 141, while casting only the first left the
140-byte diff unchanged. The volatile local aliases remain in place.

With the current volatile aliases, moving only the second `unk24` write to
after the first texture upload produced 141 differences instead of 140; using
a direct member cast for that delayed write produced the same result. Inline
assembly for the two target immediate stores expanded the body to 429 bytes
with 259 differences. Naming a shared `0xf` register local left the baseline
426-byte/140-difference output unchanged. These probes were reverted.
Using inline assembly only for the second `unk9c` store produced 429 bytes with
285 differences; loading the volatile `second` pointer into EAX first produced
435 bytes with 291 differences. An early local initialized to `6` and `/Ox`
both left the baseline output unchanged. All of these probes were reverted.

The remaining visible differences are the SEH-state store placement, the
`unk9c`/`unk98` field-store order, and scheduling/register choices in the final
field initialization and texture-upload sequence. No byte-exact match has been
verified yet.
