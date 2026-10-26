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
// NONMATCHING: RALLYTROPHY 0x438240
// 362 vs 360 bytes. Colour is a by-value Color param (EH state 0, arrays 1/2) and the quad layout is right; only the x87/int-register
// scheduling of the pos[] stores differs (~60 store orders and chain forms tried).
void MenuScreen::DrawCellA(float x, float y, float w, float h, Color color, int flag)
{
    Vec2 uv[4];
    Vec2 pos[4];
    float u = flag ? 0.25f : 0.0f;
    uv[0].x = uv[3].x = u;
    uv[1].x = uv[2].x = u + 0.125f;
    uv[0].y = uv[1].y = 0.5f;
    uv[2].y = uv[3].y = 0.75f;
    pos[1].y = pos[0].y = y;
    pos[2].x = pos[1].x = x + w;
    pos[3].x = pos[0].x = x;
    pos[3].y = pos[2].y = y + h;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(((GameView*)m_game)->FUN_00432a00());
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
}

// original: DrawCellB
// NONMATCHING: RALLYTROPHY 0x4383b0
// 381 vs 368 bytes. Same shape as 0x438240 but the left u (u + 0.125 as a double) spills through a temp here, the original keeps it in st0.
void MenuScreen::DrawCellB(float x, float y, float w, float h, Color color, int flag)
{
    Vec2 uv[4];
    Vec2 pos[4];
    float u = flag ? 0.25f : 0.0f;
    SetQuad(uv, u + 0.125, 0.5f, u + 0.25f, 0.75f);
    pos[1].y = pos[0].y = y;
    pos[2].x = pos[1].x = x + w;
    pos[3].x = pos[0].x = x;
    pos[3].y = pos[2].y = y + h;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(((GameView*)m_game)->FUN_00432a00());
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
}

// original: DrawBar
// NONMATCHING: RALLYTROPHY 0x438520
// 472 vs 469 bytes. The original keeps -1 in esi (this in edi) and stores it to the Color slot; ours folds it to immediates.
// Tried Color/DWORD/int forms and declaration orders.
void MenuScreen::DrawBar(float x, float y, float w, float h, int value)
{
    Vec2 uv[4];
    Vec2 pos[4];
    Color c(0xffffffff);
    SetQuad(uv, 0.0f, 0.0f, 1.0f, 0.25f);
    pos[1].y = pos[0].y = y;
    pos[2].x = pos[1].x = x + w;
    pos[3].x = pos[0].x = x;
    pos[3].y = pos[2].y = y + h;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(((GameView*)m_game)->FUN_00432a00());
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, c, 1.0f, 1.0f);
    float f = ((float)value * 2.24f + 16.0f) * 0.00390625f;
    uv[0].y = uv[1].y = 0.25f;
    uv[2].y = uv[3].y = 0.5f;
    uv[1].x = uv[2].x = f;
    pos[2].x = pos[1].x = f * w + x;
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, c, 1.0f, 1.0f);
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

// NONMATCHING: RALLYTROPHY 0x438dc0
// Only the prologue differs: the original loads `selected` and DAT_00508c0c before `or edx,-1` and stores the white
// colour after the EH state; every ordering of the locals tried emits the white store first.
void MenuScreen::DrawSelector(TextRenderer* text, std::string* value, float x, float y, float w, int selected,
                              int current, int lo, int hi)
{
    Color white(0xffffffff);
    Color col;
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
