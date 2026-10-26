// GameScreen24FC (0x42e930-0x4303df): the second eight-slot list screen embedded in the Game object at +0x24fc.
// Map: analysis/range-4266c0.md module 5; class in include/unk_42e930.h.
// Work groups: H5a (lifecycle, Setup, Run, module end: prefix H5a_), and the others.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_44ea60.h"
#include "unk_42e930.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "unk_44d260.h"

// ---- Group H5a ----
// Layout constants of the 1024x768 design space (the same values as SlotMenu's: 0x4e1678, 0x4e1664, 0x4e1674,
// 0x4e167c, 0x4e1698, 0x4e1668).
static const float H5a_LEFT = 54.0f;     // title and back text x
static const float H5a_TOP = 262.0f;     // title y
static const float H5a_BACKY = 610.0f;   // back text y
static const float H5a_COLX = 485.0f;    // slot column x
static const float H5a_ROWY = 260.0f;    // slot rows y
static const float H5a_PITCH = 42.0f;    // row pitch


// ---- from group draft H5b ----
struct H5b_WarpPoint {
    H5b_WarpPoint(float px, float py)
    {
        x = px;
        y = py;
    }
    ~H5b_WarpPoint() {}
    float x, y;
};
#define H5B_SAVE (*(SaveGame**)((BYTE*)m_game + 0x2dcc))


// ---- from group draft H5c ----

// ---- from group draft H5d ----
struct H5d_Str {
    int alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
    void AssignCall(const char* s, unsigned int n);   // 0x404ba0
    void AppendCall(const char* s, unsigned int n);   // 0x403ba0
    bool Grow(unsigned int n, bool trim);              // 0x402980
    void Eos(unsigned int n);                         // 0x402960
    static void Xlen();                                      // 0x4bd00b
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
#define H5d_L (*(H5d_Str*)&left)
#define H5d_R (*(H5d_Str*)&right)

// FUNCTION: RALLYTROPHY 0x42e930
GameScreen24FC::GameScreen24FC()
{
    m_selected = 0;
    m_state = 0;
}

// FUNCTION: RALLYTROPHY 0x42e9a0
// SYMBOL: ??_GGameScreen24FC@@UAEPAXI@Z
typedef void Generated_42e9a0();

// FUNCTION: RALLYTROPHY 0x42e9c0
GameScreen24FC::~GameScreen24FC()
{
    Release();
}

// FUNCTION: RALLYTROPHY 0x42ea30
// Evidence: ev_770eda4b60286cc6c214e118b921ed6fb5aefc20b39f35b4307f40cb67f601e4.
// Store the game pointer before float setup and reuse y for target stack slots.
void GameScreen24FC::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    m_game = game;
    m_state = 1;
    float x = H5a_LEFT * DAT_0056aea8;
    float y = H5a_TOP * DAT_0056aeac;
    labels[0].text = FUN_00405380(0x99);
    labels[0].x = x;
    labels[0].y = y;
    y = H5a_BACKY * DAT_0056aeac;
    labels[1].text = FUN_00405380(0x18);
    labels[1].x = x;
    labels[1].y = y;
    float sx = H5a_COLX * DAT_0056aea8;
    float sy = DAT_0056aeac * 260.0f; // literal preserves target global/constant operand order
    float step = H5a_PITCH * DAT_0056aeac;
    for (int i = 2; i < 10; i++) {
        labels[i].x = sx;
        labels[i].y = sy;
        sy += step;
    }
    BuildRects();
}

