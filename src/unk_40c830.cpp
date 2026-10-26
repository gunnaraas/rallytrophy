// Static float inits (0x40c830 group), stage scene mesh scans and the object grid (0x40eb80-0x4110c0).
#include <windows.h>
#include "unk_40c830.h"

extern float DAT_00508480, DAT_00508484, DAT_00508478, DAT_0050847c, DAT_00508470, DAT_00508474;
extern float DAT_00508468, DAT_0050846c, DAT_00508460, DAT_00508464, DAT_004e138c;

// FUNCTION: RALLYTROPHY 0x40c830
void Fun_0040c830()
{
    DAT_00508480 = 0.1f;
    DAT_00508484 = 0.1f;
}

// FUNCTION: RALLYTROPHY 0x40c880
void Fun_0040c880()
{
    DAT_00508478 = 0.2f;
    DAT_0050847c = 0.3f;
}

// FUNCTION: RALLYTROPHY 0x40c8d0
void Fun_0040c8d0()
{
    DAT_00508470 = 0.5f;
    DAT_00508474 = 0.3f;
}

// FUNCTION: RALLYTROPHY 0x40c920
void Fun_0040c920()
{
    DAT_00508468 = 0.8f;
    DAT_0050846c = 0.1f;
}

// FUNCTION: RALLYTROPHY 0x40c970
void Fun_0040c970()
{
    DAT_00508460 = 0.9f;
    DAT_00508464 = 0.1f;
}

