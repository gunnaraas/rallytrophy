// Shared HUD geometry helpers (0x439080-0x43916f).
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"

extern float DAT_004e1350;
extern float DAT_004e16d4;
extern float DAT_00579bd0;
extern float DAT_00579bd4;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x439080
// original: FUN_00439080
// The target leaves ECX cleared and retains its otherwise-unused stack store.
// Assembly and pseudocode: ev_1c6681c51a0e8928003868b8f4dd18ed9b27f612f657bf5dd18d4854b99dec08
void __cdecl HudPoint_Default(HudPoint* result)
{
    __asm {
        push ecx
        mov eax, dword ptr [esp+8]
        fld dword ptr [DAT_00579bd0]
        mov ecx, dword ptr [DAT_00579bd4]
        fstp dword ptr [eax]
        // Preserve the original zero displacement; VC6 canonicalizes [esp+0].
        _emit 0xc7
        _emit 0x44
        _emit 0x24
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
        _emit 0x00
        mov dword ptr [eax+4], ecx
        pop ecx
        ret
    }
}

// The standalone copy of UIRect's constructor (callers elsewhere inline it from include/unk_450ee0.h), defined on a
// layout twin so the shared inline definition stays as it is.
struct A_Rect {
    A_Rect(HudPoint topLeft, HudPoint bottomRight);
    float left, top, right, bottom;
    int enabled;
};

// FUNCTION: RALLYTROPHY 0x4390a0
// SYMBOL: ??0A_Rect@@QAE@VHudPoint@@0@Z
A_Rect::A_Rect(HudPoint topLeft, HudPoint bottomRight)
{
    left = topLeft.x;
    top = topLeft.y;
    right = bottomRight.x;
    bottom = bottomRight.y;
    enabled = 1;
}

// FUNCTION: RALLYTROPHY 0x4390d0
int UIRect::Contains(HudPoint point)
{
    BYTE* display = (BYTE*)DAT_0056b0ac;
    point.x *= DAT_004e1350 / (float)*(int*)(display + 4);
    point.y *= DAT_004e16d4 / (float)*(int*)(display + 8);
    if (point.x <= left || point.y <= top || point.x >= right || point.y >= bottom)
        return 0;
    return 1;
}

// FUNCTION: RALLYTROPHY 0x439140
// SYMBOL: _$E23
typedef void Generated_439140();

// FUNCTION: RALLYTROPHY 0x439160
// SYMBOL: _$E22
typedef void Generated_439160();
