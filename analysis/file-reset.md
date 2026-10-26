# File reset helper

`FUN_004a4e60` clears the file stream pointer, cached sizes, buffer positions,
mode bytes, and the stored name. The Ghidra pseudocode calls the VC6 string
`_Tidy`, `_Grow`, and `_Eos` helpers while detaching shared name storage and
setting the resulting length to zero (Evidence
`ev_6cd0bf3349a4b391be27ae337a81ef5279e5b3881a0d024704901aec9783a0e2`). The
embedded limit at `DAT_004e1340` contains `0xffffffff` in the shipped image
(plain binary observation); the minimum with the string length therefore
consumes the full current length.

The source models those helper calls through `FileNameStringOps` and retains
the stream and buffer resets at their observed object offsets. This makes the
name cleanup behavior explicit instead of relying on a C++ assignment or
`erase()` implementation whose generated code does not match the binary. The
body remains NONMATCHING (223 built bytes versus 229 original; 144 differing
bytes outside relocations). There is no runtime fixture for this helper.

`File::Close` at `0x4a5d00` has the same string detach/grow/empty sequence
after closing its stream and ending compression state (Evidence
`ev_334d5577f328ddd50a9f52a751746462258fdfa2f20a5bdc224dc96aef65e5fa`). The
source now follows that path inline and leaves only the cached size at `-1`;
the function is NONMATCHING at 240/241 bytes with 191 differing bytes outside
relocations.
