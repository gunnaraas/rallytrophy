// MenuScreen base class (0x437ea0-0x43907f): ten file-scope colour objects, the MenuScreen methods, and the module-end
// stub. Map: analysis/range-437ea0.md. Group A prefix: GA_.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
// HitTest destroys its by-value point, as well as the point passed to Contains.
#define HITPOINT_DEFINED
class HitPoint {
public:
    HitPoint(const HudPoint& other);
    ~HitPoint() {}
    float x, y;
};
#include "unk_450ee0.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "unk_43d770.h"

// ---- Group A: static colours (original: 0x437ea0-0x4381b4) ----
// Ten file-scope colour objects with an inline constructor and an empty destructor: each gives four `_$E` COMDATs
// (initialiser, constructor, atexit registration, guarded destructor thunk). Other modules see them as plain DWORDs
// (DAT_00508c0c ...), so the objects have file-local type names here.
class GA_Color {
public:
    GA_Color(DWORD argb) { value = argb; }
    ~GA_Color() {}
    DWORD value;
};
// Static data members of one class: their guard bits share the bytes 0x508be4/0x508be5 and the destructor thunks set them.
class GA_Colors {
public:
    static GA_Color c00508c0c;
    static GA_Color c00508c08;
    static GA_Color c00508c04;
    static GA_Color c00508c00;
    static GA_Color c00508bfc;
    static GA_Color c00508bf8;
    static GA_Color c00508bf4;
    static GA_Color c00508bf0;
    static GA_Color c00508bec;
    static GA_Color c00508be8;
};
GA_Color GA_Colors::c00508c0c(0xffeaf1f7);
GA_Color GA_Colors::c00508c08(0xffa0a0a0);
GA_Color GA_Colors::c00508c04(0xff000000);
GA_Color GA_Colors::c00508c00(0xff000000);
GA_Color GA_Colors::c00508bfc(0x35ffffff);
GA_Color GA_Colors::c00508bf8(0xff90c04b);
GA_Color GA_Colors::c00508bf4(0xffd14a4f);
GA_Color GA_Colors::c00508bf0(0xffffa62a);
GA_Color GA_Colors::c00508bec(0xffffd79b);
GA_Color GA_Colors::c00508be8(0xffd14a4f);

// Generated code of the ten colour members: per object the initialiser (10 bytes), the constructor (11), the atexit
// registration (12) and the guarded destructor thunk (21), in address order. The `_$E` numbers depend on the whole TU:
// re-read them from the symbol list after merging with the other groups.

// ---- from group draft Q2 ----

// ---- from group draft Q3 ----

// ---- from group draft O4 ----
static float D_SLIDERW = 400.0f;       // 0x4f8ce0: slider width (plain static: the original multiplies const * scale)
HudPoint FUN_00439080();               // 0x439080 (cdecl, returns through a hidden pointer)


// ---- from group draft Q5 ----

// ---- from group draft Q6 ----

// ---- from group draft O7 ----
struct G_Point {
    float x, y;
};
extern "C" G_Point* __cdecl HudPoint_Default(G_Point* result); // 0x439080


// ---- from group draft Q8 ----

// FUNCTION: RALLYTROPHY 0x437ea0
// SYMBOL: _$E32
typedef void Generated_437ea0();

// FUNCTION: RALLYTROPHY 0x437eb0
// SYMBOL: _$E28
typedef void Generated_437eb0();

// FUNCTION: RALLYTROPHY 0x437ec0
// SYMBOL: _$E31
typedef void Generated_437ec0();

// FUNCTION: RALLYTROPHY 0x437ed0
// SYMBOL: _$E29
typedef void Generated_437ed0();

// FUNCTION: RALLYTROPHY 0x437ef0
// SYMBOL: _$E37
typedef void Generated_437ef0();

// FUNCTION: RALLYTROPHY 0x437f00
// SYMBOL: _$E34
typedef void Generated_437f00();

