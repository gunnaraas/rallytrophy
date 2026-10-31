// MenuScreen8DC (0x4243b0-0x424a?? excluding the big draw routine 0x424960) and small accessors.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_43f9e0.h"
#include "unk_439170.h"
#include "unk_43c590.h"
#include <string.h>
#include <vector>

extern float DAT_0057ba00;
// The input object DAT_0056b0b4 (class T_56b0b4 in unk_495640.h, which conflicts with rallytrophy.h).
class InputConfig {
public:
    void ConfigureDevices();           // 0x497d80
};
extern InputConfig* DAT_0056b0b4_;

// FUNCTION: RALLYTROPHY 0x4243b0
void MenuScreen8DC::SetValue120(int v)
{
    *(int*)((BYTE*)this + 0x120) = v;
}

// FUNCTION: RALLYTROPHY 0x4243c0
HudPoint* __stdcall GetMousePosition()
{
    return (HudPoint*)&DAT_0057ba00;
}

// FUNCTION: RALLYTROPHY 0x4243d0
HudPoint UIRect::GetTopLeft()
{
    return *(HudPoint*)&left;
}

// FUNCTION: RALLYTROPHY 0x4243f0
HudPoint UIRect::GetBottomRight()
{
    return *(HudPoint*)&right;
}

// FUNCTION: RALLYTROPHY 0x424410
MenuScreen8DC::MenuScreen8DC()
{
    m_state = 0;
    m_game = 0;
    m_selected = 0;
}

// FUNCTION: RALLYTROPHY 0x4244b0
// SYMBOL: ??_GMenuScreen8DC@@UAEPAXI@Z
typedef void Generated_4244b0();

// FUNCTION: RALLYTROPHY 0x4244d0
MenuScreen8DC::~MenuScreen8DC()
{
    Release();
}

// FUNCTION: RALLYTROPHY 0x424920
int MenuScreen8DC::Reset(MenuGame* game, int arg)
{
    *(int*)((BYTE*)this + 0x168) = arg;
    Setup(game);
    while (true) {
        int r = Frame();
        if (r == -1) break;
        if (r == 0) DAT_0056b0b4_->ConfigureDevices();
    }
    return -1;
}

// ---- 0x424960 / 0x425220 ----
// The HUD rectangle constructor (0x4390a0) as this loader calls it: the lower right corner is copied inline from the
// config vector.
// A 2D point whose copy constructor is out of line (0x424300) and whose destructor is empty.
struct C_Point {
    C_Point(const C_Point& other);     // 0x424300
    ~C_Point() {}
    float x, y;
};
class C_Rect {
public:
    C_Rect(C_Point topLeft, HudPoint bottomRight);   // 0x4390a0
    float f[5];
};
// A std::string (same layout) whose _Grow (0x402980) and _Eos (0x402960) stay out of line, as in the original.
class C_String {
public:
    C_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~C_String() { Free(true); }
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
#define C_ADD(ul, lr) \
    do { \
        rect = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)

#define C_ADDR(rect, ul, lr) \
    do { \
        rect = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)

#define C_ADDR(rect, ul, lr) \
    do { \
        rect = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)
static inline int Bal(int a) { return a + 1; }

struct C_Warp {
    C_Warp(float px, float py) { x = px; y = py; }
    ~C_Warp() {}
    float x, y;
};
struct C_Pt {
    C_Pt(const C_Pt& o) { x = o.x; y = o.y; }
    ~C_Pt() {}
    float x, y;
};
class C_Item {
public:
    C_Pt GetTopLeft();      // 0x4243d0
    C_Pt GetBottomRight();  // 0x4243f0
};
#define CI(i) ((C_Item*)m_items[i])
extern ConfigKey DAT_004f64c8[];   // controlmenu.ini key table (28 keys)
class MouseInput;

// NONMATCHING: RALLYTROPHY 0x424960
// original: FUN_00424960 (fastcall, SEH). Draft: logic ported from the pseudocode, not yet compared byte for byte.
// Tried 2026-10-30 night: per-call GetPosition() args, C_Warp/C_Pt EH temps (EH states of the centre block now match), m_selected >= 6;
// still differs: the original keeps color (0x30) and two 1.0f (0x38/0x3c) as live stack slots in the draw loop plus one more EH state (0x11), frame 0x70 vs 0x68; dir*5 lea has disp32; ebx/ebp swap follows.
int MenuScreen8DC::Frame()
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
                    m_game->Play(0);
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
            m_selected += dir;
            if (m_selected < 0) {
                m_selected = 5;
            } else if (m_selected >= 6) {
                m_selected = 0;
            }
            int cx = (int)(CI(m_selected)->GetTopLeft().x +
                           (CI(m_selected)->GetBottomRight().x - CI(m_selected)->GetTopLeft().x) * 0.5f);
            int cy = (int)(CI(m_selected)->GetTopLeft().y +
                           (CI(m_selected)->GetBottomRight().y - CI(m_selected)->GetTopLeft().y) * 0.5f);
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&C_Warp(cx * sx, cy * sy));
        }
        for (int i = 0; i < 6; i++) {
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
            if (m_items[8]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) dir--;
            if (m_items[6]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) dir--;
            if (m_items[10]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) dir--;
            if (m_items[12]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) dir--;
            if (m_items[9]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) dir++;
            if (m_items[7]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) dir++;
            if (m_items[11]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) dir++;
            if (m_items[13]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) dir++;
            if (m_selected == 0) {
                m_game->Play(1);
                clicked = 1;
                continue;
            }
            if (m_selected == 5) {
                done = 1;
                continue;
            }
        }
        if (dir != 0) {
            switch (m_selected) {
            case 1:
                Toggle(&DAT_00501408, dir);
                break;
            case 2:
                AdjustVolume(&DAT_0050140c, dir * 5);
                break;
            case 3:
                AdjustVolume(&DAT_00501410, dir * 5);
                break;
            case 4:
                AdjustVolume(&DAT_00501414, dir * 5);
                break;
            }
        }
        MenuGame* game = m_game;
        game->UpdateVideo();
        if (DAT_0056b0ac->BeginScene()) {
            DAT_0056b0ac->vf38(0);
            game->DrawVideo(0);
            text->scaleX = DAT_0056aea8 * 2.0f;
            text->scaleY = DAT_0056aeac * 2.0f;
            text->color = DAT_00508bfc;
            text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x3e), 0, 1.0f, 1.0f);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            game->FUN_00435de0(517.0f, 243.0f);
            HudPoint scale = *game->GetScale();
            for (int i = 0; i < 6; i++) {
                int selected = (i == m_selected);
                text->color = selected ? DAT_00508c0c : DAT_00508c04;
                text->Print(DAT_0056b0ac, six[i].x, six[i].y, six[i].text.c_str(), 0, 1.0f, 1.0f);
                float x = 520.0f * DAT_0056aea8;   // 0x4e1500
                float y = six[i].y;
                switch (i) {
                case 1:
                    DrawSelector(text, &two[DAT_00501408].text, x, y, 400.0f * sx, selected, DAT_00501408, 0, 1);
                    break;
                case 2:
                    DrawSlider(x, y, DAT_0050140c, selected);
                    break;
                case 3:
                    DrawSlider(x, y, DAT_00501410, selected);
                    break;
                case 4:
                    DrawSlider(x, y, DAT_00501414, selected);
                    break;
                }
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 5, 0, 1, 1, -1.0f);
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

