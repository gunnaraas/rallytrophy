# Game-mode layout loader (`0x4270c0`)

Local PE/COFF observations: this loader constructs five rectangles from
`data/menu/gamemodemenu.ini`. The prior draft emitted 1,281 bytes against the
1,286-byte target, with 937 differing bytes after relocation masking.

The string twin constructs directly from the path literal, advancing the
exception state after Grow/copy/Eos. Point and rectangle twins preserve the
observed out-of-line constructors. These changes alone left the size and
mismatch count unchanged.

The vector twin preserves the VC6 allocator/first/last/end layout, checked at
compile time. The first four insertions remain calls. The fifth expands with
out-of-line Ucopy/Ufill/Ucopy, Destroy, and final Size. Its growth conditional
uses inline Size, with calls for the selected growth amount and base count.
This completed the 1,286-byte match with 67 relocations and recovered the
original stack frame and saved-this placement.

All 11 aliases resolve to existing definitions; all 27 stand-in call operands
agree with the original PE destinations. The shared config table
`DAT_004f69b0` remains external. These checks cover function/object code,
not a linked game.

REA instruction reference:
`ev_f955697259e5ae30102db3e2834c7723e8fd932a51ae47ad23cc4c5c19cab119`
reports the contiguous 1,286-byte body, exception-state placement, and helper
calls. The byte-match and operand audits above use the official executable
separately.

Focused validation passes eight exact functions with one NONMATCHING. Full
validation passes 1,801/1,801 marked functions, with 283 NONMATCHING, zero
differences, and zero compile failures.
