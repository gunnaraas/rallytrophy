// MenuScreen2A8 (0x4295c0-0x42c2af): the graphics/detail options screen at MenuManager+0x2a8. Map:
// analysis/range-4266c0.md module 3; class in include/unk_4295c0.h.
// Work groups: H3a (UIRect setters, lifecycle, Setup, detail apply, Run, module-end stubs; prefix H3a_).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_4295c0.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group H3a ----
// UIRect's corner setters (called by GameScreen11A4's layout code 0x4286d0). The points are passed as POD pairs: the
// original has no EH frame for a by-value HudPoint (empty dtor).
struct H3a_Point {
    float x, y;
};
class H3a_Rect {
public:
    void SetTopLeft(H3a_Point p);       // 0x4295c0
    void SetBottomRight(H3a_Point p);   // 0x4295d0
    float left, top, right, bottom;
    int enabled;
};


// ---- from group draft H3b ----

// ---- from group draft H3c ----
struct H3c_Point {
    H3c_Point(const H3c_Point& other); // 0x424300
    ~H3c_Point() {}
    float x, y;
};
class H3c_Rect {
public:
    H3c_Rect(H3c_Point upperLeft, HudPoint lowerRight); // 0x4390a0
    int fields[5];
};
// A std::string (same layout) whose _Grow (0x402980) and _Eos (0x402960) stay out of line, as in the original.
class H3c_String {
public:
    H3c_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~H3c_String() { Free(true); }
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

#pragma comment(linker, "/alternatename:?Tidy@H3c_String@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?Grow@H3c_String@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Eos@H3c_String@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")
#pragma comment(linker, "/alternatename:??0H3c_Point@@QAE@ABU0@@Z=?Assign@Pair@@QAEPAU1@PBU1@@Z")
#pragma comment(linker, "/alternatename:??0H3c_Rect@@QAE@UH3c_Point@@VHudPoint@@@Z=??0A_Rect@@QAE@VHudPoint@@0@Z")
// VC6 vector layout with each call/inline boundary pinned to the target loader.
struct H3c_RectVector {
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
            unsigned int count = Size() + (1 < Size() ? Size() : 1);
            UIRect** storage = alloc.allocate(count, (void*)0);
            UIRect** out = CopyInline(first, pos, storage);
            FillInline(out, 1, value);
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
    UIRect** CopyDeep(UIRect* const* from, UIRect* const* finish, UIRect** out)
    {
        for (; from != finish; ++out, ++from)
            alloc.construct(out, *from);
        return out;
    }
    void FillDeep(UIRect** out, unsigned int count, UIRect* const& value)
    {
        for (; 0 < count; --count, ++out)
            alloc.construct(out, value);
    }
    void DestroyInline(UIRect** from, UIRect** finish)
    {
        for (; from != finish; ++from)
            alloc.destroy(from);
    }
    // Keep the range operation separate, as std::fill is in the original header.
    static void AssignRange(UIRect** from, UIRect** finish, UIRect* const& value)
    {
        for (; from != finish; ++from)
            *from = value;
    }
    void PushBackFinal(UIRect* const& value)
    {
        UIRect** pos = last;
        if ((unsigned int)(end - last) < 1) {
            unsigned int count = Size() + (1 < Size() ? Size() : 1);
            UIRect** storage = alloc.allocate(count, (void*)0);
            UIRect** out = CopyDeep(first, pos, storage);
            FillDeep(out, 1, value);
            CopyInline(pos, last, out + 1);
            DestroyInline(first, last);
            alloc.deallocate(first, end - first);
            end = storage + count;
            last = storage + Size() + 1;
            first = storage;
        } else if ((unsigned int)(last - pos) < 1) {
            CopyDeep(pos, last, pos + 1);
            FillInline(last, 1 - (last - pos), value);
            AssignRange(pos, last, value);
            last += 1;
        } else {
            CopyInline(last - 1, last, last);
            UIRect** to = last;
            UIRect** from = last - 1;
            while (pos != from)
                *--to = *--from;
            AssignRange(pos, pos + 1, value);
            last += 1;
        }
    }
};
#pragma comment(linker, "/alternatename:?Insert@H3c_RectVector@@QAEXPAPAVUIRect@@IABQAV2@@Z=?insert@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXPAPAXIABQAX@Z")
#pragma comment(linker, "/alternatename:?SizeCall@H3c_RectVector@@QBEIXZ=?size@?$vector@PAXV?$allocator@PAX@std@@@std@@QBEIXZ")
#pragma comment(linker, "/alternatename:?Ucopy@H3c_RectVector@@QAEPAPAVUIRect@@PBQAV2@0PAPAV2@@Z=?_Ucopy@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEPAPAXPBQAX0PAPAX@Z")
#pragma comment(linker, "/alternatename:?Ufill@H3c_RectVector@@QAEXPAPAVUIRect@@IABQAV2@@Z=?_Ufill@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEXPAPAXIABQAX@Z")
#pragma comment(linker, "/alternatename:?Destroy@H3c_RectVector@@QAEXPAPAVUIRect@@0@Z=?_Destroy@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEXPAPAX0@Z")
#pragma comment(linker, "/alternatename:?Construct@H3c_RectVector@@SAXPAPAVUIRect@@ABQAV2@@Z=?_Construct@std@@YAXPAPAXABQAX@Z")
typedef char H3c_RectVectorLayoutCheck[sizeof(H3c_RectVector) == sizeof(std::vector<UIRect*>) ? 1 : -1];

extern ConfigKey DAT_004f6e18[];

// Inline-budget ballast (as B3_Nop in unk_43f9e0.cpp): VC6 spends its inline budget from the last call site
// backwards, so these shift which push_back sites expand and how deeply.
static inline int H3c_Nop(int a) { return a + 1; }
#define H3c_ADD(ul, lr) \
    do { \
        rect = (UIRect*)new H3c_Rect(*(const H3c_Point*)&config.GetVector2(ul), \
                                     *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)

#define H3c_ADD_PENULTIMATE(ul, lr) \
    do { \
        rect = (UIRect*)new H3c_Rect(*(const H3c_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        ((H3c_RectVector*)&m_items)->PushBackExpanded(rect); \
    } while (0)

#define H3c_ADD_FINAL(ul, lr) \
    do { \
        rect = (UIRect*)new H3c_Rect(*(const H3c_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        ((H3c_RectVector*)&m_items)->PushBackFinal(rect); \
    } while (0)

// FUNCTION: RALLYTROPHY 0x4295c0
void H3a_Rect::SetTopLeft(H3a_Point p)
{
    left = p.x;
    top = p.y;
}

// FUNCTION: RALLYTROPHY 0x4295d0
void H3a_Rect::SetBottomRight(H3a_Point p)
{
    right = p.x;
    bottom = p.y;
}

// FUNCTION: RALLYTROPHY 0x4295f0
MenuScreen2A8::MenuScreen2A8()
{
    m_selected = 0;
}

// FUNCTION: RALLYTROPHY 0x429740
// SYMBOL: ??_GMenuScreen2A8@@UAEPAXI@Z
typedef void Generated_429740();

// FUNCTION: RALLYTROPHY 0x429760
MenuScreen2A8::~MenuScreen2A8()
{
    Release();
}

// Layout constants the original reads from .rdata as separate objects (literal * scale: fld const; fmul var).
static float H3a_firstY = 262.0f;      // 0x4e1594
static float H3a_step = 42.0f;         // 0x4e1598
static float H3a_textX = 54.0f;        // 0x4e159c
static float H3a_backY = 610.0f;       // 0x4e15b0

// FUNCTION: RALLYTROPHY 0x4298c0
// Plain stores in the label loop (not SetPos: its three inline calls spend VC6's inline budget) leave exactly the
// first eight string assigns out of line, as in the original.
void MenuScreen2A8::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    float xs = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float ys = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    title = FUN_00405380(0x4d);
    textScaleX = xs * 32.0f;
    unk40 = -1;
    textScaleY = ys * 16.0f;
    items[0].text = FUN_00405380(0x4e);
    items[1].text = FUN_00405380(0x4f);
    items[7].text = FUN_00405380(0x50);
    items[6].text = FUN_00405380(0x51);
    items[5].text = FUN_00405380(0x52);
    items[2].text = FUN_00405380(0x53);
    items[3].text = FUN_00405380(0x55);
    items[4].text = FUN_00405380(0x54);
    items[8].text = FUN_00405380(0x18);
    values188[0].text = FUN_00405380(4);
    values188[1].text = FUN_00405380(5);
    values188[2].text = FUN_00405380(6);
    values1f4[0].text = FUN_00405380(4);
    values1f4[1].text = FUN_00405380(5);
    values1f4[2].text = FUN_00405380(6);
    values1f4[3].text = FUN_00405380(8);
    values284[0].text = FUN_00405380(3);
    values284[1].text = FUN_00405380(4);
    values284[2].text = FUN_00405380(5);
    values284[3].text = FUN_00405380(6);
    values284[4].text = FUN_00405380(8);
    onOff[0].text = FUN_00405380(2);
    onOff[1].text = FUN_00405380(1);
    values380[0].text = FUN_00405380(4);
    values380[1].text = FUN_00405380(6);
    values3c8[0].text = FUN_00405380(2);
    values3c8[1].text = FUN_00405380(4);
    values3c8[2].text = FUN_00405380(6);
    values434[0].text = FUN_00405380(2);
    values434[1].text = FUN_00405380(0x58);
    values434[2].text = FUN_00405380(0x5a);
    float y = H3a_firstY * ys;
    float x = H3a_textX * xs;
    float step = H3a_step * ys;
    for (int i = 0; i < 9; i++) {
        items[i].unk20 = -1;
        items[i].x = x;
        items[i].y = y;
        y += step;
    }
    Begin(game);
    items[8].y = H3a_backY * ys;
    LoadLayout();
}

extern int DAT_005012e4;               // nonzero: the device can render into textures
extern int DAT_005012f0;
extern int DAT_005012f4;               // render state 0xb (set when detail [6] is off)
extern float DAT_005012f8;             // draw distance

// original: FUN_0042a210
// FUNCTION: RALLYTROPHY 0x42a210
void MenuScreen2A8::FUN_0042a210(int arg)
{
    __asm {
        mov ecx, dword ptr [DAT_0056b0ac]
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 0x14]
        mov ecx, dword ptr [eax]
        and ecx, 0xff
        shr ecx, 2
        and ecx, 1
        mov dword ptr [DAT_005012e4], ecx
    }
    DAT_005012f4 = DAT_00501418[6] == 0;
    DAT_0056b0ac->SetRenderState(0xb, DAT_005012f4);
    DAT_005012f0 = 0;
    float distance = DAT_00501418[0] * 0.01f;
    DAT_005012f8 = distance * 400.0f + 100.0f;
    if (!DAT_005012ec && DAT_00501418[5] >= 1) {
        DAT_00501418[5] = 0;
    }
    if (!DAT_005012e8 && DAT_00501418[5] >= 2) {
        DAT_00501418[5] = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x42a2c0
int MenuScreen2A8::Run(MenuGame* game, NetRace* net_)
{
    net = net_;
    Setup(game);
    if (DAT_005012e8) {
        unk4a4 = 2;
    } else if (DAT_005012ec) {
        unk4a4 = 1;
    } else {
        unk4a4 = 0;
    }
    int r;
    do {
        r = Frame();
    } while (r != -1);
    FUN_0042a210(0);
    return -1;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// NONMATCHING: RALLYTROPHY 0x42a330
// 2976 vs 2988 bytes; 783 differing bytes. Combining rows 6/7 matches the original shared case tail; remaining open items:
// Frame of the 9-row options screen (rows 0-7 sliders/selectors, row 8 Back; 16 arrow rects at m_items[9..24]).
// Control flow, calls, jump tables and EH states match. Open: (1) the original keeps the zero constant in ebp and the
// -1 / `dir` register in ebx, mine swaps them (also seen in 0x43cc50); (2) mine hoists nothing but gives `fld mem; fmul
// 400.0` for the width instead of `fld 400.0; fmul sx`; (3) the original tail-merges draw cases 7 and 6 (shared `fld`
// tail, pushes of value/4/0 in the head); (4) (1, 1) object and scale copy sit in swapped frame slots.
int MenuScreen2A8::Frame()
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
            for (int i = 0; i < 9; i++) {
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
            if (m_selected == 8) {
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
                m_selected = 8;
            } else if (sel >= 9) {
                m_selected = 0;
            }
            int cx = (int)(m_items[m_selected]->GetTopLeft().x +
                           (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f);
            int cy = (int)(m_items[m_selected]->GetTopLeft().y +
                           (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f);
            SetCursorPos((int)(cx * sx), (int)(cy * sy));
        }
        for (int i = 0; i < 9; i++) {
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
            if (m_items[9]->Contains(mouse)) {
                dir--;
            }
            if (m_items[11]->Contains(mouse)) {
                dir--;
            }
            if (m_items[13]->Contains(mouse)) {
                dir--;
            }
            if (m_items[15]->Contains(mouse)) {
                dir--;
            }
            if (m_items[17]->Contains(mouse)) {
                dir--;
            }
            if (m_items[19]->Contains(mouse)) {
                dir--;
            }
            if (m_items[21]->Contains(mouse)) {
                dir--;
            }
            if (m_items[23]->Contains(mouse)) {
                dir--;
            }
            if (m_items[10]->Contains(mouse)) {
                dir++;
            }
            if (m_items[12]->Contains(mouse)) {
                dir++;
            }
            if (m_items[14]->Contains(mouse)) {
                dir++;
            }
            if (m_items[16]->Contains(mouse)) {
                dir++;
            }
            if (m_items[18]->Contains(mouse)) {
                dir++;
            }
            if (m_items[20]->Contains(mouse)) {
                dir++;
            }
            if (m_items[22]->Contains(mouse)) {
                dir++;
            }
            if (m_items[24]->Contains(mouse)) {
                dir++;
            }
            if (m_selected == 8) {
                done = 1;
                continue;
            }
        }
        if (dir != 0 && (unsigned)m_selected <= 7) {
            switch (m_selected) {
            case 0:
                AdjustVolume(&DAT_00501418[0], dir * 10);
                break;
            case 1:
                AdjustValue(&DAT_00501418[1], dir, 0, 3);
                break;
            case 7:
                AdjustValue(&DAT_00501418[2], dir, 0, 4);
                break;
            case 6:
                AdjustValue(&DAT_00501418[3], dir, 0, 4);
                break;
            case 5:
                AdjustValue(&DAT_00501418[4], dir, 0, 2);
                break;
            case 2:
                Toggle(&DAT_00501418[6], dir);
                break;
            case 3:
                Toggle(&DAT_00501418[7], dir);
                break;
            case 4:
                AdjustValue(&DAT_00501418[5], dir, 0, unk4a4);
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
            text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x3c), 0, 1.0f, 1.0f);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            game->FUN_00435de0(517.0f, 243.0f);
            HudPoint scale = *game->GetScale();
            for (int i = 0; i < 9; i++) {
                int selected = (i == m_selected);
                Color col;
                HudPoint one;
                col.value = selected ? DAT_00508c0c : DAT_00508c04;
                text->color = col.value;
                one.x = 1.0f;
                one.y = 1.0f;
                text->Print(DAT_0056b0ac, items[i].x, items[i].y, items[i].text.c_str(), 0, 1.0f, 1.0f);
                HudPoint pos;
                pos.x = 520.0f * DAT_0056aea8;
                pos.y = items[i].y;
                switch (i) {
                case 0:
                    DrawSlider(pos.x, pos.y, DAT_00501418[0], m_selected == 0);
                    break;
                case 1:
                    DrawToggle(text, values1f4, pos.x, pos.y, 400.0f * sx, m_selected == 1, DAT_00501418[1], 0, 3);
                    break;
                case 6:
                case 7:
                    DrawToggle(text, values284, pos.x, pos.y, 400.0f * sx, m_selected == i, DAT_00501418[9 - i], 0, 4);
                    break;
                case 5:
                    DrawToggle(text, values3c8, pos.x, pos.y, 400.0f * sx, m_selected == 5, DAT_00501418[4], 0, 2);
                    break;
                case 2:
                    DrawToggle(text, values380, pos.x, pos.y, 400.0f * sx, m_selected == 2, DAT_00501418[6], 0, 1);
                    break;
                case 3:
                    DrawToggle(text, onOff, pos.x, pos.y, 400.0f * sx, m_selected == 3, DAT_00501418[7], 0, 1);
                    break;
                case 4:
                    DrawToggle(text, values434, pos.x, pos.y, 400.0f * sx, m_selected == 4, DAT_00501418[5], 0, unk4a4);
                    break;
                }
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 8, 0, 1, 1, -1.0f);
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

/* FUNCTION: RALLYTROPHY 0x42aee0 */
// Exact 5014-byte match / 251 relocations. Final two insertion helper boundaries are explicit.
// Local comparison and call audit: docs/analysis/graphics-menu-layout.md.
// original: FUN_0042aee0 (thiscall; graphics menu layout loader, called from 0x42a330)
void MenuScreen2A8::LoadLayout()
{
    Config config;
    UIRect* rect;
    config.Define(DAT_004f6e18, 0x32);
    H3c_String path("data/menu/graphicsmenu.ini");
    config.Load(path.c_str(), true, false, 0);
    H3c_ADD("ViewDistanceText_UpperLeft", "ViewDistanceText_LowerRight");
    H3c_ADD("TrackDetailText_UpperLeft", "TrackDetailText_LowerRight");
    H3c_ADD("AlphaBlendingText_UpperLeft", "AlphaBlendingText_LowerRight");
    H3c_ADD("FogText_UpperLeft", "FogText_LowerRight");
    H3c_ADD("ReflectionsText_UpperLeft", "ReflectionsText_LowerRight");
    H3c_ADD("EffectDetailText_UpperLeft", "EffectDetailText_LowerRight");
    H3c_ADD("ShadowDetailText_UpperLeft", "ShadowDetailText_LowerRight");
    H3c_ADD("OpponentDetailText_UpperLeft", "OpponentDetailText_LowerRight");
    H3c_ADD("BackText_UpperLeft", "BackText_LowerRight");
    H3c_ADD("ViewDistanceArrow1_UpperLeft", "ViewDistanceArrow1_LowerRight");
    H3c_ADD("ViewDistanceArrow2_UpperLeft", "ViewDistanceArrow2_LowerRight");
    H3c_ADD("TrackDetailArrow1_UpperLeft", "TrackDetailArrow1_LowerRight");
    H3c_ADD("TrackDetailArrow2_UpperLeft", "TrackDetailArrow2_LowerRight");
    H3c_ADD("AlphaBlendingArrow1_UpperLeft", "AlphaBlendingArrow1_LowerRight");
    H3c_ADD("AlphaBlendingArrow2_UpperLeft", "AlphaBlendingArrow2_LowerRight");
    H3c_ADD("FogArrow1_UpperLeft", "FogArrow1_LowerRight");
    H3c_ADD("FogArrow2_UpperLeft", "FogArrow2_LowerRight");
    H3c_ADD("ReflectionsArrow1_UpperLeft", "ReflectionsArrow1_LowerRight");
    H3c_ADD("ReflectionsArrow2_UpperLeft", "ReflectionsArrow2_LowerRight");
    H3c_ADD("EffectDetailArrow1_UpperLeft", "EffectDetailArrow1_LowerRight");
    H3c_ADD("EffectDetailArrow2_UpperLeft", "EffectDetailArrow2_LowerRight");
    H3c_ADD("ShadowDetailArrow1_UpperLeft", "ShadowDetailArrow1_LowerRight");
    H3c_Nop(1);
    H3c_ADD("ShadowDetailArrow2_UpperLeft", "ShadowDetailArrow2_LowerRight");
    H3c_Nop(1);
    H3c_ADD_PENULTIMATE("OpponentDetailArrow1_UpperLeft", "OpponentDetailArrow1_LowerRight");
    H3c_ADD_FINAL("OpponentDetailArrow2_UpperLeft", "OpponentDetailArrow2_LowerRight");
}

// FUNCTION: RALLYTROPHY 0x42c280
// SYMBOL: _$E23
typedef void Generated_42c280();

// FUNCTION: RALLYTROPHY 0x42c2a0
// SYMBOL: _$E22
typedef void Generated_42c2a0();
