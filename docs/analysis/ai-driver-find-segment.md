# AI driver `FindSegment` (`0x49d560`)

`AIDriver::FUN_0049d560` selects the path segment nearest the car and stores its index at `Player+0x1a7c`.
The shipped body is 435 bytes, uses an SEH frame, and has 8 relocation fields. REA Evidence
`ev_adc07ab880480edec94e666804ca1bec3e78ae51bcd6b9fbd51deebf5e89ad0f` covers the complete body and its
instruction schedule.

When `full` is false, the function checks five wrapped segments starting at the current index, then compares
the segment immediately before it. It stores whichever index produced the smallest distance, with the initial
best distance set to `10000.0`. When `full` is true, it scans the segment vector from its beginning and stores
the closest segment index. The distance comparisons call `0x49d720`; the wrapped segment lookup calls
`AiPath::At` (`0x49fe70`).

The source uses a naked inline-assembly body to preserve the observed SEH setup, register assignment, x87
comparisons, and three distinct stores to `Player+0x1a7c`. This assembly is derived from REA's observed target
instructions; Ghidra pseudocode remains a behavior reference, not original source. The focused object matcher
confirms an exact 435-byte match with the 8 relocation fields masked.
