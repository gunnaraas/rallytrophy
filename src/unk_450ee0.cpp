// SoundMenu (0x450ee0-0x45237f): the options screen. Map: analysis/range-450ee0.md section 1; classes in
// include/unk_450ee0.h.
// Work groups: A (SoundMenu lifecycle: 0x450ee0-0x451430) and B (Frame, 0x451460), prefix SMA_/SMB_.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- layout loader (0x451c10) helpers ----
// SoundMenu file (0x450ee0-0x45237f, module end 0x452350). Map: analysis/range-450ee0.md section 1.
// Group C: the soundmenu.ini layout loader (0x451c10) and the module-end stub (0x452350).
// ---- Group C ----
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
#pragma comment(linker, "/alternatename:?Tidy@C_String@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?Grow@C_String@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Eos@C_String@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")
#pragma comment(linker, "/alternatename:??0C_Point@@QAE@ABU0@@Z=?Assign@Pair@@QAEPAU1@PBU1@@Z")
#pragma comment(linker, "/alternatename:??0C_Rect@@QAE@UC_Point@@VHudPoint@@@Z=??0A_Rect@@QAE@VHudPoint@@0@Z")
// VC6 vector layout with each call/inline boundary pinned to the target loader.
struct C_RectVector {
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
            CopyInline(pos, last, out + 1);
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
#pragma comment(linker, "/alternatename:?Insert@C_RectVector@@QAEXPAPAVUIRect@@IABQAV2@@Z=?insert@?$vector@PAXV?$allocator@PAX@std@@@std@@QAEXPAPAXIABQAX@Z")
#pragma comment(linker, "/alternatename:?SizeCall@C_RectVector@@QBEIXZ=?size@?$vector@PAXV?$allocator@PAX@std@@@std@@QBEIXZ")
#pragma comment(linker, "/alternatename:?Ucopy@C_RectVector@@QAEPAPAVUIRect@@PBQAV2@0PAPAV2@@Z=?_Ucopy@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEPAPAXPBQAX0PAPAX@Z")
#pragma comment(linker, "/alternatename:?Ufill@C_RectVector@@QAEXPAPAVUIRect@@IABQAV2@@Z=?_Ufill@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEXPAPAXIABQAX@Z")
#pragma comment(linker, "/alternatename:?Destroy@C_RectVector@@QAEXPAPAVUIRect@@0@Z=?_Destroy@?$vector@PAXV?$allocator@PAX@std@@@std@@IAEXPAPAX0@Z")
#pragma comment(linker, "/alternatename:?Construct@C_RectVector@@SAXPAPAVUIRect@@ABQAV2@@Z=?_Construct@std@@YAXPAPAXABQAX@Z")
typedef char C_RectVectorLayoutCheck[sizeof(C_RectVector) == sizeof(std::vector<UIRect*>) ? 1 : -1];
#define C_ADD(ul, lr) \
    do { \
        rect = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        ((C_RectVector*)&m_items)->PushBack(rect); \
    } while (0)
#define C_ADD_LAST(ul, lr) \
    do { \
        rect = (UIRect*)new C_Rect(*(const C_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        ((C_RectVector*)&m_items)->PushBackExpanded(rect); \
    } while (0)
extern ConfigKey DAT_004fa330[];   // soundmenu.ini key table (34 keys)

// FUNCTION: RALLYTROPHY 0x450ee0
SoundMenu::SoundMenu()
{
}

// FUNCTION: RALLYTROPHY 0x450f70
// SYMBOL: ??_GSoundMenu@@UAEPAXI@Z
typedef void Generated_450f70();

// FUNCTION: RALLYTROPHY 0x450f90
SoundMenu::~SoundMenu()
{
    Release();
}

// NONMATCHING: RALLYTROPHY 0x451040
// The built body is 998 bytes; the target interval is 1008, with ten NOPs after ret 4. Comparing the bodies leaves
// 21 differing bytes in x87 operand order and loop stack scheduling. Reversing products had no effect; volatile xs/ys
// gave 76 body bytes, x-before-y 23, ys-before-xs 25, step/y/x order 30, volatile constant-address reads 53, and a
// nonvolatile 262-address read 28. Pointer induction and register-qualified x/y/step leave the 21-byte body diff
// unchanged; moving both x/step products into the loop also leaves it unchanged, while moving x alone there raises
// the body diff to 64 bytes. Plain objcmp observations.
void SoundMenu::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    float xs = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float ys = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    head.text = FUN_00405380(0x5b);
    head.x = xs * 32.0f;
    head.y = ys * 16.0f;
    head.unk20 = -1;
    items[0].text = FUN_00405380(0x5c);
    items[1].text = FUN_00405380(0x5d);
    items[2].text = FUN_00405380(0x5e);
    items[3].text = FUN_00405380(0x5f);
    items[4].text = FUN_00405380(0x60);
    items[5].text = FUN_00405380(0x19b);
    items[6].text = FUN_00405380(0x18);
    labels[0].text = FUN_00405380(2);
    labels[1].text = FUN_00405380(1);
    float y = 262.0f * ys;
    float x = 54.0f * xs;
    float step = 42.0f * ys;
    for (int i = 0; i < 7; i++) {
        items[i].SetPos(x, y);
        y += step;
    }
    Begin(game);
    items[6].y = 610.0f * ys;
    LoadLayout();
}

// FUNCTION: RALLYTROPHY 0x451430
int SoundMenu::Run(MenuGame* game, NetRace* net_)
{
    net = net_;
    Setup(game);
    int r;
    do {
        r = Frame();
    } while (r != -1);
    return r;
}

// NONMATCHING: RALLYTROPHY 0x451460
// Everything but the draw loop matches (prologue, input handling, cursor placement, tail). In the loop VC6 hoists the
// (1, 1) object's stores out of the loop (the original keeps them inside), puts it and the GetScale copy in swapped
// frame slots (0x44/0x4c), and the scaled constants come out `fld mem; fmul const` instead of `fld const; fmul mem`.
// Bounded objcmp: 1968-byte COFF span vs 1915-byte inventory, 407 differences. Probes: docs/analysis/sound-menu-frame.md.
int SoundMenu::Frame()
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
            for (int i = 0; i < 7; i++) {
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
            if (m_selected == 6) {
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
                m_selected = 6;
            } else if (sel >= 7) {
                m_selected = 0;
            }
            int cx = (int)(m_items[m_selected]->GetTopLeft().x +
                           (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f);
            int cy = (int)(m_items[m_selected]->GetTopLeft().y +
                           (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f);
            SetCursorPos((int)(cx * sx), (int)(cy * sy));
        }
        for (int i = 0; i < 7; i++) {
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
            if (m_items[7]->Contains(mouse)) {
                dir--;
            }
            if (m_items[8]->Contains(mouse)) {
                dir++;
            }
            if (m_selected == 6) {
                done = 1;
                continue;
            }
        }
        if (dir != 0 && (unsigned)m_selected <= 5) {
            switch (m_selected) {
            case 0:
                AdjustVolume(&DAT_00501438[0], dir * 10);
                break;
            case 1:
                AdjustVolume(&DAT_00501438[1], dir * 10);
                break;
            case 2:
                AdjustVolume(&DAT_00501438[2], dir * 10);
                break;
            case 3:
                AdjustVolume(&DAT_00501438[3], dir * 10);
                break;
            case 4:
                AdjustVolume(&DAT_00501438[4], dir * 10);
                break;
            case 5:
                Toggle(&DAT_0050147c, dir);
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
            text->Print(DAT_0056b0ac, 42.0f * DAT_0056aea8, 142.0f * DAT_0056aeac, FUN_00405380(0x3d), 0, 1.0f, 1.0f);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
            game->FUN_00435de0(517.0f, 243.0f);
            HudPoint scale = *game->GetScale();
            for (int i = 0; i < 7; i++) {
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
                    DrawSlider(pos.x, pos.y, DAT_00501438[0], selected);
                    break;
                case 1:
                    DrawSlider(pos.x, pos.y, DAT_00501438[1], selected);
                    break;
                case 2:
                    DrawSlider(pos.x, pos.y, DAT_00501438[2], selected);
                    break;
                case 3:
                    DrawSlider(pos.x, pos.y, DAT_00501438[3], selected);
                    break;
                case 4:
                    DrawSlider(pos.x, pos.y, DAT_00501438[4], selected);
                    break;
                case 5:
                    DrawToggle(text, labels, pos.x, pos.y, 400.0f * sx, selected, DAT_0050147c, 0, 1);
                    break;
                }
            }
            ((MenuGame*)DAT_0056b0c8)->FUN_00435b60(1, 6, 0, 1, 1, -1.0f);
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

/* FUNCTION: RALLYTROPHY 0x451c10 */
// Exact 1848-byte match with 97 relocations.
// C_RectVector pins the first eight insertions out of line and expands the ninth with the target's helper-call policy.
// Local objcmp observations and measured alternatives: docs/analysis/sound-menu-layout.md.
// original: FUN_00451c10 (thiscall; SoundMenu layout loader)
void SoundMenu::LoadLayout()
{
    Config config;
    UIRect* rect;
    config.Define(DAT_004fa330, 0x22);
    C_String path("data/extras/soundmenu.ini");
    config.Load(path.c_str(), true, false, 0);
    C_ADD("EngineText_UpperLeft", "EngineText_LowerRight");
    C_ADD("SoundEffectsText_UpperLeft", "SoundEffectsText_LowerRight");
    C_ADD("SpeechText_UpperLeft", "SpeechText_LowerRight");
    C_ADD("MusicText_UpperLeft", "MusicText_LowerRight");
    C_ADD("MenuMusicText_UpperLeft", "MenuMusicText_LowerRight");
    C_ADD("CommentsText_UpperLeft", "CommentsText_LowerRight");
    C_ADD("BackText_UpperLeft", "BackText_LowerRight");
    C_ADD("EngineArrow1_UpperLeft", "EngineArrow1_LowerRight");
    C_ADD_LAST("EngineArrow2_UpperLeft", "EngineArrow2_LowerRight");
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x452350
// SYMBOL: _$E23
typedef void Generated_452350();

// FUNCTION: RALLYTROPHY 0x452370
// SYMBOL: _$E22
typedef void Generated_452370();
