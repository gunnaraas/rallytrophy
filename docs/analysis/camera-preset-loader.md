# Camera preset loader (`0x457b10`)

REA instruction evidence
`ev_bdaca79c74bb87c3d217f429b9edf71c77ee22d389059b35655eadc62ab77bbd`
reports a contiguous 1,599-byte body. Local PE/object comparison identifies the
runtime-error copy boundary as the main discrepancy in the 1,730-byte draft.
The target calls Tidy (0x402700) and copy-string assign (0x4024f0); the draft
expands both. The actual std::runtime_error exception type must be preserved.

Isolated probes with two discarded inline-budget calls leave 1,730 bytes.
Four restore the assign call and reduce the body to 1,601 bytes. Five, six,
eight and twelve produce the same 1,601-byte/680-difference state: Tidy remains
expanded, zero is retained in ebx, and the extra push shifts frame accesses.
The large difference count is largely caused by that frame shift, not by 680
independent behavioral differences.

The tracked source is unchanged. These probes remain NONMATCHING and outside
tracked source; no exactness or linked-game behavior is claimed. The tire
loader's related boundary work is recorded in tire-config-loader.md.
