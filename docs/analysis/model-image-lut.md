# Image lookup-table initializer (`0x4a7dc0`)

Local PE/COFF and library-symbol observations: the initializer is an exact
275-byte match with 15 relocations. Its three direct calls, at operand offsets
`+0x44`, `+0xaf`, and `+0xd4`, target `0x4c19f0`. The reconstruction aliases
`_RT_ImageLutCIpow` to CRT `__CIpow`, using x87-stack arguments. That symbol
is also used in `src/unk_478020.cpp` and is present in VC6 `libc.lib`.

The remaining 12 operands refer to five local constants and three output
tables. Independent object/PE comparison confirmed the constants' bytes:

| Original address | Type | Value | Source constant |
|---|---|---|---|
| `0x4e2968` | double | 2 | `kImageLutTwo` |
| `0x4e2960` | float | 1/4096 | `kImageLutIndexScale` |
| `0x4e1348` | float | 1 | `kImageLutOne` |
| `0x4e1fbc` | float | 1/2048 | `kImageLutHalfIndexScale` |
| `0x4e2958` | double | 0.5 | `kImageLutHalf` |

The shared constant references account for nine operands, and the three table
stores for the others. `DAT_0056bbc0`, `DAT_0056fbc0`, and `DAT_00573bc0` are
defined as 4,096-element C++ integer arrays. Their contiguous BSS ranges are
each `0x4000` bytes. The third array's emitted symbol,
`?DAT_00573bc0@@3PAHA`, matches the existing consumers in `unk_4018f0.obj`
and `unk_4b8b10.obj`.

The focused matcher, `objcmp`, data-byte audit, and relocation audit pass.
This establishes the initializer's code and operand bindings; a fully linked
executable remains separate work.
