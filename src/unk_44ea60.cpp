// SlotMenu (0x44ea60-0x450edf): the eight-slot list screen. Map: analysis/range-44ea60.md; classes in
// include/unk_44ea60.h.
// Work groups: A (lifecycle, Setup, Run, BuildRects: prefix A_), B (RefreshSlots, DrawCaret, Draw), C (Frame: prefix C_),
// D (DrawField: prefix SMD_).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_44ea60.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group A ----
// A 2D point whose copy constructor is out of line (0x424300).
struct A_Point {
    A_Point() {}
    A_Point(const A_Point& other);     // 0x424300
    ~A_Point() {}
    float x, y;
};
// The HUD rectangle as BuildRects calls it: constructor 0x4390a0 out of line.
class A_Rect {
public:
    A_Rect(A_Point topLeft, HudPoint bottomRight);   // 0x4390a0
    float f[5];
};

// Layout constants of the 1024x768 design space (kept as named constants: the original does not fold them).
static const float A_LEFT = 54.0f;       // title and back text x
static const float A_TOP = 262.0f;       // first row
static const float A_PITCH = 42.0f;      // row pitch
static const float A_BACKY = 610.0f;     // back text y
static const float A_COLX = 485.0f;      // slot column x
static const float A_ROWY = 260.0f;      // slot rows y
static const float A_MARGIN = 20.0f;     // rect margin above the title row
static const float A_TITLEW = 420.0f;
static const float A_BACKW = 200.0f;
static const float A_SLOTW = 430.0f;
static const float A_BACKMARGIN = 10.0f;

// FUNCTION: RALLYTROPHY 0x44ea60
SlotMenu::SlotMenu()
{
    m_selected = 0;
    m_state = 0;
}

// FUNCTION: RALLYTROPHY 0x44ead0
// SYMBOL: ??_GSlotMenu@@UAEPAXI@Z
typedef void Generated_44ead0();

// FUNCTION: RALLYTROPHY 0x44eaf0
SlotMenu::~SlotMenu()
{
    Release();
}

// FUNCTION: RALLYTROPHY 0x44eb60
// Evidence: ev_94392a4f4c7f0f32c6614794aa714c68ec62939dfe64d102fd817b4a11bb08d1.
// Store the game pointer before float setup and reuse y for target stack slots.
void SlotMenu::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    m_game = game;
    m_state = 1;
    float x = A_LEFT * DAT_0056aea8;
    float y = A_TOP * DAT_0056aeac;
    labels[0].text = FUN_00405380(0x170);
    labels[0].x = x;
    labels[0].y = y;
    y = A_BACKY * DAT_0056aeac;
    labels[1].text = FUN_00405380(0x18);
    labels[1].x = x;
    labels[1].y = y;
    float sx = A_COLX * DAT_0056aea8;
    float sy = DAT_0056aeac * 260.0f; // literal preserves target global/constant operand order
    float step = A_PITCH * DAT_0056aeac;
    for (int i = 2; i < 10; i++) {
        labels[i].x = sx;
        labels[i].y = sy;
        sy += step;
    }
    BuildRects();
}

// FUNCTION: RALLYTROPHY 0x44ed70
int SlotMenu::Run(MenuGame* game)
{
    Setup(game);
    return Frame();
}

// ---- Group C: Frame ----
// The renderer's two extra fields the screen loop uses.
class C_Device : public RenderDevice {
public:
    int busy;                          // 0x2c
    BYTE unk30[0x5c - 0x30];
    int pending;                       // 0x5c: typed characters waiting
};
// The game object's confirmation dialog (0x436250), and the profile object at +0x2dcc.
class C_Game : public MenuGame {
public:
    int FUN_00436250(MenuScreen* screen);
};
class C_Profile : public Player {
public:
    void FUN_0044d260(int slot);
};
// std::string operations the original keeps out of line at some call sites and expands inline at others (VC6 spends
// a small inline budget per function). The view has the layout of std::string (allocator, _Ptr, _Len, _Res);
// Tidy 0x402700, Grow 0x402980, Eos 0x402960, Split 0x402bd0, assign(const char*, n) 0x404ba0, assign(x, pos, n)
// 0x4024f0, erase 0x408de0.
// The mouse-warp temporary: inline constructor and empty destructor (see FA2_Point in unk_44b930.cpp).
struct C_WarpPoint {
    C_WarpPoint(float px, float py)
    {
        x = px;
        y = py;
    }
    ~C_WarpPoint() {}
    float x, y;
};