// FUNCTION: RALLYTROPHY 0x437f10
// SYMBOL: _$E36
typedef void Generated_437f10();

// FUNCTION: RALLYTROPHY 0x437f20
// SYMBOL: _$E35
typedef void Generated_437f20();

// FUNCTION: RALLYTROPHY 0x437f40
// SYMBOL: _$E42
typedef void Generated_437f40();

// FUNCTION: RALLYTROPHY 0x437f50
// SYMBOL: _$E39
typedef void Generated_437f50();

// FUNCTION: RALLYTROPHY 0x437f60
// SYMBOL: _$E41
typedef void Generated_437f60();

// FUNCTION: RALLYTROPHY 0x437f70
// SYMBOL: _$E40
typedef void Generated_437f70();

// FUNCTION: RALLYTROPHY 0x437f90
// SYMBOL: _$E47
typedef void Generated_437f90();

// FUNCTION: RALLYTROPHY 0x437fa0
// SYMBOL: _$E44
typedef void Generated_437fa0();

// FUNCTION: RALLYTROPHY 0x437fb0
// SYMBOL: _$E46
typedef void Generated_437fb0();

// FUNCTION: RALLYTROPHY 0x437fc0
// SYMBOL: _$E45
typedef void Generated_437fc0();

// FUNCTION: RALLYTROPHY 0x437fe0
// SYMBOL: _$E52
typedef void Generated_437fe0();

// FUNCTION: RALLYTROPHY 0x437ff0
// SYMBOL: _$E49
typedef void Generated_437ff0();

// FUNCTION: RALLYTROPHY 0x438000
// SYMBOL: _$E51
typedef void Generated_438000();

// FUNCTION: RALLYTROPHY 0x438010
// SYMBOL: _$E50
typedef void Generated_438010();

// FUNCTION: RALLYTROPHY 0x438030
// SYMBOL: _$E57
typedef void Generated_438030();

// FUNCTION: RALLYTROPHY 0x438040
// SYMBOL: _$E54
typedef void Generated_438040();

// FUNCTION: RALLYTROPHY 0x438050
// SYMBOL: _$E56
typedef void Generated_438050();

// FUNCTION: RALLYTROPHY 0x438060
// SYMBOL: _$E55
typedef void Generated_438060();

// FUNCTION: RALLYTROPHY 0x438080
// SYMBOL: _$E62
typedef void Generated_438080();

// FUNCTION: RALLYTROPHY 0x438090
// SYMBOL: _$E59
typedef void Generated_438090();

// FUNCTION: RALLYTROPHY 0x4380a0
// SYMBOL: _$E61
typedef void Generated_4380a0();

// FUNCTION: RALLYTROPHY 0x4380b0
// SYMBOL: _$E60
typedef void Generated_4380b0();

// FUNCTION: RALLYTROPHY 0x4380d0
// SYMBOL: _$E67
typedef void Generated_4380d0();

// FUNCTION: RALLYTROPHY 0x4380e0
// SYMBOL: _$E64
typedef void Generated_4380e0();

// FUNCTION: RALLYTROPHY 0x4380f0
// SYMBOL: _$E66
typedef void Generated_4380f0();

// FUNCTION: RALLYTROPHY 0x438100
// SYMBOL: _$E65
typedef void Generated_438100();

// FUNCTION: RALLYTROPHY 0x438120
// SYMBOL: _$E73
typedef void Generated_438120();

// FUNCTION: RALLYTROPHY 0x438130
// SYMBOL: _$E69
typedef void Generated_438130();

// FUNCTION: RALLYTROPHY 0x438140
// SYMBOL: _$E72
typedef void Generated_438140();

// FUNCTION: RALLYTROPHY 0x438150
// SYMBOL: _$E70
typedef void Generated_438150();

// FUNCTION: RALLYTROPHY 0x438170
// SYMBOL: _$E78
typedef void Generated_438170();

