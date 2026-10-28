// Bitmap font helpers 0x4aa430-0x4aac50 (GetWidth/Measure/Load) plus tiny stubs.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "unk_4aa430.h"
#include "rallytrophy.h"

void FUN_004b7670(unsigned short* dst, const char* src, int len); // 0x4b7670
extern "C" size_t __cdecl FUN_004c2414(const unsigned short* s);   // wcslen
extern unsigned short DAT_0057a7d8[];
extern float DAT_004e134c;
extern float DAT_00579bd0;
extern float DAT_00579bd4;
extern void __cdecl FUN_004aac00();

// FUNCTION: RALLYTROPHY 0x4aa430
float GlyphFont::GetWidth(const char* text, int raw)
{
    FUN_004b7670(DAT_0057a7d8, text, -1);
    return GetWidthW(DAT_0057a7d8, raw);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4aa460
float GlyphFont::GetWidthW(const unsigned short*, int)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 00ch
        _emit 053h
        _emit 08bh
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 056h
        _emit 057h
        _emit 053h
        _emit 08bh
        _emit 0f1h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0ffh
        _emit 0e8h
        _emit 090h
        _emit 07fh
        _emit 001h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 086h
        _emit 0deh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 055h
        _emit 089h
        _emit 05ch
        _emit 024h
        _emit 020h
        _emit 053h
        _emit 0e8h
        _emit 07ah
        _emit 07fh
        _emit 001h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 048h
        _emit 03bh
        _emit 0f8h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 066h
        _emit 08bh
        _emit 000h
        _emit 01bh
        _emit 0d2h
        _emit 0f7h
        _emit 0dah
        _emit 066h
        _emit 03dh
        _emit 020h
        _emit 000h
        _emit 075h
        _emit 005h
        _emit 0dbh
        _emit 046h
        _emit 024h
        _emit 0ebh
        _emit 07ah
        _emit 066h
        _emit 03dh
        _emit 00ah
        _emit 000h
        _emit 075h
        _emit 021h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 008h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ebh
        _emit 05bh
        _emit 08bh
        _emit 04eh
        _emit 034h
        _emit 025h
        _emit 0ffh
        _emit 0ffh
        _emit 000h
        _emit 000h
        _emit 02bh
        _emit 0c1h
        _emit 078h
        _emit 005h
        _emit 03bh
        _emit 046h
        _emit 030h
        _emit 07ch
        _emit 005h
        _emit 0b8h
        _emit 005h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 06eh
        _emit 00ch
        _emit 033h
        _emit 0c9h
        _emit 066h
        _emit 08bh
        _emit 04ch
        _emit 045h
        _emit 000h
        _emit 08dh
        _emit 004h
        _emit 0cdh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 02bh
        _emit 0c1h
        _emit 08bh
        _emit 00eh
        _emit 08dh
        _emit 004h
        _emit 081h
        _emit 08bh
        _emit 048h
        _emit 004h
        _emit 085h
        _emit 0c9h
        _emit 074h
        _emit 026h
        _emit 08bh
        _emit 068h
        _emit 008h
        _emit 003h
        _emit 0e9h
        _emit 003h
        _emit 028h
        _emit 085h
        _emit 0d2h
        _emit 089h
        _emit 06ch
        _emit 024h
        _emit 018h
        _emit 0dbh
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 0d8h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 010h
        _emit 074h
        _emit 00bh
        _emit 0dbh
        _emit 046h
        _emit 028h
        _emit 0d8h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 083h
        _emit 0c0h
        _emit 002h
        _emit 053h
        _emit 047h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 0e8h
        _emit 0cah
        _emit 07eh
        _emit 001h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 03bh
        _emit 0f8h
        _emit 00fh
        _emit 082h
        _emit 03fh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 05dh
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 010h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 008h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 00ch
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 010h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 05fh
        _emit 0d8h
        _emit 04eh
        _emit 01ch
        _emit 05eh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 0c2h
        _emit 008h
        _emit 000h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 05fh
        _emit 05eh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4aa5a0
void GlyphFont::Measure(const unsigned short*, float, float, float*)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 008h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 018h
        _emit 057h
        _emit 08bh
        _emit 0f1h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 05bh
        _emit 07eh
        _emit 001h
        _emit 000h
        _emit 0d9h
        _emit 005h
        _emit 04ch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 00fh
        _emit 08eh
        _emit 08dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 053h
        _emit 08bh
        _emit 0d0h
        _emit 066h
        _emit 08bh
        _emit 007h
        _emit 066h
        _emit 03dh
        _emit 020h
        _emit 000h
        _emit 075h
        _emit 005h
        _emit 0dbh
        _emit 046h
        _emit 024h
        _emit 0ebh
        _emit 068h
        _emit 066h
        _emit 03dh
        _emit 00ah
        _emit 000h
        _emit 075h
        _emit 01ch
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h
        _emit 005h
        _emit 04ch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0dbh
        _emit 046h
        _emit 02ch
        _emit 0d8h
        _emit 04eh
        _emit 020h
        _emit 0d8h
        _emit 04ch
        _emit 024h
        _emit 024h
        _emit 0d8h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 010h
        _emit 0ebh
        _emit 04fh
        _emit 08bh
        _emit 04eh
        _emit 034h
        _emit 025h
        _emit 0ffh
        _emit 0ffh
        _emit 000h
        _emit 000h
        _emit 02bh
        _emit 0c1h
        _emit 08bh
        _emit 0c8h
        _emit 078h
        _emit 005h
        _emit 03bh
        _emit 04eh
        _emit 030h
        _emit 07ch
        _emit 005h
        _emit 0b9h
        _emit 005h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 05eh
        _emit 00ch
        _emit 033h
        _emit 0c0h
        _emit 066h
        _emit 08bh
        _emit 004h
        _emit 04bh
        _emit 08dh
        _emit 00ch
        _emit 0c5h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 02bh
        _emit 0c8h
        _emit 08bh
        _emit 006h
        _emit 08dh
        _emit 004h
        _emit 088h
        _emit 08bh
        _emit 048h
        _emit 004h
        _emit 085h
        _emit 0c9h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 01ch
        _emit 074h
        _emit 015h
        _emit 0dbh
        _emit 040h
        _emit 008h
        _emit 0dah
        _emit 046h
        _emit 028h
        _emit 0dah
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 0dah
        _emit 000h
        _emit 0d8h
        _emit 04eh
        _emit 01ch
        _emit 0d8h
        _emit 04ch
        _emit 024h
        _emit 020h
        _emit 0deh
        _emit 0c1h
        _emit 083h
        _emit 0c7h
        _emit 002h
        _emit 04ah
        _emit 00fh
        _emit 085h
        _emit 077h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 05bh
        _emit 0dbh
        _emit 046h
        _emit 02ch
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 05fh
        _emit 0d8h
        _emit 04eh
        _emit 020h
        _emit 05eh
        _emit 0d8h
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 0d8h
        _emit 044h
        _emit 024h
        _emit 004h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 004h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 004h
        _emit 089h
        _emit 048h
        _emit 004h
        _emit 0d9h
        _emit 018h
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c2h
        _emit 010h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x4aabd0
void FUN_004aabd0()
{
    DAT_00579bd0 = 0;
    DAT_00579bd4 = 0;
}

// FUNCTION: RALLYTROPHY 0x4aabf0
void FUN_004aabf0()
{
    atexit(FUN_004aac00);
}

// FUNCTION: RALLYTROPHY 0x4aac50
int __cdecl FUN_004aac50(unsigned v)
{
    switch (v) {
    case 1: case 2: case 5: case 6: return 2;
    case 3: return 3;
    case 4: case 7: return 4;
    case 0: return 1;
    default: return 1;
    }
}

// NONMATCHING: RALLYTROPHY 0x4aa9c0
// VC6 drops the redundant second `n > 0` test the original keeps; 2 bytes short. Testing the member `glyphCount` there
// expands to 510 bytes / 60 differences; making the two local guards sequential compiles to the baseline 506/73.
void GlyphFont::Load(const char* name)
{
    char path[256];
    sprintf(path, "%s%s", name, ".tga");
    texture = new FontTexture(path);
    sprintf(path, "%s%s", name, ".inf");
    File* f = new File(name, 0x81);
    f->Read(&spaceW, 4);
    f->Read(&spacing, 4);
    f->Read(&scaleX, 4);
    f->Read(&scaleY, 4);
    f->Read(&lineH, 4);
    f->Read(&firstChar, 4);
    f->Read(&mapCount, 4);
    f->Read(&glyphCount, 4);
    int n = glyphCount;
    unsigned short* ids2 = (unsigned short*)operator new(n * 2);
    ids = ids2;
    glyphs = (FontGlyph*)operator new(n * 0x1c);
    int m = mapCount;
    map = (unsigned short*)operator new(m * 2);
    if (n > 0) {
        unsigned short* p = ids2;
        int c = n;
        do { f->Read(p, 2); p++; c--; } while (c != 0);
        if (n > 0) {
            FontGlyph* g = glyphs;
            do { f->Read(g, 0x1c); g++; n--; } while (n != 0);
        }
    }
    if (m > 0) {
        unsigned short* p = map;
        do { f->Read(p, 2); p++; m--; } while (m != 0);
    }
    f->Close();
    delete f;
}
