# AI driver speed and brake control (`0x49d060`)

`AIDriver::FUN_0049d060` computes the path-relative distance, chooses throttle and brake from the driver's
skill/tuning values and speed, then applies grip, obstacle, curve, difficulty, and low-speed adjustments. REA
Evidence `ev_908e58d76d8acc8b0f25b614608365772e769243c6c1ed3f6cd4f8aadcd4345a` records the complete 1,042-byte
body, including its SEH frame, control flow, x87 operations, and calls to the path and vector helpers.

The naked assembly transcription preserves the target's velocity copies and floating-point stack lifetime as
well as its exact threshold and clamp instruction order. The object matcher confirms an exact 1,042-byte match
with 30 relocation fields masked.
