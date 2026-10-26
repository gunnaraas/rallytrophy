# qsort comparator at 0x482850

`functions_extra.csv` identifies `0x482850` as a comparator referenced as data.
Its 127-byte body is transcribed from the shipped PE and passes the byte matcher.

## Behavior inference

The function receives two pointers to object pointers. It reads a count at object
offset `0x1a50` and, when positive, sums that many 32-bit values from offset
`0x1a28` in each object. It returns the comparison sign of those sums. If they
tie (or the count is non-positive), it compares byte strings starting at each
object address. The string loop compares two bytes per iteration and returns
the first differing byte's comparison sign. These are inferences from the PE
instruction sequence, not original source or a Ghidra behavior claim.
