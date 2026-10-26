// MenuManager (0x43c590-0x43d76f): the top-level menu screen. Map: analysis/range-43c590.md; class in
// include/unk_43c590.h.
// Work groups: M1 (lifecycle, Setup, module-end stubs; prefix M1_), M2 (Release, dispatch), M3 (Frame), M4 (BuildRects).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_43f9e0.h"
#include "unk_439170.h"
#include "unk_43c590.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group M1 ----
// Layout constants of Setup (the original reads each from .rdata as a separate object: fld const; fmul scale).
static float M1_firstY = 262.0f;         // 0x4e17b0
static float M1_textX = 54.0f;          // 0x4e17b8
static float M1_step = 42.0f;           // 0x4e17b4
static float M1_backY = 610.0f;          // 0x4e17c4


// ---- from group draft Y2 ----
class M2_Screen2 { public: int Run(MenuGame*, NetRace*); };
class M2_Screen4 { public: int Run(MenuGame*, NetRace*); };
class M2_Screen5 { public: int Run(MenuGame*, NetRace*); };
extern "C" int __stdcall FUN_0042a210(int);


// ---- from group draft Y3 ----
class M3_Unit {
public:
    M3_Unit() { x = 1.0f; y = 1.0f; }
    ~M3_Unit() {}
    volatile float x, y;
};
static float M3_titleX = 54.0f;    // 0x4e17bc (plain static: the original multiplies const * scale)
static float M3_titleY = 142.0f;   // 0x4e17c0


// ---- from group draft Y4 ----
struct M4_Point {
    M4_Point(const M4_Point& other); // 0x424300
    ~M4_Point() {}
    float x, y;
};

class M4_Rect {
public:
    M4_Rect(M4_Point upperLeft, HudPoint lowerRight); // 0x4390a0
    int fields[5];
};

class M4_String {
public:
    M4_String(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    M4_String(const char* s) : alloc(std::allocator<char>())
    {
        Tidy(false);
        Assign(s);
    }
    ~M4_String() { Free(true); }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);               // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);            // 0x402960
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

extern ConfigKey DAT_004f8da0[];

#define M4_ADD(upper, lower) \
    do { \
        rect = (UIRect*)new M4_Rect(*(const M4_Point*)&config.GetVector2(upper), \
                                    *(const HudPoint*)&config.GetVector2(lower)); \
        std::vector<UIRect*>& v = m_items; \
        v.insert(v.end(), rect); \
    } while (0)

// FUNCTION: RALLYTROPHY 0x43c590
MenuManager::MenuManager()
{
    m_state = 0;
    m_game = 0;
    m_selected = 0;
}

// FUNCTION: RALLYTROPHY 0x43c660
// SYMBOL: ??_GMenuManager@@UAEPAXI@Z
typedef void Generated_43c660();

// FUNCTION: RALLYTROPHY 0x43c680
MenuManager::~MenuManager()
{
    Release();
}

// FUNCTION: RALLYTROPHY 0x43c770
// The label loop spends VC6's inline budget through SetX (an inline call), which leaves exactly the first four string
// assigns' _Eos calls out of line; SetY or plain stores for both move the cut.
void MenuManager::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    float xs = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float ys = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    title = FUN_00405380(0x3a);
    textScaleX = xs * 32.0f;
    unk40 = -1;
    textScaleY = ys * 16.0f;
    items[0].text = FUN_00405380(0x3b);
    items[1].text = FUN_00405380(0x3c);
    items[2].text = FUN_00405380(0x3d);
    items[3].text = FUN_00405380(0x3e);
    items[4].text = FUN_00405380(0x3f);
    items[5].text = FUN_00405380(0x18);
    TextRenderer* text = game->text;
    float y = M1_firstY * ys;
    for (int i = 0; i < 6; i++) {
        items[i].unk20 = -1;
        items[i].height = (float)text->lineHeight;
        items[i].width = text->GetWidth(items[i].text.c_str(), 1);
        items[i].SetX(M1_textX * xs);
        items[i].y = y;
        y += M1_step * ys;
    }
    items[5].y = M1_backY * ys;
    Begin(game);
    sound.Setup(game);
    raceOptions.Setup(game);
    screen2a8.Setup(game);
    screen8dc.Setup(game);
    screena48.Setup(game);
    BuildRects();
}

// FUNCTION: RALLYTROPHY 0x43cae0
void MenuManager::Release()
{
    if (m_state) {
        ((MenuScreen*)&raceOptions)->Release();
        ((MenuScreen*)&screen2a8)->Release();
        ((MenuScreen*)&sound)->Release();
        ((MenuScreen*)&screen8dc)->Release();
        ((MenuScreen*)&screena48)->Release();
        m_state = 0;
        m_game = 0;
        FUN_00438d00();
    }
}