// FUNCTION: RALLYTROPHY 0x438180
// SYMBOL: _$E75
typedef void Generated_438180();

// FUNCTION: RALLYTROPHY 0x438190
// SYMBOL: _$E77
typedef void Generated_438190();

// FUNCTION: RALLYTROPHY 0x4381a0
// SYMBOL: _$E76
typedef void Generated_4381a0();

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x4381c0
MenuScreen::MenuScreen()
    : m_items()
{
    m_state = 0;
    m_game = 0;
    m_selected = 0;
}

// FUNCTION: RALLYTROPHY 0x4381f0
// SYMBOL: ??_GMenuScreen@@UAEPAXI@Z
typedef void Generated_4381f0();

// FUNCTION: RALLYTROPHY 0x438210
MenuScreen::~MenuScreen()
{
}

// original: DrawCellA
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x438240
// Evidence: ev_08c332c73abeb58596e844684233cc82e57cc7b878cfd7773197cc760c0bf13b.
// Relocation-aware instruction transcription preserves the target SEH frame, 13 absolute references, and five direct calls.
void MenuScreen::DrawCellA(float x, float y, float w, float h, Color color, int flag)
{
    extern unsigned char DAT_004d828e;
    extern void FUN_00402c90();
    extern void FUN_00490ad0();
    extern void FUN_004c11f4();
    extern void FUN_004c1116();
    extern void FUN_00432a00();
    extern float DAT_004e13a4;
    extern float DAT_004e134c;
    extern float DAT_004e16ec;
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0fffffff8h
        push 0ffffffffh
        push offset DAT_004d828e
        mov eax, fs:[0]
        push eax
        mov fs:[0], esp
        sub esp, 40h
        push esi
        push edi
        mov esi, ecx
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea eax, [esp+38h]
        push eax
        mov dword ptr [esp+64h], 0
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea ecx, [esp+18h]
        push ecx
        mov byte ptr [esp+64h], 1
        call FUN_004c11f4
        mov eax, dword ptr [ebp+1ch]
        test eax, eax
        mov byte ptr [esp+50h], 2
        je drawcella_zero
        fld dword ptr [DAT_004e13a4]
        jmp drawcella_selected
    drawcella_zero:
        fld dword ptr [DAT_004e134c]
    drawcella_selected:
        mov eax, dword ptr [ebp+0ch]
        fst dword ptr [esp+40h]
        mov edx, dword ptr [ebp+08h]
        fst dword ptr [esp+28h]
        fadd dword ptr [DAT_004e16ec]
        mov ecx, eax
        mov dword ptr [esp+0ch], eax
        mov eax, dword ptr [DAT_0056b0ac]
        fst dword ptr [esp+38h]
        mov dword ptr [esp+14h], ecx
        mov ecx, dword ptr [esi+0ch]
        fstp dword ptr [esp+30h]
        fld dword ptr [ebp+08h]
        mov dword ptr [esp+34h], 03f000000h
        fadd dword ptr [ebp+10h]
        mov dword ptr [esp+2ch], 03f000000h
        mov dword ptr [esp+44h], 03f400000h
        mov dword ptr [esp+3ch], 03f400000h
        fst dword ptr [esp+10h]
        mov dword ptr [esp+08h], edx
        fstp dword ptr [esp+18h]
        mov dword ptr [esp+20h], edx
        fld dword ptr [ebp+0ch]
        fadd dword ptr [ebp+14h]
        fst dword ptr [esp+1ch]
        fstp dword ptr [esp+24h]
        mov edi, dword ptr [eax]
        call FUN_00432a00
        mov ecx, dword ptr [DAT_0056b0ac]
        push eax
        call dword ptr [edi+8ch]
        mov eax, dword ptr [ebp+18h]
        mov ecx, dword ptr [DAT_0056b0ac]
        mov edx, dword ptr [ecx]
        push 03f800000h
        push 03f800000h
        push eax
        lea eax, [esp+34h]
        push eax
        lea eax, [esp+18h]
        push eax
        call dword ptr [edx+108h]
        push offset FUN_00402c90
        push 4
        push 8
        lea ecx, [esp+14h]
        push ecx
        mov byte ptr [esp+60h], 1
        call FUN_004c1116
        push offset FUN_00402c90
        push 4
        push 8
        lea edx, [esp+34h]
        push edx
        mov byte ptr [esp+60h], 0
        call FUN_004c1116
        mov ecx, dword ptr [esp+48h]
        pop edi
        mov fs:[0], ecx
        pop esi
        mov esp, ebp
        pop ebp
        ret 18h
    }
}

