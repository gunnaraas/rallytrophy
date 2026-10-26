// GameScreen11A4 (0x427600-0x4295bf): the menu screen embedded in the Game object at +0x11a4. Map:
// analysis/range-4266c0.md module 2; class in include/unk_427600.h.
// Work groups: H2a (ctor/dtors, Setup, Release, Run, vector insert 0x4293c0, module-end stubs; prefix H2a_).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_452380.h"
#include "unk_427600.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group H2a ----

// ---- from group draft H2b ----
extern int DAT_0050132c, DAT_00501330, DAT_00501334, DAT_00501458;
extern int DAT_004f6b04[6];            // text ids of the six items
extern float DAT_00508bd8;

class H2b_Game {
public:
    void FUN_00437e20(float a, float b, float c, float d, int e); // 0x437e20 (ret 0x14)
};
static float H2b_firstY = 262.0f;      // 0x4e155c
static float H2b_step = 42.0f;         // 0x4e1560
static float H2b_backY = 610.0f;       // 0x4e1564
static float H2b_toggleW = 64.0f;      // 0x4e1570
static float H2b_listW = 350.0f;       // 0x4e1574

// The mouse warp temporary: inline ctor, empty dtor (as ServiceMenu::Frame).
struct H2b_WarpPoint {
    H2b_WarpPoint(float px, float py)
    {
        x = px;
        y = py;
    }
    ~H2b_WarpPoint() {}
    float x, y;
};


// ---- from group draft H2c ----
struct H2c_Point {
    H2c_Point(const H2c_Point& other); // 0x424300
    ~H2c_Point() {}
    float x, y;
};
// The HUD rectangle constructor (0x4390a0) as the loader calls it.
class H2c_Rect {
public:
    H2c_Rect(H2c_Point topLeft, HudPoint bottomRight); // 0x4390a0
    float f[5];
};
// A corner passed by value to the out-of-line UIRect setters.
struct H2c_Corner {
    H2c_Corner(float px, float py) { x = px; y = py; }
    H2c_Corner(const H2c_Corner& other) { x = other.x; y = other.y; }
    ~H2c_Corner() {}
    float x, y;
};
// UIRect as the loader moves it: getters 0x4243d0/0x4243f0, out-of-line setters 0x4295c0/0x4295d0.
class H2c_UIRect {
public:
    HudPoint GetTopLeft();             // 0x4243d0
    HudPoint GetBottomRight();         // 0x4243f0
    void SetTopLeft(H2c_Corner p);     // 0x4295c0
    void SetBottomRight(H2c_Corner p); // 0x4295d0
    void SetEnabled(int e) { enabled = e; }
    float left, top, right, bottom;
    int enabled;                       // 0x10
};
// vector<UIRect*>::operator[] kept out of line (0x4242f0).
class H2c_Items {
public:
    H2c_UIRect*& Get(int index);       // 0x4242f0
};
// A std::string (same layout) whose assign(const char*, n) (0x404ba0) stays out of line, as in the original.
class H2c_String {
public:
    H2c_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        Assign(s, strlen(s));
    }
    ~H2c_String() { Free(true); }
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
    void Tidy(bool built);             // 0x402700
    void Assign(const char* s, unsigned int n); // 0x404ba0
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};
// std::vector<UIRect*> (same layout) whose insert(pos, x) (0x4293c0) stays out of line: the original expands it only for
// the last two rectangles.
class H2c_Vector {
public:
    UIRect** insert(UIRect** pos, UIRect* const& x); // 0x4293c0
    UIRect** end() { return last; }
    std::allocator<UIRect*> alloc;
    UIRect** first;
    UIRect** last;
    UIRect** unk1d0;
};
#define H2c_NEW(ul, lr) \
    (rect = (UIRect*)new H2c_Rect(*(const H2c_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)))
#define H2c_ADD(ul, lr) \
    do { \
        H2c_NEW(ul, lr); \
        ((H2c_Vector&)items).insert(((H2c_Vector&)items).end(), rect); \
    } while (0)
#define H2c_PUSH(ul, lr) \
    do { \
        H2c_NEW(ul, lr); \
        items.push_back(rect); \
    } while (0)
// Moves item i up by 42 design pixels (vector::operator[] stays out of line here; the corner temporaries carry EH states).
#define H2c_ITEM(i) ((H2c_Items&)items).Get(i)
#define H2c_RAISE(i) \
    do { \
        H2c_ITEM(i)->SetTopLeft(H2c_Corner(H2c_ITEM(i)->GetTopLeft().x, H2c_ITEM(i)->GetTopLeft().y - 42.0f)); \
        H2c_ITEM(i)->SetBottomRight(H2c_Corner(H2c_ITEM(i)->GetBottomRight().x, H2c_ITEM(i)->GetBottomRight().y - 42.0f)); \
    } while (0)
