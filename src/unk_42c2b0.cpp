// GameScreen1E9C (0x42c2b0-0x42e92f): the screen embedded in the Game object at +0x1e9c. Map: analysis/range-4266c0.md
// module 4; class in include/unk_42c2b0.h.
// Work groups: H4a (lifecycle, Setup, Release, Run, DrawFlag, module-end stubs; prefix H4a_), H4b (0x42c8e0),
// H4c (0x42d710), H4d (0x42ddc0).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_42c2b0.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group H4a ----
// The Game object as Setup reads it: the TrackSelectMenu (+0x3aec) count blocks (+0x148, 0x5e8 bytes each) and the
// flag sheet.
struct H4a_Rally {
    BYTE unk0[0x2c];
    int limitA;                     // 0x2c
    int limitB;                     // 0x30
    BYTE unk34[0x5e8 - 0x34];
};
struct H4a_Game {
    BYTE unk0[0x3aec + 0x148];
    H4a_Rally count[5];                // 0x3c34
    BYTE unk5ccc[0x1390f8 - (0x3aec + 0x148 + 5 * 0x5e8)];
    Sprite* flags;                     // 0x1390f8
};
// std::string::assign(const char*) expanded by hand (VC6 spends a small inline budget per function): the first four
// label assigns of Setup call _Eos (0x402960), the last three write the terminator inline.
struct H4a_Str {
    BYTE alloc[4];
    char* ptr;
    unsigned int len;
    unsigned int res;
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    void AssignOut(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    void AssignInline(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
};
#define H4a_OUT(label, id) ((H4a_Str*)&(label).text)->AssignOut(FUN_00405380(id))
#define H4a_IN(label, id) ((H4a_Str*)&(label).text)->AssignInline(FUN_00405380(id))
// Layout constants (the original reads each as a separate .rdata object: fld const; fmul scale).
static float H4a_step = 42.0f;         // 0x4e15cc
static float H4a_backY = 684.0f;       // 0x4e15d4


// ---- from group draft H4b ----
class H4b_String {
public:
    H4b_String(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~H4b_String() { Free(true); }
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
    bool Grow(unsigned int n, bool trim = false); // 0x402980
    unsigned int max_size() const;     // 0x402aa0
    void Copy(unsigned int n);         // 0x402ab0
    void Eos(unsigned int n) { ptr[len = n] = 0; }
    bool GrowInline(unsigned int n, bool trim)
    {
        if (max_size() < n) {
            std::_Xlen();
        }
        if (ptr != 0 && Refcnt(ptr) != 0 && Refcnt(ptr) != 255) {
            if (n == 0) {
                --Refcnt(ptr), Tidy(false);
                return false;
            } else {
                Copy(n);
                return true;
            }
        }
        if (n == 0) {
            if (trim) {
                Tidy(true);
            } else if (ptr != 0) {
                Eos(0);
            }
            return false;
        } else {
            if (trim && (31 < reserve || reserve < n)) {
                Tidy(true);
                Copy(n);
            } else if (!trim && reserve < n) {
                Copy(n);
            }
            return true;
        }
    }
    // assign(const char*) with _Grow out of line
    void AssignCall(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    // assign(const char*) with _Grow expanded
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (GrowInline(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    void Append(const char* s) { Append(s, strlen(s)); }
    void Append(const char* s, unsigned int m)
    {
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = len + m)) {
            std::char_traits<char>::copy(ptr + len, s, m);
            Eos(n);
        }
    }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

class H4b_Game {
public:
    void FUN_00435ca0(int a, int b, int c, int d, int e, float f); // 0x435ca0
};

extern int DAT_004f7548[];             // text ids of the value row's first three entries (index value - 1)
extern unsigned char DAT_0057ba35, DAT_0057ba43, DAT_0057ba4d, DAT_0057ba4e, DAT_0057ba4f, DAT_0057ba50;

// Layout constants (the original multiplies const * scale: fld const; fmul mem).
static float H4b_titleX = 54.0f;       // 0x4e15d8
static float H4b_titleY = 136.0f;      // 0x4e15dc
static float H4b_toggleX = 250.0f;     // 0x4e160c
static float H4b_toggleW = 320.0f;     // 0x4e162c
static float H4b_valueX = 300.0f;      // 0x4e1608
static float H4b_valueW = 270.0f;      // 0x4e1628


// ---- from group draft H4d ----
extern int DAT_004f7550[];             // per-class row count table

// A std::string through its layout (allocator, pointer, length, reserve): the original keeps _Tidy (0x402700), _Grow
// (0x402980) and _Xlen (0x4bd00b) out of line and expands assign/append/the destructor inline.
struct H4D_Str {
    H4D_Str(const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { Tidy(false); }
    ~H4D_Str()
    {
        if (ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, res + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, res = 0;
    }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    static void Xlen();                // 0x4bd00b
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            len = n;
            ptr[n] = 0;
        }
    }
    void Append(const char* s)
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
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

// The layout constants stay variables (the original does not fold sums like 212 + 1024).
static float H4D_X0 = 212.0f, H4D_Y0 = 251.0f, H4D_W = 1024.0f, H4D_H = 512.0f;
static float H4D_COLNAME = 277.0f, H4D_COLCLASS = 516.0f, H4D_COLTIME = 723.0f, H4D_COLDATE = 875.0f;
static float H4D_FITPAD = 10.0f, H4D_TIMEOFS = 73.0f, H4D_DATEOFS = 64.0f;

// FUNCTION: RALLYTROPHY 0x42c2b0
GameScreen1E9C::GameScreen1E9C()
{
    m_selected = 0;
    m_state = 0;
    unk124 = 0;
    unk130 = 0;
    count = 1;
    value0 = 1;
    value1 = 1;
    value = 1;
}

// FUNCTION: RALLYTROPHY 0x42c360
// SYMBOL: ??_GGameScreen1E9C@@UAEPAXI@Z
typedef void Generated_42c360();

// FUNCTION: RALLYTROPHY 0x42c380
GameScreen1E9C::~GameScreen1E9C()
{
    Release();
}

// FUNCTION: RALLYTROPHY 0x42c400
void GameScreen1E9C::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    mode = 0;
    m_game = game;
    m_state = 1;
    float x = DAT_0056aea8 * 54.0f;
    float y = DAT_0056aeac * 166.0f;
    H4a_OUT(items[0], 0xb1);
    items[0].x = x;
    items[0].y = y;
    x = DAT_0056aea8 * 54.0f;
    y += H4a_step * DAT_0056aeac;
    H4a_OUT(items[1], 0x29);
    items[1].x = x;
    items[1].y = y;
    y += H4a_step * DAT_0056aeac;
    H4a_OUT(items[2], 0x1b);
    items[2].x = x;
    items[2].y = y;
    y += H4a_step * DAT_0056aeac;
    H4a_OUT(items[3], 0x61);
    items[3].x = x;
    items[3].y = y;
    y = H4a_backY * DAT_0056aeac;
    H4a_IN(items[4], 0x18);
    items[4].x = x;
    items[4].y = y;
    H4a_IN(toggle[0], 0x35);
    H4a_IN(toggle[1], 0x2a);
    box = new Sprite();
    box->texture = new Texture("data/menu/highscorebox.tga", 0x841, 0);
    box->texture->Upload(DAT_0056b0ac);
    box->unk9c = 2;
    box->unk98 = 0xf;
    box->unk60[0] = 1.0f;
    box->unk60[1] = 1.0f;
    box->unk60[2] = 1.0f;
    box->unk60[3] = 1.0f;
    box->unk18 = 5;
    box->unk40[0] = 1.0f;
    box->unk40[1] = 1.0f;
    box->unk40[2] = 1.0f;
    box->unk40[3] = 1.0f;
    box->unk1c = 6;
    box->unk24 = 2;
    flags = ((H4a_Game*)m_game)->flags;
    for (int i = 0; i < 5; i++) {
        limitA[i] = ((H4a_Game*)m_game)->count[i].limitA;
        limitB[i] = ((H4a_Game*)m_game)->count[i].limitB;
    }
    BuildRects();
}

// FUNCTION: RALLYTROPHY 0x42c860
void GameScreen1E9C::Release()
{
    if (m_state) {
        m_state = 0;
        m_game = 0;
        if (box) {
            delete box->texture;
            delete box;
        }
        box = 0;
        FUN_00438d00();
    }
}

// FUNCTION: RALLYTROPHY 0x42c8c0
void GameScreen1E9C::Run(MenuGame* game)
{
    Setup(game);
    Frame();
}

// NONMATCHING: RALLYTROPHY 0x42c8e0
// One instruction off: in the title Print the original loads the device pointer into eax right after pushing the text
// (before the y product), mine into ecx after the x product (1 byte longer). Tried an inline Print wrapper and reordering.
// A title-only device pointer local (including register storage) changes text/device register allocation; a reference is neutral.
// Explicit x/device/y locals shift the surrounding frame; a volatile device read in the first argument is emitted after y.
// original: Frame (thiscall; GameScreen1E9C::Frame)
int GameScreen1E9C::Frame()
{
    HudPoint mouse;
    int done = 0;
    H4b_String str;
    char buf[32];
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float s = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    DAT_0056b0bc->ClearKeys();
    TextRenderer* text = m_game->text;
    int first = 1;
    int wait = 0;
    while (FUN_004b69f0(wait) && !done) {
        m_game->Update();
        mouse = *(HudPoint*)&DAT_0057ba00;
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            int hit = HitTest(mouse, 0, 3);
            if (hit != -1) {
                m_selected = hit;
            }
        }
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        int dx = 0;
        int dy = 0;
        if (DAT_0057ba43) {
            done = 1;
            continue;
        }
        if (DAT_0057ba35 && m_selected == 4) {
            done = 1;
            continue;
        }
        if (DAT_0057ba50) {
            dy = 1;
        } else if (DAT_0057ba4e) {
            dy = -1;
        }
        if (DAT_0057ba4f) {
            dx = 1;
        } else if (DAT_0057ba4d) {
            dx = -1;
        }
        m_selected += dy;
        int sel = m_selected;
        if (dy) {
            m_game->Play(0);
        }
        if (sel < 0) {
            m_selected = 4;
        } else if (sel >= 5) {
            m_selected = 0;
        }
        if (dy) {
            float cx = m_items[m_selected]->GetTopLeft().x +
                       (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f;
            float cy = m_items[m_selected]->GetTopLeft().y +
                       (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f;
            SetCursorPos((int)(cx * DAT_0056aea8), (int)(cy * DAT_0056aeac));
        }
        int hit = HitTest(mouse, 0, 4);
        if (hit != -1 && !dy) {
            if (m_selected != hit) {
                m_game->Play(0);
            }
            m_selected = hit;
        }
        if (DAT_0057ba20) {
            HudPoint click = *(HudPoint*)&DAT_0057ba00;
            m_items[6]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition());
            m_items[7]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition());
            if (m_items[4]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                done = 1;
                continue;
            }
            if (m_items[5]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                dx++;
            }
            if (m_items[6]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) ||
                m_items[7]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                dx--;
            }
        }
        if (dx) {
            switch (m_selected) {
            case 0:
                if (mode) {
                    value1 = value;
                } else {
                    value0 = value;
                }
                AdjustValue(&mode, dx, 0, 1);
                if (mode) {
                    value = value1;
                    if (limitA[count - 1] + 3 < value) {
                        value = limitA[count - 1] + 3;
                    }
                } else {
                    value = value0;
                }
                break;
            case 1:
                AdjustValue(&count, dx, 1, 5);
                if (!mode) {
                    if (limitA[count - 1] + 3 < value) {
                        value = limitA[count - 1] + 3;
                    }
                } else {
                    if (limitB[count - 1] < value) {
                        value = limitB[count - 1];
                    }
                }
                break;
            case 2:
                if (!mode) {
                    AdjustValue(&value, dx, 1, limitA[count - 1] + 3);
                } else {
                    AdjustValue(&value, dx, 1, limitB[count - 1]);
                }
                break;
            case 3:
                AdjustValue(&unk130, dx, 0, 22);
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
            text->Print(DAT_0056b0ac, H4b_titleX * DAT_0056aea8, H4b_titleY * DAT_0056aeac, FUN_00405380(0x2d), 0,
                        1.0f, 1.0f);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            DrawRanking();
            for (int i = 0; i < 5; i++) {
                int selected = (i == m_selected);
                Color col;
                col.value = selected ? DAT_00508c0c : DAT_00508c04;
                float y = items[i].y;
                float x = items[i].x;
                text->color = col.value;
                text->Print(DAT_0056b0ac, x, y, items[i].text.c_str(), 0, 1.0f, 1.0f);
                switch (i) {
                case 0:
                    x = H4b_toggleX * s;
                    DrawToggle(text, toggle, x, y, H4b_toggleW * DAT_0056aea8, selected, mode, 0, 1);
                    break;
                case 1:
                    x = H4b_valueX * s;
                    _itoa(count, buf, 10);
                    str.AssignCall(buf);
                    str.Append("/");
                    _itoa(5, buf, 10);
                    str.Append(buf);
                    DrawSelector(text, (std::string*)&str, x, y, H4b_valueW * DAT_0056aea8, selected, count, 1, 5);
                    break;
                case 2:
                    x = H4b_valueX * s;
                    if (!mode) {
                        if (value <= 3) {
                            str.Assign(FUN_00405380(DAT_004f7548[value - 1]));
                        } else {
                            _itoa(value - 3, buf, 10);
                            str.Assign(buf);
                        }
                    } else {
                        _itoa(value, buf, 10);
                        str.Assign(buf);
                    }
                    DrawSelector(text, (std::string*)&str, x, y, H4b_valueW * DAT_0056aea8, selected, value, 1,
                                 !mode ? limitA[count - 1] + 3 : limitB[count - 1]);
                    break;
                case 3:
                    x = H4b_valueX * s;
                    if (!unk130) {
                        str.Assign(FUN_00405380(0x26));
                    } else {
                        _itoa(unk130, buf, 10);
                        str.Assign(buf);
                    }
                    DrawSelector(text, (std::string*)&str, x, y, H4b_valueW * DAT_0056aea8, selected, unk130, 0, 22);
                    break;
                }
            }
            ((H4b_Game*)DAT_0056b0c8)->FUN_00435ca0(-1, 4, 0, 1, 1, 678.0f);
            DrawCursor(mouse);
            DAT_0056b0ac->EndScene();
            DAT_0056b0ac->Present();
        }
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    int r = m_selected;
    if (mode) {
        value1 = value;
    } else {
        value0 = value;
    }
    if (done || DAT_0057b9f4) {
        r = -1;
        m_game->Play(2);
    }
    return r;
}

// ---- Group H4c ----
struct H4c_Point {
    H4c_Point() {}
    H4c_Point(const H4c_Point& other);     // 0x424300
    ~H4c_Point() {}
    float x, y;
};
class H4c_Rect {
public:
    H4c_Rect(H4c_Point topLeft, HudPoint bottomRight);   // 0x4390a0
    float f[5];
};
static float H4c_x = 54.0f;        // 0x4e15e0
static float H4c_w = 720.0f;       // 0x4e1650
static float H4c_mid = 300.0f;     // 0x4e1608
static float H4c_gap = 8.0f;       // 0x4e1644
static float H4c_btn = 42.0f;      // 0x4e1640
static float H4c_back = 200.0f;    // 0x4e1648
static float H4c_margin = 10.0f;   // 0x4e1330
static float H4c_three = 3.0f;     // 0x4e13dc
static float H4c_four = 4.0f;      // 0x4e164c
static float H4c_base = 166.0f;    // 0x4e1610
static float H4c_trim = 20.0f;     // 0x4e1654

// original: FUN_0042d710 (creates the hit rectangles)
// NONMATCHING: RALLYTROPHY 0x42d710
// last push_back is inlined in the original (inline size math); ours emits an extra out-of-line size() call (+7 bytes)
void GameScreen1E9C::BuildRects()
{
    H4c_Point a;
    HudPoint b;
    UIRect* rect;
    float top = H4c_base - H4c_trim;
    b.x = H4c_x + H4c_w;
    a.x = H4c_x;
    a.y = top;
    b.y = H4a_step + top;
    rect = (UIRect*)new H4c_Rect(a, b);
    m_items.push_back(rect);
    b.x = H4c_x + H4c_w;
    a.x = H4c_x;
    a.y = H4a_step + top;
    b.y = H4a_step + H4a_step + top;
    rect = (UIRect*)new H4c_Rect(a, b);
    m_items.push_back(rect);
    b.x = H4c_x + H4c_w;
    a.x = H4c_x;
    a.y = H4a_step + H4a_step + top;
    b.y = H4a_step * H4c_three + top;
    rect = (UIRect*)new H4c_Rect(a, b);
    m_items.push_back(rect);
    b.x = H4c_x + H4c_w;
    a.x = H4c_x;
    a.y = H4a_step * H4c_three + top;
    b.y = H4a_step * H4c_four + top;
    rect = (UIRect*)new H4c_Rect(a, b);
    m_items.push_back(rect);
    b.x = H4c_x + H4c_back;
    a.x = H4c_x;
    a.y = H4a_backY - H4c_margin;
    b.y = H4a_backY + H4a_step;
    rect = (UIRect*)new H4c_Rect(a, b);
    m_items.push_back(rect);
    a.x = 554.0f;
    b.x = 596.0f;
    a.y = 150.0f;
    b.y = 329.0f;
    rect = (UIRect*)new H4c_Rect(a, b);
    m_items.push_back(rect);
    b.x = (a.x = H4c_mid - H4c_gap) + H4c_btn;
    a.y = H4a_step + top;
    b.y = H4a_step * H4c_four + top;
    rect = (UIRect*)new H4c_Rect(a, b);
    m_items.push_back(rect);
    b.y = H4a_step + top;
    a.x = 242.0f;
    b.x = 284.0f;
    a.y = top;
    rect = (UIRect*)new H4c_Rect(a, b);
    m_items.push_back(rect);
}

// original: FUN_0042ddc0
// FUNCTION: RALLYTROPHY 0x42ddc0
void GameScreen1E9C::DrawRanking()
{
    H4D_Str line;
    Vec2 uv[4];
    Vec2 pos[4];
    Color color;
    char timeBuf[0x20];
    char buf[0x20];
    TextRenderer* text = *(TextRenderer**)((BYTE*)m_game + 0x1390bc);
    pos[0].x = pos[3].x = DAT_0056aea8 * H4D_X0;
    uv[0].x = uv[3].x = 0.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[0].y = pos[1].y = DAT_0056aeac * H4D_Y0;
    pos[1].x = pos[2].x = (H4D_X0 + H4D_W) * DAT_0056aea8;
    pos[2].y = pos[3].y = (H4D_Y0 + H4D_H) * DAT_0056aeac;
    color.value = 0xffffffff;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(box);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, color, 1.0f, 1.0f);
    text->color = color.value;
    float y = 333.0f * DAT_0056aeac;
    float x = 247.0f * DAT_0056aea8;
    text->Printf(DAT_0056b0ac, x, y, FUN_00405380(0x95));
    x = H4D_COLCLASS * DAT_0056aea8;
    text->Printf(DAT_0056b0ac, x, y, FUN_00405380(0x61));
    x = H4D_COLTIME * DAT_0056aea8;
    if (mode == 0) {
        text->Printf(DAT_0056b0ac, x, y, FUN_00405380(0xb2));
    } else {
        text->Printf(DAT_0056b0ac, x, y, FUN_00405380(0xb3));
    }
    x = H4D_COLDATE * DAT_0056aea8;
    text->Printf(DAT_0056b0ac, x, y, FUN_00405380(0xb4));
    x = 613.0f * DAT_0056aea8;
    y = 261.0f * DAT_0056aeac;
    line.Assign(FUN_00405380(0x19));
    line.Append(": ");
    line.Append(FUN_00405380(0xb4 + count));
    text->Printf(DAT_0056b0ac, x, y, line.c_str());
    x = 613.0f * DAT_0056aea8;
    y = 293.0f * DAT_0056aeac;
    line.Assign(FUN_00405380(0xf9));
    line.Append(": ");
    if (mode == 0) {
        if (value > 3) {
            line.Append(FUN_00405380(0xb1 + count * 10 + value));
        }
    } else {
        line.Append(FUN_00405380(limitA[count - 1] + (count * 5 + 0x5a) * 2 + value));
    }
    text->Printf(DAT_0056b0ac, x, y, line.c_str());
    int n = unk130;
    if (n) {
        n = DAT_004f7550[n] + 1;
    }
    color.value = DAT_00508c04;
    text->color = color.value;
    y = 366.0f * DAT_0056aeac;
    for (int i = 0; i < 10; i++) {
        RecordEntry* rec;
        if (mode == 0) {
            if (value <= 3) {
                rec = ((RecordTable*)((BYTE*)m_game + 0x5c30))->FUN_00490010(count - 1, value - 1, i, n);
            } else {
                rec = ((RecordTable*)((BYTE*)m_game + 0x5c30))->FUN_0048ffd0(count - 1, value - 4, i, n);
            }
        } else {
            rec = ((RecordTable*)((BYTE*)m_game + 0x5c30))->FUN_0048ffd0(count - 1, limitA[count - 1] + value - 1, i, n);
        }
        x = 227.0f * DAT_0056aea8;
        itoa(i + 1, buf, 10);
        text->Printf(DAT_0056b0ac, x - text->GetWidth(buf, 0) * 0.5f, y, buf);
        DrawFlag(243.0f * DAT_0056aea8, y, (char)rec->unk1e);
        text->SetScaleX(1.0f);
        float w = text->GetWidth(rec->name, 0);
        float k = (H4D_COLCLASS - H4D_COLNAME - H4D_FITPAD) / w;
        if (k > 1.0f) {
            k = 1.0f;
        }
        text->SetScaleX(k * DAT_0056aea8);
        text->Printf(DAT_0056b0ac, H4D_COLNAME * DAT_0056aea8, y, rec->name);
        text->SetScaleX(DAT_0056aea8);
        x = H4D_COLCLASS * DAT_0056aea8;
        text->Printf(DAT_0056b0ac, x, y, FUN_00405380(rec->unk1f + 0x138));
        x = (H4D_COLTIME + H4D_TIMEOFS) * DAT_0056aea8;
        FormatTime((mode != 1 && value > 3) ? rec->splits[10] : rec->time, timeBuf);
        text->Printf(DAT_0056b0ac, x - text->GetWidth(timeBuf, 0) * DAT_0056aea8 * 0.5f, y, timeBuf);
        x = (H4D_COLDATE + H4D_DATEOFS) * DAT_0056aea8;
        GetDateFormatA(0x400, 0, (SYSTEMTIME*)rec->unk20, NULL, timeBuf, 0x20);
        text->Printf(DAT_0056b0ac, x - text->GetWidth(timeBuf, 0) * DAT_0056aea8 * 0.5f, y, timeBuf);
        y += 32.0f * DAT_0056aeac;
    }
}

// FUNCTION: RALLYTROPHY 0x42e760
void GameScreen1E9C::DrawFlag(float x, float y, int flag)
{
    Vec2 uv[4];
    Vec2 pos[4];
    Color color;
    color.value = 0xffffffff;
    int col = flag & 3;
    int row = flag >> 2;
    uv[0].x = uv[3].x = col * 0.25f;
    uv[0].y = uv[1].y = row * 0.0625f;
    uv[2].x = uv[1].x = (col + 1.0f) * 0.25f;
    uv[2].y = uv[3].y = (row + 1.0f) * 0.0625f;
    pos[0].x = pos[3].x = x;
    pos[0].y = pos[1].y = y;
    pos[2].x = pos[1].x = DAT_0056aea8 * 32.0f + x;
    pos[2].y = pos[3].y = DAT_0056aeac * 16.0f + y;
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(flags);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(pos, uv, color, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x42e900
// SYMBOL: _$E23
typedef void Generated_42e900();

// FUNCTION: RALLYTROPHY 0x42e920
// SYMBOL: _$E22
typedef void Generated_42e920();