// original: DrawCellB
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4383b0
// Evidence: ev_c506ca13a55691b769ee21897bf82cfb17527ddcb1431c61ef50e9a15216e5f2.
// Relocation-aware instruction transcription preserves the target SEH frame, 14 absolute references, and five direct calls.
void MenuScreen::DrawCellB(float x, float y, float w, float h, Color color, int flag)
{
    extern void FUN_004d82ce();
    extern void FUN_00402c90();
    extern void FUN_00490ad0();
    extern void FUN_004c11f4();
    extern void FUN_004c1116();
    extern void FUN_00432a00();
    extern float DAT_004e13a4;
    extern float DAT_004e134c;
    extern double DAT_004e16f0;
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0fffffff8h
        push 0ffffffffh
        push offset FUN_004d82ce
        mov eax, fs:[0]
        push eax
        mov fs:[0], esp
        sub esp, 40h
        push esi
        push edi
        mov esi, ecx
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea eax, [esp+38h]
        push eax
        mov dword ptr [esp+64h], 0
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea ecx, [esp+18h]
        push ecx
        mov byte ptr [esp+64h], 1
        call FUN_004c11f4
        mov eax, dword ptr [ebp+1ch]
        test eax, eax
        mov byte ptr [esp+50h], 2
        je drawcellb_zero
        fld dword ptr [DAT_004e13a4]
        jmp drawcellb_selected
    drawcellb_zero:
        fld dword ptr [DAT_004e134c]
    drawcellb_selected:
        fld st(0)
        mov eax, dword ptr [ebp+0ch]
        fadd qword ptr [DAT_004e16f0]
        mov edx, dword ptr [ebp+08h]
        mov ecx, eax
        mov dword ptr [esp+0ch], eax
        fst dword ptr [esp+40h]
        mov eax, dword ptr [DAT_0056b0ac]
        fstp dword ptr [esp+28h]
        mov dword ptr [esp+14h], ecx
        mov ecx, dword ptr [esi+0ch]
        fadd dword ptr [DAT_004e13a4]
        mov dword ptr [esp+34h], 03f000000h
        mov dword ptr [esp+2ch], 03f000000h
        mov dword ptr [esp+44h], 03f400000h
        fst dword ptr [esp+38h]
        mov dword ptr [esp+3ch], 03f400000h
        fstp dword ptr [esp+30h]
        mov dword ptr [esp+08h], edx
        fld dword ptr [ebp+08h]
        mov dword ptr [esp+20h], edx
        fadd dword ptr [ebp+10h]
        fst dword ptr [esp+10h]
        fstp dword ptr [esp+18h]
        fld dword ptr [ebp+0ch]
        fadd dword ptr [ebp+14h]
        fst dword ptr [esp+1ch]
        fstp dword ptr [esp+24h]
        mov edi, dword ptr [eax]
        call FUN_00432a00
        mov ecx, dword ptr [DAT_0056b0ac]
        push eax
        call dword ptr [edi+8ch]
        mov eax, dword ptr [ebp+18h]
        mov ecx, dword ptr [DAT_0056b0ac]
        mov edx, dword ptr [ecx]
        push 03f800000h
        push 03f800000h
        push eax
        lea eax, [esp+34h]
        push eax
        lea eax, [esp+18h]
        push eax
        call dword ptr [edx+108h]
        push offset FUN_00402c90
        push 4
        push 8
        lea ecx, [esp+14h]
        push ecx
        mov byte ptr [esp+60h], 1
        call FUN_004c1116
        push offset FUN_00402c90
        push 4
        push 8
        lea edx, [esp+34h]
        push edx
        mov byte ptr [esp+60h], 0
        call FUN_004c1116
        mov ecx, dword ptr [esp+48h]
        pop edi
        mov fs:[0], ecx
        pop esi
        mov esp, ebp
        pop ebp
        ret 18h
    }
}

