// Game (0x4303e0-0x437e9f): the front-end/game singleton at DAT_0056b0c8. Map: analysis/range-4303e0.md; class in
// include/unk_4303e0.h.
// Work groups: G1 (lifecycle, menu label ctor/dtor, module-end stubs; prefix G1_), G2..G17 (see the map).
#define GAME_FULL_LAYOUT                // the full Game class of unk_4303e0.h replaces rallytrophy.h's
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_445e80.h"
#include "unk_43f9e0.h"
#include "unk_439170.h"
#include "unk_43c590.h"
#include "unk_43a390.h"
#include "unk_43d770.h"
#include "unk_44ea60.h"
#include "unk_44b930.h"
#include "unk_441370.h"
#include "unk_45e480.h"
#include "sound.h"
#include "unk_4303e0.h"
#include <math.h>
#include <memory>
#include <stdlib.h>
#include <string.h>
#include <vector>

// ---- Group G1 ----

typedef char G1_sizeChk[sizeof(Game) == 0x139894 ? 1 : -1];
typedef char G1_menuChk[offsetof(Game, menu) == 0x2df4 ? 1 : -1];
typedef char G1_recChk[offsetof(Game, remoteRecords) == 0x139074 ? 1 : -1];
typedef char G1_namesChk[offsetof(Game, names) == 0x139490 ? 1 : -1];

// FUNCTION: RALLYTROPHY 0x4303e0
Game::Game()
{
    models.flag0 = 1;
    selection = 0;
    unk13907c = 0;
    bigText = 0;
    text1390bc = 0;
    text1390c0 = 0;
    smallText = 0;
    text = 0;
    unk1390d4 = 0;
    unk1390d8 = 0;
    unk1390dc = 0;
    unk1390e0 = 0;
    unk1390e4 = 0;
    unk1390e8 = 0;
    remoteRecords = 0;
    unk139108 = 0;
    unk13910c = 0;
    unk139478 = unk13947c = 24.0f;
    DAT_0056b0c8 = (GameView*)this;
    unk139214 = 0;
    unk139218 = 0;
    unk13920c = 2;
    unk139210 = 0;
    unk139220 = 100;
    unk139484 = 1;
    unk0 = 0;
    save = new GameSave;
    replayStore = new GameReplays;
    if (DAT_00501471) {
        remoteRecords = new RecordTable;
    }
}

// FUNCTION: RALLYTROPHY 0x430700
// SYMBOL: ??0MenuLabel@@QAE@XZ
// MenuLabel's inline constructor out of line for the vector iterators (folded with the other std::string-only ones).
typedef void Generated_430700();

// FUNCTION: RALLYTROPHY 0x430720
// SYMBOL: ??1MenuLabel@@QAE@XZ
typedef void Generated_430720();

// FUNCTION: RALLYTROPHY 0x430760
Game::~Game()
{
    FUN_00430eb0();
    FUN_00431c50();
    delete remoteRecords;
    DAT_0056b0c8 = 0;
    delete save;
    delete replayStore;
}

// FUNCTION: RALLYTROPHY 0x4309b0
void Game::FUN_004309b0()
{
    records.FUN_00490820("data/highscores/highscores.hs");
}

// ---- Group G2 ----

// Private layout view used only by this source group; rallytrophy.h owns Game's declaration.
struct G2_GameView {
    unsigned char unk0[0x1390b8];
    void* textRenderers[5];
};

extern "C" void __fastcall FUN_00431f40(Game*);
extern "C" void __fastcall FUN_00430f60(Game*);
extern "C" void __fastcall FUN_00432aa0(Game*);
extern "C" void __fastcall FUN_00432b00(Game*);
extern "C" int __fastcall FUN_00432e10(Game*);
extern "C" void __fastcall FUN_00433220(Game*);
extern "C" void __fastcall FUN_00433bc0(Game*);
extern "C" void __fastcall FUN_00434a50(Game*);
extern "C" void __fastcall FUN_00434df0(Game*);
extern "C" void __fastcall FUN_00434dd0(Game*);
extern "C" void __fastcall FUN_00434ba0();
extern "C" void __fastcall FUN_004327d0(Game*);
extern "C" void __fastcall FUN_00425e50(Game*);
extern "C" void __fastcall FUN_00432b30(Game*, int, int);
extern "C" void __fastcall FUN_0043cb50(void*, int);
extern "C" void __fastcall FUN_004a9520(void*, void*, const char*, float, float);
extern "C" void __fastcall FUN_004a95d0(void*);
extern int DAT_00501460;
extern int DAT_0050145c;
extern int DAT_00508bdc;
extern void (__cdecl *DAT_0059b2f8)();
extern "C" void fun_0049a550();
extern "C" void fun_0049a4f0();
extern "C" void fun_0049a580();
extern "C" void fun_0049a5a0();

// NONMATCHING: RALLYTROPHY 0x4309d0
// The dispatch and resource lifecycle are reconstructed; the virtual dispatch and exception cleanup lowering differ from the target.
void Game::FUN_004309d0() {
    Config config;
    config.Define((const ConfigKey*)0x4f7688, 12);
    config.Load("data/settings/game.ini", true, false, 0);
    *((int*)((char*)this + 0x5c20)) = config.GetInt("NumberOfCars");
    *((int*)((char*)this + 0x59d0)) = config.GetInt("NumberOfTracks");
    *((float*)((char*)this + 0x13948c)) = config.GetFloat("TorqueMultiplier");
    *((int*)((char*)this + 0x139220)) = config.GetInt("DefaultMusicVolume");
    *(float*)0x501518 = config.GetFloat("ResetTimeAir");
    *(float*)0x50151c = config.GetFloat("ResetTimeJam");
    *(float*)0x501520 = config.GetFloat("ResetTimeOutOfTrack");
    *(float*)0x501524 = config.GetFloat("ResetTimeIllegal1");
    *(float*)0x501528 = config.GetFloat("ResetTimeIllegal2");
    *(float*)0x50150c = config.GetFloat("CPUGapsNovice");
    *(float*)0x501510 = config.GetFloat("CPUGapsIntermediate");
    *(float*)0x501514 = config.GetFloat("CPUGapsExpert");
    config.Close();
    ((RecordTable*)((unsigned char*)this + 0x5c30))->FUN_00490470("data/highscores/highscores.hs");
    FUN_00430cf0();
    FUN_00431f40(this);
    FUN_00430f60(this);
    if (*(void**)((unsigned char*)this + 0x13910c))
        ((void(*)(void*))0x48e660)(*(void**)((unsigned char*)this + 0x13910c));
    FUN_0043cb50((unsigned char*)this + 0x2df4, 0);
    FUN_00432aa0(this);
    for (;;) {
        if (DAT_0057b9f4) break;
        int state = FUN_00432e10(this);
        DAT_00501460 = 0;
        if (state <= 6) {
            switch (state) {
            case 0:
                FUN_004a4870("data/settings/game.ini"); DAT_00501458 = 0; DAT_0050145c = 1; DAT_00501473 = 0;
                FUN_00433220(this); DAT_00501472 = 0; break;
            case 1:
                FUN_004a4870("data/settings/game.ini"); DAT_00501472 = 0; DAT_00501458 = 1; DAT_0050145c = 0; DAT_00501473 = 0;
                FUN_00433bc0(this); break;
            case 2:
                FUN_004a4870("data/settings/game.ini"); DAT_00501472 = 0; DAT_00501458 = 0; DAT_0050145c = 1; DAT_00501473 = 0;
                FUN_00434a50(this); break;
            case 3: FUN_00434df0(this); break;
            case 4: FUN_00432b00(this); FUN_00425e50(this); FUN_00432b30(this, 0, 0); break;
            case 5: if (DAT_00508bdc) FUN_00434ba0(); else goto G2_cleanup; break;
            case 6: FUN_00434dd0(this); break;
            }
        }
    }
G2_cleanup:
    FUN_00432b00(this);
    ((RecordTable*)((unsigned char*)this + 0x5c30))->FUN_00490820("data/highscores/highscores.hs");
    FUN_004327d0(this);
    FUN_00431c50();
    FUN_00430eb0();
}

// FUNCTION: RALLYTROPHY 0x430eb0
void Game::FUN_00430eb0() {
    G2_GameView* view = (G2_GameView*)this;
    void* p;
    p = view->textRenderers[0]; if (p) { FUN_004a95d0(p); ::operator delete(p); }
    p = view->textRenderers[1]; if (p) { FUN_004a95d0(p); ::operator delete(p); }
    p = view->textRenderers[2]; if (p) { FUN_004a95d0(p); ::operator delete(p); }
    p = view->textRenderers[3]; if (p) { FUN_004a95d0(p); ::operator delete(p); }
    p = view->textRenderers[4]; if (p) { FUN_004a95d0(p); ::operator delete(p); }
    view->textRenderers[0] = 0;
    view->textRenderers[1] = 0;
    view->textRenderers[2] = 0;
    view->textRenderers[3] = 0;
    view->textRenderers[4] = 0;
}

// ---- Group G3 ----

class G3_Game {
public:
    void FUN_00430f60();               // 0x430f60: build the front-end UI resources and labels (once)
    void FUN_00434e00();               // 0x434e00
    // Embedded screens (types in the map); only their Setup slot is called here.
    BYTE unk0[0x11a4];
    BYTE screen11a4[0x1378 - 0x11a4];  // 0x11a4: ctor 0x427630
    BYTE screen1378[0x14cc - 0x1378];  // 0x1378: Screen1378
    BYTE playerName[0x1e9c - 0x14cc];  // 0x14cc: PlayerNameMenu
    BYTE screen1e9c[0x2008 - 0x1e9c];  // 0x1e9c: ctor 0x42c2b0
    BYTE slotMenu[0x24fc - 0x2008];    // 0x2008: SlotMenu
    BYTE screen24fc[0x29e4 - 0x24fc];  // 0x24fc: ctor 0x42e930
    BYTE replayMenu[0x2dd4 - 0x29e4];  // 0x29e4: ReplayMenu
    BYTE screen2dd4[0x2df4 - 0x2dd4];  // 0x2dd4: ctor 0x4266c0
    BYTE menuManager[0x3aec - 0x2df4]; // 0x2df4: MenuManager
    BYTE unk3aec[0x13907c - 0x3aec];
    int uiBuilt;                       // 0x13907c: 1 once FUN_00430f60 has run
    BYTE unk139080[0x1390c4 - 0x139080];
    TextRenderer* smallText;           // 0x1390c4
    BYTE unk1390c8[0x1390d8 - 0x1390c8];
    Sprite* cursor;                    // 0x1390d8: "MouseCursor"
    Sprite* lineMain;                  // 0x1390dc
    Sprite* lineQuit;                  // 0x1390e0
    Sprite* lineSelect;                // 0x1390e4
    Sprite* lineLong;                  // 0x1390e8
    Sprite* lineBox;                   // 0x1390ec
    Sprite* lineLoadSave;              // 0x1390f0
    BYTE unk1390f4[0x139108 - 0x1390f4];
    OverlayImage* background;          // 0x139108: data/menu/ImageMainMenu.tga
    BinkVideo* video;                  // 0x13910c: data/video/menu<1..11>.bik
    MenuLabel labels[7];               // 0x139110: texts 0x29, 0x2a, 0x2c, 0x2d, 0x19e, 0x2f, 0x30
    BYTE unk13920c[0x139890 - 0x13920c];
    int unk139890;                     // 0x139890
};

// Label layout constants (separate .rdata objects; the original multiplies const * scale).
static float G3_firstY = 262.0f;         // 0x4f75d8
static float G3_step = 42.0f;            // 0x4f75dc
static float G3_textX = 54.0f;           // 0x4f75e0
static float G3_backY = 610.0f;          // 0x4f75ec

static inline void G3_Style(Sprite* s)
{
    s->unk9c = 2;
    s->unk98 = 15;
    s->unk18 = 5;
    s->unk60[0] = 1.0f;
    s->unk60[1] = 1.0f;
    s->unk60[2] = 1.0f;
    s->unk60[3] = 1.0f;
    s->unk1c = 6;
    s->unk40[0] = 1.0f;
    s->unk40[1] = 1.0f;
    s->unk40[2] = 1.0f;
    s->unk40[3] = 1.0f;
    s->unk24 = 2;
}

static inline void G3_Setup(BYTE* screen, G3_Game* game)
{
    ((MenuScreen*)screen)->Setup((MenuGame*)game);
}