struct C_Str {
    BYTE alloc[4];
    char* ptr;
    unsigned int len;
    unsigned int res;
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    void Split();                      // 0x402bd0
    void AssignN(const char* s, unsigned int n); // 0x404ba0
    void Assign(const std::string& x, unsigned int pos, unsigned int n); // 0x4024f0
    void Erase(unsigned int pos, unsigned int n); // 0x408de0
    const char* CStr() const { return ptr ? ptr : ""; }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    // assign(x, 0, npos) expanded; the nested erase calls and _Eos stay calls
    void AssignInline(const C_Str* x)
    {
        unsigned int m = std::string::npos;
        unsigned int n = x->len;
        if (n < m) {
            m = n;
        }
        if (this == x) {
            Erase(m, std::string::npos);
            Erase(0, 0);
        } else if (0 < m && m == x->len && Refcnt(x->CStr()) < 254) {
            Tidy(true);
            ptr = (char*)x->CStr();
            len = x->len;
            res = x->res;
            ++Refcnt(ptr);
        } else if (Grow(n = m, true)) {
            std::char_traits<char>::copy(ptr, x->CStr(), n);
            Eos(n);
        }
    }
    // erase(0, npos) expanded: _Split as a call, _Eos expanded
    void EraseAll()
    {
        unsigned int m = std::string::npos;
        Split();
        if (len < m) {
            m = len;
        }
        if (0 < m) {
            std::char_traits<char>::move(ptr, ptr + m, len - m);
            unsigned int n = len - m;
            if (Grow(n, false)) {
                ptr[len = n] = 0;
            }
        }
    }
    // erase(pos) expanded: _Split as a call, _Eos expanded
    void EraseFromA(unsigned int pos)
    {
        unsigned int m = std::string::npos;
        if (len < pos) {
            std::_Xran();
        }
        Split();
        if (len - pos < m) {
            m = len - pos;
        }
        if (0 < m) {
            std::char_traits<char>::move(ptr + pos, ptr + pos + m, len - pos - m);
            unsigned int n = len - m;
            if (Grow(n, false)) {
                ptr[len = n] = 0;
            }
        }
    }
    // erase(pos) expanded: _Split expanded, _Eos as a call
    void EraseFromB(unsigned int pos)
    {
        unsigned int m = std::string::npos;
        if (len < pos) {
            std::_Xran();
        }
        if (ptr != 0 && Refcnt(ptr) != 0 && Refcnt(ptr) != 255) {
            const char* temp = ptr;
            Tidy(true);
            AssignN(temp, strlen(temp));
        }
        if (len - pos < m) {
            m = len - pos;
        }
        if (0 < m) {
            std::char_traits<char>::move(ptr + pos, ptr + pos + m, len - pos - m);
            unsigned int n = len - m;
            if (Grow(n, false)) {
                Eos(n);
            }
        }
    }
    // append(count, ch): _Grow and _Eos as calls
    void AppendFill(unsigned int count, char ch)
    {
        if (std::string::npos - len <= count) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < count && Grow(n = count + len, false)) {
            std::char_traits<char>::assign(ptr + len, count, ch);
            Eos(n);
        }
    }
    // resize(n) with the variants above
    void ResizeA(unsigned int n)
    {
        n <= len ? EraseFromA(n) : AppendFill(n - len, '\0');
    }
    void ResizeB(unsigned int n)
    {
        n <= len ? EraseFromB(n) : AppendFill(n - len, '\0');
    }
    // operator+=(char): _Grow as a call, _Eos expanded
    void AppendChar(char ch)
    {
        if (std::string::npos - len <= 1) {
            std::_Xlen();
        }
        unsigned int n;
        if (Grow(n = 1 + len, false)) {
            std::char_traits<char>::assign(ptr + len, 1, ch);
            ptr[len = n] = 0;
        }
    }
};
#define C_STR(s) ((C_Str*)&(s))
#define C_ASSIGN(dst, src) C_STR(dst)->Assign(src, 0, std::string::npos)
#define C_ERASE(dst) C_STR(dst)->Erase(0, std::string::npos)
#define C_PROFILE (*(C_Profile**)((BYTE*)m_game + 0x2dcc))