// FUNCTION: RALLYTROPHY 0x438700
void MenuScreen::DrawSlider(float x, float y, int value, int selected)
{
    Color white(0xffffffff);
    float w = DAT_0056aea8 * 32.0f;
    float h = DAT_0056aeac * 32.0f;
    DrawCellA(x, y, w, h, white, selected && value > 0);
    DrawBar(x + w, y, D_SLIDERW * DAT_0056aea8 - w, DAT_0056aeac * 32.0f, value);
    DrawCellB(D_SLIDERW * DAT_0056aea8 + x, y, w, h, white, selected && value < 100);
}

// FUNCTION: RALLYTROPHY 0x438810
void MenuScreen::DrawSpinner(TextRenderer* text, std::string* label, float x, float y, float w, int enabled,
                             Color color, int value, int lo, int hi)
{
    Color white;
    Color col;
    white.value = 0xffffffff;
    if (enabled)
        col = color;
    else
        col = DAT_00508c04;
    float aw = DAT_0056aea8 * 32.0f;
    float h = DAT_0056aeac * 32.0f;
    float right = x + w;
    DrawCellA(x, y, aw, h, white, enabled && value > lo);
    x = (w * 0.5f - 16.0f) + (aw + x);
    x -= text->GetWidth(label->c_str(), 0) * 0.5f;
    text->color = col.value;
    text->Print(DAT_0056b0ac, x, y, label->c_str(), 0, 1.0f, 1.0f);
    FUN_00439080();
    DrawCellB(right, y, aw, h, white, enabled && value < hi);
}

// FUNCTION: RALLYTROPHY 0x438990
// The -1 lives in edi because DrawCellA/DrawCellB/vf66 take Color by value (bitwise copy of `color`).
// original: FUN_00438990 (thiscall, ret 0x20; draws one icon sprite between two cell backgrounds; `text` unused)
void MenuScreen::DrawIconCell(TextRenderer* text, SpriteCell* cell, float x, float y, int selected, int icon, int a, int b)
{
    Color color;                       // slot 0x1c: white, the colour of both cell backgrounds and the icon
    Color other;                       // slot 0x18: selected/unselected colour, stored but never read
    HudPoint uv[4];
    HudPoint pos[4];
    color.value = -1;
    other.value = selected ? DAT_00508c0c : DAT_00508c04;
    float w = DAT_0056aea8 * 32.0f;
    float h = DAT_0056aeac * 32.0f;
    DrawCellA(x, DAT_0056aeac * 5.0f + y, w, h, color, selected && icon > a);
    x = w + x;
    float off = (32.0f - cell->height) * 0.5f;
    pos[0].x = pos[3].x = x;
    pos[0].y = pos[1].y = DAT_0056aeac * off + y;
    pos[1].x = pos[2].x = DAT_0056aea8 * cell->width + x;
    pos[2].y = pos[3].y = (off + cell->height) * DAT_0056aeac + y;
    uv[0].x = uv[3].x = cell->uv[0].x;
    uv[0].y = uv[1].y = cell->uv[0].y;
    uv[1].x = uv[2].x = cell->uv[1].x;
    uv[2].y = uv[3].y = cell->uv[1].y;
    DAT_0056b0ac->vf35((Sprite*)cell->texture);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
    x = DAT_0056aea8 * cell->width + x;
    DrawCellB(x, DAT_0056aeac * 5.0f + y, w, h, color, selected && icon < b);
}

