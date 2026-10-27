# Race-options layout loader (`0x440890`)

Local PE disassembly and COFF comparison observations: this loader constructs
13 rectangles from `data/extras/racemenu.ini`. Its draft was 2,703 bytes with
257 differences against the 2,722-byte target. The first twelve blocks matched;
the final insertion called `size()` where the target expanded it.

`B3_RectVector` explicitly expands the final insertion with inline size and
pointer destruction. Allocation prefix, fill, and suffix use inline loops
that call Construct; the spare-capacity paths retain two Ucopy calls and one
Ufill call. The twin uses the real allocator and vector layout with a
compile-time size check. All 11 string/geometry/vector aliases resolve to
existing definitions, and all 35 calls through them match original PE targets.

Replacing the initial empty destruction placeholder with the actual
allocator-destroy loop preserved a four-byte spill from the target. Although
destroying pointer elements has no runtime action, the source boundary affects
VC6's temporary bookkeeping. That change completed the exact 2,722-byte C++
match with 138 relocations.

The pre-existing `B3_Nop` budget balance remains necessary in this source form.
Removing it before the twin produced 3,009 bytes / 1,120 differences;
removing it with the twin produced 3,043 / 1,161. An empty destroy placeholder
produced 2,718 / 265. These variants were reverted.

The focused module check passes nine exact-marked functions, with one still
NONMATCHING. The shared `DAT_004f8f58` config-key table remains external;
these checks verify function/object code rather than a linked game.

Full validation passes 1,799/1,799 marked functions, with 285 NONMATCHING,
zero differences, and zero compile failures.
