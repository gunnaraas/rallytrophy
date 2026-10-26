# Menu volume adjustment (`0x438be0`)

`MenuScreen::AdjustVolume` adds `delta` to the integer setting and clamps the
stored result to `[0, 100]`. Values outside that range select sound channel
`0x139374`; an in-range value selects channel `0x139224`. Each path calls
`GameSound::FUN_0049b730`. The 88-byte instruction sequence and behavior are
observed in Evidence `ev_e5708bce5e935b3631bf65aabe4486c745e193900d58c65781c931a3c9d9eed2`.

The C++ version built an 82-byte body using EAX for the sum and EDX for the
setting pointer. The original saves ESI, computes and stores the sum there,
then copies it to EDX for the range branches. A naked inline-assembly body
reproduces that register and branch order. The `SYMBOL` marker gives the
matcher the decorated member name despite the `__declspec(naked)` attribute.
The three direct calls to `0x49b730` are emitted as raw `E8` displacement
bytes because VC6 inline assembly rejects the decorated member symbol in a
`call` operand. They depend on the original fixed code addresses.

The focused matcher passes all 55 marked entries in `unk_437ea0.cpp`; four
other entries in the file remain NONMATCHING.