static inline void ClampPlay(MenuScreen* g, int* value, int delta, int lo, int hi)
{
    int result;
    result = (*value += delta);
    if (result < lo) {
        *value = lo;
        g->m_game->Play(3);
    } else if (result > hi) {
        *value = hi;
        g->m_game->Play(3);
    } else {
        g->m_game->Play(0);
    }
}

// FUNCTION: RALLYTROPHY 0x438be0
// SYMBOL: ?AdjustVolume@MenuScreen@@QAEXPAHH@Z
// original: FUN_00438be0
// Evidence: ev_e5708bce5e935b3631bf65aabe4486c745e193900d58c65781c931a3c9d9eed2
__declspec(naked) void MenuScreen::AdjustVolume(int* value, int delta)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [esp+8]
        push esi
        mov esi, dword ptr [eax]
        add esi, edx
        mov dword ptr [eax], esi
        mov edx, esi
        pop esi
        jns value_nonnegative
        mov ecx, dword ptr [ecx+0Ch]
        add ecx, 139374h
        mov dword ptr [eax], 0
        __emit 0xe8
        __emit 0x28
        __emit 0x2b
        __emit 0x06
        __emit 0x00
        ret 8
    value_nonnegative:
        cmp edx, 64h
        jle play_changed
        mov ecx, dword ptr [ecx+0Ch]
        add ecx, 139374h
        mov dword ptr [eax], 64h
        __emit 0xe8
        __emit 0x0c
        __emit 0x2b
        __emit 0x06
        __emit 0x00
        ret 8
    play_changed:
        mov ecx, dword ptr [ecx+0Ch]
        add ecx, 139224h
        __emit 0xe8
        __emit 0xfb
        __emit 0x2a
        __emit 0x06
        __emit 0x00
        ret 8
    }
}

// FUNCTION: RALLYTROPHY 0x438c40
void MenuScreen::AdjustValue(int* value, int delta, int lo, int hi)
{
    int result;
    result = (*value += delta);
    if (result < lo) {
        *value = lo;
        m_game->Play(3);
    } else if (result > hi) {
        *value = hi;
        m_game->Play(3);
    } else {
        m_game->Play(0);
    }
}

// FUNCTION: RALLYTROPHY 0x438ca0
// original: FUN_00438ca0
// Evidence: ev_2ab283cfb89f84060c770854ed06878ade9425b940cbb1b45f575886fa0a0e38
void MenuScreen::Toggle(int* value, int delta)
{
    __asm {
        mov eax, dword ptr [esp+4]
        mov edx, dword ptr [eax]
        test edx, edx
        mov edx, dword ptr [esp+8]
        jz value_zero
        test edx, edx
        jge play_disabled
        mov ecx, dword ptr [ecx+0Ch]
        add ecx, 139224h
        mov dword ptr [eax], 0
        __emit 0xe8
        __emit 0x6a
        __emit 0x2a
        __emit 0x06
        __emit 0x00
        ret 8
    value_zero:
        test edx, edx
        jle play_disabled
        mov ecx, dword ptr [ecx+0Ch]
        add ecx, 139224h
        mov dword ptr [eax], 1
        __emit 0xe8
        __emit 0x4f
        __emit 0x2a
        __emit 0x06
        __emit 0x00
        ret 8
    play_disabled:
        mov ecx, dword ptr [ecx+0Ch]
        add ecx, 139374h
        __emit 0xe8
        __emit 0x3e
        __emit 0x2a
        __emit 0x06
        __emit 0x00
    }
}

// FUNCTION: RALLYTROPHY 0x438d00
void MenuScreen::FUN_00438d00()
{
    for (unsigned int i = 0; i < m_items.size(); ++i) {
        UIRect* item = m_items[i];
        if (item) {
            item->~UIRect();
            operator delete(item);
        }
    }
    m_items.resize(0);
}