// NONMATCHING: RALLYTROPHY 0x44ed90
// 3391 vs 3386 bytes; frame layout, EH states, call sequence and the hand-expanded std::string sites match. Left: the
// Esc/Enter cancel paths are not tail-merged (the mouse warps now use the C_WarpPoint argument temporary, which
// fixed their store order).
// original: FUN_0044ed90
int SlotMenu::Frame()
{
    HudPoint mouse;
    int done = 0;
    std::string saved;
    std::string unused;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    clock = timeGetTime();
    editing = 0;
    DAT_0056b0bc->ClearKeys();
    int first = 1;
    m_selected = 0;
    RefreshSlots();
    ((C_Device*)DAT_0056b0ac)->busy = 1;
    DAT_0056b0ac->vf09();
    int wait = 0;
    while (FUN_004b69f0(wait) && !done) {
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
            if (editing == 0) {
                m_selected = 0;
                dir = 1;
            } else {
                editing = 0;
                C_ASSIGN(labels[m_selected].text, saved);
            }
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
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&C_WarpPoint(cx * sx, cy * sy));
            } else if (m_selected >= 2 && m_selected <= 9) {
                if (editing) {
                    if (labels[m_selected].text.length() > 0) {
                        if (records[m_selected - 2].kind <= 0 || ((C_Game*)m_game)->FUN_00436250(this) != 0) {
                            strcpy(records[m_selected - 2].name, labels[m_selected].text.c_str());
                            C_PROFILE->FUN_00490fb0(records[m_selected - 2].name);
                            C_PROFILE->FUN_0044d260(m_selected - 2);
                            RefreshSlots();
                            editing = 0;
                        } else {
                            editing = 0;
                            C_ASSIGN(labels[m_selected].text, saved);
                        }
                    }
                } else {
                    editing = 1;
                    C_ASSIGN(saved, labels[m_selected].text);
                    if (records[m_selected - 2].kind == 0) {
                        C_ERASE(labels[m_selected].text);
                    }
                }
            }
        }
        if (DAT_0057ba28[VK_DOWN]) {
            dir++;
        } else if (DAT_0057ba28[VK_UP]) {
            dir--;
        }
        hit = HitTest(mouse, 0, 9);
        if (hit != -1 && dir == 0 && editing == 0) {
            if (m_selected != hit) {
                m_game->Play(0);
            }
            m_selected = hit;
        }
        if (editing == 0) {
            m_selected += dir;
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
        if (dir != 0 && editing == 0) {
            m_game->Play(0);
            float cx = m_items[m_selected]->GetTopLeft().x +
                       (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f;
            float cy = m_items[m_selected]->GetTopLeft().y +
                       (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f;
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&C_WarpPoint(cx * sx, cy * sy));
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
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&C_WarpPoint(cx * sx, cy * sy));
            }
            hit = HitTest(mouse, 2, 9);
            if (hit != -1) {
                if (editing) {
                    if (labels[m_selected].text.length() > 0) {
                        if (records[m_selected - 2].kind <= 0 || ((C_Game*)m_game)->FUN_00436250(this) != 0) {
                            strcpy(records[m_selected - 2].name, labels[m_selected].text.c_str());
                            C_PROFILE->FUN_0044d260(hit - 2);
                            RefreshSlots();
                            editing = 0;
                        } else {
                            editing = 0;
                            C_ASSIGN(labels[m_selected].text, saved);
                        }
                    }
                } else {
                    editing = 1;
                    C_Str* lab = C_STR(labels[m_selected].text);
                    C_STR(saved)->AssignInline(lab);
                    if (records[m_selected - 2].kind == 0) {
                        lab->EraseAll();
                    }
                }
            }
        }
        if (editing && ((C_Device*)DAT_0056b0ac)->pending > 0) {
            char c = DAT_0056b0ac->ReadChar();
            if (c == 8) {
                C_Str* lab = C_STR(labels[m_selected].text);
                int n = (int)lab->len - 1;
                if (n >= 0) {
                    lab->ResizeA(n);
                }
            }
            for (unsigned i = 0; i < strlen("ABCDEFGHIJKLMNOPQRSTUVWXYZ\305\304\326abcdefghijklmnopqrstuvwxyz\345\344\366\334\374"
                                            "0123456789!%&/()=-+* ");
                 i++) {
                if ("ABCDEFGHIJKLMNOPQRSTUVWXYZ\305\304\326abcdefghijklmnopqrstuvwxyz\345\344\366\334\374"
                    "0123456789!%&/()=-+* "[i] == c) {
                    if (labels[m_selected].text.length() == 0x16) {
                        C_STR(labels[m_selected].text)->ResizeB(0x15);
                    }
                    C_STR(labels[m_selected].text)->AppendChar(c);
                }
            }
        }
        Slot3(0);
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    ((C_Device*)DAT_0056b0ac)->busy = 0;
    int r = m_selected;
    if (done || DAT_0057b9f4) {
        r = -1;
        m_game->Play(2);
    }
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
    DAT_0056b0bc->ClearKeys();
    return r;
}

