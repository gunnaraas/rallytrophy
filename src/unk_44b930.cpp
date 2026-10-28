// ReplayMenu (0x44b930-0x44d25f): the replay-list screen. Map: analysis/range-44b930.md; classes in
// include/unk_44b930.h.
// Work groups: FA-1 (lifecycle, Setup, Run, BuildRects, RefreshRecords, Draw: prefix FA1_), FA-2 (Frame),
// FA-3 (DrawField).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_44b930.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group FA-1 ----
// A 2D point whose copy constructor is out of line (0x424300).
struct FA1_Point {
    FA1_Point() {}
    FA1_Point(const FA1_Point& other); // 0x424300
    ~FA1_Point() {}
    float x, y;
};
// The HUD rectangle as BuildRects calls it: constructor 0x4390a0 out of line.
class FA1_Rect {
public:
    FA1_Rect(FA1_Point topLeft, HudPoint bottomRight); // 0x4390a0
    float f[5];
};

// Layout constants of the 1024x768 design space (kept as named constants: the original does not fold them).
static const float FA1_LEFT = 54.0f;     // title and back text x
static const float FA1_TOP = 262.0f;     // first row
static const float FA1_PITCH = 42.0f;    // row pitch
static const float FA1_BACKY = 610.0f;   // back text y
static const float FA1_COLX = 485.0f;    // replay column x
static const float FA1_ROWY = 260.0f;    // replay rows y
static const float FA1_MARGIN = 20.0f;   // rect margin above the title row
static const float FA1_TITLEW = 420.0f;
static const float FA1_BACKW = 200.0f;
static const float FA1_SLOTW = 430.0f;
static const float FA1_BACKMARGIN = 10.0f;

// ---- Group FA-2: Frame ----
struct FA2_Point {
    FA2_Point(float px, float py)
    {
        x = px;
        y = py;
    }
    ~FA2_Point() {}
    float x, y;
};
#define FA2_STORE (*(ReplayStore**)((BYTE*)m_game + 0x2dd0))


// ---- Group FA-3: DrawField ----
// A VC6 std::string seen through its layout (allocator, pointer, length, reserve). DrawField's string operations are
// expanded to different depths (the inline budget runs out part way through the function), so each site is pinned:
// out-of-line assign/append (0x404ba0/0x403ba0) and the expansions that end in _Eos (0x402960) or set the terminator
// inline.
struct FA3_Str {
    int alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
    void AssignCall(const char* s, unsigned int n);   // 0x404ba0
    void AppendCall(const char* s, unsigned int n);   // 0x403ba0
    bool Grow(unsigned int n, bool trim);              // 0x402980
    void Eos(unsigned int n);                         // 0x402960
    static void Xlen();                                // 0x4bd00b
    void AssignA(const char* s) { AssignCall(s, strlen(s)); }
    void AppendA(const char* s) { AppendCall(s, strlen(s)); }
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    void AssignC(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            len = n;
            ptr[n] = 0;
        }
    }
    void AppendD(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            Xlen();
        }
        if (n > 0) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                Eos(total);
            }
        }
    }
    void AppendC(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            Xlen();
        }
        if (n > 0) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                len = total;
                ptr[total] = 0;
            }
        }
    }
};
#define FA3_L (*(FA3_Str*)&left)
#define FA3_R (*(FA3_Str*)&right)

// FUNCTION: RALLYTROPHY 0x44b930
ReplayMenu::ReplayMenu()
{
    m_selected = 0;
    m_state = 0;
}

// FUNCTION: RALLYTROPHY 0x44b9a0
// SYMBOL: ??_GReplayMenu@@UAEPAXI@Z
typedef void Generated_44b9a0();

// FUNCTION: RALLYTROPHY 0x44b9c0
ReplayMenu::~ReplayMenu()
{
    Release();
}