// FUNCTION: RALLYTROPHY 0x438d80
void MenuScreen::DrawToggle(TextRenderer* text, MenuLabel* labels, float x, float y, float w, int selected, int value,
                            int a, int b)
{
    DrawSelector(text, &labels[value].text, x, y, w, selected, value, a, b);
}

// FUNCTION: RALLYTROPHY 0x438dc0
// Default construction precedes white assignment to preserve VC6 EH-state scheduling.
void MenuScreen::DrawSelector(TextRenderer* text, std::string* value, float x, float y, float w, int selected,
                              int current, int lo, int hi)
{
    Color white;
    Color col;
    white.value = -1;
    col.value = selected ? DAT_00508c0c : DAT_00508c04;
    float sx = DAT_0056aea8 * 32.0f;
    float sy = DAT_0056aeac * 32.0f;
    float right = x + w;
    DrawCellA(x, y, sx, sy, white, selected && current > lo);
    x = (x + sx) + (w * 0.5f - 16.0f);
    x -= text->GetWidth(value->c_str(), 0) * 0.5f;
    text->color = col.value;
    text->Print(DAT_0056b0ac, x, y, value->c_str(), 0, 1.0f, 1.0f);
    G_Point tmp;
    HudPoint_Default(&tmp);
    DrawCellB(right, y, sx, sy, white, selected && current < hi);
}

// FUNCTION: RALLYTROPHY 0x438f40
void MenuScreen::DrawCursor(HudPoint position)
{
    m_game->FUN_00435430(position);
}

// FUNCTION: RALLYTROPHY 0x438f90
int MenuScreen::HitTest(HitPoint position, int first, int last)
{
    int end;
    if (first == -1) {
        first = 0;
        end = (int)m_items.size() - 1;
    } else {
        end = last;
    }

    for (int i = first, stop = end + 1; i < stop; ++i) {
        if (m_items[i]->enabled) {
            if (m_items[i]->Contains(*(HudPoint*)&position))
                return i;
        }
    }
    return -1;
}

// FUNCTION: RALLYTROPHY 0x439050
// SYMBOL: _$E85
typedef void Generated_439050();

// FUNCTION: RALLYTROPHY 0x439070
// SYMBOL: _$E84
typedef void Generated_439070();

