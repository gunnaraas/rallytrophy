# Network-options setup (`0x439310`)

Local official-PE disassembly and COFF comparison observations: this setup
assigns 18 labels and lays out five rows. The prior draft was 1,443 bytes
against a 1,401-byte target. It expanded the fourth string assignment where
the target calls assign, and initialized the game/state fields before the
coordinate loop rather than after it.

`NS_String` preserves the VC6 string layout. The first four assignments expand
strlen and call assign(const char*, length). The remaining fourteen expand
strlen, Grow(length,true), conditional copy, and Eos(length), with Grow and Eos
remaining calls. These strings already exist; no construction or Tidy is added.

The coordinate setup reads the original float constants and evaluates y, x,
then step. Reversing the prior x/y order fixed the final five differences.
Begin(game) now follows the five-row loop and precedes the back-row override.
The final call uses BuildRects, matching 0x43a080; the old DrawSetup call had
no corresponding definition or alias in the repository.

The result is a 1,401-byte match with 64 relocations. All three string aliases
resolve. All 45 direct call destinations agree with the PE, including BuildRects.
All 19 data operands were checked: 12 text references, six float references,
and the renderer global. The four coordinate constants also retain their
original identities (262 at 0x4e1704, 54 at 0x4e170c, 42 at 0x4e1708,
and 610 at 0x4e1720).

Focused checks pass seven exact functions with two still NONMATCHING. The
commit candidate was also compiled with the unchanged, committed hit-box draft
to verify that the pending geometry correction is not required for this match.
These checks establish function/object code; they do not verify a linked game.

REA does not expose a function dossier at this address in its current inventory.
Raw-byte evidence `ev_bdf0ae3d7ee10e6cfc76b4b9c1e165c4d3a2c1836be004a816c72dc5574eb82f`
covers the complete 1,408-byte span; those bytes were verified equal to the
official PE. The decoded instructions and COFF comparison above are local
observations over those bytes.

The extra inventory previously recorded the aligned 1,408-byte span. The
return ends at 0x439888; seven NOPs at 0x439889–0x43988f precede the next
function at 0x439890. The inventory now records the 1,401-byte body, excluding
only this alignment padding. The focused matcher initially exposed these
seven extra bytes; the first full run was cancelled to apply this correction.

Full validation passes 1,802/1,802 marked functions, with 282 NONMATCHING,
zero differences, and zero compile failures.
