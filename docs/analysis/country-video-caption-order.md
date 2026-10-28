# Country-video caption order (`0x4526e0`)

`GameView::ShowCountryVideo` is defined in
[`src/unk_452530.cpp`](../../src/unk_452530.cpp) and mapped in
[`analysis/range-450ee0.md`](../../analysis/range-450ee0.md).

The function matched at 1,040 bytes with 78 relocation sites after changing
the final caption-coordinate initialization to:

```cpp
float y2;
*(volatile float*)&y2 = DAT_0056aeac * 465.0f;
```

The PE stores all four caption coordinates before pushing the first
`FUN_004b69f0(0)` argument. Without the volatile store, VC6 hoisted that push
above the coordinate stores, shifting the frame operands and leaving 21
differing bytes. Making the local itself volatile changed register allocation
and expanded the mismatch to 121 bytes; the typed local plus volatile store
preserves the original frame and instruction schedule. The PE and object
comparison establish the instruction order and exact match; the source-level
reason for the scheduling change is a compiler-shape inference.
