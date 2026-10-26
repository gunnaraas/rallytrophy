# File buffer refill (Patch 1.01)

Target: File::FillBuffer, original `FUN_004a6090` (308 bytes).
These are Patch 1.01 addresses, as described in `analysis/NOTES.md`.

Evidence retained in `analysis/evidence-2026-10-22-loop19.json`:

- Pseudocode: `ev_34b3183dccd814a903100ab281812d4ec6ec2b3116815196eca87efac7bc84a6`.
- Assembly: `ev_8d121ad74dda3f7ce23ba59b903c3a8b928ecb932d2157b40792d6f09cf64229`.

The following behavior is observed in those records. Field names describe
the reconstruction, not recovered original names.

The raw path reads `min(rawSize - rawPosition, 0x2000)` bytes into the output
buffer at File+0x2078. It resets output cursor +0x70, sets output count +0x74,
and increments raw position +0x20 and emitted count +0x1c by the requested
length. It calls CheckRead (`0x4a63b0`), then optionally XOR-decodes the buffer
when the byte at +0x14 is nonzero.

The compressed path sets the output pointer and capacity to that buffer and
0x2000. It refills input at +0x78 only when the unsigned available-input count
at +0x30 is zero. Refill advances raw position and optionally decodes input.
It calls `0x4cfa05` with state +0x2c and argument 1, resets output cursor,
sets output count to `0x2000 - avail_out`, and adds that count to the emitted
total. It does not call CheckRead and ignores the callee's return value.

Both loops XOR each byte with the low byte of the four-byte entry at
`0x501d10 + 4 * ((pre_read_raw_position + byte_index) & 0xff)`.
The index therefore continues across refills using raw byte position.

Observed state-prefix offsets relative to File:

| Offset | Interpretation |
|---|---|
| +0x2c | next input pointer |
| +0x30 | unsigned available input bytes |
| +0x34 | total input (z_stream ABI inference; unused here) |
| +0x38 | next output pointer |
| +0x3c | unsigned available output bytes |

Identifying `0x4cfa05` as zlib inflate and naming the prefix fields is an
inference from these uses and the previously observed zlib API/layout.
The reconstruction declares the external API, without porting library code.

Local verification observation: VC6 SP5 `/O2 /Oa /G6 /GX` matches all 308
bytes after masking six relocations. The full checker passes 90/90 marked
functions, with six pre-existing NONMATCHING bodies. This is object-code
comparison, not a linked runtime test.
