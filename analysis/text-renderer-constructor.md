# Text renderer constructor at `0x4a9520`

The 170-byte `0x4a9520` entry is treated as the text-renderer constructor based on its direct callers: Ghidra xrefs list `0x425ef6` and five Game setup callsites from `0x430d50` through `0x430e85` (`ev_753a2f11fce97e9cdc6ac42eb1e02ee067da140ce18937deded74e5d1c8ba4ba`). Plain PE disassembly shows a four-argument `thiscall` ABI (`ret 0x10`); the callers allocate 0x150-byte objects, pass a render device, font resource name, and two float values, then release objects through `0x4a95d0`. The `TextRenderer` role is supported by use and layout, but the original source class name is not recovered.

The body constructs the subobject at `this+0x48` through `0x4a8580`, zeros `this+0x11c`, sets `this+0x120` to -1, and calls `0x4aa9c0` and `0x4a9660` with the observed arguments. If the child pointer at `+0x11c` remains nonzero, it invokes `0x4aafa0` and deletes that allocation. It stores the two float arguments at `+0x1c` and `+0x20`, clears `+0x130` and `+0x11c`, restores the exception chain, returns `this` in EAX, and uses `ret 0x10`. This sequence is a plain observation from `objdump -d -Mintel` on the shipped PE.

The exact body is in `src/model_triangle_cache.cpp` as `FUN_004a9520`; the focused model check passes 42/42 functions. The helper bodies and the meaning of the fields beyond these observed writes remain open.

## Resource helper at `0x4a9660`

The 223-byte entry is an observed `thiscall` helper with two stack arguments (`ret 8`), called by `0x4a9520`. Its PE disassembly reads a device-like pointer from the first argument at `+0x20`, allocates `0x68` bytes, constructs a state object with the string at `0x502154` when allocation succeeds, and calls the device vtable slot at `+0x10` with arguments including `0x840`. It stores the result at `this+0x48`, clears fields from `+0x58` through `+0xc4`, and writes the observed defaults at `+0x60`, `+0x64`, `+0x6c`, `+0xe0`, and `+0xe4`. These are direct `objdump -d -Mintel` observations; field meanings are not established here.

The exact body is in `src/model_triangle_cache.cpp` as `FUN_004a9660`. Focused model check: 43/43 marked functions match.

## Narrow-text wrapper at `0x4a97a0`

The 69-byte `0x4a97a0` wrapper converts its string argument at `[esp+0x10]` through `0x4b7670` into the shared buffer at `0x57a7d8`, using length `-1`, then rearranges the original stack arguments and forwards them with that buffer to `0x4a97f0`. It preserves ESI and returns with `ret 0x1c`. This instruction sequence is a direct observation from the shipped PE disassembly; argument meanings beyond the observed string and forwarding are not inferred here.

The exact body is in `src/model_triangle_cache.cpp` as `FUN_004a97a0`. Focused model check: 44/44 marked functions match.

## Formatted-text wrapper at `0x4a9740`

The 82-byte cdecl wrapper at `0x4a9740` passes a stack-local variadic argument area and the pointer at `0x579bd8` to `0x4c2431`, converts that shared narrow-text buffer into `0x579fd8` through `0x4b7670` with length `-1`, then forwards the three stack values, converted buffer, zero, and two `1.0f` constants to `0x4a97f0`. It returns with plain `ret`. These are observations from the shipped PE disassembly; the underlying formatting and rendering routines are not interpreted here.

The exact body is in `src/model_triangle_cache.cpp` as `FUN_004a9740`. Focused model check: 45/45 marked functions match.

## Text rendering core at `0x4a97f0`

The 782-byte `thiscall` routine uses seven stack arguments and returns with `ret 0x1c`. Its exception frame uses handler metadata at `0x4ded06`. If `this+0x130` is set, it forwards the arguments to `0x4aa690`. Otherwise it gets the UTF-16 string length through `0x4c2414`, calls the renderer vtable at `+0x8c`, prepares two temporary two-element arrays with `0x4c11f4`, and calls `0x4aa5a0` before processing text. The loop handles space and newline separately, then looks up glyph data and constructs the draw payload for the renderer vtable slot at `+0x110`. The path destroys both temporary arrays with `0x4c1116` and restores the exception chain. These are direct observations from the shipped PE disassembly; semantic names for the fields and draw payload remain tentative.

The exact body is in `src/model_triangle_cache.cpp` as `FUN_004a97f0`. Focused model check: 46/46 marked functions match.
