// MenuScreenA48 (0x439170-0x43a38f): embedded in MenuManager at +0xa48.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_439170.h"
#include <string.h>
#include <vector>
#include <math.h>
#include <stdlib.h>

extern "C" void __fastcall FUN_00485400(void* host); // T_56b0b8::FUN_00485400 (thiscall)

// ---- from group draft Z3 ----
class B2_Unit {
public:
    B2_Unit() { x = 1.0f; y = 1.0f; }
    ~B2_Unit() {}
    volatile float x, y;
};
static float B2_titleX = 54.0f;    // 0x4e1714 (plain static: the original multiplies const * scale)
static float B2_titleY = 142.0f;   // 0x4e1718
static float B2_COLW = 400.0f;     // 0x4e171c


// ---- from group draft Z4 ----



struct NR_Point {
    NR_Point() {}
    NR_Point(const NR_Point& other);
    ~NR_Point() {}
    float x, y;
};
class NR_Rect {
public:
    NR_Rect(NR_Point upper, HudPoint lower);
    float fields[5];
};
struct NR_Vec {
    std::allocator<UIRect*> alloc;
    UIRect** first;
    UIRect** last;
    UIRect** end;
    void Insert(UIRect** pos, unsigned int n, UIRect* const& value);
    void InsertOne(UIRect** pos, UIRect* const& value) { Insert(pos, 1, value); }
    void PushBack(UIRect* const& value) { InsertOne(last, value); }
};
#pragma comment(linker, "/alternatename:??0NR_Point@@QAE@ABU0@@Z=?Assign@Pair@@QAEPAU1@PBU1@@Z")
#pragma comment(linker, "/alternatename:??0NR_Rect@@QAE@UNR_Point@@VHudPoint@@@Z=??0A_Rect@@QAE@VHudPoint@@0@Z")
#pragma comment(linker, "/alternatename:?Insert@NR_Vec@@QAEXPAPAVUIRect@@IABQAV2@@Z=?insert@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXPAPAXIABQAX@Z")
typedef char NR_VectorLayoutCheck[sizeof(NR_Vec) == sizeof(std::vector<UIRect*>) ? 1 : -1];
static float NR_Y = 262.0f;
static float NR_Margin = 10.0f;
static float NR_X = 54.0f;
static float NR_Step = 42.0f;

// FUNCTION: RALLYTROPHY 0x439170
MenuScreenA48::MenuScreenA48()
{
    m_selected = 0;
}

// FUNCTION: RALLYTROPHY 0x439230
// SYMBOL: ??_GMenuScreenA48@@UAEPAXI@Z
typedef void Generated_439230();

// FUNCTION: RALLYTROPHY 0x439250
MenuScreenA48::~MenuScreenA48()
{
    Release();
}


struct NS_String {
    int alloc;
    char* ptr;
    unsigned int len, res;
    void AssignCall(const char* s, unsigned int n); // 0x404ba0
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n); // 0x402960
    void AssignA(const char* s) { AssignCall(s, strlen(s)); }
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
};
#pragma comment(linker, "/alternatename:?AssignCall@NS_String@@QAEXPBDI@Z=?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z")
#pragma comment(linker, "/alternatename:?Grow@NS_String@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Eos@NS_String@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")
#define NS_TEXT(text) (*(NS_String*)&(text))
static float NS_X = 54.0f;
static float NS_Y = 262.0f;
static float NS_Step = 42.0f;
static float NS_BackY = 610.0f;

