# Car-model cache constructor (`0x4ade40`)

Local PE disassembly and COFF comparison observations: the constructor is
59 bytes, with no relocation operands. It clears the cache pointers and
counters, and copies an uninitialized stack byte into the flags at `+0x1c`
and `+0x2c`. The first flag store occurs between the stores to `+0x08` and
`+0x0c`.

Making only the first flag store volatile preserves that target store order
under VC6 SP5. This qualifier is a reconstruction technique, not evidence
that the original source declared the flag volatile. The uninitialized byte
copies intentionally reflect the target; their values are not specified.

Both the focused matcher and `objcmp` confirm an exact 59-byte match with
zero relocations. Other functions in this module are outside this change.
