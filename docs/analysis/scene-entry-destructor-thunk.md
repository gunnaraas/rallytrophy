# Scene-entry destructor thunk (`0x4a66a0`)

Local PE disassembly observation: this five-byte procedure is a direct jump
to `SceneEntry::~SceneEntry` at `0x4b89f0`. The inventory records the same
target as `thunk_FUN_004b89f0`.

The reconstruction uses a naked symbolic jump instead of embedding the
original relative displacement. Its COFF object contains one `DISP32`
relocation at byte offset 1 to `_RT_SceneEntryDtor4a66a0`. The linker alias
maps that name to `??1SceneEntry@@QAE@XZ`, which is defined by the existing
destructor in `src/unk_4b83a0.cpp`. The alias directive and definition were
checked in the generated object files.

The focused matcher and `objcmp` confirm the five-byte body with its one
relocation accounted for. The symbol-target audit establishes the branch
destination separately; this check does not establish a fully linked game.
