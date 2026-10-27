// GameScreen2DD4 (0x4266c0-0x4275ff): the 0x20-byte menu screen at Game+0x2dd4. Map: analysis/range-4266c0.md;
// class in include/unk_4266c0.h. Work group H1a (lifecycle, Run, module end).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_4266c0.h"
#include <vector>
#include <math.h>
#include <stdlib.h>
#include <string.h>

extern int DAT_00501458;


// ---- from group draft H1b ----
extern int DAT_00501458;               // race option: nonzero hides item 3 and draws text 0x33 instead of 0x32
extern int DAT_004f6998[5];            // text ids of the five items (0x35 .. 0x38, 0x18)

// ---- Group H1b ----
// Layout constants of Frame (the original reads each from .rdata as a separate object: fld const; fmul scale).
static float H1b_titleX = 54.0f;       // 0x4e1540
static float H1b_titleY = 142.0f;      // 0x4e1544
static float H1b_firstY = 262.0f;      // 0x4e1534
static float H1b_step = 42.0f;         // 0x4e1538
static float H1b_backY = 610.0f;       // 0x4e153c


// ---- from group draft H1c ----
struct H1c_Point {
    H1c_Point(const H1c_Point& other);   // 0x424300
    ~H1c_Point() {}
    float x, y;
};
class H1c_Rect {
public:
    H1c_Rect(H1c_Point topLeft, HudPoint bottomRight);   // 0x4390a0
    float f[5];
};

class H1c_String {
public:
    H1c_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~H1c_String() { Free(true); }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)              // std::string::_Tidy, expanded inline
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};
#pragma comment(linker, "/alternatename:?Tidy@H1c_String@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?Grow@H1c_String@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Eos@H1c_String@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")

#pragma comment(linker, "/alternatename:??0H1c_Point@@QAE@ABU0@@Z=?Assign@Pair@@QAEPAU1@PBU1@@Z")
#pragma comment(linker, "/alternatename:??0H1c_Rect@@QAE@UH1c_Point@@VHudPoint@@@Z=??0A_Rect@@QAE@VHudPoint@@0@Z")

struct H1c_RectVector {
    std::allocator<UIRect*> alloc;
    UIRect** first;
    UIRect** last;
    UIRect** end;
    void Insert(UIRect** pos, unsigned int n, UIRect* const& value); // 0x4bc680
    void InsertOne(UIRect** pos, UIRect* const& value) { Insert(pos, 1, value); }
    void PushBack(UIRect* const& value) { InsertOne(last, value); }
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    unsigned int SizeCall() const; // 0x414460
    UIRect** Ucopy(UIRect* const* f, UIRect* const* l, UIRect** d); // 0x424320
    void Ufill(UIRect** d, unsigned int n, UIRect* const& value); // 0x424350
    void Destroy(UIRect** f, UIRect** l); // 0x403130
    static void Construct(UIRect** p, UIRect* const& value); // 0x403140
    UIRect** CopyInline(UIRect* const* from, UIRect* const* finish, UIRect** out)
    {
        for (; from != finish; ++out, ++from)
            Construct(out, *from);
        return out;
    }
    void FillInline(UIRect** out, unsigned int count, UIRect* const& value)
    {
        for (; 0 < count; --count, ++out)
            Construct(out, value);
    }
    void PushBackExpanded(UIRect* const& value)
    {
        UIRect** pos = last;
        if ((unsigned int)(end - last) < 1) {
            unsigned int count = SizeCall() + (1 < Size() ? SizeCall() : 1);
            UIRect** storage = alloc.allocate(count, (void*)0);
            UIRect** out = Ucopy(first, pos, storage);
            Ufill(out, 1, value);
            Ucopy(pos, last, out + 1);
            Destroy(first, last);
            alloc.deallocate(first, end - first);
            end = storage + count;
            last = storage + SizeCall() + 1;
            first = storage;
        } else if ((unsigned int)(last - pos) < 1) {
            Ucopy(pos, last, pos + 1);
            Ufill(last, 1 - (last - pos), value);
            for (UIRect** from = pos; from != last; ++from)
                *from = value;
            last += 1;
        } else {
            Ucopy(last - 1, last, last);
            UIRect** to = last;
            UIRect** from = last - 1;
            while (pos != from)
                *--to = *--from;
            for (from = pos; from != pos + 1; ++from)
                *from = value;
            last += 1;
        }
    }
};
#pragma comment(linker, "/alternatename:?Insert@H1c_RectVector@@QAEXPAPAVUIRect@@IABQAV2@@Z=?insert@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXPAPAXIABQAX@Z")
#pragma comment(linker, "/alternatename:?SizeCall@H1c_RectVector@@QBEIXZ=?size@?$vector@PAXV?$allocator@PAX@std@@@std@@QBEIXZ")
#pragma comment(linker, "/alternatename:?Ucopy@H1c_RectVector@@QAEPAPAVUIRect@@PBQAV2@0PAPAV2@@Z=?_Ucopy@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEPAPAXPBQAX0PAPAX@Z")
#pragma comment(linker, "/alternatename:?Ufill@H1c_RectVector@@QAEXPAPAVUIRect@@IABQAV2@@Z=?_Ufill@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEXPAPAXIABQAX@Z")
#pragma comment(linker, "/alternatename:?Destroy@H1c_RectVector@@QAEXPAPAVUIRect@@0@Z=?_Destroy@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEXPAPAX0@Z")
#pragma comment(linker, "/alternatename:?Construct@H1c_RectVector@@SAXPAPAVUIRect@@ABQAV2@@Z=?_Construct@std@@YAXPAPAXABQAX@Z")
typedef char H1c_RectVectorLayoutCheck[sizeof(H1c_RectVector) == sizeof(std::vector<UIRect*>) ? 1 : -1];

