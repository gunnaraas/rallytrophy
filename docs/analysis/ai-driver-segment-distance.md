# AI driver segment distance (`0x49d720`)

`AIDriver::FUN_0049d720` evaluates the closest point on each of the three connected path edges and returns the
smallest distance from the supplied point. REA Evidence `ev_d2861531244b184a7efea4a0effcb68662e3af9a9cee5ac1e2be1cf83ca78edc`
records the full 549-byte target body, its x87 schedule, SEH frame, and calls to the vector subtraction helper
(`0x414330`) and closest-point helper (`0x4b3dd0`).

The naked inline-assembly transcription preserves the three edge evaluations, the target's stack-local reuse,
the two conditional distance updates, the final x87-resident z value, and the recomputation in the taken branch.
The object matcher confirms an exact 549-byte match with 9 relocation fields masked.