extern ConfigKey DAT_004f6b20[];       // gamesettingsmenu.ini key table (24 keys)
extern int DAT_00501458;
extern int DAT_00501464;

// FUNCTION: RALLYTROPHY 0x427630
GameScreen11A4::GameScreen11A4()
{
    m_selected = 0;
    m_state = 0;
    texts[0].text = FUN_00405380(9);
    texts[1].text = FUN_00405380(10);
    texts[2].text = FUN_00405380(11);
    values3[0].text = "3";
    values3[1].text = "6";
    values3[2].text = "9";
    values6[0].text = "1";
    values6[1].text = "2";
    values6[2].text = "3";
    values6[3].text = "4";
    values6[4].text = "5";
    values6[5].text = "6";
}

// FUNCTION: RALLYTROPHY 0x427a50
// SYMBOL: ??_GGameScreen11A4@@UAEPAXI@Z
typedef void Generated_427a50();

// FUNCTION: RALLYTROPHY 0x427a70
GameScreen11A4::~GameScreen11A4()
{
    Release();
}

// FUNCTION: RALLYTROPHY 0x427b10
void GameScreen11A4::Setup(MenuGame* game)
{
    if (!m_state) {
        m_game = game;
        m_state = 1;
    }
}

// FUNCTION: RALLYTROPHY 0x427b30
void GameScreen11A4::Release()
{
    if (m_state) {
        m_state = 0;
        m_game = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x427b40
int GameScreen11A4::Run(MenuGame* game)
{
    Setup(game);
    unk1d0 = 1;
    if ((DAT_00501458 && DAT_005014c9) || (!DAT_00501458 && DAT_005014c8)) {
        unk1d0 = 2;
    }
    if (DAT_0050132c > unk1d0) {
        DAT_0050132c = unk1d0;
    }
    if (m_selected == 2 && !DAT_00501458) {
        m_selected = 1;
    }
    if (m_selected == 1 && DAT_00501464 == 3) {
        m_selected = 0;
    }
    if (m_selected == 3 && DAT_00501464 != 0) {
        m_selected = 0;
    }
    if (m_selected == 0 && DAT_00501458) {
        m_selected = 1;
    }
    int result = Frame();
    if (DAT_00501458) {
        DAT_00501468 = DAT_004f6af8[DAT_00501330];
    } else {
        DAT_00501468 = 1;
    }
    return result;
}

// NONMATCHING: RALLYTROPHY 0x427c10
// 2747 vs 2745 bytes. Control flow, EH states and all calls match. Left: `clicked`/`sy` frame slots swapped (0x24/0x20
// vs 0x20/0x24), and in the title-text switch and the item Print the scheduler picks other scratch registers for
// the device pointer (ecx/edx/eax rotated). Tried: declaration order, case order, if-chains, locals for device/xy.
// original: FUN_00427c10 (thiscall; one frame loop of the screen)
int GameScreen11A4::Frame()
{
    int done = 0;
    HudPoint mouse;
    LoadLayout();
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    int clicked = 0;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    DAT_0056b0bc->ClearKeys();
    TextRenderer* text = m_game->text;
    int first = 1;
    int wait = 0;
    while (FUN_004b69f0(wait) && !done && !clicked) {
        m_game->Update();
        mouse = *(HudPoint*)&DAT_0057ba00;
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            int hit = HitTest(mouse, 0, 5);
            if (hit != -1) {
                m_selected = hit;
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
            if (m_selected == 5) {
                done = 1;
                continue;
            }
            if (m_selected == 4 || m_selected == 3) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
        }
        int hit = HitTest(mouse, 0, 5);
        if (hit != -1) {
            if (m_selected != hit) {
                m_game->Play(0);
            }
            m_selected = hit;
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
        }
        m_selected += dir;
        int net = DAT_00501464;
        int mode = DAT_00501458;
        for (;;) {
            int step = dir;
            if (hit != -1 && dir == 0) {
                step = 1;
            }
            int changed = 0;
            if (m_selected == 0 && mode) {
                m_selected = step;
                changed = 1;
            }
            if (m_selected == 2 && !mode) {
                m_selected = step + 2;
                changed = 1;
            }
            if (m_selected == 1 && net == 3) {
                m_selected = step + 1;
                changed = 1;
            }
            if (m_selected == 3 && net) {
                m_selected = step + 3;
                continue;
            }
            if (!changed) {
                break;
            }
        }
        if (m_selected < 0) {
            m_selected = 5;
        } else if (m_selected > 5) {
            m_selected = 0;
        }
        int lr = 0;
        if (DAT_0057ba28[VK_LEFT]) {
            lr = -1;
        }
        if (DAT_0057ba28[VK_RIGHT]) {
            lr++;
        }
        if (dir != 0) {
            int cx = (int)(m_items[m_selected]->GetTopLeft().x +
                           (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f);
            int cy = (int)(m_items[m_selected]->GetTopLeft().y +
                           (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f);
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&H2b_WarpPoint(cx * sx, cy * sy));
        }
        if (DAT_0057ba20) {
            if (m_items[5]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                done = 1;
                continue;
            }
            if ((m_items[3]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && m_items[3]->enabled) ||
                m_items[4]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
            if (m_items[6]->Contains(mouse) && m_items[6]->enabled) {
                lr--;
            }
            if (m_items[8]->Contains(mouse) && m_items[8]->enabled) {
                lr--;
            }
            if (m_items[10]->Contains(mouse) && m_items[10]->enabled) {
                lr--;
            }
            if (m_items[7]->Contains(mouse) && m_items[7]->enabled) {
                lr++;
            }
            if (m_items[9]->Contains(mouse) && m_items[9]->enabled) {
                lr++;
            }
            if (m_items[11]->Contains(mouse) && m_items[11]->enabled) {
                lr++;
            }
        }
        if (lr != 0) {
            switch (m_selected) {
            case 2:
                AdjustValue(&DAT_00501330, lr, 0, 2);
                break;
            case 1:
                AdjustValue(&DAT_0050132c, lr, 0, unk1d0);
                break;
            case 0:
                AdjustValue(&DAT_00501334, lr, 1, 6);
                break;
            }
        }
        MenuGame* game = m_game;
        game->UpdateVideo();
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            game->DrawVideo(0);
            text->scaleX = DAT_0056aea8 * 2.0f;
            text->color = DAT_00508bfc;
            text->scaleY = DAT_0056aeac * 2.0f;
            switch (DAT_00501464) {
            case 2:
                text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x37), 0, 1.0f, 1.0f);
                break;
            case 1:
                text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x36), 0, 1.0f, 1.0f);
                break;
            case 0:
                text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x35), 0, 1.0f, 1.0f);
                break;
            }
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            float y = H2b_firstY * sy;
            float x = sx * 54.0f;
            for (int i = 0; i < 6; i++) {
                if (i == 2) {
                    if (!DAT_00501458) {
                        continue;
                    }
                } else if (i == 1) {
                    if (DAT_00501464 == 3) {
                        continue;
                    }
                } else if (i == 3) {
                    if (DAT_00501464) {
                        continue;
                    }
                } else if (i == 0) {
                    if (DAT_00501458) {
                        continue;
                    }
                }
                int selected = (i == m_selected);
                Color col;
                col.value = selected ? DAT_00508c0c : DAT_00508c04;
                if (i == 5) {
                    y = H2b_backY * sy;
                }
                text->color = col.value;
                text->Print(DAT_0056b0ac, x, y, FUN_00405380(DAT_004f6b04[i]), 0, 1.0f, 1.0f);
                HudPoint pos;
                pos.x = DAT_00508bd8 * DAT_0056aea8;
                pos.y = y;
                switch (i) {
                case 2:
                    DrawToggle(text, values3, pos.x, pos.y, H2b_toggleW * sx, selected, DAT_00501330, 0, 2);
                    break;
                case 1:
                    pos.x = 520.0f * DAT_0056aea8;
                    DrawToggle(text, texts, pos.x, pos.y, H2b_listW * sx, selected, DAT_0050132c, 0, unk1d0);
                    break;
                case 0:
                    DrawToggle(text, values6, pos.x, pos.y, H2b_toggleW * sx, selected, DAT_00501334 - 1, 0, 5);
                    break;
                }
                y += H2b_step * sy;
            }
            int n = 4;
            if (DAT_00501464 == 1) {
                n = 3;
            } else if (DAT_00501464 == 3) {
                n = 2;
            } else if (DAT_00501464 == 2) {
                n = 3;
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, n, 1, 1, 1, -1.0f);
            ((H2b_Game*)DAT_0056b0c8)->FUN_00437e20(520.0f * DAT_0056aea8, H2b_firstY * DAT_0056aeac,
                                                    (H2b_listW + 24.0f) * DAT_0056aea8, (H2b_step - 6.0f) * DAT_0056aeac, 2);
            DrawCursor(mouse);
            DAT_0056b0ac->EndScene();
            DAT_0056b0ac->Present();
        }
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    int r = m_selected;
    FUN_00438d00();
    if (done || DAT_0057b9f4) {
        r = -1;
        m_game->Play(2);
    }
    return r;
}