// std::string (same layout) with each inline expansion written out as the original has it: VC6's inline budget leaves
// a different mix of _Grow/_Eos/_Tidy calls at every site of this long function.
class G3_String {
public:
    G3_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        ptr = 0;
        len = 0;
        res = 0;
        unsigned int n = strlen(s);
        if (GrowInline(n, true)) {
            memcpy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    ~G3_String()
    {
        if (ptr != 0) {
            if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
                alloc.deallocate(ptr - 1, res + 2);
            } else {
                --Refcnt(ptr);
            }
        }
    }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    unsigned int MaxSize() const;      // 0x402aa0
    void Copy(unsigned int n);         // 0x402ab0
    // _Grow expanded inline, max_size() and _Tidy/_Copy as calls.
    __forceinline bool GrowInline(unsigned int n, bool trim)
    {
        if (MaxSize() < n) {
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
            if (trim && (31 < res || res < n)) {
                Tidy(true);
                Copy(n);
            } else if (!trim && res < n) {
                Copy(n);
            }
            return true;
        }
    }
    // assign(const char*) with _Grow as a call and _Eos inline (the first five labels).
    __forceinline void AssignA(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            std::char_traits<char>::assign(ptr[len = n], 0);
        }
    }
    // assign(const char*) with _Grow inline and _Eos as a call (the last two labels).
    __forceinline void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (GrowInline(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    // _Grow with max_size() folded, as append's inline expansion has it; _Eos(0) as a call or inline.
    __forceinline bool GrowConst(unsigned int n, bool trim, bool eosCall)
    {
        if (0xfffffffd < n) {
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
                if (eosCall) {
                    Eos(0);
                } else {
                    ptr[len = 0] = 0;
                }
            }
            return false;
        } else {
            if (trim && (31 < res || res < n)) {
                Tidy(true);
                Copy(n);
            } else if (!trim && res < n) {
                Copy(n);
            }
            return true;
        }
    }
    // append(const char*) expanded inline.
    __forceinline void Append(const char* s, bool eosCall)
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && GrowConst(n = len + m, false, eosCall)) {
            std::char_traits<char>::copy(ptr + len, s, m);
            std::char_traits<char>::assign(ptr[len = n], 0);
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
#define G3_TEXT(i) ((G3_String*)&labels[i].text)
#define G3_ASSIGN_A(i, text) \
    do { \
        const char* s_ = text; \
        unsigned int n_ = strlen(s_); \
        if (G3_TEXT(i)->Grow(n_, true)) { \
            std::char_traits<char>::copy(G3_TEXT(i)->ptr, s_, n_); \
            std::char_traits<char>::assign(G3_TEXT(i)->ptr[G3_TEXT(i)->len = n_], 0); \
        } \
    } while (0)

// NONMATCHING: RALLYTROPHY 0x430f60
// 3301 vs 3300 bytes; every string expansion, call and EH state matches. VC6 swaps ebx/ebp from 0x43133d on (`this` in
// ebx, the original ebp), costing the bl store in .bik's _Eos(0), and the four line sprites' field stores don't interleave.
// Builds the front-end once: background image, line sprites, cursor, the seven main-menu labels, the embedded
// screens' Setup, and a random background video data/video/menu<1..11>.bik.
void G3_Game::FUN_00430f60()
{
    if (uiBuilt != 0) {
        return;
    }
    float xs = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    uiBuilt = 1;
    float ys = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    background = new OverlayImage(DAT_0056b0ac, "data/menu/ImageMainMenu.tga", 1, 1);
    background->SetGrid(5, 6);
    Sprite* main = lineMain = new Sprite("linemain");
    Sprite* quit = lineQuit = new Sprite("linequit");
    Sprite* select = lineSelect = new Sprite("lineselect");
    Sprite* line = lineLong = new Sprite("linelong");
    Texture* t1 = new Texture("data/menu/linemain.tga", 0x841, 1);
    main->texture = t1;
    Texture* t2 = new Texture("data/menu/linequit.tga", 0x841, 1);
    quit->texture = t2;
    Texture* t3 = new Texture("data/menu/lineselect.tga", 0x841, 1);
    select->texture = t3;
    Texture* t4 = new Texture("data/menu/linelong.tga", 0x841, 1);
    line->texture = t4;
    t1->Upload(DAT_0056b0ac);
    t2->Upload(DAT_0056b0ac);
    t3->Upload(DAT_0056b0ac);
    t4->Upload(DAT_0056b0ac);
    G3_Style(main);
    G3_Style(quit);
    G3_Style(select);
    G3_Style(line);
    Sprite* s = lineBox = new Sprite();
    Texture* t = s->texture = new Texture("data/menu/linebox.tga", 0x841, 0);
    t->Upload(DAT_0056b0ac);
    G3_Style(s);
    s = lineLoadSave = new Sprite();
    t = s->texture = new Texture("data/menu/lineloadsave.tga", 0x841, 0);
    t->Upload(DAT_0056b0ac);
    G3_Style(s);
    s = cursor = new Sprite("MouseCursor");
    t = s->texture = new Texture("data/menu/MouseCursor.tga", 0x41, 1);
    t->Upload(DAT_0056b0ac);
    G3_Style(s);
    G3_TEXT(0)->AssignA(FUN_00405380(0x29));
    G3_TEXT(1)->AssignA(FUN_00405380(0x2a));
    G3_TEXT(2)->AssignA(FUN_00405380(0x2c));
    G3_TEXT(3)->AssignA(FUN_00405380(0x2d));
    G3_TEXT(4)->AssignA(FUN_00405380(0x19e));
    G3_TEXT(5)->AssignB(FUN_00405380(0x2f));
    G3_TEXT(6)->AssignB(FUN_00405380(0x30));
    float y = G3_firstY * ys;
    for (int i = 0; i < 7; i++) {
        labels[i].unk20 = -1;
        labels[i].height = (float)smallText->lineHeight;
        labels[i].width = smallText->GetWidth(labels[i].text.c_str(), 1);
        labels[i].SetX(G3_textX * xs);
        labels[i].y = y;
        y += G3_step * ys;
    }
    labels[6].y = G3_backY * ys;
    G3_Setup(menuManager, this);
    if (!DAT_00501454) {
        G3_Setup(screen2dd4, this);
    }
    G3_Setup(screen1378, this);
    G3_Setup(playerName, this);
    if (!DAT_00501454) {
        G3_Setup(screen11a4, this);
        G3_Setup(screen1e9c, this);
        G3_Setup(screen24fc, this);
        G3_Setup(replayMenu, this);
        G3_Setup(slotMenu, this);
    }
    G3_String name("data/video/menu");
    char number[8];
    itoa(rand() % 11 + 1, number, 10);
    name.Append(number, true);
    name.Append(".bik", false);
    video = new BinkVideo(name.c_str(), DAT_0056b0ac, 0);
    unk139890 = 0;
    FUN_00434e00();
}

// ---- Group G4 ----

// One of the 32 sound-effect records loaded from data/soundfx/3dsounds.ini (0x20 bytes).
struct SfxVec2 {
    float x, y;
};
struct SfxRecord {
    std::string file;                  // 0x00: "SfxNNFile"
    float height;                      // 0x10: "SfxNNHeight"
    int volume;                        // 0x14: "SfxNNVolume"
    ConfigVector2 attenuation;         // 0x18: "SfxNNAttenuation"
};

class G4_Game {
public:
    void ReleaseResources();           // 0x431c50
    void CreateResources();            // 0x431f40

    BYTE unk0[0x13907c];               // 0x0: embedded screens (ServiceMenu at +4, ... see range-4303e0.md)
    int screensActive;                 // 0x13907c
    BYTE unk139080[0x1390d4 - 0x139080];
    Sprite* menuItems;                 // 0x1390d4 (data/menu/MenuItems.tga)
    Sprite* sprite1390d8;              // 0x1390d8
    Sprite* sprite1390dc;              // 0x1390dc
    Sprite* sprite1390e0;
    Sprite* sprite1390e4;
    Sprite* sprite1390e8;
    Sprite* sprite1390ec;
    Sprite* sprite1390f0;              // 0x1390f0
    Sprite* sureBox;                   // 0x1390f4 (data/menu/surebox.tga)
    Sprite* flags;                     // 0x1390f8 (data/extras/Flags.tga)
    Sprite* indicators;                // 0x1390fc (overlayIndicators.tga)
    Sprite* gearFonts;                 // 0x139100 (overlayGearFonts.tga)
    Sprite* meters;                    // 0x139104 (overlayMetersDis.tga)
    LoadingScreen* image139108;        // 0x139108
    BinkVideo* video13910c;            // 0x13910c
    BYTE unk139110[0x139224 - 0x139110];
    BYTE sounds[4 * 0x70];             // 0x139224: SoundSource[4] (move, select, back, limit)
    BYTE unk1393e4[0x139460 - 0x1393e4];
    std::vector<UIRect*> rects;        // 0x139460
    UIRect* rect139470;                // 0x139470
    UIRect* rect139474;                // 0x139474
    BYTE unk139478[0x139488 - 0x139478];
    Sprite* paceNotes;                 // 0x139488 (data/global/overlay/pacenotes.tga)
    BYTE unk13948c[4];
    SfxRecord sfx[32];                 // 0x139490
    BYTE unk139890[4];
};

// A point whose copy constructor is out of line (0x424300), and the HUD rectangle taking it by value (0x4390a0).
struct G4_Point {
    G4_Point() {}
    G4_Point(const G4_Point& other);   // 0x424300
    ~G4_Point() {}
    float x, y;
};
class G4_Rect {
public:
    G4_Rect(G4_Point upperLeft, HudPoint lowerRight); // 0x4390a0
    int fields[5];
};

// std::string::assign(const char*) as the original expands it: _Grow inline with max_size() and _Tidy() as calls,
// _Eos expanded. The view has the layout of std::string (allocator, _Ptr, _Len, _Res).
struct G4_Str {
    BYTE alloc[4];
    char* ptr;
    unsigned int len;
    unsigned int res;
    void Tidy(bool built);             // 0x402700
    unsigned int MaxSize() const;      // 0x402aa0
    void Copy(unsigned int n);         // 0x402ab0
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (MaxSize() < n) {
            std::_Xlen();
        }
        bool ok;
        if (ptr != 0 && Refcnt(ptr) != 0 && Refcnt(ptr) != 255) {
            if (n == 0) {
                --Refcnt(ptr), Tidy(false);
                ok = false;
            } else {
                Copy(n);
                ok = true;
            }
        } else if (n == 0) {
            Tidy(true);
            ok = false;
        } else {
            if (31 < res || res < n) {
                Tidy(true);
                Copy(n);
            }
            ok = true;
        }
        if (ok) {
            std::char_traits<char>::copy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
};

extern ConfigKey DAT_004f76e8[];       // 3dsounds.ini keys (128 entries)
// The key names are patched in place with the record number (two digits at offsets 3 and 4).
static char G4_fileKey[] = "Sfx00File";
static char G4_heightKey[] = "Sfx00Height";
static char G4_volumeKey[] = "Sfx00Volume";
static char G4_attenuationKey[] = "Sfx00Attenuation";

// Sets all four components of a colour/scale quad of a sprite (inline in the original: four consecutive stores).
static inline void G4_Quad(float* v, float f)
{
    v[0] = f;
    v[1] = f;
    v[2] = f;
    v[3] = f;
}

#define G4_SCREEN(offset) ((MenuScreen*)((BYTE*)this + (offset)))

// FUNCTION: RALLYTROPHY 0x431c50
void G4_Game::ReleaseResources()
{
    if (screensActive) {
        screensActive = 0;
        G4_SCREEN(0x2df4)->Release();
        if (!DAT_00501454) {
            G4_SCREEN(0x2dd4)->Release();
        }
        G4_SCREEN(0x1378)->Release();
        G4_SCREEN(0x14cc)->Release();
        if (!DAT_00501454) {
            G4_SCREEN(0x11a4)->Release();
            G4_SCREEN(0x1e9c)->Release();
            G4_SCREEN(0x24fc)->Release();
            G4_SCREEN(0x29e4)->Release();
            G4_SCREEN(0x2008)->Release();
        }
        if (sprite1390d8) {
            delete sprite1390d8->texture;
        }
        delete sprite1390d8;
        delete image139108;
        delete video13910c;
        if (sprite1390dc) {
            delete sprite1390dc->texture;
        }
        if (sprite1390e0) {
            delete sprite1390e0->texture;
        }
        if (sprite1390e4) {
            delete sprite1390e4->texture;
        }
        if (sprite1390e8) {
            delete sprite1390e8->texture;
        }
        Sprite* a = sprite1390ec;
        if (a) {
            delete a->texture;
        }
        Sprite* b = sprite1390f0;
        if (b) {
            delete b->texture;
        }
        delete sprite1390dc;
        delete sprite1390e0;
        delete sprite1390e4;
        delete sprite1390e8;
        delete a;
        delete b;
        sprite1390dc = 0;
        sprite1390e0 = 0;
        sprite1390e4 = 0;
        sprite1390e8 = 0;
        sprite1390ec = 0;
        sprite1390f0 = 0;
        image139108 = 0;
        video13910c = 0;
        for (unsigned int i = 0; i < rects.size(); i++) {
            UIRect* rect = rects[i];
            delete rect;
        }
        rects.resize(0);
    }
}

// FUNCTION: RALLYTROPHY 0x431f40
// Keep the record alias local to each iteration to reproduce the original instruction schedule.
void G4_Game::CreateResources()
{
    menuItems = new Sprite("MenuItems");
    menuItems->texture = new Texture("data/menu/MenuItems.tga", 0x41, 1);
    menuItems->texture->Upload(DAT_0056b0ac);
    menuItems->unk9c = 2;
    menuItems->unk98 = 0xf;
    menuItems->unk18 = 5;
    G4_Quad(menuItems->unk60, 1.0f);
    menuItems->unk1c = 6;
    G4_Quad(menuItems->unk40, 1.0f);
    menuItems->unk24 = 2;

    Sprite* pace = new Sprite();
    pace->texture = new Texture("data/global/overlay/pacenotes.tga", 0x41, 1);
    pace->texture->Upload(DAT_0056b0ac);
    G4_Quad(pace->unk60, 1.0f);
    pace->unk9c = 2;
    pace->unk98 = 0xf;
    G4_Quad(pace->unk40, 1.0f);
    pace->unk18 = 5;
    pace->unk1c = 6;
    pace->unk24 = 4;
    paceNotes = pace;

    flags = new Sprite();
    flags->texture = new Texture("data/extras/Flags.tga", 0x45, 1);
    flags->texture->Upload(DAT_0056b0ac);
    flags->unk9c = 2;
    flags->unk98 = 0xf;
    flags->unk18 = 5;
    G4_Quad(flags->unk60, 1.0f);
    flags->unk1c = 6;
    G4_Quad(flags->unk40, 1.0f);
    flags->unk24 = 2;

    sureBox = new Sprite();
    sureBox->texture = new Texture("data/menu/surebox.tga", 0x841, 1);
    sureBox->texture->Upload(DAT_0056b0ac);
    sureBox->unk9c = 2;
    sureBox->unk98 = 0xf;
    sureBox->unk18 = 5;
    G4_Quad(sureBox->unk60, 1.0f);
    sureBox->unk1c = 6;
    G4_Quad(sureBox->unk40, 1.0f);
    sureBox->unk24 = 2;

    G4_Point p1;
    HudPoint p2;
    p1.x = 420.0f;
    p2.x = 512.0f;
    p1.y = 385.0f;
    p2.y = 427.0f;
    rect139470 = (UIRect*)new G4_Rect(p1, p2);
    p1.x = 512.0f;
    p2.x = 604.0f;
    rect139474 = (UIRect*)new G4_Rect(p1, p2);

    Config config;
    config.Define(DAT_004f76e8, 0x80);
    config.Load("data/soundfx/3dsounds.ini", true, false, 0);
    for (int i = 0; i < 32; i++) {
        SfxRecord& record = sfx[i];
        int tens = '0' + i / 10;
        int ones = '0' + i % 10;
        G4_fileKey[3] = tens;
        G4_heightKey[3] = tens;
        G4_volumeKey[3] = tens;
        G4_attenuationKey[3] = tens;
        G4_fileKey[4] = ones;
        G4_heightKey[4] = ones;
        G4_volumeKey[4] = ones;
        G4_attenuationKey[4] = ones;
        ((G4_Str*)&record.file)->Assign(config.GetString(G4_fileKey));
        record.height = config.GetFloat(G4_heightKey);
        record.volume = config.GetInt(G4_volumeKey);
        const ConfigVector2& att = config.GetVector2(G4_attenuationKey);
        record.attenuation.values[0] = att.values[0];
        record.attenuation.values[1] = att.values[1];
    }
    config.Close();

    SoundSource* source = (SoundSource*)&sounds[0];
    source->SetSample(DAT_0056b0b0->LoadSample("data/soundfx/click.wav"));
    source->SetVolume(0);
    source = (SoundSource*)&sounds[0x70];
    source->SetSample(DAT_0056b0b0->LoadSample("data/soundfx/clickselect.wav"));
    source->SetVolume(0);
    source = (SoundSource*)&sounds[0xe0];
    source->SetSample(DAT_0056b0b0->LoadSample("data/soundfx/clickback.wav"));
    source->SetVolume(0);
    source = (SoundSource*)&sounds[0x150];
    source->SetSample(DAT_0056b0b0->LoadSample("data/soundfx/clickillegal.wav"));
    source->SetVolume(0);

    G4_SCREEN(0x3aec)->Setup((MenuGame*)this);
    G4_SCREEN(0x59e4)->Setup((MenuGame*)this);

    meters = new Sprite();
    meters->texture = new Texture("data/global/overlay/overlayMetersDis.tga", 0x45, 1);
    meters->texture->Upload(DAT_0056b0ac);
    meters->unk18 = 5;
    meters->unk1c = 6;
    meters->unk20 = 2;
    meters->unk24 = 2;
    meters->unk98 = 0xf;
    G4_Quad(meters->unk60, 1.0f);
    G4_Quad(meters->unk40, 1.0f);
    meters->unk9c = 2;

    indicators = new Sprite();
    indicators->texture = new Texture("data/global/overlay/overlayIndicators.tga", 0x45, 1);
    indicators->texture->Upload(DAT_0056b0ac);
    indicators->unk18 = 5;
    indicators->unk1c = 6;
    indicators->unk20 = 2;
    indicators->unk24 = 2;
    indicators->unk98 = 0xf;
    G4_Quad(indicators->unk60, 0.0f);
    G4_Quad(indicators->unk40, 1.0f);
    indicators->unk9c = 2;

    gearFonts = new Sprite();
    gearFonts->texture = new Texture("data/global/overlay/overlayGearFonts.tga", 0x45, 1);
    gearFonts->texture->Upload(DAT_0056b0ac);
    gearFonts->unk18 = 5;
    gearFonts->unk1c = 6;
    gearFonts->unk20 = 2;
    gearFonts->unk24 = 2;
    gearFonts->unk98 = 0xf;
    G4_Quad(gearFonts->unk60, 0.0f);
    G4_Quad(gearFonts->unk40, 1.0f);
    gearFonts->unk9c = 2;

    ((ServiceMenu*)((BYTE*)this + 4))->LoadTextures((MenuGame*)this);
}

// ---- Group G5 ----

extern int DAT_00501444, DAT_00501448;
extern const char* DAT_004f7af0[];
extern int DAT_004f7ae8[], DAT_004f7aec[];
extern int DAT_0050143c;
class G5_Releaseable { public: virtual void Release(int); };

// The raw view does not yet have the complete resource-wrapper declarations.
extern "C" void __cdecl RT_BaseDtor4327d0();
#pragma comment(linker, "/alternatename:_RT_BaseDtor4327d0=?DestroyStringExact@ModelResourceState@@QAEXXZ")
extern "C" void __cdecl RT_Delete4327d0();
#pragma comment(linker, "/alternatename:_RT_Delete4327d0=??3@YAXPAX@Z")
extern "C" void __cdecl RT_Stop4327d0();
#pragma comment(linker, "/alternatename:_RT_Stop4327d0=?Stop@T_56b0b0@@QAEXPAUSoundSource@@@Z")
extern "C" void __cdecl RT_ReleaseSample4327d0();
#pragma comment(linker, "/alternatename:_RT_ReleaseSample4327d0=?ReleaseSampleExact@T_56b0b0@@QAEPAVSample@@PAV2@@Z")
extern "C" void __cdecl RT_Lock4327d0();
#pragma comment(linker, "/alternatename:_RT_Lock4327d0=?Lock@RaceMode@@UAEXXZ")
extern "C" void __cdecl RT_UnloadService4327d0();
#pragma comment(linker, "/alternatename:_RT_UnloadService4327d0=?UnloadTextures@ServiceMenu@@QAEXXZ")
extern "C" void __fastcall FUN_004327d0(Game*);
#pragma comment(linker, "/alternatename:?FUN_004327d0@G9_Game@@QAEXXZ=@FUN_004327d0@4")

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4327d0
// SYMBOL: @FUN_004327d0@4
// PE instruction transcription; preserve resource teardown, sound cleanup and call order.
void __fastcall FUN_004327d0(Game* self)
{
    __asm {
        push   ebx
        push   ebp
        push   esi
        mov    esi,ecx
        push   edi
        mov    edi,dword ptr [esi+01390d4h]
        test   edi,edi
        je game_release_432800
        mov    ecx,dword ptr [edi]
        test   ecx,ecx
        je game_release_4327ec
        mov    eax,dword ptr [ecx]
        push   01h
        call   dword ptr [eax]
    game_release_4327ec:
        test   edi,edi
        je game_release_432800
        mov    ecx,edi
        call RT_BaseDtor4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_432800:
        mov    edi,dword ptr [esi+01390f8h]
        test   edi,edi
        mov    dword ptr [esi+01390d4h],00h
        je game_release_432834
        mov    ecx,dword ptr [edi]
        test   ecx,ecx
        je game_release_432820
        mov    edx,dword ptr [ecx]
        push   01h
        call   dword ptr [edx]
    game_release_432820:
        test   edi,edi
        je game_release_432834
        mov    ecx,edi
        call RT_BaseDtor4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_432834:
        mov    edi,dword ptr [esi+01390f4h]
        test   edi,edi
        je game_release_43285e
        mov    ecx,dword ptr [edi]
        test   ecx,ecx
        je game_release_43284a
        mov    eax,dword ptr [ecx]
        push   01h
        call   dword ptr [eax]
    game_release_43284a:
        test   edi,edi
        je game_release_43285e
        mov    ecx,edi
        call RT_BaseDtor4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_43285e:
        mov    edi,dword ptr [esi+0139488h]
        mov    ecx,dword ptr [edi]
        test   ecx,ecx
        je game_release_432870
        mov    edx,dword ptr [ecx]
        push   01h
        call   dword ptr [edx]
    game_release_432870:
        test   edi,edi
        je game_release_432884
        mov    ecx,edi
        call RT_BaseDtor4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_432884:
        mov    edi,dword ptr [esi+0139470h]
        test   edi,edi
        je game_release_43289e
        mov    ecx,edi
        call RT_Lock4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_43289e:
        mov    edi,dword ptr [esi+0139474h]
        test   edi,edi
        je game_release_4328b8
        mov    ecx,edi
        call RT_Lock4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_4328b8:
        mov    edi,dword ptr [esi+0139104h]
        mov    ecx,dword ptr [edi]
        test   ecx,ecx
        je game_release_4328ca
        mov    eax,dword ptr [ecx]
        push   01h
        call   dword ptr [eax]
    game_release_4328ca:
        test   edi,edi
        je game_release_4328de
        mov    ecx,edi
        call RT_BaseDtor4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_4328de:
        mov    edi,dword ptr [esi+01390fch]
        mov    ecx,dword ptr [edi]
        test   ecx,ecx
        je game_release_4328f0
        mov    edx,dword ptr [ecx]
        push   01h
        call   dword ptr [edx]
    game_release_4328f0:
        test   edi,edi
        je game_release_432904
        mov    ecx,edi
        call RT_BaseDtor4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_432904:
        mov    edi,dword ptr [esi+0139100h]
        mov    ecx,dword ptr [edi]
        test   ecx,ecx
        je game_release_432916
        mov    eax,dword ptr [ecx]
        push   01h
        call   dword ptr [eax]
    game_release_432916:
        test   edi,edi
        je game_release_43292a
        mov    ecx,edi
        call RT_BaseDtor4327d0
        push   edi
        call RT_Delete4327d0
        add    esp,04h
    game_release_43292a:
        mov    ecx,dword ptr [DAT_0056b0b0]
        lea    edi,[esi+0139224h]
        push   edi
        call RT_Stop4327d0
        mov    ecx,dword ptr [DAT_0056b0b0]
        lea    ebx,[esi+0139294h]
        push   ebx
        call RT_Stop4327d0
        mov    ecx,dword ptr [DAT_0056b0b0]
        lea    ebp,[esi+0139304h]
        push   ebp
        call RT_Stop4327d0
        mov    ecx,dword ptr [DAT_0056b0b0]
        lea    eax,[esi+0139374h]
        push   eax
        call RT_Stop4327d0
        mov    ecx,dword ptr [edi]
        push   ecx
        mov    ecx,dword ptr [DAT_0056b0b0]
        call RT_ReleaseSample4327d0
        mov    edx,dword ptr [ebx]
        mov    ecx,dword ptr [DAT_0056b0b0]
        push   edx
        call RT_ReleaseSample4327d0
        mov    eax,dword ptr [ebp+00h]
        mov    ecx,dword ptr [DAT_0056b0b0]
        push   eax
        call RT_ReleaseSample4327d0
        mov    ecx,dword ptr [esi+0139374h]
        push   ecx
        mov    ecx,dword ptr [DAT_0056b0b0]
        call RT_ReleaseSample4327d0
        mov    edx,dword ptr [esi+03aech]
        lea    ecx,[esi+03aech]
        call   dword ptr [edx+08h]
        mov    eax,dword ptr [esi+059e4h]
        lea    ecx,[esi+059e4h]
        call   dword ptr [eax+08h]
        lea    ecx,[esi+04h]
        call RT_UnloadService4327d0
        pop    edi
        pop    esi
        pop    ebp
        pop    ebx
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x4329e0
HudPoint* MenuGame::GetScale()
{
    BYTE* p = (BYTE*)this;
    *(DWORD*)(p + 0x1390cc) = 0x3f800000;
    *(DWORD*)(p + 0x1390d0) = 0x3f800000;
    return (HudPoint*)(p + 0x1390cc);
}

// FUNCTION: RALLYTROPHY 0x432a00
void* GameView::FUN_00432a00() { return *(void**)((BYTE*)this + 0x1390d4); }

// FUNCTION: RALLYTROPHY 0x432a10
void MenuGame::UpdateVideo()
{
    BYTE* p = (BYTE*)this;
    if (*(int*)(p + 0x139890) == 0 && *(void**)(p + 0x13910c) != 0 &&
        ((BinkVideo*)*(void**)(p + 0x13910c))->Update() == -1)
        *(int*)(p + 0x139890) = 1;
}

// FUNCTION: RALLYTROPHY 0x432a40
void MenuGame::FUN_00432a40()
{
    void* video = *(void**)((BYTE*)this + 0x13910c);
    if (video) ((BinkVideo*)video)->Render(DAT_0056b0ac);
}

// FUNCTION: RALLYTROPHY 0x432a60
void MenuGame::DrawVideo(volatile int flag)
{
    int f = flag;
    int a = 2, b = 1;
    if (f) { a = 5; b = 6; }
    OverlayImage* renderer = *(OverlayImage**)((BYTE*)this + 0x139108);
    renderer->SetGrid(a, b);
    renderer->Draw(DAT_0056b0ac, 0);
}

// FUNCTION: RALLYTROPHY 0x432aa0
void __fastcall G5_beginFrame(GameView* self)
{
    BYTE* p = (BYTE*)self;
    int zero = 0;
    *(int*)(p + 0x139214) = zero;
    *(int*)(p + 0x139218) = zero;
    *(int*)(p + 0x13920c) = zero;
    *(int*)(p + 0x139210) = (*(int (__cdecl **)())0x4e11d8)();
    *(int*)(p + 0x13921c) = zero;
    if (DAT_0056b0b0 != 0) {
        DAT_0056b0b0->OpenStream(DAT_004f7af0[0]);
        DAT_0056b0b0->StartStream();
        DAT_0056b0b0->SetStreamVolume(0);
    }
}

// FUNCTION: RALLYTROPHY 0x432b00
void __fastcall G5_endFrame(GameView* self)
{
    if (DAT_0056b0b0) {
        DAT_0056b0b0->CloseStream();
        *(int*)((BYTE*)self + 0x139214) = -1;
        *(int*)((BYTE*)self + 0x139218) = -1;
    }
}

// original: FUN_00432b30
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x432b30
void GameView::FUN_00432b30(int mode, int value)
{
    __asm {
        push edi
        mov edi, ecx
        cmp dword ptr [edi + 0x139214], -1
        je game_stream_switch
        mov eax, dword ptr [esp + 0xc]
        test eax, eax
        je game_stream_switch
        call dword ptr [DAT_0059b2f8]
        mov dword ptr [edi + 0x139210], eax
        mov eax, dword ptr [esp + 8]
        mov dword ptr [edi + 0x13920c], 1
        mov dword ptr [edi + 0x139218], eax
        pop edi
        ret 8
    game_stream_switch:
        mov ecx, dword ptr [DAT_0056b0b0]
        push ebx
        push esi
        call fun_0049a550
        mov esi, dword ptr [esp + 0x10]
        mov ecx, esi
        shl ecx, 4
        mov edx, dword ptr [ecx + DAT_004f7af0]
        mov ecx, dword ptr [DAT_0056b0b0]
        push edx
        call fun_0049a4f0
        mov ecx, dword ptr [DAT_0056b0b0]
        call fun_0049a580
        mov eax, dword ptr [DAT_00501444]
        mov ecx, eax
        shl ecx, 7
        sub ecx, eax
        mov eax, 0x51eb851f
        imul ecx
        sar edx, 5
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        test esi, esi
        mov dword ptr [edi + 0x139214], esi
        mov dword ptr [edi + 0x139218], esi
        mov dword ptr [edi + 0x13920c], 0
        mov ebx, edx
        je game_stream_alt_volume
        cmp esi, 3
        je game_stream_alt_volume
        cmp esi, 2
        jne game_stream_set_volume
    game_stream_alt_volume:
        mov ebx, dword ptr [DAT_00501448]
    game_stream_set_volume:
        mov ecx, dword ptr [DAT_0056b0b0]
        push ebx
        call fun_0049a5a0
        pop esi
        mov dword ptr [edi + 0x13921c], ebx
        pop ebx
        pop edi
        ret 8
    }
}

extern "C" void __cdecl RT_SetVolume432c00();
#pragma comment(linker, "/alternatename:_RT_SetVolume432c00=?SetVolume@SoundSource@@QAEXH@Z")

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x432c00
void MenuGame::Update()
{
    __asm {
        push ecx
        push ebx
        push ebp
        push esi
        mov esi, ecx
        mov ebx, dword ptr [esi + 0x13920c]
        test ebx, ebx
        push edi
        mov edi, 0x7f
        jz game_update_fade_volume
        call dword ptr [DAT_0059b2f8]
        cmp ebx, 1
        mov ebp, eax
        mov dword ptr [esp + 0x10], ebp
        jnz game_update_transition
        mov ecx, dword ptr [esi + 0x139210]
        sub eax, ecx
        cmp eax, 0x1f4
        jl game_update_fade_out
        mov ebx, dword ptr [esi + 0x139218]
        add ecx, 0x1f4
        mov dword ptr [esi + 0x139210], ecx
        mov ecx, dword ptr [DAT_0056b0b0]
        xor edi, edi
        test ecx, ecx
        mov dword ptr [esi + 0x13920c], 2
        mov dword ptr [esi + 0x139214], ebx
        jz game_update_transition
        shl ebx, 4
        mov ebp, dword ptr [ebx + DAT_004f7aec]
        call fun_0049a550
        mov ecx, dword ptr [DAT_0056b0b0]
        push edi
        call fun_0049a5a0
        mov eax, dword ptr [esi + 0x139218]
        lea eax, [ebp + eax*4]
        mov ecx, dword ptr [eax*4 + DAT_004f7af0]
        push ecx
        mov ecx, dword ptr [DAT_0056b0b0]
        call fun_0049a4f0
        mov ecx, dword ptr [DAT_0056b0b0]
        call fun_0049a580
        mov eax, dword ptr [ebx + DAT_004f7ae8]
        inc ebp
        cmp ebp, eax
        jl game_update_frame_store
        xor ebp, ebp
    game_update_frame_store:
        mov dword ptr [ebx + DAT_004f7aec], ebp
        mov ebp, dword ptr [esp + 0x10]
        jmp game_update_transition
    game_update_fade_out:
        mov ecx, eax
        shl ecx, 7
        sub ecx, eax
        mov eax, 0x10624dd3
        imul ecx
        sar edx, 5
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        mov edi, 0x7f
        sub edi, edx
    game_update_transition:
        cmp dword ptr [esi + 0x13920c], 2
        jnz game_update_fade_volume
        sub ebp, dword ptr [esi + 0x139210]
        mov eax, ebp
        cmp eax, 0x1f4
        jl game_update_fade_in
        mov edi, 0x7f
        mov dword ptr [esi + 0x13920c], 0
        jmp game_update_fade_volume
    game_update_fade_in:
        mov ecx, eax
        shl ecx, 7
        sub ecx, eax
        mov eax, 0x10624dd3
        imul ecx
        sar edx, 5
        mov ecx, edx
        shr ecx, 0x1f
        add edx, ecx
        mov edi, edx
    game_update_fade_volume:
        mov ecx, dword ptr [esi + 0x139214]
        test ecx, ecx
        jz game_update_alt_volume
        mov eax, dword ptr [esi + 0x139218]
        test eax, eax
        jz game_update_alt_volume
        cmp ecx, 3
        jz game_update_alt_volume
        cmp eax, 3
        jz game_update_alt_volume
        cmp ecx, 2
        jz game_update_alt_volume
        cmp eax, 2
        jz game_update_alt_volume
        mov ecx, dword ptr [DAT_00501444]
        imul ecx, edi
        mov eax, 0x51eb851f
        imul ecx
        sar edx, 5
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        jmp game_update_volume_ready
    game_update_alt_volume:
        mov ecx, dword ptr [DAT_00501448]
        imul ecx, edi
        mov eax, 0x51eb851f
        imul ecx
        sar edx, 5
        mov ecx, edx
        shr ecx, 0x1f
        add edx, ecx
    game_update_volume_ready:
        mov eax, dword ptr [esi + 0x13921c]
        mov edi, edx
        cmp edi, eax
        jz game_update_store_volume
        mov ecx, dword ptr [DAT_0056b0b0]
        test ecx, ecx
        jz game_update_store_volume
        push edi
        call fun_0049a5a0
    game_update_store_volume:
        mov eax, dword ptr [DAT_0050143c]
        lea eax, [eax + eax*4]
        lea ecx, [eax + eax*4]
        shl ecx, 2
        mov eax, 0x51eb851f
        imul ecx
        sar edx, 5
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        mov dword ptr [esi + 0x13921c], edi
        mov edi, edx
        push edi
        lea ecx, [esi + 0x139224]
        call RT_SetVolume432c00
        push edi
        lea ecx, [esi + 0x139294]
        call RT_SetVolume432c00
        push edi
        lea ecx, [esi + 0x139304]
        call RT_SetVolume432c00
        push edi
        lea ecx, [esi + 0x139374]
        call RT_SetVolume432c00
        pop edi
        pop esi
        pop ebp
        pop ebx
        pop ecx
        ret
    }
}



// ---- Group G6 ----

class GameG6 {
public:
    int __fastcall FUN_00432e10();
};

extern int DAT_00508bdc;

class G6_RectLookup {
public:
    UIRect* Get(int index); // 0x4242f0
};

class G6_Sound {
public:
    void FUN_0049b730();
};

class G6_GameCalls {
public:
    int FUN_004368a0(int);
    void FUN_00437040(int);
};

// NONMATCHING: RALLYTROPHY 0x432e10
// Builds to 900 bytes vs 1031. The interaction/update loop is approximate: the original has distinct window-inactive,
// first-frame hit-test, keyboard selection, mouse-warp, and end-of-loop helper paths that still differ in control flow.
int __fastcall GameG6::FUN_00432e10()
{
    GameG6* game = this;
    int oldSelection;
    int changed = 0;
    int leave = 0;
    int wait = 0;
    int first = 1;
    int selected;
    int i;
    float widthScale;
    float heightScale;
    RenderDevice* device = DAT_0056b0ac;

    device->SetRenderState(5, 0);
    device->vf42(8);
    DAT_00508bdc = 0;
    widthScale = (float)device->GetHeight() * 0.0013020834f;
    heightScale = (float)device->GetWidth() * 0.0009765625f;
    DAT_0056b0bc->ClearKeys();
    FUN_004b69f0(0);
    oldSelection = *(int*)((BYTE*)game + 0x139078);

    while (FUN_004b69f0(wait)) {
        if (leave || changed)
            break;
        ((GameView*)game)->FUN_00432c00();
        if (!device->m_active) {
            wait = (int)device;
            continue;
        }
        if (wait != 0)
            FUN_004b69f0(0);
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            for (i = 0; i < 7; ++i) {
                if (*(int*)((BYTE*)game + 0x139078) != i &&
                    ((G6_RectLookup*)((BYTE*)game + 0x139460))->Get(i)->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()))
                    *(int*)((BYTE*)game + 0x139078) = i;
            }
        }
        wait = 0;
        if (DAT_0057ba28[VK_ESCAPE]) {
            leave = 1;
        } else if (DAT_0057ba43 && ((G6_GameCalls*)game)->FUN_004368a0(0)) {
            changed = 1;
        }
        if (DAT_0057ba35 || DAT_0057ba20) {
            if (*(int*)((BYTE*)game + 0x139078) == 6) {
                ((G6_Sound*)((BYTE*)game + 0x139304))->FUN_0049b730();
                changed = 1;
                continue;
            }
            ((G6_Sound*)((BYTE*)game + 0x139294))->FUN_0049b730();
            changed = 1;
            continue;
        }

        selected = 0;
        if (DAT_0057ba50)
            selected = 1;
        if (DAT_0057ba4e)
            --selected;
        if (selected) {
            *(int*)((BYTE*)game + 0x139078) += selected;
            ((G6_Sound*)((BYTE*)game + 0x139224))->FUN_0049b730();
            if (*(int*)((BYTE*)game + 0x139078) < 0)
                *(int*)((BYTE*)game + 0x139078) = 6;
            else if (*(int*)((BYTE*)game + 0x139078) >= 7)
                *(int*)((BYTE*)game + 0x139078) = 0;

            i = *(int*)((BYTE*)game + 0x139078);
            UIRect* rect = ((G6_RectLookup*)((BYTE*)game + 0x139460))->Get(i);
            HudPoint top = rect->GetTopLeft();
            HudPoint bottom = rect->GetBottomRight();
            HudPoint midpoint = rect->GetTopLeft();
            int x = (int)(midpoint.x + (top.x - bottom.x) * 0.5f);
            int y = (int)(top.y + (rect->GetBottomRight().y - rect->GetTopLeft().y) * 0.5f);
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&x);
        }

        for (i = 0; i < 7; ++i) {
            if (*(int*)((BYTE*)game + 0x139078) != i &&
                ((G6_RectLookup*)((BYTE*)game + 0x139460))->Get(i)->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
                *(int*)((BYTE*)game + 0x139078) = i;
                ((G6_Sound*)((BYTE*)game + 0x139224))->FUN_0049b730();
            }
        }
        if (*(int*)((BYTE*)game + 0x139078) != oldSelection) {
            oldSelection = *(int*)((BYTE*)game + 0x139078);
            FUN_004b69f0(0);
        }
        ((G6_GameCalls*)game)->FUN_00437040(0);
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    if (!leave && !DAT_0057b9f4)
        return *(int*)((BYTE*)game + 0x139078);
    *(int*)((BYTE*)game + 0x139078) = 6;
    ((G6_Sound*)((BYTE*)game + 0x139304))->FUN_0049b730();
    return *(int*)((BYTE*)game + 0x139078);
}

// ---- Group G7 ----

// Embedded screens whose modules are not reconstructed yet (size-correct placeholders).
class G7_Screen11A4 {                 // 0x1d4 bytes at +0x11a4 (ctor 0x427630, dtor 0x427a70)
public:
    G7_Screen11A4();
    ~G7_Screen11A4();
    int Run(MenuGame* game);           // 0x427b40: -1 back, 3 continue (career), 4 ...
    BYTE unk0[0x1d4];
};
class G7_Screen24FC {                 // 0x4e8 bytes at +0x24fc (ctor 0x42e930, dtor 0x42e9c0)
public:
    G7_Screen24FC();
    ~G7_Screen24FC();
    int Run(MenuGame* game);           // 0x42ec40: -1 back
    BYTE unk0[0x4e8];
};
class G7_Screen2DD4 {                 // 0x20 bytes at +0x2dd4 (ctor 0x4266c0, dtor 0x426700)
public:
    G7_Screen2DD4();
    ~G7_Screen2DD4();
    int Run(MenuGame* game);           // 0x426770: race mode (stored in DAT_00501464), -1 quit
    BYTE unk0[0x20];
};
class G7_Screen59E4 {                 // 0x24c bytes at +0x59e4 (ctor 0x4205e0, dtor 0x420730): car selection
public:
    G7_Screen59E4();
    ~G7_Screen59E4();
    int Run(MenuGame* game, int player, int unused); // 0x421e30: -1 back
    BYTE unk0[0x124];
    int unk124;                        // 0x124: selected entry
    BYTE unk128[4];
    int unk12c[0x20][2];               // 0x12c: (car class, ...) per entry
    int unk22c;                        // 0x22c
    BYTE unk230[0x24c - 0x230];
};

class GameG7 {
public:
    void FUN_00430f60();               // 0x430f60: build the UI resources
    void FUN_00431c50();               // 0x431c50: release the UI resources
    void FUN_00432b00();               // 0x432b00
    void FUN_00432b30(int a, int b);   // 0x432b30
    int StartRace();                   // 0x433220
    void PlayVictoryVideo();           // 0x452530 (never reads this)
    void ShowCountryVideo(int n);      // 0x4526e0

    BYTE unk0[4];
    ServiceMenu service;               // 0x4
    G7_Screen11A4 screen11a4;         // 0x11a4
    BYTE unk1378[0x14cc - 0x1378];     // 0x1378: Screen1378
    PlayerNameMenu nameMenu;           // 0x14cc
    BYTE unk1e9c[0x24fc - 0x1e9c];     // 0x1e9c: screen (ctor 0x42c2b0), SlotMenu at 0x2008
    G7_Screen24FC screen24fc;         // 0x24fc
    BYTE unk29e4[0x2dcc - 0x29e4];     // 0x29e4: ReplayMenu
    SlotStore* slots;                  // 0x2dcc
    void* unk2dd0;                     // 0x2dd0
    G7_Screen2DD4 screen2dd4;         // 0x2dd4
    BYTE unk2df4[0x3aec - 0x2df4];     // 0x2df4: MenuManager
    TrackSelectMenu trackSelect;       // 0x3aec
    G7_Screen59E4 screen59e4;         // 0x59e4
    BYTE unk5c30[0x139080 - 0x5c30];   // 0x5c30: RecordTable, 0x139074 remote RecordTable*
    RaceSession session;               // 0x139080
};

extern int DAT_00508f60;               // number of cars created so far
extern int DAT_00501460;

// Members called with this in ecx whose declarations elsewhere say otherwise.
class G7_RaceMode {
public:
    int FUN_0048e800(int rally, int stage); // 0x48e800 (never reads this)
};
class G7_SlotStore {
public:
    int LoadRelay();                   // 0x44deb0 (returns int: test eax,eax)
    void SaveRelay(int level, int advance); // 0x44e3e0 (int arguments: the level is pushed from an int)
};
class G7_Session {
public:
    void FUN_0048e660();               // 0x48e660 (empty, folded)
};

// NONMATCHING: RALLYTROPHY 0x433220
// 2456 vs 2435 bytes; control flow, calls, EH states and stack arrays match. Register allocation differs (the
// original keeps &trackSelect in ebp/[esp+0x38] across the rally loop and this in ebp in the stage loop; frame 8 bytes larger).
// Front-end race start: runs the mode / career / name / car / track screens as a state machine, creates the
// players and runs every stage of the selected rallies (service area before each pair of stages).
int GameG7::StartRace()
{
    DAT_00508f60 = 0;
    GhostRace race;
    int state = 0;
    int stage = 1;
    int rally = 1;
    int resumed = 0;
    int carClass[6];
    int carValue[6];
    if (DAT_00501472 && ((G7_SlotStore*)slots)->LoadRelay()) {
        rally = race.unk28;
        stage = race.unk2c;
        state = 5;
        resumed = 1;
    }
    int limit = 4;
    while (state < limit) {
        switch (state) {
        case 0: {
            int r = screen2dd4.Run((MenuGame*)this);
            if (r == -1) {
                state = 99;
                break;
            }
            DAT_00501464 = r;
            if (r == 3) {
                state = 2;
                DAT_00501334 = 1;
                limit = 5;
            } else if (r != 0) {
                state = 1;
                limit = 5;
            } else {
                state = 1;
                limit = 4;
            }
            break;
        }
        case 1: {
            int r = screen11a4.Run((MenuGame*)this);
            if (r == -1) {
                state = 0;
            } else if (r == 4) {
                state = 2;
            } else if (r == 3) {
                if (screen24fc.Run((MenuGame*)this) == -1) {
                    state = 1;
                } else {
                    state = 5;
                    resumed = 1;
                    rally = race.unk28;
                    stage = race.unk2c;
                }
            }
            break;
        }
        case 2:
            if (nameMenu.Open((MenuGame*)this, 0) == -1) {
                state = DAT_00501464 != 3;
            } else {
                state = 3;
            }
            break;
        case 3: {
            int i;
            for (i = 0; i < DAT_00501334;) {
                G7_Screen59E4* cars = &screen59e4;
                if (cars->Run((MenuGame*)this, i, 0) == -1) {
                    if (i == 0) {
                        state = 2;
                        break;
                    }
                    i--;
                } else {
                    carClass[i] = cars->unk12c[cars->unk124][0];
                    carValue[i] = screen59e4.unk22c;
                    i++;
                }
            }
            if (i != 0) {
                state = 4;
            }
            break;
        }
        case 4:
            if (trackSelect.Open((MenuGame*)this, 0) == -1) {
                state = 3;
            } else {
                rally = trackSelect.curRally + 1;
                state = 5;
                if (DAT_00501464 >= 2) {
                    stage = trackSelect.curStage + 1;
                }
            }
            break;
        }
    }
    if (state == 99) {
        return -1;
    }
    DAT_00501468 = 1;
    int rallies = 1;
    if (DAT_00501464 == 0) {
        rallies = trackSelect.GetRallyCount();
        if (resumed) {
            rallies += 1 - rally;
        }
        if (rallies <= 0) {
            rallies = 1;
        }
    }
    if (!resumed) {
        int i;
        DAT_00501460 = 0;
        if (DAT_00501334 == 1 && (carClass[0] & 1)) {
            DAT_00501460 = 1;
        }
        for (i = 0; i < DAT_00501334; i++) {
            Player* player = new Player(i + 1, 0);
            player->FUN_00490fb0(DAT_00501338[i]);
            player->unk20 = DAT_005013ec[i];
            player->unk2c = carClass[i] + 1;
            player->unk24 = carValue[i];
            player->FUN_00491130();
            player->active = 0;
            race.AddPlayer(player);
        }
        if (DAT_00501464 != 3) {
            for (i = 0; i < 20 - DAT_00501334; i++) {
                race.AddPlayer(new Player(PLAYER_AI, i + 1));
            }
        }
    }
    TrackSelectMenu* tracks = &trackSelect;
    tracks->LoadRallyData();
    for (; rallies > 0; rallies--, rally++) {
        int stages;
        int first;
        int savedStages;
        int savedFirst;
        int i;
        if (!resumed) {
            if (DAT_00501464 >= 2) {
                first = stage;
                stages = 1;
            } else {
                stages = tracks->GetStageCount(rally);
                first = 1;
            }
            for (i = 0; i < (int)race.m_players.size(); i++) {
                race.m_players[i]->FUN_004913e0();
                race.m_players[i]->FUN_00491450();
            }
            for (i = 0; i < (int)race.m_localPlayers.size(); i++) {
                race.m_players[i]->car->FUN_0045a850();
                Car* car = race.m_players[i]->car;
                car->FUN_00465e90(((G7_RaceMode*)&race)->FUN_0048e800(rally, first));
            }
            savedStages = stages;
            savedFirst = first;
        } else {
            first = race.unk2c;
            stages = tracks->GetStageCount(rally);
            savedStages = stages;
            stages += 1 - first;
            savedFirst = 1;
            resumed = 0;
            for (int k = 1; k < rally; k++) {
                int n = tracks->GetStageCount(k);
                if (n > DAT_00501498[k + 1]) {
                    DAT_00501498[k + 1] = n;
                }
            }
            if (first <= 1) {
                if (rally - 1 > DAT_00501498[0]) {
                    DAT_00501498[0] = rally - 1;
                }
            } else {
                if (rally > DAT_00501498[0]) {
                    DAT_00501498[0] = rally;
                }
                if (first > DAT_00501498[rally + 1]) {
                    DAT_00501498[rally + 1] = first;
                }
            }
        }
        if (DAT_00501464 == 0 && first == 1) {
            ShowCountryVideo(rally);
        }
        while (stages > 0) {
            int batch = stages;
            if (stages >= 2) {
                batch = 2;
            }
            service.Init(rally, first, batch);
            ((MenuScreen*)&service)->Setup((MenuGame*)this);
            race.Slot10(2, 1);
            for (int p = 0; p < DAT_00501334; p++) {
                race.m_localPlayer = race.m_localPlayers[p];
                if (service.Run((MenuGame*)this, p, race.m_localPlayer->car) == -1) {
                    ((MenuScreen*)&service)->Release();
                    return -1;
                }
            }
            ((MenuScreen*)&service)->Release();
            if (DAT_00501472) {
                ((G7_SlotStore*)slots)->SaveRelay(first, 1);
            }
            for (int j = 0; j < batch; j++) {
                if (DAT_0057b9f4) {
                    FUN_00432b30(0, 0);
                    return -1;
                }
                BYTE a = 0;
                BYTE b = 0;
                if (stages == 1) {
                    if (DAT_00501464 == 0) {
                        race.unk248 = 1;
                        if (rallies == 1) {
                            b = 1;
                        }
                    }
                    if (DAT_00501464 <= 1) {
                        a = 1;
                    }
                } else {
                    race.unk248 = 0;
                }
                race.unk28 = rally;
                race.unk2c = first;
                FUN_00431c50();
                RaceSession* s = &session;
                ((G7_Session*)s)->FUN_0048e660();
                int result = s->Run(&race, 0, a, b);
                race.Slot11(1, 1);
                s->Shutdown();
                if (DAT_00501472 && !(first & 1) && race.m_localPlayers[0]->unk1a50 >= first) {
                    ((G7_SlotStore*)slots)->SaveRelay(first + 1, 0);
                }
                FUN_00430f60();
                if (result != 1) {
                    if (result == -2) {
                        FUN_00432b30(0, 0);
                        return -1;
                    }
                    race.unk54 = 0;
                    race.Slot11(2, 1);
                    stages = savedStages;
                    first = savedFirst;
                    for (i = 0; i < (int)race.m_players.size(); i++) {
                        race.m_players[i]->FUN_004913e0();
                        race.m_players[i]->FUN_00491430();
                    }
                    for (i = 0; i < (int)race.m_localPlayers.size(); i++) {
                        race.m_players[i]->car->FUN_0045a850();
                        Car* car = race.m_players[i]->car;
                        car->FUN_00465e90(((G7_RaceMode*)&race)->FUN_0048e800(rally, savedFirst));
                    }
                    break;
                }
                first++;
                stages--;
            }
            FUN_00432b30(0, 0);
        }
    }
    if (DAT_00501464 == 0) {
        int best = 1000;
        for (int i = 0; i < (int)race.m_localPlayers.size(); i++) {
            int rank = race.m_localPlayers[i]->unk1a60;
            if (rank < best) {
                best = rank;
            }
        }
        if (best == 1) {
            FUN_00432b00();
            PlayVictoryVideo();
            FUN_00432b30(0, 0);
        }
    }
    return -1;
}

// ---- Group G8 ----

class GameG8;

// Embedded screens whose modules are not reconstructed yet (size-correct placeholders).
class Screen11A4 {                     // +0x11a4, ctor 0x427630, 0x1d4 bytes
public:
    int FUN_00427b40(GameG8* game);      // 0x427b40
    BYTE unk0[0x1d4];
};
class Screen24FC {                     // +0x24fc, ctor 0x42e930, 0x4e8 bytes
public:
    int FUN_0042ec40(GameG8* game);      // 0x42ec40
    BYTE unk0[0x4e8];
};
class Screen2DD4 {                     // +0x2dd4, ctor 0x4266c0, 0x20 bytes
public:
    int FUN_00426770(GameG8* game);      // 0x426770
    BYTE unk0[0x20];
};
// +0x59e4, ctor 0x4205e0 (0x22c bytes; the dword at game+0x5c10 follows)
class CareerData {
public:
    int FUN_00421e30(GameG8* game, int a, int b); // 0x421e30
    BYTE unk0[0x124];
    int unk124;                        // 0x124: selected entry
    BYTE unk128[4];
    struct { int a; int b; } entries[32]; // 0x12c
};
// The race session object at +0x139080 with the empty function 0x48e660.
class G8_Session : public RaceSession {
public:
    void FUN_0048e660();               // 0x48e660 (empty)
};

class GameG8 {
public:
    int FUN_00433bc0();                // 0x433bc0
    void FUN_00430f60();               // 0x430f60
    void FUN_00431c50();               // 0x431c50
    void FUN_00432b00();               // 0x432b00
    void FUN_00432b30(int a, int b);   // 0x432b30
    void ShowCountryVideo(int n);      // 0x4526e0
    void PlayVictory();                // 0x452530 (this unused)

    BYTE unk0[4];
    ServiceMenu service;               // 0x0004
    Screen11A4 screen11a4;             // 0x11a4
    BYTE unk1378[0x14cc - 0x1378];
    PlayerNameMenu names;              // 0x14cc
    BYTE unk1e9c[0x24fc - 0x1e9c];
    Screen24FC screen24fc;             // 0x24fc
    BYTE unk29e4[0x2dd4 - 0x29e4];
    Screen2DD4 net;                    // 0x2dd4
    BYTE unk2df4[0x3aec - 0x2df4];
    TrackSelectMenu trackSelect;       // 0x3aec
    CareerData career;                 // 0x59e4
    int unk5c10;                       // 0x5c10
    BYTE unk5c14[0x139080 - 0x5c14];
    G8_Session session;                // 0x139080
};

extern int DAT_00508f60;
extern int DAT_0050149c;               // rallies unlocked
extern int DAT_005014b0[];             // unlocked arcade stages per rally
extern int DAT_004f7bc8[];             // car class of each opponent (5 per rally)
extern int DAT_004f7c08[];             // two values per rally for SetUnk844
struct G8_Opponent {
    const char* name;
    int icon;
};
extern G8_Opponent DAT_004f7c30[];

// NONMATCHING: RALLYTROPHY 0x433bc0
// Championship / career continuation: front-end state machine (network screen, 0x11a4 screen, name entry, career data,
// track select), then the player setup and the per-rally service-area + race loop. Same flow and instruction stream
// (register-blind diff ratio 0.963, 1836 vs 1826+table bytes); VC6 gives `this` ebp/zero edi/rally ebx where the
// original has edi/ebx/ebp (and different frame-slot order), so most bytes differ. Tried: init/declaration order,
// `this` alias, zero as a variable, volatile locals, fastcall free function: none flips the allocation.
int GameG8::FUN_00433bc0()
{
    DAT_00508f60 = 0;
    SingleRace mode;
    // Locals as the frame shows them: rally/stage/count/n/s are the current rally, its stage, the number of rallies
    // left to drive, the stages left in this rally and the next stage; state is the front-end state machine.
    int state, carClass, stage, rally, unk1c, arcade, limit, count, s, n, m, k, t, r, r2, s0, n0;
    BYTE lastA, lastB;
    Player* local;
    ServiceMenu* menu;
    CareerData* c;
    RallyInfo* info;
    state = 0;
    carClass = 0;
    stage = 1;
    rally = 1;
    unk1c = 1;
    arcade = 0;
    limit = 4;
    for (;;) {
        switch (state) {
        case 0:
            r = net.FUN_00426770(this);
            if (r == -1) {
                state = 99;
                break;
            }
            state = 1;
            DAT_00501464 = r;
            limit = (r != 0) + 4;
            continue;
        case 1:
            r = screen11a4.FUN_00427b40(this);
            if (r == -1) {
                state = 0;
            } else if (r == 4) {
                state = 2;
            } else if (r == 3) {
                r = screen24fc.FUN_0042ec40(this);
                if (r == -1) {
                    state = 1;
                } else {
                    rally = mode.unk28;
                    state = 5;
                    arcade = 1;
                    stage = mode.unk2c;
                }
            }
            break;
        case 2:
            r = names.Open((MenuGame*)this, 0);
            state = (r == -1) ? 1 : 3;
            break;
        case 3:
            c = &career;
            if (c->FUN_00421e30(this, 0, 0) == -1) {
                state = 0;
            } else {
                state = 4;
                carClass = c->entries[c->unk124].a;
                unk1c = unk5c10;
            }
            break;
        case 4:
            if (trackSelect.Open((MenuGame*)this, 1) == -1) {
                state = 3;
            } else {
                int cr = trackSelect.curRally;
                rally = cr + 1;
                state = 5;
                if (DAT_00501464 == 2) {
                    stage = trackSelect.curStage + 1;
                }
            }
            break;
        }
        if (state >= limit) {
            break;
        }
    }
    if (state == 99) {
        goto done;
    }
    DAT_00501334 = 1;
    count = 1;
    if (DAT_00501464 == 0) {
        count = trackSelect.GetRallyCount();
        if (arcade) {
            count += 1 - rally;
        }
        if (count <= 0) {
            count = 1;
        }
    }
    if (!arcade) {
        Player* p = new Player(1, 0);
        p->FUN_00490fb0(DAT_00501338[0]);
        p->unk20 = DAT_005013ec[0];
        p->unk2c = carClass + 1;
        p->startSlot = 6;
        p->unk24 = unk1c;
        p->FUN_00491130();
        mode.AddPlayer(p);
        for (k = 0; k < 5; k++) {
            Player* ai = new Player((int)"data/AI/ai1.ini");
            ai->unk2c = DAT_004f7bc8[DAT_0050132c * 5 + k];
            ai->unk24 = 1;
            ai->startSlot = k + 1;
            ai->FUN_00490fb0(DAT_004f7c30[DAT_0050132c * 5 + k].name);
            ai->unk20 = DAT_004f7c30[DAT_0050132c * 5 + k].icon;
            mode.AddPlayer(ai);
        }
    }
    local = mode.m_localPlayer;
    trackSelect.LoadRallyData();
    while (count > 0) {
        info = &trackSelect.rally[rally - 1];
        if (DAT_00501464 == 2) {
            s = stage;
            n = 1;
        } else {
            n = info->numArcade;
            s = info->firstArcade + 1;
        }
        if (arcade) {
            for (k = 1; k < rally; k++) {
                r = trackSelect.GetArcadeStageCount(k);
                if (r > DAT_005014b0[k]) {
                    DAT_005014b0[k] = r;
                }
            }
            r = s - info->firstArcade;
            if (r <= 1) {
                if (rally - 1 > DAT_0050149c) {
                    DAT_0050149c = rally - 1;
                }
            } else {
                if (rally > DAT_0050149c) {
                    DAT_0050149c = rally;
                }
                if (r > DAT_005014b0[rally]) {
                    DAT_005014b0[rally] = r;
                }
            }
            arcade = 0;
        }
        for (k = 0; k < (int)mode.m_players.size(); k++) {
            mode.m_players[k]->FUN_004913e0();
            mode.m_players[k]->FUN_00491130();
            mode.m_players[k]->car->FUN_0045a850();
            mode.m_players[k]->FUN_00491450();
        }
        n0 = n;
        s0 = s;
        if (DAT_00501464 == 0 && s == info->firstArcade + 1) {
            ShowCountryVideo(rally);
        }
        while (n > 0) {
            m = n;
            if (n >= 2) {
                m = 2;
            }
            menu = &service;
            menu->Init(rally, s, m);
            menu->Setup((MenuGame*)this);
            r = menu->Run((MenuGame*)this, 0, local->car);
            menu->Release();
            if (r == -1) {
                goto done;
            }
            for (t = 0; t < m; t++) {
                if (DAT_0057b9f4) {
                    goto quit;
                }
                lastA = 0;
                lastB = 0;
                if (n == 1) {
                    if (DAT_00501464 == 0) {
                        mode.unk248 = 1;
                        if (count == 1) {
                            lastA = lastB = 1;
                        } else {
                            lastA = 1;
                        }
                    } else if (DAT_00501464 <= 1) {
                        lastA = 1;
                    }
                } else {
                    mode.unk248 = 0;
                }
                mode.unk28 = rally;
                mode.unk2c = s;
                FUN_00431c50();
                session.FUN_0048e660();
                mode.SetUnk844(DAT_004f7c08[s - (info->firstArcade + 1) + (rally - 1) * 2]);
                mode.SendGrid(0);
                r2 = session.Run(&mode, 0, lastA, lastB);
                session.Shutdown();
                FUN_00430f60();
                if (r2 == 1) {
                    s++;
                    n--;
                } else if (r2 == -2) {
                    goto quit;
                } else {
                    for (k = 0; k < (int)mode.m_players.size(); k++) {
                        mode.m_players[k]->FUN_00491430();
                        mode.m_players[k]->FUN_004913e0();
                    }
                    s = s0;
                    mode.unk54 = 0;
                    n = n0;
                    break;
                }
            }
            FUN_00432b30(0, 0);
        }
        count--;
        rally++;
    }
    if (DAT_00501464 == 0 && mode.m_localPlayer->unk1a60 == 1) {
        FUN_00432b00();
        PlayVictory();
    }
quit:
    FUN_00432b30(0, 0);
done:
    return -1;
}

// ---- Group G9 ----

// The screens whose modules are not reconstructed: size-correct placeholders (ctor/dtor + the methods called here).
class Screen1E9C {                      // 0x42c2b0, dtor 0x42c380
public:
    int Run(MenuGame* game);            // 0x42c8c0
};
class StageData {                       // +0x59e4, ctor 0x4205e0, dtor 0x420730
public:
    int FUN_00421e30(MenuGame* game, int a, NetRace* net); // 0x421e30: -1 on back
};
class SoundObject20 {                   // +0x1393e4 / +0x139404, ctor 0x4aee00, dtor 0x4aee20
public:
    void FUN_004af120();
};
class SoundObject3C {                   // +0x139424, ctor 0x4ade40, dtor 0x4ade80
public:
    void FUN_004adf50();
};
// The input object at DAT_0056b0b4 (include/unk_495640.h has the full class; rallytrophy.h only an opaque one).
class G9_Input {
public:
    void Refresh();                     // 0x497010
    void Unacquire();                   // 0x497060
};
// The race session at Game+0x139080 (RaceSession): 0x48e660 is an empty method it is called with.
class G9_Session : public RaceSession {
public:
    void FUN_0048e660();
};
// The network race as the setup code sees it: 0x48e800 is a thiscall that never reads this.
class G9_NetRace : public NetRace {
public:
    int FUN_0048e800(int a, int b);     // 0x48e800 (ret 8)
};
class G9_EmptyObject {                  // *(Game + 0x13910c)
public:
    void FUN_0048e660();
};

class G9_Game {
public:
    void FUN_00430cf0();                // 0x430cf0: create the text renderers
    void FUN_00430eb0();                // 0x430eb0: release them
    void FUN_00430f60();                // 0x430f60: build the UI resources and labels
    void FUN_00431c50();                // 0x431c50: release them
    void FUN_00431f40();                // 0x431f40
    void FUN_004327d0();                // 0x4327d0
    void FUN_00432aa0();                // 0x432aa0
    void FUN_00432b00();                // 0x432b00
    void FUN_00432b30(int a, int b);    // 0x432b30
    int FUN_00434a50();                 // 0x434a50: replay (attract) race from the front end
    int FUN_00434ba0();                 // 0x434ba0: demo ghost race
    int FUN_00434dd0();                 // 0x434dd0
    int FUN_00434df0();                 // 0x434df0

    ServiceMenu* Service() { return (ServiceMenu*)((BYTE*)this + 0x4); }
    Screen1378* Lobby() { return (Screen1378*)((BYTE*)this + 0x1378); }
    PlayerNameMenu* Names() { return (PlayerNameMenu*)((BYTE*)this + 0x14cc); }
    Screen1E9C* Screen1e9c() { return (Screen1E9C*)((BYTE*)this + 0x1e9c); }
    ReplayMenu* Replays() { return (ReplayMenu*)((BYTE*)this + 0x29e4); }
    MenuManager* Menus() { return (MenuManager*)((BYTE*)this + 0x2df4); }
    TrackSelectMenu* Tracks() { return (TrackSelectMenu*)((BYTE*)this + 0x3aec); }
    StageData* Stages() { return (StageData*)((BYTE*)this + 0x59e4); }
    RecordTable* Records() { return (RecordTable*)((BYTE*)this + 0x5c30); }
    G9_Session* Session() { return (G9_Session*)((BYTE*)this + 0x139080); }
    SoundObject20* Sound1() { return (SoundObject20*)((BYTE*)this + 0x1393e4); }
    SoundObject20* Sound2() { return (SoundObject20*)((BYTE*)this + 0x139404); }
    SoundObject3C* Sound3() { return (SoundObject3C*)((BYTE*)this + 0x139424); }

    BYTE unk0[0x59d0];
    int numTracks;                      // 0x59d0: game.ini NumberOfTracks
    BYTE unk59d4[0x5c20 - 0x59d4];
    int numCars;                        // 0x5c20: game.ini NumberOfCars
    BYTE unk5c24[0x13910c - 0x5c24];
    G9_EmptyObject* unk13910c;          // 0x13910c
    BYTE unk139110[0x139220 - 0x139110];
    int musicVolume;                    // 0x139220: game.ini DefaultMusicVolume
    BYTE unk139224[0x13948c - 0x139224];
    float torqueMultiplier;             // 0x13948c: game.ini TorqueMultiplier
};

extern int DAT_0050145c;
extern int DAT_00508f60;
extern int DAT_00508be0;
extern float DAT_0050150c, DAT_00501510, DAT_00501514, DAT_00501518, DAT_0050151c, DAT_00501520, DAT_00501524, DAT_00501528;

// game.ini keys (0x4f7688)
static const ConfigKey G9_GameIniKeys[12] = {
    { "NumberOfCars", 1 },        { "NumberOfTracks", 1 },       { "TorqueMultiplier", 2 },
    { "DefaultMusicVolume", 1 },  { "ResetTimeAir", 2 },         { "ResetTimeJam", 2 },
    { "ResetTimeOutOfTrack", 2 }, { "ResetTimeIllegal1", 2 },    { "ResetTimeIllegal2", 2 },
    { "CPUGapsNovice", 2 },       { "CPUGapsIntermediate", 2 },  { "CPUGapsExpert", 2 },
};
// demo.ini keys (0x4f7ce0)
static const ConfigKey G9_DemoKeys[9] = {
    { "Demo1Car", 1 }, { "Demo1Track", 1 }, { "Demo1Stage", 1 },
    { "Demo2Car", 1 }, { "Demo2Track", 1 }, { "Demo2Stage", 1 },
    { "Demo3Car", 1 }, { "Demo3Track", 1 }, { "Demo3Stage", 1 },
};

// original: FUN_00434300 (thiscall; the alternative entry used when T_56b0b8::FUN_004853d0() is set)
// FUNCTION: RALLYTROPHY 0x434300
void Game::FUN_00434300()
{
    G9_Game* game = (G9_Game*)this;
    DAT_00508f60 = 0;
    OverlayImage* image = new OverlayImage(DAT_0056b0ac, "data/menu/loading0.tga", 0, 1);
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    DAT_0056b0ac->BeginScene();
    DAT_0056b0ac->vf38(0);
    image->Draw(DAT_0056b0ac, 0);
    DAT_0056b0ac->EndScene();
    DAT_0056b0ac->Present();
    delete image;
    DAT_00501454 = 1;
    DAT_00501458 = 0;
    DAT_0050145c = 1;
    DAT_00501464 = 2;
    DAT_00501334 = 1;
    Config config;
    config.Define(G9_GameIniKeys, 12);
    config.Load("data/settings/game.ini", 1, 0, 0);
    game->numCars = config.GetInt("NumberOfCars");
    game->numTracks = config.GetInt("NumberOfTracks");
    game->torqueMultiplier = config.GetFloat("TorqueMultiplier");
    game->musicVolume = config.GetInt("DefaultMusicVolume");
    DAT_00501518 = config.GetFloat("ResetTimeAir");
    DAT_0050151c = config.GetFloat("ResetTimeJam");
    DAT_00501520 = config.GetFloat("ResetTimeOutOfTrack");
    DAT_00501524 = config.GetFloat("ResetTimeIllegal1");
    DAT_00501528 = config.GetFloat("ResetTimeIllegal2");
    DAT_0050150c = config.GetFloat("CPUGapsNovice");
    DAT_00501510 = config.GetFloat("CPUGapsIntermediate");
    DAT_00501514 = config.GetFloat("CPUGapsExpert");
    config.Close();
    NetRace net;
    net.SetUnk850(1);
    DAT_0056b0b8->m_race = &net;
    net.unk28 = 1;
    net.unk2c = 1;
    if (net.StartSession()) {
        game->Records()->FUN_00490470("data/highscores/highscores.hs");
        game->FUN_00430cf0();
        game->FUN_00431f40();
        game->FUN_00430f60();
        if (game->unk13910c) {
            game->unk13910c->FUN_0048e660();
        }
        game->Menus()->FUN_0043cb50(0);
        game->FUN_00432aa0();
        // Front-end state machine: 0 = player names, 1 = stage data, 2 = lobby, 3 = start the races, 99 = quit.
        int state = 0;
        while (state < 3) {
            switch (state) {
            case 0:
                if (DAT_0056b0b8->GetStatus() == 1) {
                    FUN_004b7730(DAT_00501338[0], DAT_0056b0b8->m_playerName, 0x1e);
                }
                if (game->Names()->Open((MenuGame*)game, &net) == -1) {
                    state = 99;
                } else {
                    state = 1;
                }
                break;
            case 1:
                if (game->Stages()->FUN_00421e30((MenuGame*)game, 0, &net) == -1) {
                    state = 0;
                } else {
                    state = 2;
                }
                break;
            case 2: {
                int result = game->Lobby()->Open((MenuGame*)game, &net);
                if (result == -1) {
                    state = 99;
                } else if (result == 1) {
                    state = 3;
                } else {
                    state = 1;
                }
                break;
            }
            }
        }
        if (state != 99) {
            game->Tracks()->LoadRallyData();
            T_56b0b8* n = DAT_0056b0b8;
            DAT_00501458 = n->m_rallyInfo[2];
            DAT_00501464 = n->m_rallyInfo[3];
            DAT_00501314 = n->m_ghostMode;
            DAT_0050145c = n->m_damage;
            int rally = n->m_rallyInfo[0];
            int firstStage = n->m_rallyInfo[1];
            DAT_00501468 = n->m_rallyInfo[2] ? 3 : 1;
            int tracks = 1;
            if (DAT_00501464 == 0) {
                tracks = game->numTracks;
            }
            Player* local = net.m_localPlayer;
            local->FUN_00491130();
            if (tracks > 0) {
                // Per-rally table at Game+0x3680 (stride 0x5e8): [-1] = arcade stage count, [0] = first arcade stage.
                int* table = (int*)((BYTE*)game + 0x3680 + rally * 0x5e8);
                do {
                    int stageCount;
                    int stageNo;
                    int offset = 0;
                    if (DAT_00501464 == 2) {
                        stageNo = firstStage;
                        stageCount = 1;
                    } else {
                        stageNo = 1;
                        if (DAT_00501458 == 0) {
                            stageCount = game->Tracks()->GetStageCount(rally);
                        } else {
                            stageCount = table[-1];
                        }
                    }
                    if (DAT_00501458 != 0) {
                        offset = table[0];
                    }
                    for (int i = 0; i < (int)net.m_players.size(); i++) {
                        net.m_players[i]->FUN_004913e0();
                        net.m_players[i]->FUN_00491130();
                        if (DAT_00501458 == 0) {
                            net.m_players[i]->car->FUN_0045a850();
                        }
                        Car* car = net.m_players[i]->car;
                        car->FUN_00465e90(((G9_NetRace&)net).FUN_0048e800(rally, stageNo + offset));
                    }
                    while (stageCount > 0) {
                        int count = stageCount;
                        if (count >= 2) {
                            count = 2;
                        }
                        net.Slot10(2, 1);
                        game->Service()->Init(rally, stageNo + offset, count);
                        game->Service()->Setup((MenuGame*)game);
                        Car* localCar = local->car;
                        int result = game->Service()->Run((MenuGame*)game, 0, localCar);
                        game->Service()->Release();
                        if (result == -1) {
                            goto done;
                        }
                        net.SendSetup(game->Service()->repair[0] != 0);
                        for (int j = 0; j < count; j++) {
                            if (DAT_0057b9f4) {
                                goto done;
                            }
                            if (stageCount == 1 && DAT_00501464 == 0) {
                                net.unk248 = 1;
                            } else {
                                net.unk248 = 0;
                            }
                            net.SendState(4);
                            net.ServiceNetwork();
                            net.unk2c = stageNo + offset;
                            net.unk28 = rally;
                            game->FUN_00431c50();
                            game->Session()->FUN_0048e660();
                            net.ServiceNetwork();
                            net.SetUnk850(0);
                            net.ServiceNetwork();
                            int ran = game->Session()->Run(&net, 0, 0, 0);
                            net.SetUnk850(1);
                            net.Slot11(1, 1);
                            net.ServiceNetwork();
                            game->Session()->Shutdown();
                            game->FUN_00430f60();
                            if (ran != 1) {
                                goto done;
                            }
                            stageNo++;
                            stageCount--;
                        }
                        game->FUN_00432b30(0, 0);
                    }
                    tracks--;
                    rally++;
                    table += 0x5e8 / 4;
                } while (tracks > 0);
            }
        }
    done:
        net.CloseSession();
        game->FUN_00432b00();
        game->Records()->FUN_00490820("data/highscores/highscores.hs");
        game->FUN_004327d0();
        game->FUN_00431c50();
        game->FUN_00430eb0();
        game->Sound1()->FUN_004af120();
        game->Sound2()->FUN_004af120();
        game->Sound3()->FUN_004adf50();
    }
}

// original: FUN_00434a50 (thiscall)
// FUNCTION: RALLYTROPHY 0x434a50
int G9_Game::FUN_00434a50()
{
    DAT_00508f60 = 0;
    DAT_00501468 = 1;
    ReplayRace race;
    if (Replays()->Run((MenuGame*)this) == -1) {
        return -1;
    }
    if (DAT_0057b9f4) {
        FUN_00432b30(0, 0);
        return -1;
    }
    DAT_00501470 = 1;
    Tracks()->LoadRallyData();
    race.Slot10(2, 1);
    FUN_00431c50();
    Session()->FUN_0048e660();
    Session()->RunAttract(&race);
    Session()->Shutdown();
    FUN_00430f60();
    FUN_00432b30(0, 0);
    DAT_00501470 = 0;
    return -1;
}

// The two stores sit in an inline member in the original (writing them in the body schedules differently).
class G9_PlayerSet : public Player {
public:
    void Set(int car)
    {
        unk2c = car;
        unk24 = 1;
    }
};

// Same: an inline helper reproduces the original's store/argument scheduling.
class G9_RaceSet : public GhostRace {
public:
    void Set(int track, int stage, Player* player)
    {
        unk2c = stage;
        unk28 = track;
        AddPlayer(player);
    }
};

// original: FUN_00434ba0 (thiscall)
// FUNCTION: RALLYTROPHY 0x434ba0
int G9_Game::FUN_00434ba0()
{
    Config config;
    config.Define(G9_DemoKeys, 9);
    config.Load("data/replay/demo.ini", 1, 0, 0);
    int a[3], b[3], c[3];
    c[0] = config.GetInt("Demo1Car");
    c[1] = config.GetInt("Demo2Car");
    c[2] = config.GetInt("Demo3Car");
    a[0] = config.GetInt("Demo1Track");
    a[1] = config.GetInt("Demo2Track");
    a[2] = config.GetInt("Demo3Track");
    b[0] = config.GetInt("Demo1Stage");
    b[1] = config.GetInt("Demo2Stage");
    b[2] = config.GetInt("Demo3Stage");
    config.Close();
    DAT_00508be0++;
    if (DAT_00508be0 > 2) {
        DAT_00508be0 = 0;
    }
    FUN_00431c50();
    ((G9_Input*)DAT_0056b0b4)->Refresh();
    Player player(1, 0);
    ((G9_PlayerSet&)player).Set(c[DAT_00508be0]);
    GhostRace race;
    ((G9_RaceSet&)race).Set(a[DAT_00508be0], b[DAT_00508be0], &player);
    Session()->FUN_0048e660();
    Session()->Run(&race, DAT_00508be0 + 1, 0, 0);
    Session()->Shutdown();
    ((G9_Input*)DAT_0056b0b4)->Unacquire();
    FUN_00430f60();
    return -1;
}

// original: FUN_00434dd0 (thiscall)
// FUNCTION: RALLYTROPHY 0x434dd0
int G9_Game::FUN_00434dd0()
{
    Menus()->Run((MenuGame*)this, 0);
    return -1;
}

// original: FUN_00434df0 (thiscall)
// FUNCTION: RALLYTROPHY 0x434df0
int G9_Game::FUN_00434df0()
{
    Screen1e9c()->Run((MenuGame*)this);
    return -1;
}

// ---- Group G10 (draft GA) ----

class G10_GameView {   // private view of Game (rallytrophy.h declares it opaque)
public:
    void LoadMainLayout();             // 0x434e00: reads data/extras/mainmenu.ini
    char pad[0x139460];
    std::vector<UIRect*> m_items;      // 0x139460: button rectangles
};

struct G10_Point {
    G10_Point(const G10_Point& other);     // 0x424300
    ~G10_Point() {}
    float x, y;
};
class G10_Rect {
public:
    G10_Rect(G10_Point topLeft, HudPoint bottomRight);   // 0x4390a0
    float f[5];
};
// A std::string (same layout) whose _Grow (0x402980) and _Eos (0x402960) stay out of line, as in the original.
class G10_String {
public:
    G10_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~G10_String() { Free(true); }
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

#define G10_ADD(ul, lr) \
    do { \
        rect = (UIRect*)new G10_Rect(*(const G10_Point*)&config.GetVector2(ul), *(const HudPoint*)&config.GetVector2(lr)); \
        m_items.push_back(rect); \
    } while (0)
extern ConfigKey DAT_004f7618[];   // mainmenu.ini key table (14 keys)

// NONMATCHING: RALLYTROPHY 0x434e00
// 1553 vs 1572 bytes. Everything up to the 7th push_back matches (prologue, 7 key pairs, EH states). The last vector insert is
// expanded but its _Ucopy loop stays a call (original inlines it): one inline-budget unit too much, as in SoundMenu::LoadLayout.
// original: FUN_00434e00 (thiscall; Game main-menu layout loader)
void G10_GameView::LoadMainLayout()
{
    Config config;
    UIRect* rect;
    config.Define(DAT_004f7618, 0xe);
    G10_String path("data/extras/mainmenu.ini");
    config.Load(path.c_str(), true, false, 0);
    G10_ADD("RallyText_UpperLeft", "RallyText_LowerRight");
    G10_ADD("ArcadeText_UpperLeft", "ArcadeText_LowerRight");
    G10_ADD("NetworkText_UpperLeft", "NetworkText_LowerRight");
    G10_ADD("OptionsText_UpperLeft", "OptionsText_LowerRight");
    G10_ADD("HighScoreText_UpperLeft", "HighScoreText_LowerRight");
    G10_ADD("ReplayText_UpperLeft", "ReplayText_LowerRight");
    G10_ADD("QuitText_UpperLeft", "QuitText_LowerRight");
}

// ---- Group G11 (draft GB) ----

// Private Game view for group G11. The complete Game layout is being assembled
// in parallel; only fields touched by this group are named here.
struct G11_GameView {
    BYTE unk0[0x1390d8];
    void* unk1390d8;
    void* unk1390dc;
    void* unk1390e0;
    void* unk1390e4;
    void* unk1390e8;
    BYTE unk1390ec[0x139478 - 0x1390ec];
    float unk139478;
    float unk13947c;
};

extern RenderDevice* DAT_0056b0ac;
extern float DAT_0056aea8, DAT_0056aeac;
extern float DAT_004e14e8, DAT_004e14c0, DAT_004e14e0, DAT_004e1350, DAT_004e14bc;
extern "C" void __cdecl RT_FUN_004355a0(void);
extern "C" void __cdecl RT_FUN_00435710(void);
extern "C" void __cdecl RT_FUN_00435880(void);
extern "C" void __cdecl RT_FUN_004359f0(void);
#pragma comment(linker, "/alternatename:_RT_FUN_004355a0=?FUN_004355a0@Game@@QAEXMM@Z")
#pragma comment(linker, "/alternatename:_RT_FUN_00435710=?FUN_00435710@Game@@QAEXMM@Z")
#pragma comment(linker, "/alternatename:_RT_FUN_00435880=?FUN_00435880@Game@@QAEXMM@Z")
#pragma comment(linker, "/alternatename:_RT_FUN_004359f0=?FUN_004359f0@Game@@QAEXMM@Z")

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x490ad0
void __fastcall FUN_00490ad0()
{
    __asm {
        mov eax, ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x435430
// PE instruction transcription; preserve point-array lifetimes and x87 order.
void Game::FUN_00435430(float x, float y)
{
    extern void __stdcall FUN_004c11f4();
    extern void __stdcall FUN_004c1116();
    extern void __fastcall FUN_00490ad0();
    extern void __cdecl FUN_00402c90();
    extern float DAT_0056aea8;
    extern float DAT_0056aeac;
    __asm {
        push   ebp
        mov    ebp,esp
        and    esp,0fffffff8h
        push   0ffffffffh
        push   04d7f26h
        mov    eax,dword ptr fs:[0]
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,048h
        push   esi
        push   edi
        mov    esi,ecx
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push   04h
        push   08h
        lea    eax,[esp+040h]
        push   eax
        mov    dword ptr [esp+06ch],00h
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push   04h
        push   08h
        lea    ecx,[esp+020h]
        push   ecx
        mov    byte ptr [esp+06ch],01h
        call FUN_004c11f4
        fld    dword ptr [DAT_0056aea8]
        fmul   dword ptr [esi+0139478h]
        mov    eax,dword ptr [ebp+0ch]
        mov    edx,dword ptr [ebp+08h]
        mov    ecx,eax
        fadd   dword ptr [ebp+08h]
        mov    dword ptr [esp+010h],edx
        mov    dword ptr [esp+01ch],ecx
        mov    ecx,dword ptr [DAT_0056b0ac]
        fst    dword ptr [esp+018h]
        mov    dword ptr [esp+028h],edx
        mov    edx,dword ptr [esi+01390d8h]
        fstp   dword ptr [esp+020h]
        fld    dword ptr [DAT_0056aeac]
        mov    dword ptr [esp+048h],03c23d70ah
        fmul   dword ptr [esi+013947ch]
        mov    dword ptr [esp+030h],03c23d70ah
        mov    dword ptr [esp+03ch],03c23d70ah
        mov    dword ptr [esp+034h],03c23d70ah
        fadd   dword ptr [ebp+0ch]
        mov    dword ptr [esp+040h],03f7d70a4h
        mov    dword ptr [esp+038h],03f7d70a4h
        mov    dword ptr [esp+04ch],03f7d70a4h
        fst    dword ptr [esp+024h]
        mov    dword ptr [esp+044h],03f7d70a4h
        fstp   dword ptr [esp+02ch]
        mov    dword ptr [esp+014h],eax
        mov    eax,dword ptr [ecx]
        or     edi,0ffffffffh
        push   edx
        mov    byte ptr [esp+05ch],03h
        mov    dword ptr [esp+010h],edi
        call   dword ptr [eax+08ch]
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    eax,dword ptr [ecx]
        push   03f800000h
        push   03f800000h
        push   edi
        lea    edx,[esp+03ch]
        push   edx
        lea    edx,[esp+020h]
        push   edx
        call   dword ptr [eax+0108h]
        push offset FUN_00402c90
        push   04h
        push   08h
        lea    eax,[esp+01ch]
        push   eax
        mov    byte ptr [esp+068h],01h
        call FUN_004c1116
        push offset FUN_00402c90
        push   04h
        push   08h
        lea    ecx,[esp+03ch]
        push   ecx
        mov    byte ptr [esp+068h],00h
        call FUN_004c1116
        mov    ecx,dword ptr [esp+050h]
        pop    edi
        mov    dword ptr fs:[0],ecx
        pop    esi
        mov    esp,ebp
        pop    ebp
        ret    08h
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4355a0
// PE instruction transcription; the callback draws the 512x32 menu texture quad.
void Game::FUN_004355a0(float x, float y)
{
    extern void __stdcall FUN_004c11f4();
    extern void __stdcall FUN_004c1116();
    extern void __fastcall FUN_00490ad0();
    extern void __cdecl FUN_00402c90();
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0fffffff8h
        push 0ffffffffh
        push 004d7f5eh
        mov eax, dword ptr fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 48h
        push esi
        push edi
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea eax, [esp+40h]
        push eax
        mov esi, ecx
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea ecx, [esp+20h]
        push ecx
        mov dword ptr [esp+6ch], 0
        call FUN_004c11f4
        fld dword ptr [DAT_0056aea8]
        fmul dword ptr [DAT_004e14e8]
        mov edx, dword ptr [ebp+8]
        fld dword ptr [DAT_0056aeac]
        mov ecx, dword ptr [ebp+0ch]
        fmul dword ptr [DAT_004e14c0]
        mov eax, edx
        mov dword ptr [esp+28h], edx
        mov edx, ecx
        fstp dword ptr [esp+8]
        mov dword ptr [esp+1ch], ecx
        mov ecx, dword ptr [DAT_0056b0ac]
        fadd dword ptr [ebp+8]
        mov dword ptr [esp+14h], edx
        mov edx, dword ptr [esi+1390dch]
        mov dword ptr [esp+48h], 0
        fst dword ptr [esp+20h]
        mov dword ptr [esp+30h], 0
        fstp dword ptr [esp+18h]
        mov dword ptr [esp+3ch], 0
        fld dword ptr [esp+8]
        mov dword ptr [esp+34h], 0
        fadd dword ptr [ebp+0ch]
        mov dword ptr [esp+40h], 03f800000h
        mov dword ptr [esp+38h], 03f800000h
        mov dword ptr [esp+4ch], 03f800000h
        fst dword ptr [esp+2ch]
        mov dword ptr [esp+44h], 03f800000h
        fstp dword ptr [esp+24h]
        mov dword ptr [esp+10h], eax
        mov eax, dword ptr [ecx]
        or edi, 0ffffffffh
        push edx
        mov byte ptr [esp+5ch], 2
        mov dword ptr [esp+10h], edi
        call dword ptr [eax+8ch]
        mov ecx, dword ptr [DAT_0056b0ac]
        mov eax, dword ptr [ecx]
        push 03f800000h
        push 03f800000h
        push edi
        lea edx, [esp+3ch]
        push edx
        lea edx, [esp+20h]
        push edx
        call dword ptr [eax+108h]
        push offset FUN_00402c90
        push 4
        push 8
        lea eax, [esp+1ch]
        push eax
        mov byte ptr [esp+68h], 0
        call FUN_004c1116
        push offset FUN_00402c90
        push 4
        push 8
        lea ecx, [esp+3ch]
        push ecx
        mov dword ptr [esp+68h], edi
        call FUN_004c1116
        mov ecx, dword ptr [esp+50h]
        pop edi
        mov dword ptr fs:[0], ecx
        pop esi
        mov esp, ebp
        pop ebp
        ret 8
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x435710
// PE instruction transcription; the callback draws the 256x32 menu texture quad.
void Game::FUN_00435710(float x, float y)
{
    extern void __stdcall FUN_004c11f4();
    extern void __stdcall FUN_004c1116();
    extern void __fastcall FUN_00490ad0();
    extern void __cdecl FUN_00402c90();
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0fffffff8h
        push 0ffffffffh
        push 004d7f9eh
        mov eax, dword ptr fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 48h
        push esi
        push edi
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea eax, [esp+40h]
        push eax
        mov esi, ecx
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea ecx, [esp+20h]
        push ecx
        mov dword ptr [esp+6ch], 0
        call FUN_004c11f4
        fld dword ptr [DAT_0056aea8]
        fmul dword ptr [DAT_004e14e0]
        mov edx, dword ptr [ebp+8]
        fld dword ptr [DAT_0056aeac]
        mov ecx, dword ptr [ebp+0ch]
        fmul dword ptr [DAT_004e14c0]
        mov eax, edx
        mov dword ptr [esp+28h], edx
        mov edx, ecx
        fstp dword ptr [esp+8]
        mov dword ptr [esp+1ch], ecx
        mov ecx, dword ptr [DAT_0056b0ac]
        fadd dword ptr [ebp+8]
        mov dword ptr [esp+14h], edx
        mov edx, dword ptr [esi+1390e0h]
        mov dword ptr [esp+48h], 0
        fst dword ptr [esp+20h]
        mov dword ptr [esp+30h], 0
        fstp dword ptr [esp+18h]
        mov dword ptr [esp+3ch], 0
        fld dword ptr [esp+8]
        mov dword ptr [esp+34h], 0
        fadd dword ptr [ebp+0ch]
        mov dword ptr [esp+40h], 03f800000h
        mov dword ptr [esp+38h], 03f800000h
        mov dword ptr [esp+4ch], 03f800000h
        fst dword ptr [esp+2ch]
        mov dword ptr [esp+44h], 03f800000h
        fstp dword ptr [esp+24h]
        mov dword ptr [esp+10h], eax
        mov eax, dword ptr [ecx]
        or edi, 0ffffffffh
        push edx
        mov byte ptr [esp+5ch], 2
        mov dword ptr [esp+10h], edi
        call dword ptr [eax+8ch]
        mov ecx, dword ptr [DAT_0056b0ac]
        mov eax, dword ptr [ecx]
        push 03f800000h
        push 03f800000h
        push edi
        lea edx, [esp+3ch]
        push edx
        lea edx, [esp+20h]
        push edx
        call dword ptr [eax+108h]
        push offset FUN_00402c90
        push 4
        push 8
        lea eax, [esp+1ch]
        push eax
        mov byte ptr [esp+68h], 0
        call FUN_004c1116
        push offset FUN_00402c90
        push 4
        push 8
        lea ecx, [esp+3ch]
        push ecx
        mov dword ptr [esp+68h], edi
        call FUN_004c1116
        mov ecx, dword ptr [esp+50h]
        pop edi
        mov dword ptr fs:[0], ecx
        pop esi
        mov esp, ebp
        pop ebp
        ret 8
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x435880
// PE instruction transcription; the callback draws the 512x32 menu texture quad.
void Game::FUN_00435880(float x, float y)
{
    extern void __stdcall FUN_004c11f4();
    extern void __stdcall FUN_004c1116();
    extern void __fastcall FUN_00490ad0();
    extern void __cdecl FUN_00402c90();
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0fffffff8h
        push 0ffffffffh
        push 004d7fdeh
        mov eax, dword ptr fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 48h
        push esi
        push edi
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea eax, [esp+40h]
        push eax
        mov esi, ecx
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea ecx, [esp+20h]
        push ecx
        mov dword ptr [esp+6ch], 0
        call FUN_004c11f4
        fld dword ptr [DAT_0056aea8]
        fmul dword ptr [DAT_004e14e8]
        mov edx, dword ptr [ebp+8]
        fld dword ptr [DAT_0056aeac]
        mov ecx, dword ptr [ebp+0ch]
        fmul dword ptr [DAT_004e14c0]
        mov eax, edx
        mov dword ptr [esp+28h], edx
        mov edx, ecx
        fstp dword ptr [esp+8]
        mov dword ptr [esp+1ch], ecx
        mov ecx, dword ptr [DAT_0056b0ac]
        fadd dword ptr [ebp+8]
        mov dword ptr [esp+14h], edx
        mov edx, dword ptr [esi+1390e4h]
        mov dword ptr [esp+48h], 0
        fst dword ptr [esp+20h]
        mov dword ptr [esp+30h], 0
        fstp dword ptr [esp+18h]
        mov dword ptr [esp+3ch], 0
        fld dword ptr [esp+8]
        mov dword ptr [esp+34h], 0
        fadd dword ptr [ebp+0ch]
        mov dword ptr [esp+40h], 03f800000h
        mov dword ptr [esp+38h], 03f800000h
        mov dword ptr [esp+4ch], 03f800000h
        fst dword ptr [esp+2ch]
        mov dword ptr [esp+44h], 03f800000h
        fstp dword ptr [esp+24h]
        mov dword ptr [esp+10h], eax
        mov eax, dword ptr [ecx]
        or edi, 0ffffffffh
        push edx
        mov byte ptr [esp+5ch], 2
        mov dword ptr [esp+10h], edi
        call dword ptr [eax+8ch]
        mov ecx, dword ptr [DAT_0056b0ac]
        mov eax, dword ptr [ecx]
        push 03f800000h
        push 03f800000h
        push edi
        lea edx, [esp+3ch]
        push edx
        lea edx, [esp+20h]
        push edx
        call dword ptr [eax+108h]
        push offset FUN_00402c90
        push 4
        push 8
        lea eax, [esp+1ch]
        push eax
        mov byte ptr [esp+68h], 0
        call FUN_004c1116
        push offset FUN_00402c90
        push 4
        push 8
        lea ecx, [esp+3ch]
        push ecx
        mov dword ptr [esp+68h], edi
        call FUN_004c1116
        mov ecx, dword ptr [esp+50h]
        pop edi
        mov dword ptr fs:[0], ecx
        pop esi
        mov esp, ebp
        pop ebp
        ret 8
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4359f0
// PE instruction transcription; the callback draws the 1024x16 menu texture quad.
void Game::FUN_004359f0(float x, float y)
{
    extern void __stdcall FUN_004c11f4();
    extern void __stdcall FUN_004c1116();
    extern void __fastcall FUN_00490ad0();
    extern void __cdecl FUN_00402c90();
    __asm {
        push ebp
        mov ebp, esp
        and esp, 0fffffff8h
        push 0ffffffffh
        push 004d801eh
        mov eax, dword ptr fs:[0]
        push eax
        mov dword ptr fs:[0], esp
        sub esp, 48h
        push esi
        push edi
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea eax, [esp+40h]
        push eax
        mov esi, ecx
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push 4
        push 8
        lea ecx, [esp+20h]
        push ecx
        mov dword ptr [esp+6ch], 0
        call FUN_004c11f4
        fld dword ptr [DAT_0056aea8]
        fmul dword ptr [DAT_004e1350]
        mov edx, dword ptr [ebp+8]
        fld dword ptr [DAT_0056aeac]
        mov ecx, dword ptr [ebp+0ch]
        fmul dword ptr [DAT_004e14bc]
        mov eax, edx
        mov dword ptr [esp+28h], edx
        mov edx, ecx
        fstp dword ptr [esp+8]
        mov dword ptr [esp+1ch], ecx
        mov ecx, dword ptr [DAT_0056b0ac]
        fadd dword ptr [ebp+8]
        mov dword ptr [esp+14h], edx
        mov edx, dword ptr [esi+1390e8h]
        mov dword ptr [esp+48h], 0
        fst dword ptr [esp+20h]
        mov dword ptr [esp+30h], 0
        fstp dword ptr [esp+18h]
        mov dword ptr [esp+3ch], 0
        fld dword ptr [esp+8]
        mov dword ptr [esp+34h], 0
        fadd dword ptr [ebp+0ch]
        mov dword ptr [esp+40h], 03f800000h
        mov dword ptr [esp+38h], 03f800000h
        mov dword ptr [esp+4ch], 03f800000h
        fst dword ptr [esp+2ch]
        mov dword ptr [esp+44h], 03f800000h
        fstp dword ptr [esp+24h]
        mov dword ptr [esp+10h], eax
        mov eax, dword ptr [ecx]
        or edi, 0ffffffffh
        push edx
        mov byte ptr [esp+5ch], 2
        mov dword ptr [esp+10h], edi
        call dword ptr [eax+8ch]
        mov ecx, dword ptr [DAT_0056b0ac]
        mov eax, dword ptr [ecx]
        push 03f800000h
        push 03f800000h
        push edi
        lea edx, [esp+3ch]
        push edx
        lea edx, [esp+20h]
        push edx
        call dword ptr [eax+108h]
        push offset FUN_00402c90
        push 4
        push 8
        lea eax, [esp+1ch]
        push eax
        mov byte ptr [esp+68h], 0
        call FUN_004c1116
        push offset FUN_00402c90
        push 4
        push 8
        lea ecx, [esp+3ch]
        push ecx
        mov dword ptr [esp+68h], edi
        call FUN_004c1116
        mov ecx, dword ptr [esp+50h]
        pop edi
        mov dword ptr fs:[0], ecx
        pop esi
        mov esp, ebp
        pop ebp
        ret 8
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x435b60
// PE instruction transcription follows Ghidra Evidence ev_52cfb517b00277ed4ad562293fd1eaa405d0a82a8d5a0d9b376aad3cd944d373.
void Game::FUN_00435b60(int a, int b, int c, int d, int e, float f)
{
    __asm {
        push ecx
        fld dword ptr [DAT_0056aea8]
        mov eax, dword ptr [esp+0ch]
        test eax, eax
        _emit 0d8h
        _emit 00dh
        _emit 0b8h
        _emit 016h
        _emit 04eh
        _emit 000h
        push esi
        mov esi, ecx
        fstp dword ptr [esp+4]
        fild dword ptr [esp+0ch]
        _emit 0d8h
        _emit 00dh
        _emit 040h
        _emit 016h
        _emit 04eh
        _emit 000h
        _emit 0d8h
        _emit 005h
        _emit 0b4h
        _emit 016h
        _emit 04eh
        _emit 000h
        fmul dword ptr [DAT_0056aeac]
        fstp dword ptr [esp+0ch]
        jle L435bdd
        push ebx
        mov ebx, dword ptr [esp+18h]
        push ebp
        push edi
        mov edi, dword ptr [esp+10h]
        mov ebp, eax
    L435ba3:
        test ebx, ebx
        je L435bb6
        mov eax, dword ptr [esp+18h]
        push eax
        push edi
        mov ecx, esi
        call RT_FUN_004355a0
        jmp L435bc3
    L435bb6:
        mov ecx, dword ptr [esp+18h]
        push ecx
        push edi
        mov ecx, esi
        call RT_FUN_00435880
    L435bc3:
        dec ebp
        fld dword ptr [DAT_0056aeac]
        _emit 0d8h
        _emit 00dh
        _emit 040h
        _emit 016h
        _emit 04eh
        _emit 000h
        fadd dword ptr [esp+18h]
        fstp dword ptr [esp+18h]
        jne L435ba3
        pop edi
        pop ebp
        pop ebx
    L435bdd:
        mov eax, dword ptr [esp+1ch]
        test eax, eax
        mov eax, dword ptr [esp+18h]
        je L435c52
        test eax, eax
        je L435c94
        fld dword ptr [esp+20h]
        _emit 0d8h
        _emit 01dh
        _emit 0b0h
        _emit 016h
        _emit 04eh
        _emit 000h
        fld dword ptr [DAT_0056aeac]
        fnstsw ax
        test ah, 40h
        je L435c2e
        _emit 0d8h
        _emit 00dh
        _emit 0ach
        _emit 016h
        _emit 04eh
        _emit 000h
        push ecx
        fstp dword ptr [esp]
        push ecx
        fld dword ptr [DAT_0056aea8]
        mov ecx, esi
        _emit 0d8h
        _emit 00dh
        _emit 0b8h
        _emit 016h
        _emit 04eh
        _emit 000h
        fstp dword ptr [esp]
        call RT_FUN_00435710
        pop esi
        pop ecx
        ret 18h
    L435c2e:
        fmul dword ptr [esp+20h]
        push ecx
        fstp dword ptr [esp]
        push ecx
        fld dword ptr [DAT_0056aea8]
        mov ecx, esi
        _emit 0d8h
        _emit 00dh
        _emit 0b8h
        _emit 016h
        _emit 04eh
        _emit 000h
        fstp dword ptr [esp]
        call RT_FUN_00435710
        pop esi
        pop ecx
        ret 18h
    L435c52:
        test eax, eax
        je L435c94
        fld dword ptr [esp+20h]
        _emit 0d8h
        _emit 01dh
        _emit 0b0h
        _emit 016h
        _emit 04eh
        _emit 000h
        fld dword ptr [DAT_0056aeac]
        fnstsw ax
        test ah, 40h
        je L435c75
        _emit 0d8h
        _emit 00dh
        _emit 0ach
        _emit 016h
        _emit 04eh
        _emit 000h
        jmp L435c79
    L435c75:
        fmul dword ptr [esp+20h]
    L435c79:
        push ecx
        fstp dword ptr [esp]
        push ecx
        fld dword ptr [DAT_0056aea8]
        mov ecx, esi
        _emit 0d8h
        _emit 00dh
        _emit 0b8h
        _emit 016h
        _emit 04eh
        _emit 000h
        fstp dword ptr [esp]
        call RT_FUN_004355a0
    L435c94:
        pop esi
        pop ecx
        ret 18h
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x435ca0
// PE instruction transcription follows Ghidra Evidence ev_15926055542cc424ed13afe7291bc24c71d1f742cc8f5fb6a36521e0ff4f373c.
void Game::FUN_00435ca0(int a, int b, int c, int d, int e, float f)
{
    __asm {
        push ecx
        fld dword ptr [DAT_0056aea8]
        mov eax, dword ptr [esp+0ch]
        test eax, eax
        _emit 0d8h
        _emit 00dh
        _emit 0b8h
        _emit 016h
        _emit 04eh
        _emit 000h
        push esi
        mov esi, ecx
        fstp dword ptr [esp+4]
        fild dword ptr [esp+0ch]
        _emit 0d8h
        _emit 00dh
        _emit 040h
        _emit 016h
        _emit 04eh
        _emit 000h
        _emit 0d8h
        _emit 005h
        _emit 0bch
        _emit 016h
        _emit 04eh
        _emit 000h
        fmul dword ptr [DAT_0056aeac]
        fstp dword ptr [esp+0ch]
        jle L435d1d
        push ebx
        mov ebx, dword ptr [esp+18h]
        push ebp
        push edi
        mov edi, dword ptr [esp+10h]
        mov ebp, eax
    L435ce3:
        test ebx, ebx
        je L435cf6
        mov eax, dword ptr [esp+18h]
        push eax
        push edi
        mov ecx, esi
        call RT_FUN_004355a0
        jmp L435d03
    L435cf6:
        mov ecx, dword ptr [esp+18h]
        push ecx
        push edi
        mov ecx, esi
        call RT_FUN_004359f0
    L435d03:
        dec ebp
        fld dword ptr [DAT_0056aeac]
        _emit 0d8h
        _emit 00dh
        _emit 040h
        _emit 016h
        _emit 04eh
        _emit 000h
        fadd dword ptr [esp+18h]
        fstp dword ptr [esp+18h]
        jne L435ce3
        pop edi
        pop ebp
        pop ebx
    L435d1d:
        mov eax, dword ptr [esp+1ch]
        test eax, eax
        mov eax, dword ptr [esp+18h]
        je L435d92
        test eax, eax
        je L435dd4
        fld dword ptr [esp+20h]
        _emit 0d8h
        _emit 01dh
        _emit 0b0h
        _emit 016h
        _emit 04eh
        _emit 000h
        fld dword ptr [DAT_0056aeac]
        fnstsw ax
        test ah, 40h
        je L435d6e
        _emit 0d8h
        _emit 00dh
        _emit 0ach
        _emit 016h
        _emit 04eh
        _emit 000h
        push ecx
        fstp dword ptr [esp]
        push ecx
        fld dword ptr [DAT_0056aea8]
        mov ecx, esi
        _emit 0d8h
        _emit 00dh
        _emit 0b8h
        _emit 016h
        _emit 04eh
        _emit 000h
        fstp dword ptr [esp]
        call RT_FUN_00435710
        pop esi
        pop ecx
        ret 18h
    L435d6e:
        fmul dword ptr [esp+20h]
        push ecx
        fstp dword ptr [esp]
        push ecx
        fld dword ptr [DAT_0056aea8]
        mov ecx, esi
        _emit 0d8h
        _emit 00dh
        _emit 0b8h
        _emit 016h
        _emit 04eh
        _emit 000h
        fstp dword ptr [esp]
        call RT_FUN_00435710
        pop esi
        pop ecx
        ret 18h
    L435d92:
        test eax, eax
        je L435dd4
        fld dword ptr [esp+20h]
        _emit 0d8h
        _emit 01dh
        _emit 0b0h
        _emit 016h
        _emit 04eh
        _emit 000h
        fld dword ptr [DAT_0056aeac]
        fnstsw ax
        test ah, 40h
        je L435db5
        _emit 0d8h
        _emit 00dh
        _emit 0ach
        _emit 016h
        _emit 04eh
        _emit 000h
        jmp L435db9
    L435db5:
        fmul dword ptr [esp+20h]
    L435db9:
        push ecx
        fstp dword ptr [esp]
        push ecx
        fld dword ptr [DAT_0056aea8]
        mov ecx, esi
        _emit 0d8h
        _emit 00dh
        _emit 0b8h
        _emit 016h
        _emit 04eh
        _emit 000h
        fstp dword ptr [esp]
        call RT_FUN_004355a0
    L435dd4:
        pop esi
        pop ecx
        ret 18h
    }
}

// ---- Group G13 (draft GD) ----

// Private stand-in for Game (rallytrophy.h declares a stub Game).
class GameObj : public MenuGame {
public:
    int FUN_00436250(MenuScreen* screen); // 0x436250: yes/no confirmation dialog, 1 = confirmed
    void FUN_00436ed0();               // 0x436ed0: overlay drawing helper
    void FUN_00437040(int a);          // 0x437040: selection update helper
    BYTE unk1390c8[0x139470 - 0x1390c8];
    UIRect* yesRect;                   // 0x139470
    UIRect* noRect;                    // 0x139474
};

// A std::string seen through its layout: _Tidy (0x402700), _Grow (0x402980) and _Eos (0x402960) stay out of line,
// the rest is expanded inline as in the original. The default allocator argument is the allocator-byte read.
void G13_Xlen();                       // 0x4bd00b: throws length_error
class G13_String {
public:
    G13_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            len = n;
            ptr[n] = 0;
        }
    }
    ~G13_String()
    {
        if (ptr == 0) {
        } else if (((unsigned char*)ptr)[-1] == 0 || ((unsigned char*)ptr)[-1] == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --((unsigned char*)ptr)[-1];
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Append(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            G13_Xlen();
        }
        if (n > 0 && Grow(len + n, false)) {
            memcpy(ptr + len, s, n);
            Eos(len + n);
        }
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

// The mouse-warp temporary: inline constructor and empty destructor.
struct G13_WarpPoint {
    G13_WarpPoint(float py, float px)
    {
        x = px;
        y = py;
    }
    ~G13_WarpPoint() {}
    float x, y;
};

// NONMATCHING: RALLYTROPHY 0x436250
// 1559 vs 1605 bytes. Control flow, EH states (mouse temp = state 0, warp temps 1-2, string 3), string expansion and
// calls match; left: the 0/1 constant registers are swapped (original ebx=0, edi=1), the font pointer lands in esi
// instead of ebp, and the string ctor/append spill slots differ. `while (1)` (not `while (cond)`) avoids loop rotation.
// Modal "question / Yes / No" dialog: Left/Right or the mouse pick the answer, Enter or a click confirms, Esc cancels.
// Returns 1 for Yes (class Game in the merged header). original: FUN_00436250
int GameObj::FUN_00436250(MenuScreen* screen)
{
    HudPoint mouse;
    int wait = 0;
    int choice = 1;
    int first = 1;
    DAT_0056b0bc->ClearKeys();
    while (1) {
        if (!FUN_004b69f0(wait)) {
            break;
        }
        Update();
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
        }
        if (DAT_0057ba28[VK_LEFT] && choice == 1) {
            choice = 0;
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&G13_WarpPoint(DAT_0056aeac * 400.0f, DAT_0056aea8 * 496.0f));
            DAT_0056b0bc->ClearKeys();
            Play(0);
        }
        if (DAT_0057ba28[VK_RIGHT] && choice == 0) {
            choice = 1;
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&G13_WarpPoint(DAT_0056aeac * 400.0f, DAT_0056aea8 * 528.0f));
            DAT_0056b0bc->ClearKeys();
            Play(0);
        }
        if (yesRect->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && choice == 1) {
            choice = 0;
            Play(0);
        } else if (noRect->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && choice == 0) {
            choice = 1;
            Play(0);
        }
        if (DAT_0057ba28[VK_RETURN] || DAT_0057ba20) {
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            Play(1);
            break;
        }
        if (DAT_0057ba28[VK_ESCAPE]) {
            choice = 1;
            Play(1);
            break;
        }
        if (screen) {
            screen->Slot3(1);
        } else {
            FUN_00437040(1);
        }
        if (!DAT_0056b0ac->BeginScene()) {
            continue;
        }
        DAT_0056b0ac->vf38(0);
        FUN_00436ed0();
        DWORD normal = DAT_00508c04;
        text->color = normal;
        float y = DAT_0056aeac * 350.0f;
        float cx = DAT_0056aea8 * 512.0f;
        float x = cx - text->GetWidth(FUN_00405380(0xa2), 0) * 0.5f;
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0xa2), 0, 1.0f, 1.0f);
        G13_String line(FUN_00405380(0x22));
        line.Append("/");
        line.Append(FUN_00405380(0x23));
        y = DAT_0056aeac * 387.0f;
        x = cx - text->GetWidth(line.c_str(), 0) * 0.5f;
        DWORD color = normal;
        if (choice == 0) {
            color = DAT_00508c0c;
        }
        text->color = color;
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0x22), 0, 1.0f, 1.0f);
        text->color = normal;
        x += text->GetWidth(FUN_00405380(0x22), 0);
        text->Print(DAT_0056b0ac, x, y, "/", 0, 1.0f, 1.0f);
        x += text->GetWidth("/", 0);
        if (choice == 1) {
            normal = DAT_00508c0c;
        }
        text->color = normal;
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0x23), 0, 1.0f, 1.0f);
        text->color = DAT_00508c04;
        FUN_00435430(*((MouseInput*)DAT_0056b0c0)->GetPosition());
        DAT_0056b0ac->EndScene();
        DAT_0056b0ac->Present();
    }
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
    DAT_0056b0bc->ClearKeys();
    return choice == 0;
}

// ---- Group G14 (draft GE) ----

// rallytrophy.h already declares an opaque `Game`; this module-local view is the one the parent merges into class Game.
class G14_Game : public MenuGame {
public:
    int FUN_004368a0(MenuScreen* screen); // 0x4368a0: Yes/No confirmation dialog (1 = confirmed)
    void FUN_00436ed0();               // 0x436ed0: draws the dialog background
    void FUN_00437040(int noPresent);  // 0x437040
    BYTE unk1390c8[0x139470 - 0x1390c8];
    UIRect* yesRect;                   // 0x139470
    UIRect* noRect;                    // 0x139474
};

extern unsigned char DAT_0057ba35, DAT_0057ba43, DAT_0057ba4d, DAT_0057ba4f;

// The mouse-warp temporary: inline constructor and empty destructor.
struct G14_WarpPoint {
    G14_WarpPoint(float px, float py)
    {
        x = px;
        y = py;
    }
    ~G14_WarpPoint() {}
    float x, y;
};

// std::string layout twin: the constructor stays out of line (_Tidy 0x402700, allocator byte read), the destructor
// is expanded inline.
class G14_Str {
public:
    G14_Str(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        str() = s;
    }
    ~G14_Str()
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
    std::string& str() { return *(std::string*)this; }
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    // operator+=(const char*): _Grow and _Eos as calls
    void Append(const char* s)
    {
        unsigned int count = strlen(s);
        if (std::string::npos - len <= count) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < count && Grow(n = count + len, false)) {
            std::char_traits<char>::copy(ptr + len, s, count);
            Eos(n);
        }
    }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

// original: FUN_004368a0
// FUNCTION: RALLYTROPHY 0x4368a0
int G14_Game::FUN_004368a0(MenuScreen* screen)
{
    float x, y;
    int wait = 0;
    int sel = 1;
    int first = 1;
    Color color;
    DAT_0056b0bc->ClearKeys();
    while (1) {
        if (!FUN_004b69f0(wait)) {
            break;
        }
        Update();
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
        }
        if (DAT_0057ba4d && sel == 1) {
            sel = 0;
            float wx = DAT_0056aea8 * 496.0f;
            float wy = DAT_0056aeac * 400.0f;
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&G14_WarpPoint(wx, wy));
            DAT_0056b0bc->ClearKeys();
            Play(0);
        }
        if (DAT_0057ba4f && sel == 0) {
            sel = 1;
            float wx = DAT_0056aea8 * 528.0f;
            float wy = DAT_0056aeac * 400.0f;
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&G14_WarpPoint(wx, wy));
            DAT_0056b0bc->ClearKeys();
            Play(0);
        }
        if (yesRect->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && sel == 1) {
            sel = 0;
            Play(0);
        } else if (noRect->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition()) && sel == 0) {
            sel = 1;
            Play(0);
        }
        if (DAT_0057ba35 || DAT_0057ba20) {
            Play(1);
            break;
        }
        if (DAT_0057ba43) {
            sel = 1;
            Play(1);
            break;
        }
        if (screen) {
            screen->Slot3(1);
        } else {
            FUN_00437040(1);
        }
        if (!DAT_0056b0ac->BeginScene()) {
            continue;
        }
        DAT_0056b0ac->vf38(0);
        FUN_00436ed0();
        y = DAT_0056aeac * 350.0f;
        color.value = DAT_00508c04;
        TextRenderer* t = text;
        t->color = color.value;
        x = DAT_0056aea8 * 512.0f - t->GetWidth(FUN_00405380(0x21), 0) * 0.5f;
        t->Print(DAT_0056b0ac, x, y, FUN_00405380(0x21), 0, 1.0f, 1.0f);
        G14_Str line(FUN_00405380(0x22));
        line.Append("/");
        line.Append(FUN_00405380(0x23));
        y = DAT_0056aeac * 387.0f;
        const char* p = line.str().c_str();
        x = DAT_0056aea8 * 512.0f - t->GetWidth(p, 0) * 0.5f;
        if (sel == 0) {
            color.value = DAT_00508c0c;
        }
        t->color = color.value;
        t->Print(DAT_0056b0ac, x, y, FUN_00405380(0x22), 0, 1.0f, 1.0f);
        color.value = DAT_00508c04;
        t->color = color.value;
        x = x + t->GetWidth(FUN_00405380(0x22), 0);
        t->Print(DAT_0056b0ac, x, y, "/", 0, 1.0f, 1.0f);
        x = x + t->GetWidth("/", 0);
        if (sel == 1) {
            color.value = DAT_00508c0c;
        }
        t->color = color.value;
        t->Print(DAT_0056b0ac, x, y, FUN_00405380(0x23), 0, 1.0f, 1.0f);
        color.value = DAT_00508c04;
        t->color = color.value;
        FUN_00435430(*((MouseInput*)DAT_0056b0c0)->GetPosition());
        DAT_0056b0ac->EndScene();
        DAT_0056b0ac->Present();
    }
    DAT_0056b0bc->ClearKeys();
    DAT_0057ba20 = 0;
    DAT_0057ba24 = 0;
    return sel == 0;
}

// ---- Group G17 (draft GH) ----


void __fastcall G17_437cc0(GameView* game, float x, float y, float size);

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x436ed0
// PE instruction transcription; preserve point-array lifetimes and x87 order.
void __fastcall G17_436ed0(GameView* game, void* edx)
{
    extern void __stdcall FUN_004c11f4();
    extern void __stdcall FUN_004c1116();
    extern void __fastcall FUN_00490ad0();
    extern void __cdecl FUN_00402c90();
    extern float DAT_004e14e0;
    extern float DAT_004e16d0;
    extern float DAT_004e16d4;
    extern float DAT_004e16d8;
    extern float DAT_0056aea8;
    extern float DAT_0056aeac;
    __asm {
        push   ebp
        mov    ebp,esp
        and    esp,0fffffff8h
        push   0ffffffffh
        push   04d817eh
        mov    eax,dword ptr fs:[0]
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,048h
        push   esi
        push   edi
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push   04h
        push   08h
        lea    eax,[esp+040h]
        push   eax
        mov    esi,ecx
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push   04h
        push   08h
        lea    ecx,[esp+020h]
        push   ecx
        mov    dword ptr [esp+06ch],00h
        call FUN_004c11f4
        fld    dword ptr [DAT_0056aea8]
        fmul   dword ptr [DAT_004e14e0]
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    eax,dword ptr [esi+01390f4h]
        mov    dword ptr [esp+048h],00h
        fst    dword ptr [esp+028h]
        mov    dword ptr [esp+030h],00h
        fstp   dword ptr [esp+010h]
        mov    dword ptr [esp+03ch],00h
        fld    dword ptr [DAT_0056aeac]
        mov    dword ptr [esp+034h],00h
        fmul   dword ptr [DAT_004e16d8]
        mov    dword ptr [esp+040h],03f800000h
        mov    dword ptr [esp+038h],03f800000h
        mov    dword ptr [esp+04ch],03f800000h
        fst    dword ptr [esp+01ch]
        mov    dword ptr [esp+044h],03f800000h
        fstp   dword ptr [esp+014h]
        or     edi,0ffffffffh
        fld    dword ptr [DAT_0056aea8]
        push   eax
        fmul   dword ptr [DAT_004e16d4]
        mov    byte ptr [esp+05ch],02h
        mov    dword ptr [esp+010h],edi
        fst    dword ptr [esp+024h]
        fstp   dword ptr [esp+01ch]
        fld    dword ptr [DAT_0056aeac]
        fmul   dword ptr [DAT_004e16d0]
        fst    dword ptr [esp+030h]
        fstp   dword ptr [esp+028h]
        mov    edx,dword ptr [ecx]
        call   dword ptr [edx+08ch]
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    edx,dword ptr [ecx]
        push   03f800000h
        push   03f800000h
        push   edi
        lea    eax,[esp+03ch]
        push   eax
        lea    eax,[esp+020h]
        push   eax
        call   dword ptr [edx+0108h]
        push offset FUN_00402c90
        push   04h
        push   08h
        lea    ecx,[esp+01ch]
        push   ecx
        mov    byte ptr [esp+068h],00h
        call FUN_004c1116
        push offset FUN_00402c90
        push   04h
        push   08h
        lea    edx,[esp+03ch]
        push   edx
        mov    dword ptr [esp+068h],edi
        call FUN_004c1116
        mov    ecx,dword ptr [esp+050h]
        pop    edi
        mov    dword ptr fs:[0],ecx
        pop    esi
        mov    esp,ebp
        pop    ebp
        ret
    }
}

// ---- Group G15 (draft GF) ----

// Module-local view of the Game object (cast from this; the real class is in rallytrophy.h).
class GameDlgView {
public:
    void Play(int index) { ((GameSound*)((BYTE*)this + 0x139224 + index * 0x70))->FUN_0049b730(); }
    BYTE unk0[0x1390c4];
    TextRenderer* smallText;           // 0x1390c4
    BYTE unk1390c8[0x139470 - 0x1390c8];
    UIRect* yesRect;                   // 0x139470
    UIRect* noRect;                    // 0x139474
    BYTE unk139478[0x139484 - 0x139478];
    int confirmSel;                    // 0x139484: 0 = first answer (yes), 1 = second
};

// The mouse-warp temporary: inline constructor and empty destructor (see FA2_Point in unk_44b930.cpp).
// Note the parameter order: VC6 evaluates the arguments right to left, so passing (y, x) puts x on the x87 stack first
// and y has to be spilled to a stack slot, as in the original.
struct G15_Point {
    G15_Point(float py, float px)
    {
        x = px;
        y = py;
    }
    ~G15_Point() {}
    float x, y;
};

// A std::string whose _Tidy/_Grow/_Eos stay out of line: constructor from a literal, append and an expanded destructor.
struct G15_Str {
    G15_Str(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    ~G15_Str()
    {
        if (ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, res + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, res = 0;
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
                Eos(total);
            }
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    static void Xlen();                // 0x4bd00b
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

// NONMATCHING: RALLYTROPHY 0x437220
// 1384 vs 1346 bytes. All call sequences, EH states (Color at function scope = state 0, two mouse-warp temporaries,
// string = 3), the string expansion and the float spill (G15_Point takes (y, x): VC6 then spills y) match. Left: the
// frame is 0x2c (original 0x28: the warp temporary should overlap the float locals), sel/y2 swap esi/ebx, and the
// two inline-destructor returns each get their own epilogue (original jumps to one shared epilogue).
// original: FUN_00437220 (fastcall; confirmation dialog, -1 while open, else the chosen answer)
int Game::FUN_00437220(int a)
{
    GameDlgView* v = (GameDlgView*)this;
    int closed = 0;
    Color color;
    if (DAT_0057ba28[VK_LEFT] && v->confirmSel == 1) {
        v->confirmSel = 0;
        ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&G15_Point(DAT_0056aeac * 400.0f, DAT_0056aea8 * 496.0f));
        DAT_0056b0bc->ClearKeys();
    }
    if (DAT_0057ba28[VK_RIGHT] && v->confirmSel == 0) {
        v->confirmSel = 1;
        ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&G15_Point(DAT_0056aeac * 400.0f, DAT_0056aea8 * 528.0f));
        DAT_0056b0bc->ClearKeys();
    }
    if (v->yesRect->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
        v->confirmSel = 0;
    } else if (v->noRect->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
        v->confirmSel = 1;
    }
    if (DAT_0057ba28[VK_RETURN] || DAT_0057ba20) {
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
        DAT_0056b0bc->ClearKeys();
        v->Play(1);
        closed = 1;
    }
    if (DAT_0057ba28[VK_ESCAPE]) {
        v->confirmSel = 1;
        v->Play(1);
        closed = 1;
        DAT_0056b0bc->ClearKeys();
    }
    FUN_00436ed0();
    TextRenderer* text = v->smallText;
    float y = DAT_0056aeac * 350.0f;
    color.value = DAT_00508c04;
    text->color = color.value;
    float x = DAT_0056aea8 * 512.0f;
    float left = x - text->GetWidth(FUN_00405380(0x21), 0) * 0.5f;
    text->Print(DAT_0056b0ac, left, y, FUN_00405380(0x21), 0, 1.0f, 1.0f);
    G15_Str both(FUN_00405380(0x22));
    both.Append("/");
    both.Append(FUN_00405380(0x23));
    float y2 = DAT_0056aeac * 387.0f;
    x = DAT_0056aea8 * 512.0f - text->GetWidth(both.c_str(), 0) * 0.5f;
    int sel = v->confirmSel;
    if (sel == 0) {
        color.value = DAT_00508c0c;
    }
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y2, FUN_00405380(0x22), 0, 1.0f, 1.0f);
    color.value = DAT_00508c04;
    text->color = color.value;
    float xs = x + text->GetWidth(FUN_00405380(0x22), 0);
    text->Print(DAT_0056b0ac, xs, y2, "/", 0, 1.0f, 1.0f);
    x += text->GetWidth("/", 0);
    if (sel == 1) {
        color.value = DAT_00508c0c;
    }
    text->color = color.value;
    text->Print(DAT_0056b0ac, x, y2, FUN_00405380(0x23), 0, 1.0f, 1.0f);
    text->color = DAT_00508c04;

    if (closed) {
        return sel;
    }
    return -1;
}

// ---- Group G16 (draft GG) ----

extern unsigned char DAT_0057ba35, DAT_0057ba43, DAT_0057ba4d, DAT_0057ba4f;

class G16_Game {
public:
    void FUN_00436ed0();                // 0x436ed0
    int FUN_00437770(int unused);       // 0x437770: second modal confirmation (returns the choice, -1 while open)
    void Play(int index) { ((GameSound*)((BYTE*)this + 0x139224 + index * 0x70))->FUN_0049b730(); }

    BYTE unk0[0x1390c4];
    TextRenderer* smallText;            // 0x1390c4
    BYTE unk1390c8[0x139224 - 0x1390c8];
    BYTE sounds[0x139470 - 0x139224];   // 0x139224: four SoundSources (0x70 each), then other objects
    UIRect* yesRect;                    // 0x139470
    UIRect* noRect;                     // 0x139474
    BYTE unk139478[0x139484 - 0x139478];
    int selected;                       // 0x139484: 0 = first answer, 1 = second
};

// The mouse-warp temporary: inline constructor and empty destructor.
struct G16_WarpPoint {
    G16_WarpPoint(float py, float px)
    {
        x = px;
        y = py;
    }
    ~G16_WarpPoint() {}
    float x, y;
};

// std::string as the original expands it here: out-of-line _Tidy (0x402700), _Grow (0x402980), _Eos (0x402960); the
// destructor is _Tidy(true) inline.
class G16_String {
public:
    G16_String(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    ~G16_String() { Free(); }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free()
    {
        if (ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);
    bool Grow(unsigned int n, bool trim);
    void Eos(unsigned int n);
    void Append(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            std::_Xlen();
        }
        if (0 < n) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                Eos(total);
            }
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

// NONMATCHING: RALLYTROPHY 0x437770
// 1336 vs 1352 bytes; everything but the tail matches. Open: the original keeps the choice in esi and y in ebx (mine
// the reverse), and expands the string destructor in both return paths behind one shared epilogue (`return` inside
// the string's scope duplicates the epilogue: 1390 bytes). The braces make x/y share the mouse-warp temporary's slots.
// Second modal confirmation (two answers "first/second" drawn as `a/b` under a title; text 0x19d, 0x22, 0x23): the
// arrow keys 0x57ba4d/0x57ba4f move the selection and warp the mouse, the rectangles at +0x139470/+0x139474 select by
// hover, 0x57ba35/left click or 0x57ba43 confirm. Returns the answer, or -1 while the dialog stays open.
int G16_Game::FUN_00437770(int unused)
{
    int pressed = 0;
    Color color;
    if (DAT_0057ba4d && selected == 1) {
        selected = 0;
        ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&G16_WarpPoint(DAT_0056aeac * 400.0f, DAT_0056aea8 * 496.0f));
        DAT_0056b0bc->ClearKeys();
    }
    if (DAT_0057ba4f && selected == 0) {
        selected = 1;
        ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&G16_WarpPoint(DAT_0056aeac * 400.0f, DAT_0056aea8 * 528.0f));
        DAT_0056b0bc->ClearKeys();
    }
    if (yesRect->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
        selected = 0;
    } else if (noRect->Contains(*((MouseInput*)DAT_0056b0c0)->GetPosition())) {
        selected = 1;
    }
    if (DAT_0057ba35 || DAT_0057ba20) {
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
        DAT_0056b0bc->ClearKeys();
        Play(1);
        pressed = 1;
    }
    if (DAT_0057ba43) {
        selected = 1;
        Play(1);
        pressed = 1;
        DAT_0056b0bc->ClearKeys();
    }
    FUN_00436ed0();
    TextRenderer* t = smallText;
    {
    float y = DAT_0056aeac * 350.0f;
    color.value = DAT_00508c04;
    float x = DAT_0056aea8 * 512.0f;
    t->color = color.value;
    x -= t->GetWidth(FUN_00405380(0x19d), 0) * 0.5f;
    t->Print(DAT_0056b0ac, x, y, FUN_00405380(0x19d), 0, 1.0f, 1.0f);
    G16_String text(FUN_00405380(0x22));
    text.Append("/");
    text.Append(FUN_00405380(0x23));
    y = DAT_0056aeac * 387.0f;
    x = DAT_0056aea8 * 512.0f - t->GetWidth(text.c_str(), 0) * 0.5f;
    int sel = selected;
    if (sel == 0) {
        color.value = DAT_00508c0c;
    }
    t->color = color.value;
    t->Print(DAT_0056b0ac, x, y, FUN_00405380(0x22), 0, 1.0f, 1.0f);
    color.value = DAT_00508c04;
    t->color = color.value;
    x += t->GetWidth(FUN_00405380(0x22), 0);
    t->Print(DAT_0056b0ac, x, y, "/", 0, 1.0f, 1.0f);
    x += t->GetWidth("/", 0);
    if (sel == 1) {
        color.value = DAT_00508c0c;
    }
    t->color = color.value;
    t->Print(DAT_0056b0ac, x, y, FUN_00405380(0x23), 0, 1.0f, 1.0f);
    t->color = DAT_00508c04;
    }
    if (pressed) {
        return selected;
    }
    return -1;
}

// ---- Group G17 (draft GH, continued) ----
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x437cc0
// PE instruction transcription; preserve point-array lifetimes and x87 order.
void __fastcall G17_437cc0(GameView* game, float x, float y, float size)
{
    extern void __stdcall FUN_004c11f4();
    extern void __stdcall FUN_004c1116();
    extern void __fastcall FUN_00490ad0();
    extern void __cdecl FUN_00402c90();
    extern float DAT_004e14c0;
    extern float DAT_0056aeac;
    __asm {
        push   ebp
        mov    ebp,esp
        and    esp,0fffffff8h
        push   0ffffffffh
        push   04d824eh
        mov    eax,dword ptr fs:[0]
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,048h
        push   esi
        push   edi
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push   04h
        push   08h
        lea    eax,[esp+040h]
        push   eax
        mov    esi,ecx
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push   04h
        push   08h
        lea    ecx,[esp+020h]
        push   ecx
        mov    dword ptr [esp+06ch],00h
        call FUN_004c11f4
        fld    dword ptr [DAT_0056aeac]
        fmul   dword ptr [DAT_004e14c0]
        mov    edx,dword ptr [ebp+08h]
        fld    dword ptr [ebp+010h]
        mov    ecx,dword ptr [ebp+0ch]
        fadd   dword ptr [ebp+08h]
        mov    eax,edx
        mov    dword ptr [esp+028h],edx
        fst    dword ptr [esp+020h]
        mov    edx,ecx
        fstp   dword ptr [esp+018h]
        mov    dword ptr [esp+01ch],ecx
        mov    ecx,dword ptr [DAT_0056b0ac]
        fadd   dword ptr [ebp+0ch]
        mov    dword ptr [esp+014h],edx
        mov    edx,dword ptr [esi+01390dch]
        mov    dword ptr [esp+048h],03e4ccccdh
        fst    dword ptr [esp+02ch]
        mov    dword ptr [esp+030h],03e4ccccdh
        fstp   dword ptr [esp+024h]
        mov    dword ptr [esp+03ch],00h
        mov    dword ptr [esp+034h],00h
        mov    dword ptr [esp+040h],03e99999ah
        mov    dword ptr [esp+038h],03e99999ah
        mov    dword ptr [esp+04ch],03f800000h
        mov    dword ptr [esp+044h],03f800000h
        mov    dword ptr [esp+010h],eax
        mov    eax,dword ptr [ecx]
        or     edi,0ffffffffh
        push   edx
        mov    byte ptr [esp+05ch],02h
        mov    dword ptr [esp+010h],edi
        call   dword ptr [eax+08ch]
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    eax,dword ptr [ecx]
        push   03f800000h
        push   03f800000h
        push   edi
        lea    edx,[esp+03ch]
        push   edx
        lea    edx,[esp+020h]
        push   edx
        call   dword ptr [eax+0108h]
        push offset FUN_00402c90
        push   04h
        push   08h
        lea    eax,[esp+01ch]
        push   eax
        mov    byte ptr [esp+068h],00h
        call FUN_004c1116
        push offset FUN_00402c90
        push   04h
        push   08h
        lea    ecx,[esp+03ch]
        push   ecx
        mov    dword ptr [esp+068h],edi
        call FUN_004c1116
        mov    ecx,dword ptr [esp+050h]
        mov    dword ptr fs:[0],ecx
        pop    edi
        pop    esi
        mov    esp,ebp
        pop    ebp
        ret    0ch
    }
}

// FUNCTION: RALLYTROPHY 0x437e20
void __fastcall G17_437e20(GameView* game, void* edx, float x, float y, float size, float step, int count)
{
    float cy = y;
    for (int i = 0; i < count; ++i) {
        G17_437cc0(game, x, cy, size);
        cy += step;
    }
}

// ---- module end (G1): the STL header statics' guard initialiser and atexit registration ----
// The numbers can shift if the TU gains function-local statics or reference-bound temporaries.

// FUNCTION: RALLYTROPHY 0x437e70
// SYMBOL: _$E23
typedef void Generated_437e70();

// FUNCTION: RALLYTROPHY 0x437e90
// SYMBOL: _$E22
typedef void Generated_437e90();

// TextRenderer allocation layout (constructor at 0x4a9520).
class GameFont {
public:
    GameFont(RenderDevice* device, const char* name, float sx, float sy);
    BYTE data[0x150];
};

// FUNCTION: RALLYTROPHY 0x430cf0
void Game::FUN_00430cf0()
{
    if (!bigText) {
        bigText = (TextRenderer*)new GameFont(DAT_0056b0ac, "data/global/fonts/CounterFont", DAT_0056aea8, DAT_0056aeac);
        text1390bc = (TextRenderer*)new GameFont(DAT_0056b0ac, "data/global/fonts/mpFont16", DAT_0056aea8, DAT_0056aeac);
        text1390c0 = (TextRenderer*)new GameFont(DAT_0056b0ac, "data/global/fonts/mpfont20", DAT_0056aea8, DAT_0056aeac);
        smallText = (TextRenderer*)new GameFont(DAT_0056b0ac, "data/global/fonts/mpFont32", DAT_0056aea8, DAT_0056aeac);
        text = (TextRenderer*)new GameFont(DAT_0056b0ac, "data/global/fonts/mpfont32ariel", DAT_0056aea8, DAT_0056aeac);
    }
}

// FUNCTION: RALLYTROPHY 0x435de0
void Game::FUN_00435de0(float x, float y)
{
    Vec2 uvs[4], positions[4];
    Color color;
    positions[0].x = positions[3].x = DAT_0056aea8 * x;
    positions[0].y = positions[1].y = DAT_0056aeac * y;
    uvs[0].x = uvs[3].x = 0.0f;
    uvs[0].y = uvs[1].y = 0.0f;
    uvs[1].x = uvs[2].x = 1.0f;
    uvs[2].y = uvs[3].y = 1.0f;
    color.value = 0xffffffff;
    positions[1].x = positions[2].x = (x + 512.0f) * DAT_0056aea8;
    positions[2].y = positions[3].y = (y + 512.0f) * DAT_0056aeac;
    DAT_0056b0ac->vf35((Sprite*)unk1390ec[0]);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)positions, (Vec2*)uvs, color, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x435f50
void Game::FUN_00435f50(float x, float y)
{
    Vec2 uvs[4], positions[4];
    Color color;
    positions[0].x = positions[3].x = DAT_0056aea8 * x;
    positions[0].y = positions[1].y = DAT_0056aeac * y;
    uvs[0].x = uvs[3].x = 0.0f;
    uvs[0].y = uvs[1].y = 0.0f;
    uvs[1].x = uvs[2].x = 1.0f;
    uvs[2].y = uvs[3].y = 1.0f;
    color.value = 0xffffffff;
    positions[1].x = positions[2].x = (x + 512.0f) * DAT_0056aea8;
    positions[2].y = positions[3].y = (y + 128.0f) * DAT_0056aeac;
    DAT_0056b0ac->vf35((Sprite*)unk1390ec[1]);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)positions, (Vec2*)uvs, color, 1.0f, 1.0f);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4360c0
// PE instruction transcription; preserve point-array lifetimes and x87 order.
void Game::FUN_004360c0(float x, float y, float w, float h, unsigned int flag)
{
    extern void __stdcall FUN_004c11f4();
    extern void __stdcall FUN_004c1116();
    extern void __fastcall FUN_00490ad0();
    extern void __cdecl FUN_00402c90();
    extern float DAT_004e1348;
    extern float DAT_004e13a4;
    extern float DAT_004e1660;
    extern float DAT_0056aea8;
    extern float DAT_0056aeac;
    __asm {
        push   ebp
        mov    ebp,esp
        and    esp,0fffffff8h
        push   0ffffffffh
        push   04d80deh
        mov    eax,dword ptr fs:[0]
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,050h
        push   esi
        push   edi
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push   04h
        push   08h
        lea    eax,[esp+048h]
        push   eax
        mov    esi,ecx
        call FUN_004c11f4
        push offset FUN_00402c90
        push offset FUN_00490ad0
        push   04h
        push   08h
        lea    ecx,[esp+028h]
        push   ecx
        mov    dword ptr [esp+074h],00h
        call FUN_004c11f4
        mov    eax,dword ptr [ebp+018h]
        mov    edx,eax
        and    edx,03h
        mov    dword ptr [esp+0ch],edx
        fild   dword ptr [esp+0ch]
        sar    eax,02h
        mov    dword ptr [esp+010h],eax
        mov    eax,dword ptr [ebp+08h]
        fld    st(0)
        mov    edx,dword ptr [ebp+0ch]
        fmul   dword ptr [DAT_004e13a4]
        mov    ecx,eax
        mov    dword ptr [esp+030h],eax
        mov    eax,edx
        fst    dword ptr [esp+050h]
        mov    dword ptr [esp+018h],ecx
        fstp   dword ptr [esp+038h]
        mov    ecx,dword ptr [DAT_0056b0ac]
        fild   dword ptr [esp+010h]
        mov    dword ptr [esp+01ch],eax
        mov    eax,dword ptr [esi+01390f8h]
        or     edi,0ffffffffh
        fst    dword ptr [esp+0ch]
        mov    dword ptr [esp+024h],edx
        fmul   dword ptr [DAT_004e1660]
        push   eax
        mov    byte ptr [esp+064h],02h
        mov    dword ptr [esp+018h],edi
        fst    dword ptr [esp+048h]
        fstp   dword ptr [esp+040h]
        fadd   dword ptr [DAT_004e1348]
        fmul   dword ptr [DAT_004e13a4]
        fst    dword ptr [esp+044h]
        fstp   dword ptr [esp+04ch]
        fld    dword ptr [esp+010h]
        fadd   dword ptr [DAT_004e1348]
        fmul   dword ptr [DAT_004e1660]
        fst    dword ptr [esp+058h]
        fstp   dword ptr [esp+050h]
        fld    dword ptr [DAT_0056aea8]
        fmul   dword ptr [ebp+010h]
        fadd   dword ptr [ebp+08h]
        fst    dword ptr [esp+024h]
        fstp   dword ptr [esp+02ch]
        fld    dword ptr [DAT_0056aeac]
        fmul   dword ptr [ebp+014h]
        fadd   dword ptr [ebp+0ch]
        fst    dword ptr [esp+038h]
        fstp   dword ptr [esp+030h]
        mov    edx,dword ptr [ecx]
        call   dword ptr [edx+08ch]
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    edx,dword ptr [ecx]
        push   03f800000h
        push   03f800000h
        push   edi
        lea    eax,[esp+044h]
        push   eax
        lea    eax,[esp+028h]
        push   eax
        call   dword ptr [edx+0108h]
        push offset FUN_00402c90
        push   04h
        push   08h
        lea    ecx,[esp+024h]
        push   ecx
        mov    byte ptr [esp+070h],00h
        call FUN_004c1116
        push offset FUN_00402c90
        push   04h
        push   08h
        lea    edx,[esp+044h]
        push   edx
        mov    dword ptr [esp+070h],edi
        call FUN_004c1116
        mov    ecx,dword ptr [esp+058h]
        pop    edi
        mov    dword ptr fs:[0],ecx
        pop    esi
        mov    esp,ebp
        pop    ebp
        ret    014h
    }
}

class GameMenuFont : public TextRenderer {
public:
    __forceinline void SetMenuColor(Color c) { color = c.value; }
};
extern DWORD DAT_00508bfc;
extern "C" void __cdecl RT_Menu_004329e0();
#pragma comment(linker, "/alternatename:_RT_Menu_004329e0=?GetScale@MenuGame@@QAEPAVHudPoint@@XZ")
extern "C" void __cdecl RT_Menu_00432a10();
#pragma comment(linker, "/alternatename:_RT_Menu_00432a10=?UpdateVideo@MenuGame@@QAEXXZ")
extern "C" void __cdecl RT_Menu_00432a40();
#pragma comment(linker, "/alternatename:_RT_Menu_00432a40=?FUN_00432a40@MenuGame@@QAEXXZ")
extern "C" void __cdecl RT_Menu_00432a60();
#pragma comment(linker, "/alternatename:_RT_Menu_00432a60=?DrawVideo@MenuGame@@QAEXH@Z")
extern "C" void __cdecl RT_Menu_00435b60();
#pragma comment(linker, "/alternatename:_RT_Menu_00435b60=?FUN_00435b60@Game@@QAEXHHHHHM@Z")
extern "C" void __cdecl RT_Menu_00435430();
#pragma comment(linker, "/alternatename:_RT_Menu_00435430=?FUN_00435430@Game@@QAEXMM@Z")

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x437040
// PE instruction transcription; preserve temporary lifetimes, x87 order and loop registers.
void Game::FUN_00437040(int noPresent)
{
    extern void __cdecl FUN_004a97a0();
    extern HudPoint* __stdcall GetMousePosition();
    extern float DAT_004f75e8, DAT_004f75e4;
    extern char DAT_004e1344[];
    __asm {
        push   0ffffffffh
        push   04d81a8h
        mov    eax,dword ptr fs:[0]
        push   eax
        mov    dword ptr fs:[0],esp
        sub    esp,014h
        push   ebx
        push   ebp
        push   esi
        push   edi
        mov    ebp,ecx
        call RT_Menu_004329e0
        mov    ecx,dword ptr [eax]
        mov    edx,dword ptr [eax+04h]
        mov    dword ptr [esp+014h],ecx
        mov    dword ptr [esp+018h],edx
        mov    esi,dword ptr [ebp+01390c4h]
        xor    ebx,ebx
        mov    ecx,ebp
        mov    dword ptr [esp+02ch],ebx
        call RT_Menu_00432a10
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    eax,dword ptr [ecx]
        call   dword ptr [eax+06ch]
        test   eax,eax
        je menu_draw_4371fe
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    edx,dword ptr [ecx]
        push   ebx
        call   dword ptr [edx+098h]
        mov    ecx,ebp
        call RT_Menu_00432a40
        push   01h
        mov    ecx,ebp
        call RT_Menu_00432a60
        fld    dword ptr [DAT_0056aea8]
        mov    eax,[DAT_00508bfc]
        fadd   st(0),st
        push   03f800000h
        push   03f800000h
        fstp   dword ptr [esi+01ch]
        push   ebx
        fld    dword ptr [DAT_0056aeac]
        push   031h
        fadd   st(0),st
        mov    dword ptr [esi+0120h],eax
        fstp   dword ptr [esi+020h]
        call FUN_00405380
        fld    dword ptr [DAT_004f75e8]
        add    esp,04h
        fmul   dword ptr [DAT_0056aeac]
        push   eax
        mov    eax,[DAT_0056b0ac]
        push   ecx
        fstp   dword ptr [esp]
        push   ecx
        fld    dword ptr [DAT_004f75e4]
        mov    ecx,esi
        fmul   dword ptr [DAT_0056aea8]
        fstp   dword ptr [esp]
        push   eax
        call FUN_004a97a0
        fld    dword ptr [DAT_0056aea8]
        fstp   dword ptr [esi+01ch]
        fld    dword ptr [DAT_0056aeac]
        mov    dword ptr [esp+01ch],03f800000h
        fstp   dword ptr [esi+020h]
        mov    dword ptr [esp+020h],03f800000h
        lea    edi,[ebp+0139114h]
    menu_draw_437141:
        mov    edx,dword ptr [ebp+0139078h]
        xor    eax,eax
        cmp    ebx,edx
        sete   al
        test   eax,eax
        mov    eax,[DAT_00508c0c]
        mov    byte ptr [esp+02ch],02h
        jne menu_draw_437161
        mov    eax,[DAT_00508c04]
    menu_draw_437161:
        mov    dword ptr [esp+010h],eax
        mov    dword ptr [esi+0120h],eax
        mov    eax,dword ptr [edi]
        test   eax,eax
        jne menu_draw_437176
        mov eax, offset DAT_004e1344
    menu_draw_437176:
        mov    ecx,dword ptr [edi+018h]
        mov    edx,dword ptr [edi+014h]
        push   03f800000h
        push   03f800000h
        push   00h
        push   eax
        mov    eax,[DAT_0056b0ac]
        push   ecx
        push   edx
        push   eax
        mov    ecx,esi
        call FUN_004a97a0
        inc    ebx
        add    edi,024h
        cmp    ebx,07h
        mov    byte ptr [esp+02ch],00h
        jl menu_draw_437141
        push   0bf800000h
        push   00h
        push   01h
        push   01h
        push   06h
        push   01h
        mov    ecx,ebp
        call RT_Menu_00435b60
        mov    ecx,dword ptr [DAT_0056b0c0]
        call GetMousePosition
        mov    edx,dword ptr [eax]
        mov    eax,dword ptr [eax+04h]
        sub    esp,08h
        mov    ecx,esp
        mov    dword ptr [ecx],edx
        mov    dword ptr [ecx+04h],eax
        mov    ecx,ebp
        mov    dword ptr [esp+018h],esp
        call RT_Menu_00435430
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    edx,dword ptr [ecx]
        call   dword ptr [edx+070h]
        cmp    dword ptr [esp+034h],01h
        je menu_draw_4371fe
        mov    ecx,dword ptr [DAT_0056b0ac]
        mov    eax,dword ptr [ecx]
        call   dword ptr [eax+034h]
    menu_draw_4371fe:
        mov    ecx,dword ptr [esp+024h]
        pop    edi
        pop    esi
        pop    ebp
        mov    dword ptr fs:[0],ecx
        pop    ebx
        add    esp,020h
        ret    04h
    }
}
