# GameScreen24FC rectangle construction (`0x42f360`)

`GameScreen24FC::BuildRects` appends ten hit rectangles: a large list-area rectangle, a lower/back rectangle,
and eight row rectangles. It computes the row bounds from the common top coordinate and 42-pixel pitch. REA
Evidence `ev_29ac5b3fcfa6d3278d544d70d3f65371422aa2801be1541ea692ba9cf25081b4` records the full 593-byte body,
including the SEH frame, x87 coordinate calculations, constructor calls, and vector insertions.

The source constructs the two large rectangles separately and then loops over eight rows, preserving the
target's local lifetime and x87 calculation order. The object matcher confirms an exact 593-byte match with
34 relocation fields masked.