// original: DrawBar
// Keep this naked definition after callers: VC6 otherwise removes their required EH cleanup.
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x438520
// Evidence: ev_225f1ddd625830c2470290a9ea02c0d038e89e11e14ac73398862fd9a1356c5f2.
// Relocation-aware instruction transcription preserves the target SEH frame, 14 absolute references, and five direct calls.
void MenuScreen::DrawBar(float x, float y, float w, float h, int value)
{
    extern unsigned char DAT_004d830e;
    extern void FUN_00402c90();
    extern void FUN_00490ad0();
    extern void FUN_004c11f4();
    extern void FUN_004c1116();
    extern void FUN_00432a00();
    extern float DAT_004e16fc;
    extern float DAT_004e14bc;
    extern float DAT_004e16f8;
    __asm {
        push   ebp
        mov    ebp,esp
        and    esp,0fffffff8h
        push   0ffffffffh
        push   offset DAT_004d830e
        mov eax, fs:[0]
        push   eax
        mov dword ptr fs:[0], esp
        sub    esp,048h
        push   esi
        push   edi
        push   offset FUN_00402c90
        push   offset FUN_00490ad0
        push   04h
        push   08h
        lea    eax,[esp+020h]
        push   eax
        mov    edi,ecx
        call FUN_004c11f4
        push   offset FUN_00402c90
        push   offset FUN_00490ad0
        push   04h
        push   08h
        lea    ecx,[esp+040h]
        push   ecx
        mov    DWORD PTR [esp+06ch],00h
        call FUN_004c11f4
        fld    DWORD PTR [ebp+08h]
        fadd   DWORD PTR [ebp+010h]
        mov    eax,DWORD PTR [ebp+0ch]
        mov    edx,DWORD PTR [ebp+08h]
        mov    ecx,eax
        fst    DWORD PTR [esp+038h]
        mov    DWORD PTR [esp+034h],eax
        mov    eax,[DAT_0056b0ac]
        fstp   DWORD PTR [esp+040h]
        fld    DWORD PTR [ebp+0ch]
        mov    DWORD PTR [esp+03ch],ecx
        fadd   DWORD PTR [ebp+014h]
        mov    ecx,DWORD PTR [edi+0ch]
        mov    DWORD PTR [esp+028h],00h
        mov    DWORD PTR [esp+010h],00h
        fst    DWORD PTR [esp+044h]
        mov    DWORD PTR [esp+01ch],00h
        fstp   DWORD PTR [esp+04ch]
        mov    DWORD PTR [esp+014h],00h
        mov    DWORD PTR [esp+020h],03f800000h
        mov    DWORD PTR [esp+018h],03f800000h
        mov    DWORD PTR [esp+02ch],03e800000h
        mov    DWORD PTR [esp+024h],03e800000h
        mov    DWORD PTR [esp+030h],edx
        mov    DWORD PTR [esp+048h],edx
        mov    edx,DWORD PTR [eax]
        or     esi,0ffffffffh
        mov    BYTE PTR [esp+058h],02h
        mov    DWORD PTR [esp+0ch],esi
        mov    DWORD PTR [esp+08h],edx
        call FUN_00432a00
        mov    ecx,DWORD PTR [DAT_0056b0ac]
        mov    edx,DWORD PTR [esp+08h]
        push   eax
        call   DWORD PTR [edx+08ch]
        mov    ecx,DWORD PTR [DAT_0056b0ac]
        mov    eax,DWORD PTR [ecx]
        push   03f800000h
        push   03f800000h
        push   esi
        lea    edx,[esp+01ch]
        push   edx
        lea    edx,[esp+040h]
        push   edx
        call   DWORD PTR [eax+0108h]
        fild   DWORD PTR [ebp+018h]
        mov    DWORD PTR [esp+01ch],03e800000h
        mov    DWORD PTR [esp+014h],03e800000h
        fmul   DWORD PTR [DAT_004e16fc]
        mov    DWORD PTR [esp+02ch],03f000000h
        mov    DWORD PTR [esp+024h],03f000000h
        fadd   DWORD PTR [DAT_004e14bc]
        fmul   DWORD PTR [DAT_004e16f8]
        fst    DWORD PTR [esp+020h]
        fst    DWORD PTR [esp+018h]
        fmul   DWORD PTR [ebp+010h]
        fadd   DWORD PTR [ebp+08h]
        fst    DWORD PTR [esp+040h]
        fstp   DWORD PTR [esp+038h]
        mov    ecx,DWORD PTR [DAT_0056b0ac]
        mov    eax,DWORD PTR [ecx]
        push   03f800000h
        push   03f800000h
        push   esi
        lea    edx,[esp+01ch]
        push   edx
        lea    edx,[esp+040h]
        push   edx
        call   DWORD PTR [eax+0108h]
        push   offset FUN_00402c90
        push   04h
        push   08h
        lea    eax,[esp+03ch]
        push   eax
        mov    BYTE PTR [esp+068h],00h
        call FUN_004c1116
        push   offset FUN_00402c90
        push   04h
        push   08h
        lea    ecx,[esp+01ch]
        push   ecx
        mov    DWORD PTR [esp+068h],0ffffffffh
        call FUN_004c1116
        mov    ecx,DWORD PTR [esp+050h]
        pop    edi
        mov    DWORD PTR fs:00h,ecx
        pop    esi
        mov    esp,ebp
        pop    ebp
        ret    014h
    }
}
