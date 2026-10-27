# Texture upload helper (`0x4a8350`)

Local PE disassembly and COFF observations: this 309-byte helper now uses
symbolic direct calls with five `DISP32` relocations. The pending raw call
displacements were replaced with linker aliases to the symbols below. Both
the focused matcher and `objcmp` confirm the exact body with those five
relocations accounted for; the targets were audited separately from the
masked byte comparison.

| Call address | Operand offset | PE target | Existing symbol |
|---|---:|---|---|
| `0x4a838b` | `+0x3c` | `0x4bc930` | `?Get@CStr@@QAEPBDXZ` |
| `0x4a8393` | `+0x44` | `0x4a81c0` | `?InitializeResourceExact@ModelNameState@@QAEXH@Z` |
| `0x4a8434` | `+0xe5` | `0x4acd60` | `?Downsample@BitmapOps@@QAEXHHH@Z` |
| `0x4a8469` | `+0x11a` | `0x4aafa0` | `?Free@VidImage@@QAEXXZ` |
| `0x4a846f` | `+0x120` | `0x4bd000` | `??3@YAXPAX@Z` |

The first four definitions were checked in existing objects. `Downsample`
is still NONMATCHING; the last target is the excluded CRT `operator delete`.
The indirect renderer calls at `0x4a8450` and `0x4a845d` need no static
destination relocation. This is a relocation audit, not a source-behavior
verification or a linked executable test.