/* FUNCTION: RALLYTROPHY 0x439310 */
// Exact 1401-byte match / 64 relocations. First four assigns remain calls.
// Operand audit: docs/analysis/network-options-setup.md.
void MenuScreenA48::Setup(MenuGame* game)
{
    if (m_state) return;
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    NS_TEXT(items[0].text).AssignA(FUN_00405380(0x119));
    NS_TEXT(items[1].text).AssignA(FUN_00405380(0x11a));
    NS_TEXT(items[2].text).AssignA(FUN_00405380(0x11c));
    NS_TEXT(items[3].text).AssignA(FUN_00405380(0x11b));
    NS_TEXT(items[4].text).AssignB(FUN_00405380(0x18));
    NS_TEXT(onOff[0].text).AssignB(FUN_00405380(2));
    NS_TEXT(onOff[1].text).AssignB(FUN_00405380(1));
    NS_TEXT(four[0].text).AssignB(FUN_00405380(9));
    NS_TEXT(four[1].text).AssignB(FUN_00405380(10));
    NS_TEXT(four[2].text).AssignB(FUN_00405380(11));
    NS_TEXT(four[3].text).AssignB(FUN_00405380(0x26));
    NS_TEXT(seven[0].text).AssignB(FUN_00405380(0x28));
    NS_TEXT(seven[1].text).AssignB("0.5s");
    NS_TEXT(seven[2].text).AssignB("1.0s");
    NS_TEXT(seven[3].text).AssignB("1.5s");
    NS_TEXT(seven[4].text).AssignB("2.0s");
    NS_TEXT(seven[5].text).AssignB("2.5s");
    NS_TEXT(seven[6].text).AssignB("3.0s");
    float y = NS_Y * sy;
    float x = NS_X * sx;
    float step = NS_Step * sy;
    for (int i = 0; i < 5; ++i) {
        items[i].unk20 = -1;
        items[i].x = x;
        items[i].y = y;
        y += step;
    }
    Begin(game);
    items[4].y = NS_BackY * sy;
    BuildRects();
}

// FUNCTION: RALLYTROPHY 0x439890
int MenuScreenA48::Run(MenuGame* game, NetRace* session)
{
    net = session;
    Setup(game);
    int result;
    do {
        result = Frame();
    } while (result != -1);
    if (DAT_00501454 && DAT_0056b0b8) FUN_00485400(DAT_0056b0b8);
    return -1;
}