// FUNCTION: RALLYTROPHY 0x43cb50
void MenuManager::FUN_0043cb50(int arg)
{
    screen2a8.FUN_0042a210(arg);
}

// FUNCTION: RALLYTROPHY 0x43cb70
int MenuManager::Run(MenuGame* game, NetRace* session)
{
    net = session;
    Setup(game);
    for (;;) {
        int index = Frame() + 1;
        if ((unsigned int)index > 5)
            continue;
        switch (index) {
        case 0: FUN_004a4870("data/options.cfg"); return -1;
        case 1: raceOptions.Run(m_game, net); break;
        case 2: ((M2_Screen2*)&screen2a8)->Run(m_game, net); break;
        case 3: sound.Run(m_game, net); break;
        case 4: ((M2_Screen4*)&screen8dc)->Run(m_game, net); break;
        case 5: ((M2_Screen5*)&screena48)->Run(m_game, net); break;
        }
    }
}

// NONMATCHING: RALLYTROPHY 0x43cc50
// 1365 vs 1362 bytes, ~410 differing, all control flow and calls match. Remaining: the original keeps the zero constant in
// ebp for the whole body (`cmp eax,ebp` after the second Contains loop, `push ebp` args, imm -1 EH stores) while mine puts
// -1 in ebp after the Up/Down block; the (1, 1) object and the GetScale copy sit in swapped frame slots (0x34/0x3c); the
// title Print loads the device pointer after the y product in the original (register choice).
// original: FUN_0043cc50 (thiscall; MenuManager::Frame)
int MenuManager::Frame()
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
            for (int i = 0; i < 6; i++) {
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
        if (DAT_0057ba28[VK_RETURN] || DAT_0057ba20) {
            if (m_selected == 5) {
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
                m_selected = 5;
            } else if (sel >= 6) {
                m_selected = 0;
            }
            int cx = (int)(m_items[m_selected]->GetTopLeft().x +
                           (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f);
            int cy = (int)(m_items[m_selected]->GetTopLeft().y +
                           (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f);
            SetCursorPos((int)(cx * sx), (int)(cy * sy));
        }
        for (int i = 0; i < 6; i++) {
            if (m_selected != i && m_items[i]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_selected = i;
                m_game->Play(0);
            }
        }
        MenuGame* game = m_game;
        HudPoint scale = *game->GetScale();
        game->UpdateVideo();
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            game->FUN_00432a40();
            game->DrawVideo(1);
            text->color = DAT_00508bfc;
            text->scaleX = DAT_0056aea8 * 2.0f;
            text->scaleY = DAT_0056aeac * 2.0f;
            text->Print(DAT_0056b0ac, M3_titleX * DAT_0056aea8, M3_titleY * DAT_0056aeac, FUN_00405380(0x2c), 0, 1.0f, 1.0f);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            for (int i = 0; i < 6; i++) {
                int selected = (i == m_selected);
                Color col;
                M3_Unit one;
                col.value = selected ? DAT_00508c0c : DAT_00508c04;
                text->color = col.value;
                text->Print(DAT_0056b0ac, items[i].x, items[i].y, items[i].text.c_str(), 0, 1.0f, 1.0f);
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 5, 1, 1, 1, -1.0f);
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

// FUNCTION: RALLYTROPHY 0x43d1b0
// Config-driven creation of the six top-level menu hit rectangles.
void MenuManager::BuildRects()
{
    Config config;
    UIRect* rect;
    config.Define(DAT_004f8da0, 12);
    M4_String path((const char*)0x4f8efc);
    config.Load(path.c_str(), true, false, 0);
    M4_ADD("RaceText_UpperLeft", "RaceText_LowerRight");
    M4_ADD("GraphicsText_UpperLeft", "GraphicsText_LowerRight");
    M4_ADD("SoundAndMusicText_UpperLeft", "SoundAndMusicText_LowerRight");
    M4_ADD("ControlsText_UpperLeft", "ControlsText_LowerRight");
    M4_ADD("MultiplayerText_UpperLeft", "MultiplayerText_LowerRight");
    M4_ADD("BackText_UpperLeft", "BackText_LowerRight");
}

// FUNCTION: RALLYTROPHY 0x43d740
// SYMBOL: _$E23
typedef void Generated_43d740();

// FUNCTION: RALLYTROPHY 0x43d760
// SYMBOL: _$E22
typedef void Generated_43d760();
