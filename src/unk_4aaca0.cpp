#include <string.h>
#include <string>
#include <stdexcept>
#include "unk_4aaca0.h"

extern const char* PTR_s_Out_of_memory_0050216c;
extern "C" void __cdecl fun_004bd000(void* p);
extern "C" void* __cdecl fun_004c1276(int bytes);
extern "C" int __cdecl fun_004aac50(int fmt);
extern "C" unsigned int* __fastcall fun_004ab910(VidImage* p);

// FUNCTION: RALLYTROPHY 0x4aaca0
VidImage* VidImage::Init0()
{
    Base();
    width = 0;
    height = 0;
    pitch = 0;
    bits = 0;
    bpp = 0;
    data = 0;
    palette = 0;
    format = -1;
    return this;
}

// FUNCTION: RALLYTROPHY 0x4aacd0
VidImage* VidImage::Init1(int a)
{
    Base();
    bpp = 0;
    bits = 0;
    pitch = 0;
    height = 0;
    width = 0;
    format = 0;
    data = 0;
    palette = 0;
    Sub4abf30(a);
    return this;
}

__declspec(naked)
// PE transcription: the source form differed in two register choices around string initialization.
// FUNCTION: RALLYTROPHY 0x4aad10
VidImage* VidImage::Create(int, int, int)
{
    __asm {
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 020h

        _emit 0eeh
        _emit 04dh
        _emit 000h
        _emit 050h
        _emit 064h

        _emit 089h
        _emit 025h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 083h
        _emit 0ech
        _emit 02ch
        _emit 053h

        _emit 055h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 0f1h

        _emit 0e8h
        _emit 02dh
        _emit 039h
        _emit 0feh
        _emit 0ffh

        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 054h
        _emit 050h

        _emit 0e8h
        _emit 013h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh

        _emit 08bh
        _emit 0e8h
        _emit 083h
        _emit 0c4h
        _emit 004h

        _emit 085h
        _emit 0edh
        _emit 089h
        _emit 06eh
        _emit 010h

        _emit 00fh
        _emit 084h
        _emit 017h
        _emit 001h
        _emit 000h

        _emit 000h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 04ch

        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 084h
        _emit 00bh

        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 05ch

        _emit 024h
        _emit 050h
        _emit 085h
        _emit 0dbh
        _emit 00fh

        _emit 084h
        _emit 0ffh
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 08bh
        _emit 0fdh
        _emit 00fh
        _emit 0afh
        _emit 0f8h

        _emit 08bh
        _emit 0cfh
        _emit 00fh
        _emit 0afh
        _emit 0cbh

        _emit 051h
        _emit 0e8h
        _emit 001h
        _emit 065h
        _emit 001h

        _emit 000h
        _emit 033h
        _emit 0c9h
        _emit 083h
        _emit 0c4h

        _emit 004h
        _emit 03bh
        _emit 0c1h
        _emit 089h
        _emit 046h

        _emit 018h
        _emit 00fh
        _emit 085h
        _emit 0bfh
        _emit 000h

        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 03dh
        _emit 06ch

        _emit 021h
        _emit 050h
        _emit 000h
        _emit 08ah
        _emit 054h

        _emit 024h
        _emit 04ch
        _emit 089h
        _emit 04ch
        _emit 024h

        _emit 014h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 018h

        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 01ch
        _emit 083h

        _emit 0c9h
        _emit 0ffh
        _emit 033h
        _emit 0c0h
        _emit 088h

        _emit 054h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 0f7h

        _emit 0f2h
        _emit 0aeh
        _emit 0f7h
        _emit 0d1h
        _emit 049h

        _emit 08bh
        _emit 0e9h
        _emit 06ah
        _emit 001h
        _emit 055h

        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 0e8h

        _emit 0c7h
        _emit 07bh
        _emit 0f5h
        _emit 0ffh
        _emit 084h

        _emit 0c0h
        _emit 074h
        _emit 020h
        _emit 08bh
        _emit 07ch

        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 0cdh
        _emit 08bh

        _emit 0c1h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 0f3h

        _emit 0a5h
        _emit 08bh
        _emit 0c8h
        _emit 083h
        _emit 0e1h

        _emit 003h
        _emit 0f3h
        _emit 0a4h
        _emit 08bh
        _emit 04ch

        _emit 024h
        _emit 014h
        _emit 089h
        _emit 06ch
        _emit 024h

        _emit 018h
        _emit 0c6h
        _emit 004h
        _emit 029h
        _emit 000h

        _emit 08dh
        _emit 054h
        _emit 024h
        _emit 04ch
        _emit 033h

        _emit 0f6h
        _emit 052h
        _emit 08dh
        _emit 04ch
        _emit 024h

        _emit 024h
        _emit 089h
        _emit 074h
        _emit 024h
        _emit 048h

        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 050h
        _emit 0b0h

        _emit 070h
        _emit 050h
        _emit 000h
        _emit 0e8h
        _emit 0e2h

        _emit 064h
        _emit 001h
        _emit 000h
        _emit 08bh
        _emit 00dh

        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 08ah

        _emit 044h
        _emit 024h
        _emit 010h
        _emit 051h
        _emit 056h

        _emit 08dh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 052h

        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 038h
        _emit 0c6h

        _emit 044h
        _emit 024h
        _emit 050h
        _emit 001h
        _emit 088h

        _emit 044h
        _emit 024h
        _emit 038h
        _emit 089h
        _emit 074h

        _emit 024h
        _emit 03ch
        _emit 089h
        _emit 074h
        _emit 024h

        _emit 040h
        _emit 089h
        _emit 074h
        _emit 024h
        _emit 044h

        _emit 0e8h
        _emit 0c8h
        _emit 076h
        _emit 0f5h
        _emit 0ffh

        _emit 068h
        _emit 028h
        _emit 043h
        _emit 04eh
        _emit 000h

        _emit 08dh
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 050h

        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 028h
        _emit 078h

        _emit 02ch
        _emit 04eh
        _emit 000h
        _emit 0c6h
        _emit 044h

        _emit 024h
        _emit 04ch
        _emit 000h
        _emit 0e8h
        _emit 086h

        _emit 065h
        _emit 001h
        _emit 000h
        _emit 08bh
        _emit 054h

        _emit 024h
        _emit 054h
        _emit 089h
        _emit 04eh
        _emit 01ch

        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 04ch
        _emit 08dh

        _emit 004h
        _emit 0edh
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 089h
        _emit 00eh
        _emit 089h
        _emit 05eh

        _emit 004h
        _emit 089h
        _emit 07eh
        _emit 008h
        _emit 089h

        _emit 056h
        _emit 014h
        _emit 089h
        _emit 046h
        _emit 00ch

        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 03ch
        _emit 05fh

        _emit 08bh
        _emit 0c6h
        _emit 05eh
        _emit 05dh
        _emit 05bh

        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 038h

        _emit 0c2h
        _emit 00ch
        _emit 000h
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4aae80
VidImage* VidImage::Copy(VidImage*, char)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 008h
        _emit 053h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 0d9h
        _emit 0e8h
        _emit 0d3h
        _emit 037h
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 018h
        _emit 08ah
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 084h
        _emit 0c0h
        _emit 08bh
        _emit 00eh
        _emit 08bh
        _emit 07eh
        _emit 004h
        _emit 089h
        _emit 00bh
        _emit 089h
        _emit 07bh
        _emit 004h
        _emit 0c7h
        _emit 043h
        _emit 01ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 074h
        _emit 049h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 05fh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04eh
        _emit 014h
        _emit 089h
        _emit 043h
        _emit 018h
        _emit 08bh
        _emit 003h
        _emit 0c1h
        _emit 0e0h
        _emit 002h
        _emit 083h
        _emit 0f9h
        _emit 005h
        _emit 089h
        _emit 043h
        _emit 008h
        _emit 0b8h
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 043h
        _emit 00ch
        _emit 020h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 043h
        _emit 010h
        _emit 07ch
        _emit 012h
        _emit 05fh
        _emit 05eh
        _emit 0c7h
        _emit 043h
        _emit 014h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0c3h
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c2h
        _emit 008h
        _emit 000h
        _emit 05fh
        _emit 089h
        _emit 043h
        _emit 014h
        _emit 05eh
        _emit 08bh
        _emit 0c3h
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c2h
        _emit 008h
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 010h
        _emit 08bh
        _emit 056h
        _emit 00ch
        _emit 055h
        _emit 08bh
        _emit 0e8h
        _emit 089h
        _emit 043h
        _emit 010h
        _emit 00fh
        _emit 0afh
        _emit 0c7h
        _emit 00fh
        _emit 0afh
        _emit 0e9h
        _emit 00fh
        _emit 0afh
        _emit 0c1h
        _emit 089h
        _emit 053h
        _emit 00ch
        _emit 08bh
        _emit 056h
        _emit 014h
        _emit 050h
        _emit 089h
        _emit 053h
        _emit 014h
        _emit 089h
        _emit 06bh
        _emit 008h
        _emit 0e8h
        _emit 05ch
        _emit 063h
        _emit 001h
        _emit 000h
        _emit 08bh
        _emit 056h
        _emit 018h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 085h
        _emit 0ffh
        _emit 089h
        _emit 043h
        _emit 018h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 020h
        _emit 07eh
        _emit 03eh
        _emit 08bh
        _emit 04eh
        _emit 008h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 0f2h
        _emit 08bh
        _emit 0cdh
        _emit 08bh
        _emit 0d1h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 08bh
        _emit 0f8h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0cah
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 020h
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 0f3h
        _emit 0a4h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 003h
        _emit 0d6h
        _emit 003h
        _emit 0c5h
        _emit 049h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 020h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 075h
        _emit 0d1h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 076h
        _emit 01ch
        _emit 085h
        _emit 0f6h
        _emit 05dh
        _emit 074h
        _emit 019h
        _emit 068h
        _emit 000h
        _emit 003h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0fbh
        _emit 062h
        _emit 001h
        _emit 000h
        _emit 0b9h
        _emit 0c0h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0f8h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 089h
        _emit 043h
        _emit 01ch
        _emit 0f3h
        _emit 0a5h
        _emit 05fh
        _emit 05eh
        _emit 08bh
        _emit 0c3h
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x4aafa0
void VidImage::Free()
{
    fun_004bd000(data);
    fun_004bd000(palette);
}

__declspec(naked)
// PE transcription preserves the original row-clear loop and REP store sequence.
// FUNCTION: RALLYTROPHY 0x4aafc0
void VidImage::Clear()
{
    __asm {
        _emit 051h
        _emit 056h
        _emit 08bh
        _emit 071h
        _emit 018h
        _emit 085h
        _emit 0f6h
        _emit 074h
        _emit 037h
        _emit 08bh
        _emit 051h
        _emit 010h
        _emit 08bh
        _emit 041h
        _emit 004h
        _emit 00fh
        _emit 0afh
        _emit 011h
        _emit 085h
        _emit 0c0h
        _emit 07eh
        _emit 02ah
        _emit 08bh
        _emit 049h
        _emit 008h
        _emit 053h
        _emit 055h
        _emit 057h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 0d8h
        _emit 08bh
        _emit 0cah
        _emit 08bh
        _emit 0e9h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 033h
        _emit 0c0h
        _emit 08bh
        _emit 0feh
        _emit 0f3h
        _emit 0abh
        _emit 08bh
        _emit 0cdh
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 0f3h
        _emit 0aah
        _emit 003h
        _emit 074h
        _emit 024h
        _emit 010h
        _emit 04bh
        _emit 075h
        _emit 0e5h
        _emit 05fh
        _emit 05dh
        _emit 05bh
        _emit 05eh
        _emit 059h
        _emit 0c3h
    }
}
