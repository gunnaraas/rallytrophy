# Scene-object base (1.01)

The reconstruction uses `SceneNode` in `include/scene_object.h` and
`src/scene_object.cpp`. It is a layout view of the common object base,
not an assertion about the original C++ class name. The base is also seen as
`EffectBase`, `ViewObjectBase`, and the base portion of model objects elsewhere
in this reconstruction (plain source observation).

## Entries and validation

Plain validation observation: the focused VC6 matcher verifies these fourteen
entries against the official executable, totaling 1938 bytes:

| Address | Bytes | Reconstructed operation |
|---|---:|---|
| `0x4b2830` | 358 | default construction with string and member unwind states |
| `0x4b29a0` | 30 | scalar deleting destructor |
| `0x4b29c0` | 79 | destructor body |
| `0x4b2a10` | 257 | selective state copy, returning this |
| `0x4b2b20` | 13 | copy-forwarding wrapper |
| `0x4b2b30` | 83 | append to a parent's child list |
| `0x4b2b90` | 119 | detach from a parent's child list |
| `0x4b2c10` | 17 | transform traversal wrapper, vtable slot 1 |
| `0x4b2c30` | 273 | iterative transform traversal |
| `0x4b2d50` | 613 | merge child bounds and rebuild sphere |
| `0x4b2fc0` | 49 | visit non-leaf children through slot 4 |
| `0x4b3000` | 3 | empty two-argument slot 5 |
| `0x4b3010` | 27 | change flag bit 0x04 |
| `0x4b3030` | 17 | compare object stamp with a global |

The constructor and the thirteen previously matched entries all pass together.

## Object layout and lifetime

Constructor evidence
`ev_0ae1de2e8c77d0bb858b08f18951682a3a9b8a5f6926784aa959a6b54ea4f385`
establishes type=0 at +4, ID=-1 at +8, flag byte at +0xc, stamp=0 at +0x10,
position at +0x14, identity matrices at +0x20/+0x60, and an empty VC6 string
at +0x114. It zeroes the selected data at +0xa0..+0xc8 and the hierarchy
pointers, and initializes flag bytes at +0xcc/+0xfc. It does not initialize
the float payload at +0xd0..+0xf8. The source preserves that distinction.

Plain PE-byte observation: handler `0x4df58d` refers to FuncInfo `0x4eefd8`,
with five unwind states. Their cleanup thunks address members at +0x14,
+0x20, +0x60, +0xa0, and +0xd0. This supports the empty-destructor vector,
matrix, and two 0x30-byte bounds-like member layout used by the reconstruction.
The exact meanings of all bounds scalar fields remain unknown.

The destructor installs vtable `0x4e2988`, releases or decrements the
reference-counted string allocation, and clears its pointer/length/capacity.
It does not unlink the node or destroy its children. Evidence:
`ev_3e814481e09f6fbd733daa0f5485186d3f32708a266060c9c4c768860d4cf68e`.

## Hierarchy and copying

The child links occupy next=+0x100, previous=+0x104, first child=+0x108,
parent=+0x10c, last child=+0x110. Attaching to a non-null parent first detaches
the node, then appends it at the tail. Passing null does nothing, including
leaving an existing attachment in place. Evidence:
`ev_0bfbfddea9c00267f5fad3d3be66974a6219026c712f3246f30bff5def326c6a`.
Detaching patches the parent's endpoints and neighboring links and clears
next, previous, and parent; it leaves the node's own children intact. Evidence:
`ev_14c9cc7264d5ed3fe8c7177db5d654969943d5bd60a91f16b246a1c117d556b4`.

State copy assigns the string, stamp, type, flag byte, position, ID, both
matrices, and selected +0xa0..+0xcc data. It preserves the destination's
hierarchy pointers and +0xd0..+0xff region. Evidence:
`ev_d278543862283fd70c93c9190a4b5614427429d6e9c6f594264aca012f7dd177`.
The reconstruction keeps the observed string-assign call out of line through
a small ABI view of the existing `0x4024f0` string instance; it does not replace
VC6 string ownership semantics.

## Transform traversal and flags

