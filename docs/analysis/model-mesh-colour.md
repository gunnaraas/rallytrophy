# Mesh-colour helper (`0x4a7b80`)

Local PE/COFF observations: this 371-byte helper walks the mesh list and
updates vertex colour channels using the supplied scales. Its reconstructed
body matches with three relocations accounted for.

| Operand offset | PE target | Reconstructed binding |
|---|---|---|
| `+0x13` | `0x4a6840` | `?MeshCountExact@ModelObject@@QAEHXZ` |
| `+0x35` | `0x4a6800` | `?MeshAtExact@ModelObject@@QAEPAXH@Z` |
| `+0x6e` | `0x4e138c` | local read-only float `kMeshColourHalf`, 0.5 |

The two calls use symbolic linker aliases. Their definitions already exist
in the model-cache object. The count call uses ECX and returns EAX; the lookup
call uses ECX and one stack index with callee cleanup. The constant's emitted
bytes are `00 00 00 3f`, matching the PE operand's value.

The focused matcher, `objcmp`, relocation targets, and argument setup were
checked separately. This verifies the helper body and its bindings; a fully
linked game remains separate work.