// original: FUN_0044fad0 (thiscall; creates the ten hit rectangles)
// FUNCTION: RALLYTROPHY 0x44fad0
void SlotMenu::BuildRects()
{
    float top;
    UIRect* rect;
    A_Point a;
    HudPoint b;
    top = A_TOP - A_MARGIN;
    b.x = A_LEFT + A_TITLEW;
    a.x = A_LEFT;
    a.y = top;
    b.y = A_PITCH + top;
    rect = (UIRect*)new A_Rect(a, b);
    m_items.push_back(rect);
    b.x = A_LEFT + A_BACKW;
    a.x = A_LEFT;
    a.y = A_BACKY - A_BACKMARGIN;
    b.y = A_BACKY + A_PITCH;
    rect = (UIRect*)new A_Rect(a, b);
    m_items.push_back(rect);
    for (int i = 0; i < 8; i++) {
        b.x = A_COLX + A_SLOTW;
        a.x = A_COLX;
        a.y = i * A_PITCH + top;
        b.y = (i + 1) * A_PITCH + top;
        rect = (UIRect*)new A_Rect(a, b);
        m_items.push_back(rect);
    }
}

// A VC6 std::string seen through its layout (allocator at +0, pointer, length, reserve). DrawField's string
// operations are expanded to different depths (the inline budget runs out part way through the function), so
// each site is pinned: out-of-line assign/append (0x404ba0/0x403ba0), expansions that call _Eos (0x402960) or set
// the terminator inline.
struct SMD_Str {
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
#define SMD_L (*(SMD_Str*)&left)
#define SMD_R (*(SMD_Str*)&right)

// original: FUN_0044fd30
// FUNCTION: RALLYTROPHY 0x44fd30
void SlotMenu::DrawField(int k)
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
        SMD_L.AssignA(FUN_00405380(0x9a));
        SMD_L.AppendA(": ");
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        x = 908.0f * DAT_0056aea8 - text->GetWidth(records[k].text, 0);
        text->Print(DAT_0056b0ac, x, y, records[k].text, 0, 1.0f, 1.0f);
    } else if (records[k].kind == 0) {
        return;
    } else {
        SMD_L.AssignA(FUN_00405380(0x96));
        SMD_L.AppendA(": ");
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        itoa(records[k].kind, buf, 10);
        SMD_L.AssignA(buf);
        x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    }
    y = DAT_0056aeac * 18.0f + y;
    x = DAT_0056aea8 * 485.0f;
    SMD_L.AssignA(FUN_00405380(0x9f));
    SMD_L.AppendA(": ");
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    SMD_L.AssignB(FUN_00405380(0xb4 + records[k].index));
    SMD_L.AppendA(" ");
    SMD_L.AppendA(FUN_00405380(0x1b));
    SMD_L.AppendA(" ");
    int v = records[k].value;
    if (DAT_00501458) {
        v -= *(int*)((BYTE*)m_game + records[k].index * 1512 + 0x3680);
    }
    itoa(v, buf, 10);
    SMD_L.AppendA(buf);
    x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    if (records[k].kind == 1) {
        y = DAT_0056aeac * 18.0f + y;
        x = DAT_0056aea8 * 485.0f;
        SMD_L.AssignB(FUN_00405380(0xa0));
        SMD_L.AppendA(": ");
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        if (records[k].count1 > 0) {
            itoa(records[k].count1, buf, 10);
            SMD_L.AssignB(buf);
        } else {
            SMD_L.AssignB("-");
        }
        x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        y = DAT_0056aeac * 18.0f + y;
        x = DAT_0056aea8 * 485.0f;
        SMD_L.AssignC(FUN_00405380(0xa1));
        SMD_L.AppendD(": ");
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
        if (records[k].count2 > 0) {
            itoa(records[k].count2, buf, 10);
            SMD_L.AssignC(buf);
        } else {
            SMD_L.AssignC("-");
        }
        x = 908.0f * DAT_0056aea8 - text->GetWidth(left.c_str(), 0);
        text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    }
    y = DAT_0056aeac * 18.0f + y;
    x = DAT_0056aea8 * 485.0f;
    SMD_L.AssignC(FUN_00405380(0x171));
    SMD_L.AppendC(": ");
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y, left.c_str(), 0, 1.0f, 1.0f);
    GetDateFormatA(0x400, 0, &records[k].when, NULL, dateBuf, 0x20);
    SMD_R.AssignC(dateBuf);
    GetTimeFormatA(0x400, 0, &records[k].when, NULL, dateBuf, 0x20);
    SMD_R.AppendC(" / ");
    SMD_R.AppendC(dateBuf);
    x = 908.0f * DAT_0056aea8 - text->GetWidth(right.c_str(), 0);
    text->Print(DAT_0056b0ac, x, y, right.c_str(), 0, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x450920
void SlotMenu::RefreshSlots()
{
    for (int i = 0; i < 8; i++) {
        (*(SlotStore**)((BYTE*)m_game + 0x2dcc))->GetSlotInfo(i, &records[i]);
        if (records[i].kind) {
            labels[i + 2].text = records[i].name;
        } else {
            labels[i + 2].text = FUN_00405380(0x27);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x450a70
// original: FUN_00450a70 (thiscall that never reads this; the by-value Color parameter gives EH state 0)
void SlotMenu::DrawCaret(RenderDevice* device, float x, float y, Color color)
{
    void* material = DAT_0056b0c8->FUN_00432a00();
    HudPoint uv[4];
    HudPoint pos[4];
    uv[0].x = uv[3].x = 0.25f;
    uv[0].y = uv[1].y = 0.296875f;
    uv[1].x = uv[2].x = 0.2890625f;
    uv[2].y = uv[3].y = 0.375f;
    pos[0].x = x;
    pos[0].y = y;
    pos[2].x = pos[1].x = DAT_0056aea8 * 12.0f + x;
    pos[1].y = y;
    pos[3].y = pos[2].y = DAT_0056aeac * 24.0f + y;
    pos[3].x = x;
    device->vf35((Sprite*)material);
    ((RaceDeviceSlots*)device)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
}

// NONMATCHING: RALLYTROPHY 0x450be0
// Capturing the title device before Print reduces the bounded object diff 358→272 bytes and moves its global load
// ahead of the float arguments; it currently uses ebp instead of the target's eax. The loop still allocates the
// label cursor to ebp and index to a stack slot, opposite the target.
// original: FUN_00450be0 (vtable slot 3, Draw)
void SlotMenu::Slot3(int skipPresent)
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
        RenderDevice* titleDevice = DAT_0056b0ac;
        text->Print(titleDevice, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x16f), 0, 1.0f, 1.0f);
        text->SetScale(DAT_0056aea8, DAT_0056aeac);
        game->FUN_00435de0(480.0f, 240.0f);
        game->FUN_00435f50(480.0f, 590.0f);
        for (int i = 0; i < 10; i++) {
            Color col;
            float lx = labels[i].x;
            float ly = labels[i].y;
            int sel = m_selected;
            int selected = (i == sel);
            col.value = selected ? DAT_00508c0c : DAT_00508c04;
            text->color = col.value;
            text->Print(DAT_0056b0ac, lx, ly, labels[i].text.c_str(), 0, 1.0f, 1.0f);
            if (i == sel && editing) {
                int elapsed = timeGetTime() - clock;
                if (elapsed >= 80 && !caretOn) {
                    clock = timeGetTime();
                    caretOn = 1;
                } else if (elapsed >= 120) {
                    if (!caretOn) {
                        continue;
                    }
                    clock = timeGetTime();
                    caretOn = 0;
                }
                if (caretOn) {
                    lx = text->GetWidth(labels[i].text.c_str(), 0) + lx;
                    col.value = 0xffffff;
                    DrawCaret(DAT_0056b0ac, lx, ly, col);
                }
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

// FUNCTION: RALLYTROPHY 0x450eb0
// SYMBOL: _$E23
typedef void Generated_450eb0();

// FUNCTION: RALLYTROPHY 0x450ed0
// SYMBOL: _$E22
typedef void Generated_450ed0();