// NONMATCHING: RALLYTROPHY 0x425220
// original: FUN_00425220 (SEH). Loads the 14 hit-test rectangles from data/menu/controlmenu.ini.
// Tried 2026-10-31: three UIRect* locals (site 1 / 2-13 / 14) give the original's rect slots and frame size; one discarded Bal(1) before site 14 makes exactly the last two push_backs inline insert (401->77 insns differ; Bal at other sites, 2+ calls, 5 locals, per-site block scopes were worse); still differs: push_back temp slots (original [esp+0x18]/[esp+0x1c] at sites 8/9, ours 0x1c at site 2 and 0x18 at site 14) and the first inlined insert calls size() out of line for max(size(),1).
void MenuScreen8DC::LoadLayout()
{
    Config config;
    UIRect* ra;
    UIRect* rb;
    UIRect* rc;
    config.Define(DAT_004f64c8, 0x1c);
    C_String path("data/menu/controlmenu.ini");
    config.Load(path.c_str(), true, false, 0);
    C_ADDR(ra, "ConfigureControllersText_UpperLeft", "ConfigureControllersText_LowerRight");
    C_ADDR(rb, "ForceFeedbackStateText_UpperLeft", "ForceFeedbackStateText_LowerRight");
    C_ADDR(rb, "ForceFeedbackLevelText_UpperLeft", "ForceFeedbackLevelText_LowerRight");
    C_ADDR(rb, "SaturationLevelText_UpperLeft", "SaturationLevelText_LowerRight");
    C_ADDR(rb, "DeadzoneLevelText_UpperLeft", "DeadzoneLevelText_LowerRight");
    C_ADDR(rb, "BackText_UpperLeft", "BackText_LowerRight");
    C_ADDR(rb, "ForceFeedbackStateArrow1_UpperLeft", "ForceFeedbackStateArrow1_LowerRight");
    C_ADDR(rb, "ForceFeedbackStateArrow2_UpperLeft", "ForceFeedbackStateArrow2_LowerRight");
    C_ADDR(rb, "ForceFeedbackLevelArrow1_UpperLeft", "ForceFeedbackLevelArrow1_LowerRight");
    C_ADDR(rb, "ForceFeedbackLevelArrow2_UpperLeft", "ForceFeedbackLevelArrow2_LowerRight");
    C_ADDR(rb, "SaturationLevelArrow1_UpperLeft", "SaturationLevelArrow1_LowerRight");
    C_ADDR(rb, "SaturationLevelArrow2_UpperLeft", "SaturationLevelArrow2_LowerRight");
    C_ADDR(rb, "DeadzoneLevelArrow1_UpperLeft", "DeadzoneLevelArrow1_LowerRight");
    Bal(1);
    C_ADDR(rc, "DeadzoneLevelArrow2_UpperLeft", "DeadzoneLevelArrow2_LowerRight");
}