// FUNCTION: RALLYTROPHY 0x4286d0
// Builds the 12 hit rectangles of gamesettingsmenu.ini, then hides and shifts rows by game mode (DAT_00501458,
// DAT_00501464). Matched with: out-of-line insert pinned for the first 10 rects, the vector reference taken after the
// first `new`, an inline end(), a corner type with copy ctor + empty dtor (built in the argument slot) and SetEnabled.
// original: FUN_004286d0 (thiscall; GameScreen11A4 layout loader, called by 0x427c10)
void GameScreen11A4::LoadLayout()
{
    Config config;
    UIRect* rect;
    config.Define(DAT_004f6b20, 0x18);
    H2c_String path("data/menu/gamesettingsmenu.ini");
    config.Load(path.c_str(), true, false, 0);
    H2c_NEW("PlayersText_UpperLeft", "PlayersText_LowerRight");
    std::vector<UIRect*>& items = m_items;
    ((H2c_Vector&)items).insert(((H2c_Vector&)items).end(), rect);
    H2c_ADD("DifficultyText_UpperLeft", "DifficultyText_LowerRight");
    H2c_ADD("LapsText_UpperLeft", "LapsText_LowerRight");
    H2c_ADD("LoadGameText_UpperLeft", "LoadGameText_LowerRight");
    H2c_ADD("ContinueText_UpperLeft", "ContinueText_LowerRight");
    H2c_ADD("BackText_UpperLeft", "BackText_LowerRight");
    H2c_ADD("PlayersArrow1_UpperLeft", "PlayersArrow1_LowerRight");
    H2c_ADD("PlayersArrow2_UpperLeft", "PlayersArrow2_LowerRight");
    H2c_ADD("DifficultyArrow1_UpperLeft", "DifficultyArrow1_LowerRight");
    H2c_ADD("DifficultyArrow2_UpperLeft", "DifficultyArrow2_LowerRight");
    H2c_PUSH("LapsArrow1_UpperLeft", "LapsArrow1_LowerRight");
    H2c_PUSH("LapsArrow2_UpperLeft", "LapsArrow2_LowerRight");
    int i;
    if (DAT_00501458) {
        for (i = 1; i < 5; i++)
            H2c_RAISE(i);
        for (i = 0; i < 4; i++)
            H2c_RAISE(i + 8);
        ((H2c_UIRect*)m_items[0])->SetEnabled(0);
        ((H2c_UIRect*)m_items[6])->SetEnabled(0);
        ((H2c_UIRect*)m_items[7])->SetEnabled(0);
    }
    if (DAT_00501464 == 3) {
        for (i = 2; i < 5; i++)
            H2c_RAISE(i);
        for (i = 0; i < 2; i++)
            H2c_RAISE(i + 10);
        ((H2c_UIRect*)m_items[1])->SetEnabled(0);
        ((H2c_UIRect*)m_items[8])->SetEnabled(0);
        ((H2c_UIRect*)m_items[9])->SetEnabled(0);
    }
    if (DAT_00501464) {
        H2c_RAISE(4);
        ((H2c_UIRect*)m_items[3])->SetEnabled(0);
    }
    if (!DAT_00501458) {
        for (i = 3; i < 5; i++)
            H2c_RAISE(i);
        ((H2c_UIRect*)m_items[2])->SetEnabled(0);
        ((H2c_UIRect*)m_items[10])->SetEnabled(0);
        ((H2c_UIRect*)m_items[11])->SetEnabled(0);
    }
}

// FUNCTION: RALLYTROPHY 0x4293c0
// SYMBOL: ?insert@?$vector@PAVUIRect@@V?$allocator@PAVUIRect@@@std@@@std@@QAEPAPAVUIRect@@PAPAV3@ABQAV3@@Z
// std::vector<UIRect*>::insert(it, x), instantiated by the rect pushes of 0x4286d0. Until that function is in this
// file, the explicit instantiation below emits it.
template class std::vector<UIRect*>;
typedef void Generated_4293c0();

// ---- module end (H2a): the STL header statics' guard initialiser and atexit registration ----

// FUNCTION: RALLYTROPHY 0x429590
// SYMBOL: _$E23
typedef void Generated_429590();

// FUNCTION: RALLYTROPHY 0x4295b0
// SYMBOL: _$E22
typedef void Generated_4295b0();