extern ConfigKey DAT_004f69b0[];

// FUNCTION: RALLYTROPHY 0x4266c0
GameScreen2DD4::GameScreen2DD4()
{
    m_selected = 0;
    m_state = 0;
}

// FUNCTION: RALLYTROPHY 0x4266e0
// SYMBOL: ??_GGameScreen2DD4@@UAEPAXI@Z
typedef void Generated_4266e0();

// FUNCTION: RALLYTROPHY 0x426700
GameScreen2DD4::~GameScreen2DD4()
{
    Release();
}

// FUNCTION: RALLYTROPHY 0x426750
void GameScreen2DD4::Setup(MenuGame* game)
{
    if (m_state == 0) {
        m_game = game;
        m_state = 1;
        LoadLayout();
    }
}

// FUNCTION: RALLYTROPHY 0x426770
int GameScreen2DD4::Run(MenuGame* game)
{
    Setup(game);
    if (DAT_00501458 && m_selected == 3) {
        m_selected = 2;
    }
    return Frame();
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// NONMATCHING: RALLYTROPHY 0x4267a0
// original: FUN_004267a0. Same size and code except 54 bytes of scheduling in the title-print prologue: the original
// loads DAT_00501458 after the text->color store (color in eax), VC6 here hoists the load above the stores (color in
// ecx) and picks other temps for the DAT_0056b0ac loads. Tried statement order, !x, swapped arms, SetScale/SetColor;
// a volatile title-mode read kept the size and diff count unchanged but changed store order; a named titleColor
// temporary emitted identical bytes to baseline.
int GameScreen2DD4::Frame()
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
    while (FUN_004b69f0(wait)) {
        if (done || clicked) {
            break;
        }
        m_game->Update();
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 0) {
                m_selected = 0;
                m_game->Play(0);
            }
            if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 1) {
                m_selected = 1;
                m_game->Play(0);
            }
            if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 2) {
                m_selected = 2;
                m_game->Play(0);
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
            m_selected += dir;
            if (m_selected < 0) {
                m_selected = 4;
            } else if (m_selected > 4) {
                m_selected = 0;
            }
            if (m_selected == 3 && DAT_00501458) {
                m_selected = dir > 0 ? 4 : 2;
            }
            int cx = (int)(m_items[m_selected]->GetTopLeft().x +
                           (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f);
            int cy = (int)(m_items[m_selected]->GetTopLeft().y +
                           (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f);
            SetCursorPos((int)(cx * sx), (int)(cy * sy));
        }
        if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 0) {
            m_selected = 0;
            m_game->Play(0);
        }
        if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 1) {
            m_selected = 1;
            m_game->Play(0);
        }
        if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 2) {
            m_selected = 2;
            m_game->Play(0);
        }
        if (DAT_00501458 == 0) {
            if (m_items[3]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 3) {
                m_selected = 3;
                m_game->Play(0);
            }
        }
        if (m_items[4]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_selected != 4) {
            m_selected = 4;
            m_game->Play(0);
        }
        if (DAT_0057ba20) {
            if (m_items[4]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                done = 1;
                continue;
            }
            if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
            if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
            if (m_items[2]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
            if (m_items[3]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
        }
        MenuGame* game = m_game;
        game->UpdateVideo();
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            game->FUN_00432a40();
            game->DrawVideo(1);
            text->scaleX = DAT_0056aea8 * 2.0f;
            text->color = DAT_00508bfc;
            text->scaleY = DAT_0056aeac * 2.0f;
            if (DAT_00501458 == 0) {
                text->Print(DAT_0056b0ac, H1b_titleX * DAT_0056aea8, H1b_titleY * DAT_0056aeac, FUN_00405380(0x32), 0, 1.0f, 1.0f);
            } else {
                text->Print(DAT_0056b0ac, H1b_titleX * DAT_0056aea8, H1b_titleY * DAT_0056aeac, FUN_00405380(0x33), 0, 1.0f, 1.0f);
            }
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            float y = H1b_firstY * sy;
            float x = sx * 54.0f;
            for (int i = 0; i < 5; i++) {
                if (DAT_00501458 == 0 || i != 3) {
                    int selected = (i == m_selected);
                    Color col;
                    col.value = selected ? DAT_00508c0c : DAT_00508c04;
                    if (i == 4) {
                        y = H1b_backY * sy;
                    }
                    text->color = col.value;
                    text->Print(DAT_0056b0ac, x, y, FUN_00405380(DAT_004f6998[i]), 0, 1.0f, 1.0f);
                    y += H1b_step * sy;
                }
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, DAT_00501458 ? 3 : 4, 1, 1, 1, -1.0f);
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

/* FUNCTION: RALLYTROPHY 0x4270c0 */
// Exact 1286-byte match / 67 relocations. Five insertions use explicit helper boundaries.
// Local comparison and call audit: docs/analysis/game-mode-layout.md.
void GameScreen2DD4::LoadLayout()
{
    Config config;
    config.Define(DAT_004f69b0, 10);
    H1c_String filename("data/menu/gamemodemenu.ini");
    config.Load(filename.c_str(), 1, 0, 0);

    UIRect* rect;
    rect = (UIRect*)new H1c_Rect(*(const H1c_Point*)&config.GetVector2("ChampionshipText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("ChampionshipText_LowerRight"));
    ((H1c_RectVector*)&m_items)->PushBack(rect);
    rect = (UIRect*)new H1c_Rect(*(const H1c_Point*)&config.GetVector2("SingleRaceText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("SingleRaceText_LowerRight"));
    ((H1c_RectVector*)&m_items)->PushBack(rect);
    rect = (UIRect*)new H1c_Rect(*(const H1c_Point*)&config.GetVector2("SingleStageText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("SingleStageText_LowerRight"));
    ((H1c_RectVector*)&m_items)->PushBack(rect);
    rect = (UIRect*)new H1c_Rect(*(const H1c_Point*)&config.GetVector2("TimeTrialText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("TimeTrialText_LowerRight"));
    ((H1c_RectVector*)&m_items)->PushBack(rect);
    rect = (UIRect*)new H1c_Rect(*(const H1c_Point*)&config.GetVector2("BackText_UpperLeft"),
                      *(HudPoint*)&config.GetVector2("BackText_LowerRight"));
    ((H1c_RectVector*)&m_items)->PushBackExpanded(rect);
}

// FUNCTION: RALLYTROPHY 0x4275d0
// SYMBOL: _$E23
typedef void Generated_4275d0();

// FUNCTION: RALLYTROPHY 0x4275f0
// SYMBOL: _$E22
typedef void Generated_4275f0();
