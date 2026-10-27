// RaceOptionsMenu (0x43f9e0-0x44136f): the race options screen. Map: analysis/range-43d770.md section 2; class in
// include/unk_43f9e0.h. Work group B1 (lifecycle, Setup, Run, module end), prefix B1_.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_43f9e0.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>


// ---- from group draft X6 ----
class B2_Unit {
public:
    B2_Unit() { x = 1.0f; y = 1.0f; }
    ~B2_Unit() {}
    float x, y;
};
static float B2_COLW = 400.0f;   // value column width (0x4e1834; plain static: the original multiplies const * scale)

// ---- from group draft X7 ----

// ---- from group draft X7 (helpers) ----

// ---- layout loader (0x440890) helpers ----
// A 2D point whose copy constructor is out of line (0x424300) and whose destructor is empty.
struct B3_Point {
    B3_Point(const B3_Point& other);   // 0x424300
    ~B3_Point() {}
    float x, y;
};
class B3_Rect {
public:
    B3_Rect(B3_Point topLeft, HudPoint bottomRight);   // 0x4390a0
    float f[5];
};
// A std::string (same layout) whose _Grow (0x402980) and _Eos (0x402960) stay out of line, as in the original.
class B3_String {
public:
    B3_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~B3_String() { Free(true); }
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
#pragma comment(linker, "/alternatename:?Tidy@B3_String@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?Grow@B3_String@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Eos@B3_String@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")
#pragma comment(linker, "/alternatename:??0B3_Point@@QAE@ABU0@@Z=?Assign@Pair@@QAEPAU1@PBU1@@Z")
#pragma comment(linker, "/alternatename:??0B3_Rect@@QAE@UB3_Point@@VHudPoint@@@Z=??0A_Rect@@QAE@VHudPoint@@0@Z")
// VC6 vector layout with each call/inline boundary pinned to the target loader.
struct B3_RectVector {
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
    void DestroyInline(UIRect** from, UIRect** finish)
    {
        for (; from != finish; ++from)
            alloc.destroy(from);
    }
    void PushBackExpanded(UIRect* const& value)
    {
        UIRect** pos = last;
        if ((unsigned int)(end - last) < 1) {
            unsigned int count = Size() + (1 < Size() ? Size() : 1);
            UIRect** storage = alloc.allocate(count, (void*)0);
            UIRect** out = CopyInline(first, pos, storage);
            FillInline(out, 1, value);
            CopyInline(pos, last, out + 1);
            DestroyInline(first, last);
            alloc.deallocate(first, end - first);
            end = storage + count;
            last = storage + Size() + 1;
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
#pragma comment(linker, "/alternatename:?Insert@B3_RectVector@@QAEXPAPAVUIRect@@IABQAV2@@Z=?insert@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXPAPAXIABQAX@Z")
#pragma comment(linker, "/alternatename:?SizeCall@B3_RectVector@@QBEIXZ=?size@?$vector@PAXV?$allocator@PAX@std@@@std@@QBEIXZ")
#pragma comment(linker, "/alternatename:?Ucopy@B3_RectVector@@QAEPAPAVUIRect@@PBQAV2@0PAPAV2@@Z=?_Ucopy@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEPAPAXPBQAX0PAPAX@Z")
#pragma comment(linker, "/alternatename:?Ufill@B3_RectVector@@QAEXPAPAVUIRect@@IABQAV2@@Z=?_Ufill@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEXPAPAXIABQAX@Z")
#pragma comment(linker, "/alternatename:?Destroy@B3_RectVector@@QAEXPAPAVUIRect@@0@Z=?_Destroy@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEXPAPAX0@Z")
#pragma comment(linker, "/alternatename:?Construct@B3_RectVector@@SAXPAPAVUIRect@@ABQAV2@@Z=?_Construct@std@@YAXPAPAXABQAX@Z")
typedef char B3_RectVectorLayoutCheck[sizeof(B3_RectVector) == sizeof(std::vector<UIRect*>) ? 1 : -1];
#define B3_ADD(ul, lr) \
    do { \
        rect = (UIRect*)new B3_Rect(*(const B3_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)

#define B3_ADD_LAST(ul, lr) \
    do { \
        rect = (UIRect*)new B3_Rect(*(const B3_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        ((B3_RectVector*)&m_items)->PushBackExpanded(rect); \
    } while (0)

// Retain the existing inline-budget balance for the matching first twelve blocks;
// the final insertion's helper boundaries are controlled explicitly above.
static inline int B3_Nop(int a) { return a + 1; }

// FUNCTION: RALLYTROPHY 0x43f9e0
RaceOptionsMenu::RaceOptionsMenu()
{
    m_selected = 0;
}

// FUNCTION: RALLYTROPHY 0x43faa0
// SYMBOL: ??_GRaceOptionsMenu@@UAEPAXI@Z
typedef void Generated_43faa0();

// FUNCTION: RALLYTROPHY 0x43fac0
RaceOptionsMenu::~RaceOptionsMenu()
{
    Release();
}

// Layout constants the original reads from .rdata as separate objects (literal * scale: fld const; fmul var).
static float B1_firstY = 262.0f;       // 0x4e181c
static float B1_step = 42.0f;          // 0x4e1820
static float B1_textX = 54.0f;         // 0x4e1824
static float B1_backY = 610.0f;        // 0x4e1838

static inline void SetRaceOptionPos(MenuLabel& item, float x, float y)
{
    *(volatile int*)&item.unk20 = -1;
    item.SetX(x);
    item.SetY(y);
}

// FUNCTION: RALLYTROPHY 0x43fba0
// Evidence: ev_661facc1a875ec8b1093b9b1f68ce6eea90333c45f5b7b898d8fc7b7becb3766.
// A volatile sentinel store preserves the target order before the x-coordinate load.
void RaceOptionsMenu::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    float xs = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float ys = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    title = FUN_00405380(0x46);
    textScaleX = xs * 32.0f;
    unk40 = -1;
    textScaleY = ys * 16.0f;
    items[0].text = FUN_00405380(0x48);
    items[1].text = FUN_00405380(0x49);
    items[2].text = FUN_00405380(0x4c);
    items[3].text = FUN_00405380(0x19a);
    items[4].text = FUN_00405380(0x18);
    onOff[0].text = FUN_00405380(2);
    onOff[1].text = FUN_00405380(1);
    units[0].text = FUN_00405380(0x1f);
    units[1].text = FUN_00405380(0x20);
    float y = B1_firstY * ys;
    float x = B1_textX * xs;
    float step = B1_step * ys;
    for (int i = 0; i < 5; i++) {
        SetRaceOptionPos(items[i], x, y);
        y += step;
    }
    Begin(game);
    items[4].y = B1_backY * ys;
    LoadLayout();
}

// FUNCTION: RALLYTROPHY 0x43ff80
void RaceOptionsMenu::Release()
{
    if (m_state) {
        m_state = 0;
        m_game = 0;
        FUN_00438d00();
    }
}

// FUNCTION: RALLYTROPHY 0x43ffa0
int RaceOptionsMenu::Run(MenuGame* game, NetRace* net_)
{
    net = net_;
    Setup(game);
    int r;
    do {
        r = Frame();
    } while (r != -1);
    return r;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// NONMATCHING: RALLYTROPHY 0x43ffd0
// Same size (2240 with tables), 226 bytes differ: VC6 hoists the dead (1, 1) object's stores out of the draw loop (original
// keeps them inside, same as SoundMenu::Frame), loads Print's x before y (original y first) and so rotates the registers in
// the four DrawToggle case bodies. Input handling, mouse hit-test chain and both jump tables match.
int RaceOptionsMenu::Frame()
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
                if (m_items[i]->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
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
            } else if (m_items[7]->Contains(mouse)) {
                dir--;
            } else if (m_items[9]->Contains(mouse)) {
                dir--;
            } else if (m_items[11]->Contains(mouse)) {
                dir--;
            } else if (m_items[6]->Contains(mouse) || m_items[8]->Contains(mouse) || m_items[10]->Contains(mouse) ||
                       m_items[12]->Contains(mouse)) {
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
                Toggle(&DAT_00501300[0], dir);
                break;
            case 1:
                Toggle(&DAT_00501304, dir);
                break;
            case 2:
                Toggle(&DAT_00501300[6], dir);
                break;
            case 3:
                Toggle(&DAT_00501478, dir);
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
            text->Print(DAT_0056b0ac, 54.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x3b), 0, 1.0f, 1.0f);
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
                    DrawToggle(text, units, pos.x, pos.y, B2_COLW * sx, selected, DAT_00501300[0], 0, 1);
                    break;
                case 1:
                    DrawToggle(text, onOff, pos.x, pos.y, B2_COLW * sx, selected, DAT_00501304, 0, 1);
                    break;
                case 2:
                    DrawToggle(text, onOff, pos.x, pos.y, B2_COLW * sx, selected, DAT_00501300[6], 0, 1);
                    break;
                case 3:
                    DrawToggle(text, onOff, pos.x, pos.y, B2_COLW * sx, selected, DAT_00501478, 0, 1);
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

/* FUNCTION: RALLYTROPHY 0x440890 */
// Exact 2722-byte match / 138 relocations. Final insertion uses inline size and pointer destruction.
// Local comparison and operand audit: docs/analysis/race-options-layout.md.
// original: FUN_00440890 (thiscall; RaceOptionsMenu layout loader)
void RaceOptionsMenu::LoadLayout()
{
    Config config;
    UIRect* rect;
    config.Define(DAT_004f8f58, 0x1a);
    B3_String path("data/extras/racemenu.ini");
    config.Load(path.c_str(), true, false, 0);
    B3_ADD("UnitsText_UpperLeft", "UnitsText_LowerRight");
    B3_ADD("GhostCarText_UpperLeft", "GhostCarText_LowerRight");
    B3_ADD("WildlifeText_UpperLeft", "WildlifeText_LowerRight");
    B3_ADD("PaceNotesText_UpperLeft", "PaceNotesText_LowerRight");
    B3_ADD("BackText_UpperLeft", "BackText_LowerRight");
    B3_ADD("UnitsArrow1_UpperLeft", "UnitsArrow1_LowerRight");
    B3_ADD("UnitsArrow2_UpperLeft", "UnitsArrow2_LowerRight");
    B3_ADD("GhostCarArrow1_UpperLeft", "GhostCarArrow1_LowerRight");
    B3_ADD("GhostCarArrow2_UpperLeft", "GhostCarArrow2_LowerRight");
    B3_ADD("WildlifeArrow1_UpperLeft", "WildlifeArrow1_LowerRight");
    B3_ADD("WildlifeArrow2_UpperLeft", "WildlifeArrow2_LowerRight");
    B3_ADD("PaceNotesArrow1_UpperLeft", "PaceNotesArrow1_LowerRight");
    B3_Nop(1);
    B3_ADD_LAST("PaceNotesArrow2_UpperLeft", "PaceNotesArrow2_LowerRight");
}

// FUNCTION: RALLYTROPHY 0x441340
// SYMBOL: _$E23
typedef void Generated_441340();

// FUNCTION: RALLYTROPHY 0x441360
// SYMBOL: _$E22
typedef void Generated_441360();
