# TextRenderer name-tag wrapper

`TextRenderer::DrawNameTag` (`0x4a9b00`, 59 bytes) reads its fourth stack argument, preserves ESI, and calls `0x4b7670` with buffer `0x57a7d8`, the player pointer, and `-1`. It then forwards the drawing arguments and same buffer to `0x4a9b40`, restores ESI, and returns with `ret 0x14`. This sequence is a plain observation from the shipped PE disassembly (`objdump -d -Mintel`); the source reproduces the exact wrapper in `src/text_renderer_name_tag.cpp`.

The class declaration is `TextRenderer::DrawNameTag(RenderDevice*, ViewPoint*, Vec3Slot*, Player*, float)` in `include/unk_481390.h:689`. The known caller is the name-tag loop at `src/unk_4856e0.cpp:1408`.

`FUN_004a9b40` (`0x4a9b40`, 2277 bytes) is the drawing helper forwarded to by the wrapper. Function dossier `ev_fd9f1bcdecacc634774f1ddc8cd0fd7e66af694b044ca223c84e1d619babccee` observes resource setup, text iteration with space/newline handling, glyph geometry assembly, and per-glyph renderer submission. The exact body is recorded as emitted PE instruction bytes in `src/text_renderer_name_tag.cpp`; static disassembly establishes the instruction sequence, while the operation labels summarize that flow.