// FUNCTION: RALLYTROPHY 0x42ec40
int GameScreen24FC::Run(MenuGame* game)
{
    Setup(game);
    m_selected = 0;
    return Frame();
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// original: FUN_0042ec70
// FUNCTION: RALLYTROPHY 0x42ec70
int GameScreen24FC::Frame()
{
    HudPoint mouse;
    int done = 0;
    std::string unused;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    int clicked = 0;
    DAT_0056b0bc->ClearKeys();
    int first = 1;
    RefreshSlots();
    int wait = 0;
    while (FUN_004b69f0(wait) && !done && !clicked) {
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
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&H5b_WarpPoint(cx * sx, cy * sy));
            } else if (m_selected >= 2 && m_selected <= 9) {
                if (H5B_SAVE->LoadSlot(m_selected - 2)) {
                    m_game->Play(1);
                    clicked = 1;
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
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&H5b_WarpPoint(cx * sx, cy * sy));
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
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&H5b_WarpPoint(cx * sx, cy * sy));
            }
            hit = HitTest(mouse, 2, 9);
            if (hit != -1) {
                if (H5B_SAVE->LoadSlot(hit - 2)) {
                    m_game->Play(1);
                    clicked = 1;
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

// NONMATCHING: RALLYTROPHY 0x42f360
// Rectangle construction and vector insertion are recovered; exact temporary layout and x87 expression order differ.
void GameScreen24FC::BuildRects()
{
    float top = 0.0f;
    float left = 0.0f;
    float bottom = 0.0f;
    float right = 0.0f;
    int i;
    for (i = 0; i != 10; ++i) {
        // The first two rectangles frame the list; the remaining eight are row hit areas.
        if (i == 0) {
            left = 54.0f;
            top = 242.0f;
            right = 474.0f;
            bottom = 504.0f;
        } else if (i == 1) {
            left = 54.0f;
            top = 484.0f;
            right = 600.0f;
            bottom = 652.0f;
        } else {
            left = 485.0f;
            right = 527.0f;
            top = 430.0f + (float)(i - 2) * 42.0f;
            bottom = top + 42.0f;
        }
        HudPoint a, b;
        a.x = left; a.y = top;
        b.x = right; b.y = bottom;
        UIRect* rect = new UIRect(a, b);
        m_items.push_back(rect);
    }
}

// original: FUN_0042f5c0
// FUNCTION: RALLYTROPHY 0x42f5c0
void GameScreen24FC::DrawField(int k)
{
    TextRenderer* text = *(TextRenderer**)((BYTE*)m_game + 0x1390bc);
    std::string left;
    std::string right;
    Color color(DAT_00508c04);
    float x = DAT_0056aea8 * 485.0f;
    float y = DAT_0056aeac * 598.0f;
    char buf[0x20];
    char dateBuf[0x20];
    if (records[k].kind == 1) {
        H5d_L.AssignA(FUN_00405380(0x9a));
        H5d_L.AppendA(": ");
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        x = 908.0f * DAT_0056aea8 - text->GetWidth(records[k].text, 0);
        text->Print(DAT_0056b0ac, x, y, records[k].text, 0, 1.0f, 1.0f);
    } else if (records[k].kind == 0) {
        return;
    } else {
        H5d_L.AssignA(FUN_00405380(0x96));
        H5d_L.AppendA(": ");
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        itoa(records[k].kind, buf, 10);
        H5d_L.AssignA(buf);
        x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    }
    y = DAT_0056aeac * 18.0f + y;
    x = DAT_0056aea8 * 485.0f;
    H5d_L.AssignA(FUN_00405380(0x9f));
    H5d_L.AppendA(": ");
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    H5d_L.AssignB(FUN_00405380(0xb4 + records[k].index));
    H5d_L.AppendA(" ");
    H5d_L.AppendA(FUN_00405380(0x1b));
    H5d_L.AppendA(" ");
    int v = records[k].value;
    if (DAT_00501458) {
        v -= *(int*)((BYTE*)m_game + records[k].index * 1512 + 0x3680);
    }
    itoa(v, buf, 10);
    H5d_L.AppendA(buf);
    x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    if (records[k].kind == 1) {
        y = DAT_0056aeac * 18.0f + y;
        x = DAT_0056aea8 * 485.0f;
        H5d_L.AssignB(FUN_00405380(0xa0));
        H5d_L.AppendA(": ");
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        if (records[k].count1 > 0) {
            itoa(records[k].count1, buf, 10);
            H5d_L.AssignB(buf);
        } else {
            H5d_L.AssignB("-");
        }
        x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        y = DAT_0056aeac * 18.0f + y;
        x = DAT_0056aea8 * 485.0f;
        H5d_L.AssignC(FUN_00405380(0xa1));
        H5d_L.AppendD(": ");
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        if (records[k].count2 > 0) {
            itoa(records[k].count2, buf, 10);
            H5d_L.AssignC(buf);
        } else {
            H5d_L.AssignC("-");
        }
        x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    }
    y = DAT_0056aeac * 18.0f + y;
    x = DAT_0056aea8 * 485.0f;
    H5d_L.AssignC(FUN_00405380(0x171));
    H5d_L.AppendC(": ");
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    GetDateFormatA(0x400, 0, &records[k].when, NULL, dateBuf, 0x20);
    H5d_R.AssignC(dateBuf);
    GetTimeFormatA(0x400, 0, &records[k].when, NULL, dateBuf, 0x20);
    H5d_R.AppendC(" / ");
    H5d_R.AppendC(dateBuf);
    x = 908.0f * DAT_0056aea8 - text->GetWidth(right.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, right.c_str(), 0, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x4303b0
// SYMBOL: _$E23
typedef void Generated_4303b0();

// FUNCTION: RALLYTROPHY 0x4303d0
// SYMBOL: _$E22
typedef void Generated_4303d0();