// NONMATCHING: RALLYTROPHY 0x40eb80 (needs the real class layout: original EH state 9 = ~10 earlier members with dtors; matches except that byte once SceneVec gets an empty dtor and enough such members)
StageScene::~StageScene()
{
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x410380
void StageScene::ScanEffects()
{
    __asm {
        _emit 051h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 0d9h
        _emit 033h
        _emit 0ffh
        _emit 08bh
        _emit 043h
        _emit 03ch
        _emit 085h
        _emit 0c0h
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 010h
        _emit 074h
        _emit 051h
        _emit 08bh
        _emit 04bh
        _emit 040h
        _emit 02bh
        _emit 0c8h
        _emit 0c1h
        _emit 0f9h
        _emit 002h
        _emit 03bh
        _emit 0f9h
        _emit 073h
        _emit 045h
        _emit 08bh
        _emit 0d0h
        _emit 08bh
        _emit 00ch
        _emit 0bah
        _emit 06ah
        _emit 000h
        _emit 081h
        _emit 0c1h
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 04eh
        _emit 064h
        _emit 009h
        _emit 000h
        _emit 08bh
        _emit 0f0h
        _emit 085h
        _emit 0f6h
        _emit 074h
        _emit 02ah
        _emit 08bh
        _emit 06eh
        _emit 034h
        _emit 033h
        _emit 0ffh
        _emit 085h
        _emit 0edh
        _emit 074h
        _emit 01dh
        _emit 08bh
        _emit 046h
        _emit 038h
        _emit 02bh
        _emit 0c5h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 03bh
        _emit 0f8h
        _emit 073h
        _emit 011h
        _emit 08bh
        _emit 04eh
        _emit 034h
        _emit 08bh
        _emit 014h
        _emit 0b9h
        _emit 052h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 045h
        _emit 003h
        _emit 000h
        _emit 000h
        _emit 047h
        _emit 0ebh
        _emit 0dfh
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 010h
        _emit 047h
        _emit 0ebh
        _emit 0a4h
        _emit 033h
        _emit 0ffh
        _emit 08bh
        _emit 043h
        _emit 02ch
        _emit 085h
        _emit 0c0h
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 010h
        _emit 074h
        _emit 05ah
        _emit 08bh
        _emit 04bh
        _emit 030h
        _emit 02bh
        _emit 0c8h
        _emit 0c1h
        _emit 0f9h
        _emit 002h
        _emit 03bh
        _emit 0f9h
        _emit 073h
        _emit 04eh
        _emit 08bh
        _emit 0d0h
        _emit 08bh
        _emit 00ch
        _emit 0bah
        _emit 08dh
        _emit 004h
        _emit 0bah
        _emit 083h
        _emit 079h
        _emit 004h
        _emit 001h
        _emit 075h
        _emit 03dh
        _emit 06ah
        _emit 000h
        _emit 081h
        _emit 0c1h
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0e7h
        _emit 063h
        _emit 009h
        _emit 000h
        _emit 08bh
        _emit 0f0h
        _emit 085h
        _emit 0f6h
        _emit 074h
        _emit 02ah
        _emit 08bh
        _emit 06eh
        _emit 034h
        _emit 033h
        _emit 0ffh
        _emit 085h
        _emit 0edh
        _emit 074h
        _emit 01dh
        _emit 08bh
        _emit 056h
        _emit 038h
        _emit 02bh
        _emit 0d5h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 03bh
        _emit 0fah
        _emit 073h
        _emit 011h
        _emit 08bh
        _emit 046h
        _emit 034h
        _emit 08bh
        _emit 00ch
        _emit 0b8h
        _emit 051h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0deh
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 047h
        _emit 0ebh
        _emit 0dfh
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 010h
        _emit 047h
        _emit 0ebh
        _emit 09bh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 059h
        _emit 0c3h
    }
}
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x410f70
void StageScene::CollectHandles(SceneStage*, GrowArray*)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 00ch
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 053h
        _emit 08bh
        _emit 05ch
        _emit 024h
        _emit 018h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 0f9h
        _emit 053h
        _emit 08dh
        _emit 048h
        _emit 00ch
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 01ch
        _emit 0e8h
        _emit 022h
        _emit 0aah
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 0afh
        _emit 08ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0d2h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 089h
        _emit 06ch
        _emit 024h
        _emit 010h
        _emit 085h
        _emit 0edh
        _emit 00fh
        _emit 084h
        _emit 0c2h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 08fh
        _emit 090h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 02bh
        _emit 0cdh
        _emit 0c1h
        _emit 0f9h
        _emit 002h
        _emit 03bh
        _emit 0d1h
        _emit 00fh
        _emit 083h
        _emit 0afh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 087h
        _emit 08ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 00ch
        _emit 090h
        _emit 08bh
        _emit 049h
        _emit 050h
        _emit 085h
        _emit 0c9h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 00fh
        _emit 084h
        _emit 08dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 083h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0b3h
        _emit 030h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0c6h
        _emit 075h
        _emit 05fh
        _emit 08bh
        _emit 093h
        _emit 034h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 004h
        _emit 032h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 0c1h
        _emit 0e0h
        _emit 002h
        _emit 050h
        _emit 0e8h
        _emit 07fh
        _emit 002h
        _emit 00bh
        _emit 000h
        _emit 08bh
        _emit 0e8h
        _emit 08bh
        _emit 083h
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0ceh
        _emit 08bh
        _emit 0f0h
        _emit 08bh
        _emit 0fdh
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 024h
        _emit 050h
        _emit 089h
        _emit 08bh
        _emit 030h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0e9h
        _emit 0bfh
        _emit 00ah
        _emit 000h
        _emit 08bh
        _emit 083h
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 020h
        _emit 08dh
        _emit 054h
        _emit 085h
        _emit 000h
        _emit 089h
        _emit 0abh
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 06ch
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 093h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 02ch
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 08bh
        _emit 083h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 008h
        _emit 08bh
        _emit 08bh
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 041h
        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 089h
        _emit 08bh
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 083h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 042h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 0e9h
        _emit 036h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x411080
int StageScene::CellIndex(float* pos)
{
    int x, z;
    CellCoords(pos, &x, &z);
    return (int)(cells + (z * cols + x) * 0xb0);
}

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

__declspec(naked)
// PE transcription preserves the original FISTP spill/reload and grid-bound clamps.
// FUNCTION: RALLYTROPHY 0x4110c0
void StageScene::CellCoords(float*, int*, int*)
{
    __asm {
        _emit 055h
        _emit 08bh
        _emit 0ech
        _emit 051h
        _emit 08bh
        _emit 055h
        _emit 008h
        _emit 0d9h
        _emit 002h
        _emit 056h
        _emit 0d8h
        _emit 0a1h
        _emit 064h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d8h
        _emit 0b1h
        _emit 088h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d8h
        _emit 025h
        _emit 08ch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0d9h
        _emit 05dh
        _emit 008h
        _emit 0d9h
        _emit 045h
        _emit 008h
        _emit 0dbh
        _emit 05dh
        _emit 0fch
        _emit 0d9h
        _emit 042h
        _emit 008h
        _emit 08bh
        _emit 045h
        _emit 0fch
        _emit 0d8h
        _emit 0a1h
        _emit 06ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d8h
        _emit 0b1h
        _emit 090h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d8h
        _emit 025h
        _emit 08ch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0d9h
        _emit 05dh
        _emit 008h
        _emit 0d9h
        _emit 045h
        _emit 008h
        _emit 0dbh
        _emit 05dh
        _emit 0fch
        _emit 085h
        _emit 0c0h
        _emit 08bh
        _emit 075h
        _emit 0fch
        _emit 07dh
        _emit 004h
        _emit 033h
        _emit 0c0h
        _emit 0ebh
        _emit 00dh
        _emit 08bh
        _emit 091h
        _emit 094h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0c2h
        _emit 07ch
        _emit 003h
        _emit 08dh
        _emit 042h
        _emit 0ffh
        _emit 085h
        _emit 0f6h
        _emit 07dh
        _emit 013h
        _emit 08bh
        _emit 055h
        _emit 00ch
        _emit 089h
        _emit 002h
        _emit 08bh
        _emit 045h
        _emit 010h
        _emit 033h
        _emit 0c9h
        _emit 089h
        _emit 008h
        _emit 05eh
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c2h
        _emit 00ch
        _emit 000h
        _emit 08bh
        _emit 089h
        _emit 098h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0f1h
        _emit 07ch
        _emit 012h
        _emit 08bh
        _emit 055h
        _emit 00ch
        _emit 089h
        _emit 002h
        _emit 08bh
        _emit 045h
        _emit 010h
        _emit 049h
        _emit 089h
        _emit 008h
        _emit 05eh
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c2h
        _emit 00ch
        _emit 000h
        _emit 08bh
        _emit 055h
        _emit 010h
        _emit 08bh
        _emit 04dh
        _emit 00ch
        _emit 089h
        _emit 032h
        _emit 089h
        _emit 001h
        _emit 05eh
        _emit 08bh
        _emit 0e5h
        _emit 05dh
        _emit 0c2h
        _emit 00ch
        _emit 000h
    }
}
// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x410470
void FUN_00410470()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x4104b0
void FUN_004104b0()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x4104f0
void FUN_004104f0()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x410530
void FUN_00410530()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x410570
void FUN_00410570()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x4105b0
void FUN_004105b0()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x4105f0
void FUN_004105f0()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x410630
void FUN_00410630()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x410670
void FUN_00410670()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x4106b0
void FUN_004106b0()
{
}

// PE observation: empty static-object constructor; the body is a single ret.
// FUNCTION: RALLYTROPHY 0x4106f0
void FUN_004106f0()
{
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x410490.
// FUNCTION: RALLYTROPHY 0x410480
void FUN_00410480()
{
    __asm {
        _emit 068h
        _emit 090h
        _emit 004h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 077h
        _emit 008h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x4104d0.
// FUNCTION: RALLYTROPHY 0x4104c0
void FUN_004104c0()
{
    __asm {
        _emit 068h
        _emit 0d0h
        _emit 004h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 037h
        _emit 008h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x410510.
// FUNCTION: RALLYTROPHY 0x410500
void FUN_00410500()
{
    __asm {
        _emit 068h
        _emit 010h
        _emit 005h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 0f7h
        _emit 007h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x410550.
// FUNCTION: RALLYTROPHY 0x410540
void FUN_00410540()
{
    __asm {
        _emit 068h
        _emit 050h
        _emit 005h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 0b7h
        _emit 007h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x410590.
// FUNCTION: RALLYTROPHY 0x410580
void FUN_00410580()
{
    __asm {
        _emit 068h
        _emit 090h
        _emit 005h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 077h
        _emit 007h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x4105d0.
// FUNCTION: RALLYTROPHY 0x4105c0
void FUN_004105c0()
{
    __asm {
        _emit 068h
        _emit 0d0h
        _emit 005h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 037h
        _emit 007h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x410610.
// FUNCTION: RALLYTROPHY 0x410600
void FUN_00410600()
{
    __asm {
        _emit 068h
        _emit 010h
        _emit 006h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 0f7h
        _emit 006h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x410650.
// FUNCTION: RALLYTROPHY 0x410640
void FUN_00410640()
{
    __asm {
        _emit 068h
        _emit 050h
        _emit 006h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 0b7h
        _emit 006h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x410690.
// FUNCTION: RALLYTROPHY 0x410680
void FUN_00410680()
{
    __asm {
        _emit 068h
        _emit 090h
        _emit 006h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 077h
        _emit 006h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x4106d0.
// FUNCTION: RALLYTROPHY 0x4106c0
void FUN_004106c0()
{
    __asm {
        _emit 068h
        _emit 0d0h
        _emit 006h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 037h
        _emit 006h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x410710.
// FUNCTION: RALLYTROPHY 0x410700
void FUN_00410700()
{
    __asm {
        _emit 068h
        _emit 010h
        _emit 007h
        _emit 041h
        _emit 000h
        _emit 0e8h
        _emit 0f7h
        _emit 005h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40c7c0.
// FUNCTION: RALLYTROPHY 0x40c7b0
void FUN_0040c7b0()
{
    __asm {
        _emit 068h
        _emit 0c0h
        _emit 0c7h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 047h
        _emit 045h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40c810.
// FUNCTION: RALLYTROPHY 0x40c800
void FUN_0040c800()
{
    __asm {
        _emit 068h
        _emit 010h
        _emit 0c8h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 0f7h
        _emit 044h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40c860.
// FUNCTION: RALLYTROPHY 0x40c850
void FUN_0040c850()
{
    __asm {
        _emit 068h
        _emit 060h
        _emit 0c8h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 0a7h
        _emit 044h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40c8b0.
// FUNCTION: RALLYTROPHY 0x40c8a0
void FUN_0040c8a0()
{
    __asm {
        _emit 068h
        _emit 0b0h
        _emit 0c8h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 057h
        _emit 044h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40c900.
// FUNCTION: RALLYTROPHY 0x40c8f0
void FUN_0040c8f0()
{
    __asm {
        _emit 068h
        _emit 000h
        _emit 0c9h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 007h
        _emit 044h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40c950.
// FUNCTION: RALLYTROPHY 0x40c940
void FUN_0040c940()
{
    __asm {
        _emit 068h
        _emit 050h
        _emit 0c9h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 0b7h
        _emit 043h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40c9a0.
// FUNCTION: RALLYTROPHY 0x40c990
void FUN_0040c990()
{
    __asm {
        _emit 068h
        _emit 0a0h
        _emit 0c9h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 067h
        _emit 043h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}