// NONMATCHING: RALLYTROPHY 0x44ba30
// 452 bytes; 8 differ in the shared y/y2 temporary and inline-assign pointer slots (target +0x20/+0x18, ours swapped).
// Early declarations of y or y2 and a named nonvolatile text pointer change no bytes. Hoisting y2's computation
// gives 144 differences, volatile x gives 28, and a volatile title pointer gives 313.
// Computing y2 after the second label assignment expands the diff to 173; a scope around the original block is unchanged.
// Writing label 1's y before x to mirror the target's stores raises the diff 8→11 bytes.
// Plain objcmp observations.
// original: FUN_0044ba30 (slot 1)
void ReplayMenu::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    float x = DAT_0056aea8 * FA1_LEFT;
    float y = DAT_0056aeac * FA1_TOP;
    m_game = game;
    m_state = 1;
    labels[0].text = FUN_00405380(0x19f);
    labels[0].x = x;
    labels[0].y = y;
    float y2 = FA1_BACKY * DAT_0056aeac;
    labels[1].text = FUN_00405380(0x18);
    labels[1].x = x;
    labels[1].y = y2;
    BuildRects();
}

// FUNCTION: RALLYTROPHY 0x44bc00
int ReplayMenu::Run(MenuGame* game)
{
    Setup(game);
    m_selected = 0;
    return Frame();
}

// FUNCTION: RALLYTROPHY 0x44bc30
// original: FUN_0044bc30. The mouse warp builds a temporary with an inline constructor and destructor (FA2_Point).
// The unused local std::string is only there for the frame layout and its inline constructor.
int ReplayMenu::Frame()
{
    HudPoint mouse;
    int done = 0;
    std::string unused;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    int picked = 0;
    DAT_0056b0bc->ClearKeys();
    int first = 1;
    RefreshRecords();
    int wait = 0;
    while (FUN_004b69f0(wait) && !done && !picked) {
        int hit;
        m_game->Update();
        mouse = *(HudPoint*)&DAT_0057ba00;
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            hit = HitTest(mouse, 0, 9);
            if (hit != -1) {
                m_selected = hit;
            }
        }
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        int dir = 0;
        if (DAT_0057ba28[VK_ESCAPE]) {
            if (m_selected <= 1) {
                done = 1;
                continue;
            }
            m_selected = 0;
            dir = 1;
        } else if (DAT_0057ba28[VK_RETURN]) {
            if (m_selected == 1) {
                done = 1;
                continue;
            }
            if (m_selected == 0) {
                m_game->Play(1);
                m_selected = 2;
                float cx = m_items[2]->GetTopLeft().x +
                           (m_items[2]->GetBottomRight().x - m_items[2]->GetTopLeft().x) * 0.5f;
                float cy = m_items[2]->GetTopLeft().y +
                           (m_items[2]->GetBottomRight().y - m_items[2]->GetTopLeft().y) * 0.5f;
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&FA2_Point(cx * sx, cy * sy));
            } else if (m_selected >= 2 && m_selected <= 9) {
                if (FA2_STORE->IsUsable(m_selected - 2)) {
                    m_game->Play(1);
                    picked = 1;
                    continue;
                }
            }
        }
        if (DAT_0057ba28[VK_DOWN]) {
            dir++;
        } else if (DAT_0057ba28[VK_UP]) {
            dir--;
        }
        hit = HitTest(mouse, 0, 9);
        if (hit != -1 && dir == 0) {
            if (hit != m_selected) {
                m_game->Play(0);
            }
            m_selected = hit;
        }
        m_selected += dir;
        if (dir != 0) {
            m_game->Play(0);
        }
        if (m_selected < 0) {
            m_selected = 0;
        } else if (dir == 1 && m_selected == 2) {
            m_selected = dir;
        } else if (dir == -1 && m_selected == 1) {
            m_selected = 2;
        } else if (m_selected > 9) {
            m_selected = 9;
        }
        if (dir != 0) {
            float cx = m_items[m_selected]->GetTopLeft().x +
                       (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f;
            float cy = m_items[m_selected]->GetTopLeft().y +
                       (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f;
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&FA2_Point(cx * sx, cy * sy));
        }
        if (DAT_0057ba20) {
            if (m_items[1]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                done = 1;
                continue;
            }
            if (m_items[0]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                m_selected = 2;
                float cx = m_items[2]->GetTopLeft().x +
                           (m_items[2]->GetBottomRight().x - m_items[2]->GetTopLeft().x) * 0.5f;
                float cy = m_items[2]->GetTopLeft().y +
                           (m_items[2]->GetBottomRight().y - m_items[2]->GetTopLeft().y) * 0.5f;
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&FA2_Point(cx * sx, cy * sy));
            }
            hit = HitTest(mouse, 2, 9);
            if (hit != -1) {
                if (FA2_STORE->IsUsable(hit - 2)) {
                    m_game->Play(1);
                    picked = 1;
                    continue;
                }
            }
        }
        Slot3(0);
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

