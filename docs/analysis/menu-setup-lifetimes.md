# Menu setup lifetimes and boundaries

| Reconstructed method | Entry | Instruction bytes |
|---|---|---|
| `GameScreen24FC::Setup` | `0x42ea30` | 525 |
| `SlotMenu::Setup` | `0x44eb60` | 525 |

REA raw-byte evidence covers each entry through its following function:
`ev_770eda4b60286cc6c214e118b921ed6fb5aefc20b39f35b4307f40cb67f601e4`
(load screen) and
`ev_94392a4f4c7f0f32c6614794aa714c68ec62939dfe64d102fd817b4a11bb08d1`
(slot menu). These entries lack Ghidra function dossiers in the current
session, so boundary decoding and source-form comparisons below are local
PE/COFF observations.

## Source form

Storing the game pointer and initialized state before calculating label
coordinates, then reusing `y` for the second label, reproduces the target
stack slots. The source retains the same label assignments and rectangle
construction call. The focused matcher passes all 20 exact-marked functions
in the two modules, with three other functions still `NONMATCHING`.

The coordinate products also preserve operand identity. The x and title-y
products use the named design constant before the scale global in source.
The row origin uses `DAT_0056aeac * 260.0f`. Using the named ROWY float
produced a masked byte match but exchanged two relocation identities; the
literal reproduces the target global load and constant multiply at their
original sites. Sequential assignment restored those identities but moved
four instruction bytes, so that trial was discarded. These are compiler
observations, not original-source spelling claims.

Independent audit confirms all 25 relocations per function: 12 absolute
references at matching PE base-relocation sites and 13 direct calls.
Constants 54, 262, 610, 485, 260, and 42 match the original bytes. The row
constant is `0x4e1698` (`00008243`, little endian). Calls target the language
lookup, length-error helper, string helpers, and each screen's rectangle
builder. Neither function has an EH table or local owned object cleanup.

## Slot-menu inventory correction

The original SlotMenu return instruction occupies `0x44ed6a`–`0x44ed6c`.
Three NOPs at `0x44ed6d`–`0x44ed6f` align the next function at `0x44ed70`.
Thus the body is 525 bytes, not the 528-byte span recorded previously in
`analysis/functions_extra.csv`. The load-screen function has the same
525-byte/three-NOP layout and was already recorded correctly. The SlotMenu
size correction excludes alignment padding; no instructions are omitted.
The evidence above and independent local PE disassembly confirm this.

The relocation-copy helper stops on the unresolved `std::_Xlen` symbol, so
it does not produce copies of these methods. Object comparison and the
independent relocation audit provide the checks here. Runtime execution
was not tested.
