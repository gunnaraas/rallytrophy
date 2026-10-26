# Menu toggle (`0x438ca0`)

`MenuScreen::Toggle` switches an integer setting between zero and one when
the requested direction permits it. A successful change selects sound channel
`0x139224`; an unchanged value selects `0x139374`. Both selections call
`GameSound::FUN_0049b730`. Evidence: `ev_2ab283cfb89f84060c770854ed06878ade9425b940cbb1b45f575886fa0a0e38`.

The 85-byte body loads `*value` into EDX, tests it, then loads `delta` into EDX.
It lays out the nonzero branch first: a negative delta clears the value and
returns; the zero-value branch sets it only for a positive delta; both other
paths select the unchanged sound channel. The normal C++ version compiled to
an 84-byte body by comparing `[EAX]` directly and arranging the branches
differently. An inline-assembly body preserves the observed register and
branch sequence. Its three `E8` calls are emitted as bytes with displacements
to `0x49b730`, because VC6 inline assembly does not accept the decorated
member symbol in a `call` operand. These displacements assume the original
fixed function and callee addresses.

The focused matcher passes all 55 marked entries in `unk_437ea0.cpp`; four
other entries in that file remain NONMATCHING.