Traversal uses a fixed array of 128 ancestor pointers. Flag 0x04 skips a
node's transform computation while traversal still visits its descendants.
With flag 0x10 and a non-null view, it copies the view matrix at +0x184 into
both node matrices and installs the node's position in the world matrix.
Otherwise it copies local to world, installs the position, and calls matrix
multiplication `0x4b55e0` with the parent's world matrix when a parent exists.
Evidence: `ev_6798ceaf7fda05d58ec9a6c4be1bfdac99719e6eb8d8d0d0b08269ba71702731`.

That same evidence shows an unusual traversal exit: when a node has no child
or next sibling, the routine pops only one ancestor. If that ancestor has no
next sibling, traversal ends even if more ancestors remain. The source
preserves this behavior and the original unchecked stack depth.

`0x4b3010` sets or clears flag 0x04 according to whether its integer argument
is nonzero. Evidence:
`ev_c3ca546b6b495030b59e9a2fef17c7a9245820ae3443b59198ce6e4323a6f60a`.
`0x4b3030` returns whether +0x10 equals global `0x502470`; interpreting this
as a current-frame stamp is an inference. Evidence for the comparison:
`ev_c1a9df74081b2199462224f9c142af020a80f36cb92642a15370be44ba765462`.

Plain local-disassembly observations: slot 4 iterates children and dispatches
their slot 4 only if they have children of their own. Slot 5 returns with two
arguments popped and no other work. Those entries and slot 1 were absent from
the old function inventory and are now included in `functions_extra.csv`.
Slot 3 is also inventoried (613 bytes, ending at the return
at `0x4b2fb4`). REA did not recognize these four addresses as procedures in
this session; their entry boundaries were established from direct PE-byte
disassembly instead.

## Bounds update

Plain disassembly observations, grounded in the complete 613-byte read
`ev_ae97c157d1228d34b72590c4691aebbcee655a98143430e39b568357d7e7cc9b`:
slot 3 first copies the named fields of +0xa0 bounds to +0xd0 bounds, leaving
padding untouched. If there are children, it saves/adjusts the x87 control word
through `0x4a7da0`, invokes slot 3 on each child that itself has children,
resets the shared sphere-point list, and zeroes a local sphere. It then invokes
slot 4 on non-leaf children to collect geometry and merges minima/maxima from
children whose +0xfc flag includes bit 1. It builds the sphere through
`0x4b4820`, copies its center and radius to +0xe8/+0xf4, sets flag bits 0 and 1,
and restores the control word through `0x4a7d90`.

Those operations identify the first two vectors of each bounds block as
minimum/maximum extents and the third vector plus first scalar as sphere
center/radius. The second scalar remains unidentified. The source keeps local
bounds as the initial extents; it does not replace them with the first child's
bounds. The original restores the control word on normal completion only;
this reconstruction does not add exception-time restoration. See
`bounding-sphere.md` for the sphere helpers and `analysis/fpu-control-word.md`
for the existing control-word evidence.

## Constructor source form and limits

The 358-byte constructor's current dossier is
`ev_10517f3127cb2180a7660aaf933f4be1b46ec86260f54c5ed748a4e00f32acbc`.
It copies the allocator byte, calls string cleanup `0x402700` with false,
then installs the vtable and initializes the fields. The source's `SceneName`
layout view retains that out-of-line construction call and expands the
reference-counted string release inline in destruction. Using an ordinary
`std::string` member instead had inlined construction cleanup and removed
the observed exception frame (314 bytes).

Matcher/source observations: explicitly ordered volatile stores reproduce
the interleaved field initialization. The first position store and the nine
matrix stores between the bounds-flag mask and writeback remain ordinary
stores, allowing VC6 to place the `1.0f` constant load and flag mask at their
observed instruction positions. No extra fields are initialized. The wrapper
retains the character-pointer assignment used by `BmfObject::name = "block"`.
Its explicit copy constructor and copy assignment delegate to VC6 string
assignment, preserving reference-counted ownership rather than copying a raw
allocation pointer. The inline release follows VC6's null, unique/frozen,
and shared-allocation branches.
A compile-time size check verifies its size equals the VC6 string size;
the complete `SceneNode` remains `0x124` bytes. Focused checks verify 14/14
entries here and 32/32 marked entries across scene, BMF, sphere, and view files.

No linked executable, runtime hierarchy test, or runtime traversal test has been run.
Byte matching masks relocation fields according to the existing tooling.