// FUNCTION: RALLYTROPHY 0x44c320
void ReplayMenu::BuildRects()
{
    float top;
    UIRect* rect;
    FA1_Point a;
    HudPoint b;
    top = FA1_TOP - FA1_MARGIN;
    b.x = FA1_LEFT + FA1_TITLEW;
    a.x = FA1_LEFT;
    a.y = top;
    b.y = FA1_PITCH + top;
    rect = (UIRect*)new FA1_Rect(a, b);
    m_items.push_back(rect);
    b.x = FA1_LEFT + FA1_BACKW;
    a.x = FA1_LEFT;
    a.y = FA1_BACKY - FA1_BACKMARGIN;
    b.y = FA1_BACKY + FA1_PITCH;
    rect = (UIRect*)new FA1_Rect(a, b);
    m_items.push_back(rect);
    for (int i = 0; i < 8; i++) {
        b.x = FA1_COLX + FA1_SLOTW;
        a.x = FA1_COLX;
        a.y = i * FA1_PITCH + top;
        b.y = (i + 1) * FA1_PITCH + top;
        rect = (UIRect*)new FA1_Rect(a, b);
        m_items.push_back(rect);
    }
}

// NONMATCHING: RALLYTROPHY 0x44c580
// Identical size and instruction mix; only the register for the rally index in the "v -= table[rally]" step differs
// (original edx with the Game load after the multiply, mine esi with the Game load hoisted; 13 bytes).
// Named offsets, factored multiplication, an inline rally accessor, and a volatile Game* all emit worse or unchanged code.
// original: FUN_0044c580
void ReplayMenu::DrawField(int k)
{
    TextRenderer* text = *(TextRenderer**)((BYTE*)m_game + 0x1390bc);
    std::string left;
    std::string right;
    Color color(DAT_00508c04);
    ReplayRecord* rec = &records[k];
    char buf[0x20];
    char dateBuf[0x20];
    if (rec->empty) {
        return;
    }
    float x = DAT_0056aea8 * 485.0f;
    float y = DAT_0056aeac * 598.0f;
    FA3_L.AssignA(FUN_00405380(0x9a));
    FA3_L.AppendA(": ");
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    x = 908.0f * DAT_0056aea8 - text->GetWidth(rec->text, 0);
    text->Print(DAT_0056b0ac, x, y, rec->text, 0, 1.0f, 1.0f);
    y = DAT_0056aeac * 18.0f + y;
    x = DAT_0056aea8 * 485.0f;
    FA3_L.AssignA(FUN_00405380(0x61));
    FA3_L.AppendA(": ");
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    FA3_L.AssignA(FUN_00405380(0x138 + rec->textIndex));
    x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    y = DAT_0056aeac * 18.0f + y;
    x = DAT_0056aea8 * 485.0f;
    FA3_L.AssignA(FUN_00405380(0xf9));
    FA3_L.AppendA(": ");
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    FA3_L.AssignB(FUN_00405380(0xb4 + rec->rally));
    FA3_L.AppendA(" ");
    BYTE flag = rec->flagE;
    const char* s;
    if (flag) {
        s = FUN_00405380(0x2a);
    } else {
        s = FUN_00405380(0x1b);
    }
    FA3_L.AppendA(s);
    FA3_L.AppendA(" ");
    int v = rec->value;
    if (flag) {
        v -= *(int*)((BYTE*)m_game + rec->rally * 1512 + 0x3680);
    }
    itoa(v, buf, 10);
    FA3_L.AppendA(buf);
    x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    y = DAT_0056aeac * 18.0f + y;
    x = DAT_0056aea8 * 485.0f;
    FA3_L.AssignB(FUN_00405380(0x1a0));
    FA3_L.AppendD(": ");
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    if (rec->flagD) {
        FA3_L.AssignC(FUN_00405380(0x1a2));
    } else {
        FA3_L.AssignC(FUN_00405380(0x1a1));
    }
    x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    y = DAT_0056aeac * 18.0f + y;
    x = DAT_0056aea8 * 485.0f;
    FA3_L.AssignC(FUN_00405380(0x171));
    FA3_L.AppendD(": ");
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    GetDateFormatA(0x400, 0, &rec->when, NULL, dateBuf, 0x20);
    FA3_R.AssignC(dateBuf);
    GetTimeFormatA(0x400, 0, &rec->when, NULL, dateBuf, 0x20);
    FA3_R.AppendC(" / ");
    FA3_R.AppendC(dateBuf);
    x = 908.0f * DAT_0056aea8 - text->GetWidth(right.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, right.c_str(), 0, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x44cf80
void ReplayMenu::RefreshRecords()
{
    for (int i = 0; i < 8; i++) {
        (*(ReplayStore**)((BYTE*)m_game + 0x2dd0))->Load(i, &records[i]);
    }
}

// NONMATCHING: RALLYTROPHY 0x44cfb0
// Same shape, 625 vs 632 bytes: the original spills `this` (saved at entry) and keeps the label and record pointers in
// ebp/ebx; mine keeps `this` in ebx and spills the record pointer; the SEH prologue also schedules differently.
// original: FUN_0044cfb0 (vtable slot 3, Draw)
void ReplayMenu::Slot3(int skipPresent)
{
    MenuGame* game = m_game;
    TextRenderer* text = game->text;
    game->UpdateVideo();
    if (DAT_0056b0ac->BeginScene()) {
        DAT_0056b0ac->vf38(0);
        game->DrawVideo(0);
        text->scaleX = DAT_0056aea8 * 2.0f;
        text->color = DAT_00508bfc;
        text->scaleY = DAT_0056aeac * 2.0f;
        text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x19e), 0, 1.0f, 1.0f);
        text->SetScale(DAT_0056aea8, DAT_0056aeac);
        game->FUN_00435de0(480.0f, 240.0f);
        game->FUN_00435f50(480.0f, 590.0f);
        for (int i = 0; i < 10; i++) {
            Color col;
            int selected = (i == m_selected);
            col.value = selected ? DAT_00508c0c : DAT_00508c04;
            text->color = col.value;
            if (i < 2) {
                text->Print(DAT_0056b0ac, labels[i].x, labels[i].y, labels[i].text.c_str(), 0, 1.0f, 1.0f);
            } else {
                float px = FA1_COLX * DAT_0056aea8;
                float py = ((i - 2) * FA1_PITCH + FA1_ROWY) * DAT_0056aeac;
                const char* s = records[i - 2].empty ? FUN_00405380(0x27) : records[i - 2].name;
                text->Print(DAT_0056b0ac, px, py, s, 0, 1.0f, 1.0f);
            }
        }
        if (m_selected > 1) {
            DrawField(m_selected - 2);
        }
        ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 1, 1, 1, 1, -1.0f);
        DrawCursor(*((MouseInput*)DAT_0056b0c0)->GetPosition());
        DAT_0056b0ac->EndScene();
        if (skipPresent != 1) {
            DAT_0056b0ac->Present();
        }
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x44d230
// SYMBOL: _$E23
typedef void Generated_44d230();

// FUNCTION: RALLYTROPHY 0x44d250
// SYMBOL: _$E22
typedef void Generated_44d250();
