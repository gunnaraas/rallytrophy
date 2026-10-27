# CarEmitterC particle spawn (`0x47c1a0`)

`CarEmitterC::SpawnOne` consumes the emitter budget, removes a particle from the shared pool, initializes its
age and physical properties, calculates randomized velocity around the emission direction, offsets its position,
and links it into the emitter's live list. REA Evidence
`ev_28eadd3fa190ddb0b8a96ece7143318c6a925169630482c5c9468738d47dd373` records the full 500-byte target body
and its call and data references.

The naked assembly transcription preserves the target's interleaved free-list and particle stores, x87 random
angle calculations, vector helper calls, and velocity/position update order. The object matcher confirms an
exact 500-byte match with 21 relocation fields masked.