// NONMATCHING: RALLYTROPHY 0x4398e0
// Same size (1913 + tables), 264 bytes differ: VC6 puts the GetScale copy and the (1, 1) object in swapped frame slots
// (0x44/0x4c vs original 0x4c/0x44) and hoists the object's stores out of the loop; Print loads x before y (original y first).
// original: FUN_004398e0 (thiscall; MenuScreenA48::Frame)
int MenuScreenA48::Frame()
{
    int done = 0;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    int clicked = 0;
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    DAT_0056b0bc->ClearKeys();
    TextRenderer* text = m_game->text;
    int first = 1;
    int wait = 0;
    while (FUN_004b69f0(wait) && !done && !clicked) {
        if (net) {
            net->ServiceNetwork();
        }
        m_game->Update();
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            for (int i = 0; i < 5; i++) {
                if (m_selected != i && m_items[i]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                    m_selected = i;
                }
            }
        }
        if (DAT_0057ba28[VK_ESCAPE]) {
            done = 1;
        }
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        if (DAT_0057ba28[VK_RETURN]) {
            if (m_selected == 4) {
                done = 1;
            } else {
                m_game->Play(1);
                clicked = 1;
            }
            continue;
        }
        int dir = 0;
        if (DAT_0057ba28[VK_DOWN]) {
            dir = 1;
        }
        if (DAT_0057ba28[VK_UP]) {
            dir--;
        }
        if (dir != 0) {
            m_game->Play(0);
            int sel = m_selected + dir;
            m_selected = sel;
            if (sel < 0) {
                m_selected = 4;
            } else if (sel >= 5) {
                m_selected = 0;
            }
            int cx = (int)(m_items[m_selected]->GetTopLeft().x +
                           (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f);
            int cy = (int)(m_items[m_selected]->GetTopLeft().y +
                           (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f);
            SetCursorPos((int)(cx * sx), (int)(cy * sy));
        }
        for (int i = 0; i < 5; i++) {
            if (m_selected != i && m_items[i]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_selected = i;
                m_game->Play(0);
            }
        }
        dir = 0;
        if (DAT_0057ba28[VK_LEFT]) {
            dir = -1;
        }
        if (DAT_0057ba28[VK_RIGHT]) {
            dir++;
        }
        if (DAT_0057ba20) {
            HudPoint mouse = *(HudPoint*)&DAT_0057ba00;
            if (m_items[5]->Contains(mouse)) {
                dir--;
            } else if (m_items[6]->Contains(mouse)) {
                dir++;
            }
            if (m_selected == 4) {
                done = 1;
                continue;
            }
        }
        if (dir != 0 && (unsigned)m_selected <= 3) {
            switch (m_selected) {
            case 0:
                Toggle(&DAT_00501318, dir);
                break;
            case 1:
                Toggle(&DAT_00501314, dir);
                break;
            case 2:
                FUN_00438c40(&DAT_00501320, dir, 0, 3);
                break;
            case 3:
                FUN_00438c40(&DAT_00501324, dir, 0, 6);
                break;
            }
        }
        MenuGame* game = m_game;
        game->UpdateVideo();
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            game->DrawVideo(0);
            text->color = DAT_00508bfc;
            text->scaleX = DAT_0056aea8 * 2.0f;
            text->scaleY = DAT_0056aeac * 2.0f;
            text->Print(DAT_0056b0ac, B2_titleX * DAT_0056aea8, B2_titleY * DAT_0056aeac, FUN_00405380(0x3f), 0, 1.0f, 1.0f);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            game->FUN_00435de0(517.0f, 243.0f);
            HudPoint scale = *game->GetScale();
            for (int i = 0; i < 5; i++) {
                int selected = (i == m_selected);
                Color col;
                B2_Unit one;
                col.value = selected ? DAT_00508c0c : DAT_00508c04;
                text->color = col.value;
                text->Print(DAT_0056b0ac, items[i].x, items[i].y, items[i].text.c_str(), 0, 1.0f, 1.0f);
                HudPoint pos;
                pos.x = 520.0f * DAT_0056aea8;
                pos.y = items[i].y;
                switch (i) {
                case 0:
                    DrawToggle(text, onOff, pos.x, pos.y, B2_COLW * sx, selected, DAT_00501318, 0, 1);
                    break;
                case 1:
                    DrawToggle(text, onOff, pos.x, pos.y, B2_COLW * sx, selected, DAT_00501314, 0, 1);
                    break;
                case 2:
                    DrawToggle(text, four, pos.x, pos.y, B2_COLW * sx, selected, DAT_00501320, 0, 3);
                    break;
                case 3:
                    DrawToggle(text, seven, pos.x, pos.y, B2_COLW * sx, selected, DAT_00501324, 0, 6);
                    break;
                }
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 4, 0, 1, 1, -1.0f);
            DrawCursor(*((MouseInput*)DAT_0056b0c0)->GetPosition());
            DAT_0056b0ac->EndScene();
            DAT_0056b0ac->Present();
        }
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    int r = m_selected;
    if (done || DAT_0057b9f4) {
        r = -1;
        m_game->Play(2);
    }
    return r;
}

/* FUNCTION: RALLYTROPHY 0x43a080 */
// Exact 727-byte match / 38 relocations; all 16 calls and 18 float operands audited.
// Evidence: docs/analysis/network-options-rects.md.
void MenuScreenA48::BuildRects()
{
    NR_Point top, bottom;
    float y = NR_Y - NR_Margin;
    UIRect* r;
    for (int i = 0; i < 4; ++i) {
        top.x = NR_X;
        bottom.x = NR_X + 960.0f;
        top.y = y + (float)i * NR_Step;
        bottom.y = y + (float)(i + 1) * NR_Step;
        r = (UIRect*)new NR_Rect(top, *(HudPoint*)&bottom);
        ((NR_Vec*)&m_items)->PushBack(r);
    }
    top.x = NR_X;
    top.y = 607.0f;
    bottom.x = NR_X + 420.0f;
    bottom.y = NR_Step * 607.0f;
    r = (UIRect*)new NR_Rect(top, *(HudPoint*)&bottom);
    ((NR_Vec*)&m_items)->PushBack(r);
    top.x = 500.0f;
    bottom.x = 550.0f;
    top.y = NR_Y;
    bottom.y = NR_Y + 221.0f;
    r = (UIRect*)new NR_Rect(top, *(HudPoint*)&bottom);
    ((NR_Vec*)&m_items)->PushBack(r);
    top.x = 900.0f;
    bottom.x = 950.0f;
    top.y = NR_Y;
    bottom.y = NR_Y + 221.0f;
    r = (UIRect*)new NR_Rect(top, *(HudPoint*)&bottom);
    ((NR_Vec*)&m_items)->PushBack(r);
}

// FUNCTION: RALLYTROPHY 0x43a360
// SYMBOL: _$E23
typedef void Generated_43a360();

// FUNCTION: RALLYTROPHY 0x43a380
// SYMBOL: _$E22
typedef void Generated_43a380();
