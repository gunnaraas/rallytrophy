# Graphics-menu layout loader (`0x42aee0`)

Local PE disassembly and COFF comparison observations: this loader constructs
25 rectangles from `data/menu/graphicsmenu.ini`. The prior draft was 5,031
bytes with 763 differences against the 5,014-byte target. Config/string setup,
all rectangle blocks, and the first 23 insertions already agreed; the last two
insertions expanded vector helpers to different depths.

`H3c_RectVector` preserves the VC6 allocator/first/last/end layout, checked at
compile time. The penultimate insertion expands prefix copy and fill into
loops calling Construct; suffix copy, destruction, and final size remain calls.
The final insertion expands prefix copy and fill through allocator construction
into guarded pointer stores. Suffix copy remains a loop calling Construct.
Destruction and size expand inline, retaining the real allocator-destroy loop.
In its spare-capacity paths, the first copy uses guarded stores, while fill and
second copy use loops calling Construct.

The range-assignment loop is kept in its own inline helper, mirroring the
standard header's `fill` boundary. Writing it directly inside the insertion
removed a loop still emitted by the original, producing 4,996 bytes. Restoring
the helper completed a 5,014-byte match with 251 relocations. An intermediate
penultimate-only twin produced 5,029 bytes; those probe states were superseded.

All 11 stand-in aliases resolve to existing definitions, and all 64 calls
through these aliases agree with the original PE destinations. The focused
module check passes 11 exact functions with one still NONMATCHING. The config
key table `DAT_004f6e18` remains external; verification covers function/object
code rather than a linked game.

REA instruction reference: `ev_50bf9d8d1b1735445bceb150d81e3c4e3d3726de9ca8a5b01a47e3cc51aa9b8b`
reports the contiguous 5,014-byte body and the helper call sequence. The
byte-match and operand audits above use the official executable separately.

Full validation passes 1,800/1,800 marked functions, with 284 NONMATCHING,
zero differences, and zero compile failures.
