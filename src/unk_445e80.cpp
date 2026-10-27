// ServiceMenu (0x445e80-0x44b92f): the rally service-area screen. Map: analysis/range-445e80.md; class in
// include/unk_445e80.h.
// Work groups: SM-A (ctor, dtors, Release, UnloadTextures, module end: prefix SMA_), and the other eight groups.
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_445e80.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include "unk_44ea60.h"

// ---- Group SM-A ----
// The repairtimes.ini key table (12 integer keys).
ConfigKey SMA_Keys[12] = {
    { "TuneTime_Tyres", 1 },
    { "TuneTime_Gearbox", 1 },
    { "TuneTime_Suspension", 1 },
    { "TuneTime_BrakeBalance", 1 },
    { "TuneTime_Steering", 1 },
    { "RepairTime_Body", 1 },
    { "RepairTime_Gearbox", 1 },
    { "RepairTime_Engine", 1 },
    { "RepairTime_Driveshaft", 1 },
    { "RepairTime_Brakes", 1 },
    { "RepairTime_Suspension", 1 },
    { "RepairTime_Steering", 1 },
};

// The inline Sprite set-up of the constructor (the same field writes as the ping-indicator sprite).
#define SMA_SPRITE(s, last) \
    (s).unk9c = (last); \
    (s).unk98 = 15; \
    (s).unk60[0] = 1.0f; \
    (s).unk60[1] = 1.0f; \
    (s).unk60[2] = 1.0f; \
    (s).unk60[3] = 1.0f; \
    (s).unk40[0] = 1.0f; \
    (s).unk40[1] = 1.0f; \
    (s).unk40[2] = 1.0f; \
    (s).unk40[3] = 1.0f; \
    (s).unk18 = 5; \
    (s).unk1c = 6; \
    (s).unk24 = 2; \
    (s).unk20 = 2;


// ---- from group draft V2 ----

// ---- from group draft V3 ----
extern char DAT_005070b0[];            // global text buffer appended first to every stage's surface string

// A view of std::string whose assign(const char*) is expanded by hand at two sites (VC6 spends a small inline budget
// per function): AssignOut calls _Grow and _Eos (0x402960), AssignInline calls _Grow and writes the terminator inline.
struct C_Str {
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


// ---- from group draft V4 ----
struct SMD_Point {
    SMD_Point() {}
    SMD_Point(const SMD_Point& other); // 0x424300
    ~SMD_Point() {}
    float x, y;
};
// The HUD rectangle as BuildRects calls it: constructor 0x4390a0 out of line.
class SMD_Rect {
public:
    SMD_Rect(SMD_Point topLeft, HudPoint bottomRight); // 0x4390a0
    int f[5];
};

// Layout values of the 1024x768 design space. The original keeps these in .data (0x4f9f94..0x4f9fa8), so they are
// plain (non-const) globals here too.
static float SMD_TABX = 52.0f;      // 0x4f9f94: tab column x
static float SMD_ROWX = 339.0f;     // 0x4f9f98: repair row x
static float SMD_TUNEX = 598.0f;    // 0x4f9f9c: tune cell x
static float SMD_TUNEY = 552.0f;    // 0x4f9fa0: tune cell y
static float SMD_PITCH = 42.0f;     // 0x4f9fa4: row pitch
static float SMD_TOP = 262.0f;      // 0x4f9fa8: first tab row
extern int DAT_0050145c;


// ---- from group draft V5 ----
extern int DAT_0050145c;
typedef char SME_SizeCheck[sizeof(ServiceMenu) == 0x11a0 ? 1 : -1];

// ---- Group SM-E: Frame ----
// The mouse-warp temporary: inline constructor and empty destructor (see C_WarpPoint in unk_44ea60.cpp).
struct SME_WarpPoint {
    SME_WarpPoint(float px, float py)
    {
        x = px;
        y = py;
    }
    ~SME_WarpPoint() {}
    float x, y;
};
// The game object's confirmation dialog (0x4368a0).
// Layout constants of the 1024x768 design space (the original does not fold the sum).
static const float SME_ROWY = 552.0f;
static const float SME_ROWPAD = 16.0f;
static const float SME_COLX = 598.0f;
static const float SME_COLW = 400.0f;
class SME_Game : public MenuGame {
public:
    int FUN_004368a0(MenuScreen* screen);
};


// ---- from group draft V6 ----
#define SMF_STR(off) (*(std::string*)((BYTE*)DAT_0056b0c8 + (off)))

// A std::string whose constructor stays out of line (_Tidy, 0x402700) while its destructor is expanded inline.
struct SMF_Str {
    SMF_Str(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~SMF_Str()
    {
        if (ptr) {
            BYTE ref = ((BYTE*)ptr)[-1];
            if (ref == 0 || ref == 0xff) {
                delete[] (ptr - 1);
            } else {
                ((BYTE*)ptr)[-1] = ref - 1;
            }
        }
    }
    void Tidy(bool built);             // 0x402700
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};


// ---- from group draft V7 ----

// ---- from group draft V8 ----
extern const int SMH_PartText[7];   // 0x4f9cf0
extern const int SMH_PartSprite[7]; // 0x4f9d0c
#define SMH_FONT2(game) (*(TextRenderer**)((BYTE*)(game) + 0x1390bc))


// ---- from group draft V9 ----
extern int DAT_00501334;               // human player count
struct SMI_Name {
    char text[30];
};
extern SMI_Name DAT_00501338[];        // player-name table (stride 30)
extern int DAT_0050145c;
extern int DAT_00501458;
static const float SMI_TITLEX = 54.0f; // 0x4f9f84
static const float SMI_TITLEY = 142.0f; // 0x4f9f88
static const float SMI_RALLYY = 630.0f; // 0x4f9f90
extern int DAT_004f9ca8[];             // tab text ids {120, 121, 122, 124, 23, 30}

// NONMATCHING: RALLYTROPHY 0x445e80
// Same size (1603) and same call/EH skeleton; only the store scheduling of the inline Sprite field writes differs: the
// original issues the 1.0f stores of the first sprites ahead of the mode/int blocks and delays the 5/6/2/0xf int stores
// (the sprite order damageIcons, damageBar, tuneSelectors, repair, tune, info, startRace, load is right). original: FUN_00445e80
ServiceMenu::ServiceMenu()
{
    m_selected = 0;
    mode = 0;
    unk24 = 0;
    unk28 = 0;
    unk2c = 0;
    totalTime = 60;
    timeLeft = 60;
    SMA_SPRITE(damageIcons, 2)
    SMA_SPRITE(damageBar, 2)
    SMA_SPRITE(tuneSelectors, 2)
    SMA_SPRITE(repairSprite, 2)
    SMA_SPRITE(tuneSprite, 2)
    SMA_SPRITE(infoMap, 14)
    SMA_SPRITE(startRace, 2)
    SMA_SPRITE(loadSprite, 2)
    for (int i = 0; i < 8; i++) {
        SMA_SPRITE(partSprites[i], 2)
    }
    for (int j = 0; j < 3; j++) {
        SMA_SPRITE(stageSprites[j], 14)
    }
    Config config;
    config.Define(SMA_Keys, 12);
    config.Load("data/menu/repairtimes.ini", true, false, 0);
    tuneTime[0] = config.GetInt("TuneTime_Tyres");
    tuneTime[1] = config.GetInt("TuneTime_Gearbox");
    tuneTime[2] = config.GetInt("TuneTime_Suspension");
    tuneTime[3] = config.GetInt("TuneTime_BrakeBalance");
    tuneTime[4] = config.GetInt("TuneTime_Steering");
    repairTime[0] = config.GetInt("RepairTime_Body");
    repairTime[1] = config.GetInt("RepairTime_Gearbox");
    repairTime[2] = config.GetInt("RepairTime_Engine");
    repairTime[3] = config.GetInt("RepairTime_Driveshaft");
    repairTime[4] = config.GetInt("RepairTime_Brakes");
    repairTime[5] = config.GetInt("RepairTime_Suspension");
    repairTime[6] = config.GetInt("RepairTime_Steering");
}

// FUNCTION: RALLYTROPHY 0x4464d0
// SYMBOL: ??_GServiceMenu@@UAEPAXI@Z
typedef void Generated_4464d0();

// FUNCTION: RALLYTROPHY 0x4464f0
ServiceMenu::~ServiceMenu()
{
}

// FUNCTION: RALLYTROPHY 0x4466b0
void ServiceMenu::Setup(MenuGame* game)
{
    if (m_state) {
        return;
    }
    m_game = game;
    m_state = 1;
    char number[32];
    itoa(rally + 1, number, 10);
    std::string base = "data/tracks/track";
    base += number;
    std::string path = base;
    path += "/textures/infomap.tga";
    infoMap.texture = new Texture(path.c_str(), 0x841, 1);
    infoMap.texture->Upload(DAT_0056b0ac);
    for (int i = 0; i < stageCount; i++) {
        path = base;
        path += "/textures/RTMStage";
        itoa(stage + i + 1, number, 10);
        path += number;
        path += ".tga";
        stageSprites[i].texture = new Texture(path.c_str(), 0x841, 1);
        stageSprites[i].texture->Upload(DAT_0056b0ac);
    }
}

// FUNCTION: RALLYTROPHY 0x446a90
void ServiceMenu::Release()
{
    if (m_state) {
        m_state = 0;
        m_game = 0;
        delete infoMap.texture;
        infoMap.texture = 0;
        for (int i = 0; i < stageCount; i++) {
            delete stageSprites[i].texture;
            stageSprites[i].texture = 0;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x446b00
void ServiceMenu::LoadTextures(MenuGame* game)
{
    overlay = new OverlayImage(DAT_0056b0ac, "data/menu/ImageRepair.tga", 1, 1);
    damageIcons.texture = new Texture("data/menu/damageicons.tga", 0x841, 1);
    damageIcons.texture->Upload(DAT_0056b0ac);
    damageBar.texture = new Texture("data/menu/damagebar.tga", 0x841, 1);
    damageBar.texture->Upload(DAT_0056b0ac);
    tuneSelectors.texture = new Texture("data/menu/tuneselectors.tga", 0x841, 1);
    tuneSelectors.texture->Upload(DAT_0056b0ac);
    partSprites[0].texture = new Texture("data/menu/tyres.tga", 0x841, 1);
    partSprites[0].texture->Upload(DAT_0056b0ac);
    partSprites[1].texture = new Texture("data/menu/body.tga", 0x841, 1);
    partSprites[1].texture->Upload(DAT_0056b0ac);
    partSprites[2].texture = new Texture("data/menu/gearbox.tga", 0x841, 1);
    partSprites[2].texture->Upload(DAT_0056b0ac);
    partSprites[3].texture = new Texture("data/menu/engine.tga", 0x841, 1);
    partSprites[3].texture->Upload(DAT_0056b0ac);
    partSprites[4].texture = new Texture("data/menu/driveshaft.tga", 0x841, 1);
    partSprites[4].texture->Upload(DAT_0056b0ac);
    partSprites[5].texture = new Texture("data/menu/brakes.tga", 0x841, 1);
    partSprites[5].texture->Upload(DAT_0056b0ac);
    partSprites[6].texture = new Texture("data/menu/suspension.tga", 0x841, 1);
    partSprites[6].texture->Upload(DAT_0056b0ac);
    partSprites[7].texture = new Texture("data/menu/steering.tga", 0x841, 1);
    partSprites[7].texture->Upload(DAT_0056b0ac);
    repairSprite.texture = new Texture("data/menu/repair.tga", 0x841, 1);
    repairSprite.texture->Upload(DAT_0056b0ac);
    tuneSprite.texture = new Texture("data/menu/tune.tga", 0x841, 1);
    tuneSprite.texture->Upload(DAT_0056b0ac);
    startRace.texture = new Texture("data/menu/startrace.tga", 0x841, 1);
    startRace.texture->Upload(DAT_0056b0ac);
    loadSprite.texture = new Texture("data/menu/load.tga", 0x841, 1);
    loadSprite.texture->Upload(DAT_0056b0ac);
}

// FUNCTION: RALLYTROPHY 0x446fc0
void ServiceMenu::UnloadTextures()
{
    delete overlay;
    delete damageIcons.texture;
    delete damageBar.texture;
    delete tuneSelectors.texture;
    delete repairSprite.texture;
    delete tuneSprite.texture;
    delete startRace.texture;
    delete loadSprite.texture;
    damageIcons.texture = 0;
    damageBar.texture = 0;
    tuneSelectors.texture = 0;
    repairSprite.texture = 0;
    tuneSprite.texture = 0;
    startRace.texture = 0;
    loadSprite.texture = 0;
    for (int i = 0; i < 8; i++) {
        delete partSprites[i].texture;
        partSprites[i].texture = 0;
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// NONMATCHING: RALLYTROPHY 0x4470a0
// 2806 vs 2806 bytes; 65 differing bytes. The code and every std::string inline/out-of-line decision match; left: the
// loop prologue schedule (the original keeps the induction start in eax, mine in esi) and the frame slots of the
// double d / float f temporaries (original -0x1c/-0x14 above the 5-byte text array at -0x24, mine 8 bytes lower).
// original: FUN_004470a0 (thiscall, ret 0xc; info.ini loader: per-stage strings)
void ServiceMenu::Init(int rallyNo, int stageNo, int count)
{
    stageCount = count;
    rallyNo--;
    stageNo--;
    rally = rallyNo;
    stage = stageNo;
    Config config;
    config.Define(DAT_004f9d68, 0x42);
    char b1[0x20];
    char b2[0x20];
    char b3[0x20];
    _itoa(rallyNo + 1, b1, 10);
    std::string path("data/tracks/track");
    path += b1;
    std::string full(path);
    full += "/data/info.ini";
    config.Load(full.c_str(), true, false, 0);
    rallyName = FUN_00405380(0xb5 + rallyNo);
    config.GetInt("NumberOfStages");
    config.GetInt("NumberOfExpertStages");
    config.GetInt("NumberOfArcadeStages");
    int firstArcade = config.GetInt("FirstArcadeStage") - 1;
    for (int i = 0; i < count; i++) {
        int n = stageNo + 1 + i;
        stageNames[i] = FUN_00405380((rallyNo * 5 + 0x5f) * 2 + n);
        std::string section("Stage");
        if (DAT_00501458) {
            n -= firstArcade;
            _itoa(n, b1, 10);
            _itoa(n, b2, 10);
        } else {
            _itoa(n, b1, 10);
            _itoa(n, b2, 10);
        }
        _itoa(stageNo + 1 + i, b3, 10);
        section += b3;
        std::string key;
        stageCaptions[i] = FUN_00405380(0x1b);
        stageCaptions[i] += ' ';
        stageCaptions[i] += b2;
        key = section + "Length";
        float len = config.GetFloat(key.c_str());
        if (rallyNo == 0 && stageNo + i == 5) {
            len = 4.8f;
        }
        if (DAT_00501300[0]) {
            len *= 0.6213712f;
        }
        double d = len;
        float f;
        int whole, tenth;
        f = (float)floor(d);
        __asm {
            fld f
            fistp whole
        }
        f = (float)((d - floor(d)) * 10.0);
        __asm {
            fld f
            fistp tenth
        }
        char text[5] = "0.0 ";
        text[0] = "0123456789"[whole];
        text[2] = "0123456789"[tenth];
        stageLengths[i] = text;
        if (DAT_00501300[0]) {
            stageLengths[i] += FUN_00405380(0xf);
        } else {
            stageLengths[i] += FUN_00405380(0xe);
        }
        key = section + "Time";
        int t = config.GetInt(key.c_str());
        _itoa(t / 100, b1, 10);
        stageTimes[i] = b1;
        stageTimes[i] += ':';
        int a = (t / 10) % 10;
        int b = t & 1;
        stageTimes[i] += (char)('0' + a);
        stageTimes[i] += (char)('0' + b);
        key = section + "Surface1Type";
        int type1 = config.GetInt(key.c_str());
        key = section + "Surface1Amount";
        int amount1 = config.GetInt(key.c_str());
        ((C_Str*)&stageSurfaces[i])->AssignOut(DAT_005070b0);
        if (amount1 > 0) {
            _itoa(amount1, b2, 10);
            ((C_Str*)&stageSurfaces[i])->AssignInline(b2);
            stageSurfaces[i] += "% ";
            stageSurfaces[i] += FUN_00405380(type1 + 0xf1);
            stageSurfaces[i] += "\n";
        }
        key = section + "Surface2Type";
        int type2 = config.GetInt(key.c_str());
        key = section + "Surface2Amount";
        int amount2 = config.GetInt(key.c_str());
        if (amount2 > 0) {
            _itoa(amount2, b2, 10);
            stageSurfaces[i] += b2;
            stageSurfaces[i] += "% ";
            stageSurfaces[i] += FUN_00405380(type2 + 0xf1);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x447ba0
int ServiceMenu::CanTune(int i)
{
    int r = 0;
    if (tune[i] != tuneOrig[i] || timeLeft >= tuneTime[i]) {
        r = 1;
    }
    return r;
}

// FUNCTION: RALLYTROPHY 0x447bd0
int ServiceMenu::CanRepair(int i)
{
    int ok = 0;
    if (repair[i] == 0) {
        if (damage[i] == 0.0f) {
            return 0;
        }
        if (timeLeft >= repairTime[i]) {
            ok = 1;
        }
    } else {
        ok = 1;
    }
    return ok;
}

// FUNCTION: RALLYTROPHY 0x447c20
void ServiceMenu::RecalcTimeLeft()
{
    timeLeft = totalTime;
    for (int i = 0; i < 7; i++) {
        if (repair[i]) {
            timeLeft -= repairTime[i];
        }
    }
    for (int j = 0; j < 5; j++) {
        if (tune[j] != tuneOrig[j]) {
            timeLeft -= tuneTime[j];
        }
    }
}

// Car damage fields the screen reads (Car+0x1c74c.., see unk_46a8a0.h).
#define SMD_CARF(car, off) (*(float*)((BYTE*)(car) + (off)))

// FUNCTION: RALLYTROPHY 0x447c70
int ServiceMenu::Run(MenuGame* game, int playerIndex, Car* car)
{
    unk30 = playerIndex;
    Setup(game);
    mode = 0;
    tuneOrig[0] = car->unk0;
    tuneOrig[1] = car->unk4;
    tuneOrig[2] = car->unk8;
    tuneOrig[3] = car->unkc;
    tuneOrig[4] = car->unk10;
    for (int i = 0; i < 5; i++) {
        tune[i] = tuneOrig[i];
    }
    for (int j = 0; j < 7; j++) {
        repair[j] = 0;
        damage[j] = 0;
    }
    damage[0] = SMD_CARF(car, 0x1c74c);
    damage[1] = SMD_CARF(car, 0x1c758);
    damage[2] = SMD_CARF(car, 0x1c750);
    damage[3] = SMD_CARF(car, 0x1c754);
    damage[4] = car->GetBrakeDamage();
    damage[5] = car->GetWheelDamage();
    damage[6] = car->GetSteeringDamage();
    for (int k = 0; k < 7; k++) {
        if (damage[k] < 0.01f) {
            damage[k] = 0.0f;
        } else if (damage[k] > 0.95f) {
            damage[k] = 0.95f;
        }
    }
    timeLeft = totalTime;
    m_selected = 0;
    int r = Frame();
    if (r != -1) {
        car->FUN_00465e90(tune[0]);
        car->FUN_00466500(tune[1]);
        car->FUN_00466520(tune[2]);
        car->FUN_00466590(tune[3]);
        car->FUN_00466610(tune[4]);
        if (repair[0]) {
            car->FUN_0045a890();
        }
        if (repair[1]) {
            car->ResetZone1();
        }
        if (repair[2]) {
            car->ResetZone2();
        }
        if (repair[3]) {
            car->ResetZone3();
        }
        if (repair[4]) {
            car->ResetBrakeDamage();
        }
        if (repair[5]) {
            car->ResetWheelDamage();
        }
        if (repair[6]) {
            car->ResetSteeringDamage();
        }
    }
    return r;
}

// FUNCTION: RALLYTROPHY 0x447e50
// original: FUN_00447e50. The mouse warps build an inline-constructed temporary (SME_WarpPoint). The first arcade-tab
// test reads DAT_00501454 directly while the later ones use the hoisted local copy (a local there lets the compiler
// thread the hidden-tab jump, the original does not).
int ServiceMenu::Frame()
{
    int done = 0;
    HudPoint mouse;
    DAT_0056b0ac->SetRenderState(5, 0);
    DAT_0056b0ac->vf42(8);
    float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
    float sy = DAT_0056b0ac->GetHeight() * 0.0013020834f;
    int picked = 0;
    DAT_0056b0bc->ClearKeys();
    BuildRects();
    int first = 1;
    int wait = 0;
    while (FUN_004b69f0(wait) && !done && !picked) {
        int hit;
        if (DAT_00501454) {
            DAT_00501450->ServiceNetwork();
        }
        mouse = *(HudPoint*)&DAT_0057ba00;
        m_game->Update();
        if (first) {
            first = 0;
            DAT_0056b0bc->ClearKeys();
            DAT_0057ba20 = 0;
            DAT_0057ba24 = 0;
            hit = HitTest(mouse, 0, 5);
            if (hit != -1) {
                m_selected = hit;
            }
        }
        if (!DAT_0056b0ac->m_active) {
            wait = (int)DAT_0056b0ac;
            continue;
        }
        wait = 0;
        int back = 0;
        int vert = 0;
        int horiz = 0;
        int activate = 0;
        if (mode == 0 || mode == 1) {
            hit = HitTest(mouse, 0, 5);
            if (hit != -1) {
                if (m_selected != hit) {
                    m_game->Play(0);
                }
                m_selected = hit;
                if (DAT_0057ba20) {
                    DAT_0057ba20 = 0;
                    DAT_0057ba24 = 0;
                    activate = 1;
                }
                if (mode == 1) {
                    mode = 0;
                }
            }
        }
        if (mode == 1) {
            switch (m_selected) {
            case 0:
                hit = HitTest(mouse, 6, stageCount + 5);
                if (hit != -1) {
                    int row = hit - 6;
                    if (unk24 != row) {
                        m_game->Play(0);
                    }
                    unk24 = row;
                    if (DAT_0057ba20) {
                        DAT_0057ba20 = 0;
                        DAT_0057ba24 = 0;
                        activate = 1;
                    }
                }
                break;
            case 1:
                hit = HitTest(mouse, 6, 10);
                if (hit != -1) {
                    int row = hit - 6;
                    if (unk28 != row) {
                        m_game->Play(0);
                    }
                    unk28 = row;
                    if (DAT_0057ba20) {
                        DAT_0057ba20 = 0;
                        DAT_0057ba24 = 0;
                        activate = 1;
                    }
                }
                break;
            case 2:
                hit = HitTest(mouse, 6, 12);
                if (hit != -1) {
                    int row = hit - 6;
                    if (unk2c != row) {
                        m_game->Play(0);
                    }
                    unk2c = row;
                    if (DAT_0057ba20) {
                        DAT_0057ba20 = 0;
                        DAT_0057ba24 = 0;
                        activate = 1;
                    }
                }
                break;
            }
        } else if (mode == 2) {
            if (m_selected == 1) {
                hit = HitTest(mouse, unk1198, unk1198 + 4);
                if (hit != -1) {
                    tune[unk28] = hit - unk1198;
                    if (DAT_0057ba20) {
                        DAT_0057ba20 = 0;
                        DAT_0057ba24 = 0;
                        activate = 1;
                    }
                }
            }
        }
        if (DAT_0057ba28[VK_ESCAPE]) {
            back = 1;
        }
        if (DAT_0057ba28[VK_RETURN]) {
            activate = 1;
        }
        if (DAT_0057ba28[VK_DOWN]) {
            vert = 1;
        }
        if (DAT_0057ba28[VK_UP]) {
            vert--;
        }
        if (DAT_0057ba28[VK_LEFT]) {
            horiz = -1;
        }
        if (DAT_0057ba28[VK_RIGHT]) {
            horiz++;
        }
        if (vert != 0) {
            m_game->Play(0);
        }
        if (back) {
            if (mode == 0 && ((SME_Game*)m_game)->FUN_004368a0(this)) {
                done = 1;
            } else if (mode == 1) {
                int idx = m_selected;
                float cx = m_items[idx]->GetTopLeft().x +
                           (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                float cy = m_items[idx]->GetTopLeft().y +
                           (m_items[idx]->GetBottomRight().y - m_items[idx]->GetTopLeft().y) * 0.5f;
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, cy * DAT_0056aeac));
                mode = 0;
            } else if (mode == 2) {
                int idx = unk119c;
                if (m_selected == 1) {
                    idx += unk28;
                } else if (m_selected == 2) {
                    idx += unk2c;
                }
                float cx = m_items[idx]->GetTopLeft().x +
                           (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                float cy = m_items[idx]->GetTopLeft().y +
                           (m_items[idx]->GetBottomRight().y - m_items[idx]->GetTopLeft().y) * 0.5f;
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, cy * DAT_0056aeac));
                mode = 1;
            }
        } else if (activate) {
            if (mode == 0) {
                switch (m_selected) {
                case 3:
                    ((SlotMenu*)((BYTE*)m_game + 0x2008))->Run(m_game);
                    DAT_0056b0bc->ClearKeys();
                    continue;
                case 4:
                    m_game->Play(1);
                    picked = 1;
                    break;
                case 5:
                    if (((SME_Game*)m_game)->FUN_004368a0(this)) {
                        done = 1;
                    }
                    break;
                default: {
                    int idx = unk119c;
                    mode = 1;
                    float cx = m_items[idx]->GetTopLeft().x +
                               (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                    float cy = m_items[idx]->GetTopLeft().y +
                               (m_items[idx]->GetBottomRight().y - m_items[idx]->GetTopLeft().y) * 0.5f;
                    ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, cy * DAT_0056aeac));
                    break;
                }
                }
            } else if (mode == 1) {
                switch (m_selected) {
                case 1:
                    if (CanTune(unk28)) {
                        mode = 2;
                        int idx = tune[unk28] + unk1198;
                        float cx = m_items[idx]->GetTopLeft().x +
                                   (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                        ((MouseInput*)DAT_0056b0c0)->SetPosition(
                            (Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, (SME_ROWY + SME_ROWPAD) * DAT_0056aeac));
                    } else {
                        m_game->Play(3);
                    }
                    break;
                case 2:
                    if (CanRepair(unk2c)) {
                        repair[unk2c] ^= 1;
                    } else {
                        m_game->Play(3);
                    }
                    break;
                }
            } else if (mode == 2) {
                mode = 1;
                if (m_selected == 1) {
                    int idx = unk119c + unk28;
                    float cx = m_items[idx]->GetTopLeft().x +
                               (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                    float cy = m_items[idx]->GetTopLeft().y +
                               (m_items[idx]->GetBottomRight().y - m_items[idx]->GetTopLeft().y) * 0.5f;
                    ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, cy * DAT_0056aeac));
                }
            }
        } else if (vert) {
            if (mode == 0) {
                m_game->Play(0);
                int hidden;
                int career = DAT_00501464;
                int net = DAT_00501454;
                for (;;) {
                    hidden = 0;
                    m_selected += vert;
                    if (m_selected < 0) {
                        m_selected = 5;
                    } else if (m_selected > 5) {
                        m_selected = 0;
                    }
                    if (DAT_00501458 && !DAT_00501454 && (m_selected == 1 || m_selected == 2)) {
                        hidden = 1;
                    } else {
                        if (!career && !net) {
                            break;
                        }
                        if (m_selected == 3 ||
                            ((career == 3 || career == 2) && m_selected == 2)) {
                            hidden = 1;
                        }
                    }
                    if (net && m_selected == 2 && !DAT_0050145c) {
                        continue;
                    }
                    if (!hidden) {
                        break;
                    }
                }
                float cx = m_items[m_selected]->GetTopLeft().x +
                           (m_items[m_selected]->GetBottomRight().x - m_items[m_selected]->GetTopLeft().x) * 0.5f;
                float cy = m_items[m_selected]->GetTopLeft().y +
                           (m_items[m_selected]->GetBottomRight().y - m_items[m_selected]->GetTopLeft().y) * 0.5f;
                ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(cx * sx, cy * sy));
            } else if (mode == 1) {
                switch (m_selected) {
                case 0: {
                    unk24 += vert;
                    if (unk24 < 0) {
                        unk24 = stageCount - 1;
                    } else if (unk24 > stageCount - 1) {
                        unk24 = 0;
                    }
                    int idx = unk119c + unk24;
                    float cx = m_items[idx]->GetTopLeft().x +
                               (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                    float cy = m_items[idx]->GetTopLeft().y +
                               (m_items[idx]->GetBottomRight().y - m_items[idx]->GetTopLeft().y) * 0.5f;
                    ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, cy * DAT_0056aeac));
                    break;
                }
                case 1: {
                    unk28 += vert;
                    if (unk28 < 0) {
                        unk28 = 4;
                    } else if (unk28 > 4) {
                        unk28 = 0;
                    }
                    int idx = unk119c + unk28;
                    float cx = m_items[idx]->GetTopLeft().x +
                               (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                    float cy = m_items[idx]->GetTopLeft().y +
                               (m_items[idx]->GetBottomRight().y - m_items[idx]->GetTopLeft().y) * 0.5f;
                    ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, cy * DAT_0056aeac));
                    m_game->Play(0);
                    break;
                }
                case 2: {
                    unk2c += vert;
                    if (unk2c < 0) {
                        unk2c = 6;
                    } else if (unk2c > 6) {
                        unk2c = 0;
                    }
                    int idx = unk119c + unk2c;
                    float cx = m_items[idx]->GetTopLeft().x +
                               (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                    float cy = m_items[idx]->GetTopLeft().y +
                               (m_items[idx]->GetBottomRight().y - m_items[idx]->GetTopLeft().y) * 0.5f;
                    ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, cy * DAT_0056aeac));
                    m_game->Play(0);
                    break;
                }
                }
            }
        } else if (horiz != 0) {
            if (m_selected == 1) {
                if (mode == 2) {
                    FUN_00438c40(&tune[unk28], horiz, 0, 4);
                    int idx = tune[unk28] + unk1198;
                    float cx = m_items[idx]->GetTopLeft().x +
                               (m_items[idx]->GetBottomRight().x - m_items[idx]->GetTopLeft().x) * 0.5f;
                    ((MouseInput*)DAT_0056b0c0)->SetPosition(
                        (Vec2*)&SME_WarpPoint(cx * DAT_0056aea8, (SME_ROWY + SME_ROWPAD) * DAT_0056aeac));
                }
            }
        }
        if (mode == 2) {
            float x = DAT_0057ba00;
            float lo = SME_COLX * DAT_0056aea8;
            if (x < lo) {
                x = lo;
            } else {
                float hi = (SME_COLX + SME_COLW) * DAT_0056aea8;
                if (x > hi) {
                    x = hi;
                }
            }
            float y = (SME_ROWY + SME_ROWPAD) * DAT_0056aeac;
            ((MouseInput*)DAT_0056b0c0)->SetPosition((Vec2*)&SME_WarpPoint(x, y));
        }
        RecalcTimeLeft();
        Slot3(0);
        DAT_0056b0bc->ClearKeys();
        DAT_0057ba20 = 0;
        DAT_0057ba24 = 0;
    }
    FUN_00438d00();
    int r = m_selected;
    if (done || DAT_0057b9f4) {
        r = -1;
        m_game->Play(2);
    }
    return r;
}

// NONMATCHING: RALLYTROPHY 0x448f80
// Same instruction count (586) and nearly the same code; differs only in register allocation and frame slot order:
// VC6 hoists the constant 0 into ebp from the function entry (merged with the loop index i = 0), where the original
// materialises it only at the loop; the original also spills the sprite pointer and keeps -1 in ebx. Unsolved after
// ~20 variants (loop forms, bool selected, Color ctor). original: FUN_00448f80 (page 0 of ServiceMenu::Slot3)
void ServiceMenu::DrawInfo()
{
    SMF_Str unused;
    float x = DAT_0056aea8 * 339.0f;
    TextRenderer* text = m_game->text;
    float y = DAT_0056aeac * 262.0f;
    HudPoint uv[4];
    HudPoint pos[4];
    Color color;
    if (mode > 0) {
        int i;
        for (i = 0; i < stageCount; i++) {
            bool selected = (unk24 == i);
            if (selected) {
                pos[0].x = pos[3].x = DAT_0056aea8 * 602.0f;
                uv[0].x = uv[3].x = 0.0f;
                uv[1].x = uv[2].x = 1.0f;
                uv[0].y = uv[1].y = 0.0f;
                uv[2].y = uv[3].y = 1.0f;
                pos[1].x = pos[2].x = DAT_0056aea8 * 1001.0f;
                color.value = 0xffffffff;
                pos[0].y = pos[1].y = DAT_0056aeac * 239.0f;
                pos[2].y = pos[3].y = DAT_0056aeac * 551.0f;
                DAT_0056b0ac->vf35(&stageSprites[i]);
                ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
                color.value = DAT_00508c0c;
            } else {
                color.value = DAT_00508c04;
            }
            text->color = color.value;
            text->Print(DAT_0056b0ac, x, y, stageCaptions[i].c_str(), 0, 1.0f, 1.0f);
            y = DAT_0056aeac * 42.0f + y;
        }
        int sel = unk24;
        y = DAT_0056aeac * 212.0f;
        x = DAT_0056aea8 * 785.0f;
        x = x - text->GetWidth(stageTimes[sel].c_str(), 0);
        color.value = 0xff000000;
        text->color = color.value;
        text->Print(DAT_0056b0ac, x, y, stageTimes[sel].c_str(), 0, 1.0f, 1.0f);
        x = DAT_0056aea8 * 985.0f;
        x = x - text->GetWidth(stageLengths[sel].c_str(), 0);
        text->Print(DAT_0056b0ac, x, y, stageLengths[sel].c_str(), 0, 1.0f, 1.0f);
        y = DAT_0056aeac * 550.0f;
        x = DAT_0056aea8 * 608.0f;
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0xfe), 0, 1.0f, 1.0f);
        x = DAT_0056aea8 * 999.0f;
        x = x - text->GetWidth(FUN_00405380(0xf9), 0);
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0xf9), 0, 1.0f, 1.0f);
        y = DAT_0056aeac * 22.0f;
        text = *(TextRenderer**)((BYTE*)m_game + 0x1390bc);
        x = DAT_0056aea8 * 601.0f;
        text->color = 0xff000000;
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0x72), 0, 1.0f, 1.0f);
        x = DAT_0056aea8 * 806.0f;
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0xfa), 0, 1.0f, 1.0f);
        y = DAT_0056aeac * 580.0f;
        x = DAT_0056aea8 * 613.0f;
        text->Print(DAT_0056b0ac, x, y, stageSurfaces[sel].c_str(), 0, 1.0f, 1.0f);
        x = DAT_0056aea8 * 995.0f;
        x = x - text->GetWidth(stageNames[sel].c_str(), 0);
        text->Print(DAT_0056b0ac, x, y, stageNames[sel].c_str(), 0, 1.0f, 1.0f);
        x = DAT_0056aea8 * 613.0f;
        int rally1 = rally + 1;
        y = DAT_0056aeac * 620.0f;
        int stage1 = stage + sel + 1;
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0xff), 0, 1.0f, 1.0f);
        y = DAT_0056aeac * 15.0f + y;
        int off = rally1 * 1512 + stage1 * 16;
        x = DAT_0056aea8 * 32.0f + x;
        text->Print(DAT_0056b0ac, x, y, SMF_STR(off + 0x39a4).c_str(), 0, 1.0f, 1.0f);
        y = DAT_0056aeac * 15.0f + y;
        text->Print(DAT_0056b0ac, x, y, SMF_STR(off + 0x3b24).c_str(), 0, 1.0f, 1.0f);
        y = DAT_0056aeac * 15.0f + y;
        text->Print(DAT_0056b0ac, x, y, FUN_00405380(0x138 + *((BYTE*)DAT_0056b0c8 + rally1 * 1512 + stage1 + 0x3bf3)),
                    0, 1.0f, 1.0f);
        std::string& extra = SMF_STR(off + 0x3a64);
        if (extra.size() > 0) {
            y = DAT_0056aeac * 30.0f + y;
            x = x - DAT_0056aea8 * 32.0f;
            text->Print(DAT_0056b0ac, x, y, FUN_00405380(0x100), 0, 1.0f, 1.0f);
            y = DAT_0056aeac * 15.0f + y;
            x = DAT_0056aea8 * 32.0f + x;
            text->Print(DAT_0056b0ac, x, y, extra.c_str(), 0, 1.0f, 1.0f);
        }
    } else {
        pos[0].x = pos[3].x = DAT_0056aea8 * 602.0f;
        uv[0].x = uv[3].x = 0.0f;
        uv[1].x = uv[2].x = 1.0f;
        uv[0].y = uv[1].y = 0.0f;
        uv[2].y = uv[3].y = 1.0f;
        pos[1].x = pos[2].x = DAT_0056aea8 * 1001.0f;
        color.value = 0xffffffff;
        pos[0].y = pos[1].y = DAT_0056aeac * 239.0f;
        pos[2].y = pos[3].y = DAT_0056aeac * 551.0f;
        DAT_0056b0ac->vf35(&infoMap);
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
    }
}

// NONMATCHING: RALLYTROPHY 0x449860
// Logic and instruction stream ~88% aligned (3024 vs 3004 bytes incl. jump table). Open: the original keeps the -1 colour
// in ebp (or ebp,-1 / mov [slot],ebp / push ebp) and has its EH Color slot above pos[]/uv[] (frame 0x90, mine 0x78).
// original: FUN_00449860 (page 1, the Tune tab)
void ServiceMenu::DrawTune()
{
    std::string unused;
    TextRenderer* lil = m_game->text;
    float x = DAT_0056aea8 * 339.0f;
    float y = DAT_0056aeac * 262.0f;
    HudPoint uv[4];
    HudPoint pos[4];
    Color color(0xFFFFFFFF);
    char buf[16];
    int i;
    uv[0].x = uv[3].x = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[0].x = pos[3].x = DAT_0056aea8 * 684.0f;
    pos[1].x = pos[2].x = DAT_0056aea8 * 943.0f;
    pos[0].y = pos[1].y = DAT_0056aeac * 266.0f;
    pos[2].y = pos[3].y = DAT_0056aeac * 522.0f;
    if (mode > 0) {
        static const int G_names[5] = { 126, 127, 128, 129, 130 };
        static const int G_icon[5] = { 0, 2, 6, 5, 7 };
        static const int G_values[5] = { 135, 136, 137, 138, 139 };
        static const int G_left[5] = { 0, 140, 142, 144, 146 };
        static const int G_right[5] = { 0, 141, 143, 145, 147 };
        color = Color(0xFFFFFFFF);
        DAT_0056b0ac->vf35(&partSprites[G_icon[unk28]]);
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
        for (i = 0; i < 5; i++) {
            bool selected = unk28 == i;
            color.value = selected ? DAT_00508c0c : DAT_00508c04;
            lil->color = color.value;
            lil->Print(DAT_0056b0ac, x, y, FUN_00405380(G_names[i]), 0, 1.0f, 1.0f);
            y = DAT_0056aeac * 42.0f + y;
        }
        DAT_0056b0ac->vf35(&damageIcons);
        color = Color(0xFFFFFFFF);
        x = DAT_0056aea8 * 601.0f;
        y = DAT_0056aeac * 248.0f;
        for (i = 0; i < 5; i++) {
            if (tune[i] != tuneOrig[i]) {
                pos[0].x = pos[3].x = x;
                pos[0].y = pos[1].y = y;
                pos[1].x = pos[2].x = DAT_0056aea8 * 32.0f + x;
                pos[2].y = pos[3].y = DAT_0056aeac * 65.0f + y;
                uv[0].x = uv[3].x = 0.0f;
                uv[0].y = uv[1].y = 0.0f;
                uv[1].x = uv[2].x = 0.5f;
                uv[2].y = uv[3].y = 0.5f;
                ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
            }
            y = DAT_0056aeac * 42.0f + y;
        }
        x = DAT_0056aea8 * 600.0f;
        uv[0].x = uv[3].x = 0.0f;
        uv[0].y = uv[1].y = 0.0f;
        uv[1].x = uv[2].x = 0.779296875f;
        uv[2].y = uv[3].y = 0.28125f;
        y = DAT_0056aeac * 551.0f;
        pos[0].x = pos[3].x = x;
        pos[0].y = pos[1].y = y;
        pos[1].x = pos[2].x = DAT_0056aea8 * 402.0f + x;
        pos[2].y = pos[3].y = DAT_0056aeac * 22.0f + y;
        DAT_0056b0ac->vf35(&damageBar);
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
        pos[0].x = pos[3].x = x - DAT_0056aea8;
        pos[0].y = pos[1].y = y;
        pos[1].x = pos[2].x = DAT_0056aea8 * 403.0f + x;
        pos[2].y = pos[3].y = DAT_0056aeac * 32.0f + y;
        uv[0].x = uv[3].x = 0.0f;
        uv[0].y = uv[1].y = 0.5f;
        uv[1].x = uv[2].x = 0.787109375f;
        uv[2].y = uv[3].y = 1.0f;
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
        // selector marker for the original tuning
        pos[0].x = pos[3].x = ((float)tuneOrig[unk28] * 80.0f + 601.0f) * DAT_0056aea8;
        pos[0].y = pos[1].y = y = DAT_0056aeac * 551.0f;
        pos[1].x = pos[2].x = DAT_0056aea8 * 128.0f + pos[0].x;
        pos[2].y = pos[3].y = DAT_0056aeac * 35.0f + y;
        uv[0].x = uv[3].x = 0.0f;
        uv[0].y = uv[1].y = 0.0f;
        uv[1].x = uv[2].x = 0.5f;
        uv[2].y = uv[3].y = 0.5f;
        DAT_0056b0ac->vf35(&tuneSelectors);
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
        if (mode > 1) {
            uv[0].x = uv[3].x = 0.0f;
            uv[0].y = uv[1].y = 0.5f;
            uv[1].x = uv[2].x = 0.5f;
            uv[2].y = uv[3].y = 1.0f;
        } else {
            uv[0].x = uv[3].x = 0.5f;
            uv[0].y = uv[1].y = 0.0f;
            uv[1].x = uv[2].x = 1.0f;
            uv[2].y = uv[3].y = 0.5f;
        }
        pos[0].x = pos[3].x = ((float)tune[unk28] * 80.0f + 601.0f) * DAT_0056aea8;
        pos[0].y = pos[1].y = y = DAT_0056aeac * 551.0f;
        pos[1].x = pos[2].x = DAT_0056aea8 * 128.0f + pos[0].x;
        pos[2].y = pos[3].y = DAT_0056aeac * 35.0f + y;
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
        TextRenderer* bigText = *(TextRenderer**)((BYTE*)m_game + 0x1390bc);
        y = DAT_0056aeac * 580.0f;
        color.value = 0xFF000000;
        bigText->color = color.value;
        if (unk28 == 0) {
            for (i = 0; i < 5; i++) {
                x = ((float)i * 80.0f + 641.0f) * DAT_0056aea8;
                const char* text = FUN_00405380(G_values[i]);
                float half = bigText->GetWidth(text, 0) * 0.5f;
                bigText->Print(DAT_0056b0ac, x - half, y, text, 0, 1.0f, 1.0f);
            }
        } else {
            const char* left = FUN_00405380(G_left[unk28]);
            const char* right = FUN_00405380(G_right[unk28]);
            bigText->Print(DAT_0056b0ac, DAT_0056aea8 * 617.0f, y, left, 0, 1.0f, 1.0f);
            x = DAT_0056aea8 * 985.0f;
            x = x - bigText->GetWidth(right, 0);
            bigText->Print(DAT_0056b0ac, x, y, right, 0, 1.0f, 1.0f);
        }
        lil->SetScale(DAT_0056aea8 * 0.75f, DAT_0056aeac * 0.75f);
        lil->color = DAT_00508c04;
        x = DAT_0056aea8 * 804.0f;
        y = DAT_0056aeac * 192.0f;
        lil->Print(DAT_0056b0ac, x, y, FUN_00405380(0x73), 0, 1.0f, 1.0f);
        lil->Print(DAT_0056b0ac, DAT_0056aea8 * 604.0f, y, FUN_00405380(0x77), 0, 1.0f, 1.0f);
        lil->color = DAT_00508c04;
        int row = unk28;
        int cost = tuneTime[row];
        lil->SetScale(DAT_0056aea8 * 1.5f, DAT_0056aeac * 1.5f);
        itoa(cost, buf, 10);
        y = DAT_0056aeac * 200.0f;
        x = DAT_0056aea8 * 785.0f;
        x = x - lil->GetWidth(buf, 0);
        lil->Print(DAT_0056b0ac, x, y, buf, 0, 1.0f, 1.0f);
        itoa(timeLeft, buf, 10);
        x = DAT_0056aea8 * 985.0f;
        x = x - lil->GetWidth(buf, 0);
        if (timeLeft < cost && tune[row] == tuneOrig[row]) {
            lil->SetColor(ColorArg(0xFFD14A4F));
        } else {
            lil->color = DAT_00508c04;
        }
        lil->Print(DAT_0056b0ac, x, y, buf, 0, 1.0f, 1.0f);
        lil->SetScale(DAT_0056aea8, DAT_0056aeac);
        lil->color = DAT_00508c04;
        *(BYTE*)((BYTE*)bigText + 0x130) = 1;
        x = DAT_0056aea8 * 608.0f;
        y = DAT_0056aeac * 606.0f;
        *(float*)((BYTE*)bigText + 0x138) = (float)bigText->lineHeight * DAT_0056aeac;
        *(float*)((BYTE*)bigText + 0x134) = DAT_0056aea8 * 387.0f;
        switch (row) {
        case 0:
            bigText->Print(DAT_0056b0ac, x, y, FUN_00405380(393), 0, 1.0f, 1.0f);
            break;
        case 1:
            bigText->Print(DAT_0056b0ac, x, y, FUN_00405380(390), 0, 1.0f, 1.0f);
            break;
        case 2:
            bigText->Print(DAT_0056b0ac, x, y, FUN_00405380(396), 0, 1.0f, 1.0f);
            break;
        case 3:
            bigText->Print(DAT_0056b0ac, x, y, FUN_00405380(400), 0, 1.0f, 1.0f);
            break;
        case 4:
            bigText->Print(DAT_0056b0ac, x, y, FUN_00405380(404), 0, 1.0f, 1.0f);
            break;
        }
        *(BYTE*)((BYTE*)bigText + 0x130) = 0;
    } else {
        color = Color(0xFFFFFFFF);
        DAT_0056b0ac->vf35(&tuneSprite);
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, color, 1.0f, 1.0f);
    }
}

// NONMATCHING: RALLYTROPHY 0x44a420
// Same structure and constants (built 2206 vs 2256 bytes). VC6 hoists the zero constant into ebx for the entry block
// (the loop index reuses it) where the original keeps -1 in ebx and the loop index in ebp, so registers and frame slots
// differ (the original also has a separate memory loop counter for the damage loop).
// original: FUN_0044a420 (page 2 of Draw)
void ServiceMenu::DrawRepair()
{
    TextRenderer* text = m_game->text;
    float x = DAT_0056aea8 * 339.0f;
    float y = DAT_0056aeac * 262.0f;
    HudPoint uv[4];
    HudPoint pos[4];
    Color col;
    char buf[32];
    pos[0].x = pos[3].x = DAT_0056aea8 * 684.0f;
    uv[0].x = uv[3].x = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[1].x = pos[2].x = DAT_0056aea8 * 943.0f;
    pos[0].y = pos[1].y = DAT_0056aeac * 266.0f;
    pos[2].y = pos[3].y = DAT_0056aeac * 522.0f;
    if (mode > 0) {
        int i;
        col.value = -1;
        DAT_0056b0ac->vf35(&partSprites[SMH_PartSprite[unk2c]]);
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, col, 1.0f, 1.0f);
        for (i = 0; i < 7; i++) {
            int selected = (unk2c == i);
            col.value = selected ? DAT_00508c0c : DAT_00508c04;
            text->color = col.value;
            text->Print(DAT_0056b0ac, x, y, FUN_00405380(SMH_PartText[i]), 0, 1.0f, 1.0f);
            y = DAT_0056aeac * 42.0f + y;
        }
        DAT_0056b0ac->vf35(&damageIcons);
        x = DAT_0056aea8 * 601.0f;
        col.value = -1;
        y = DAT_0056aeac * 248.0f;
        for (i = 0; i < 7; i++) {
            if (damage[i] > 0.0f) {
                pos[0].x = pos[3].x = x;
                pos[1].x = pos[2].x = DAT_0056aea8 * 32.0f + x;
                pos[0].y = pos[1].y = y;
                pos[2].y = pos[3].y = DAT_0056aeac * 65.0f + y;
                int level;
                if (repair[i]) {
                    level = 0;
                } else if (damage[i] > 0.66f) {
                    level = 3;
                } else if (damage[i] > 0.33f) {
                    level = 2;
                } else {
                    level = 1;
                }
                float u = 0.0f;
                if (level & 1) {
                    u = 0.5f;
                }
                float v = 0.0f;
                if (level & 2) {
                    v = 0.5f;
                }
                uv[0].x = uv[3].x = u;
                uv[0].y = uv[1].y = v;
                uv[1].x = uv[2].x = u + 0.5f;
                uv[2].y = uv[3].y = v + 0.5f;
                ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, col, 1.0f, 1.0f);
            }
            y = DAT_0056aeac * 42.0f + y;
        }
        float d = damage[unk2c];
        if (repair[unk2c]) {
            d = 0.0f;
        }
        x = DAT_0056aea8 * 600.0f;
        uv[0].x = uv[3].x = 0.0f;
        uv[0].y = uv[1].y = 0.0f;
        uv[2].y = uv[3].y = 0.28125f;
        y = DAT_0056aeac * 551.0f;
        pos[0].x = pos[3].x = x;
        pos[0].y = pos[1].y = y;
        uv[1].x = uv[2].x = d * 0.779296875f;
        pos[1].x = pos[2].x = d * DAT_0056aea8 * 402.0f + x;
        pos[2].y = pos[3].y = DAT_0056aeac * 22.0f + y;
        DAT_0056b0ac->vf35(&damageBar);
        DAT_0056b0ac->vf66(pos, uv, -1, 1.0f, 1.0f);
        pos[0].x = pos[3].x = x - DAT_0056aea8;
        pos[0].y = pos[1].y = y;
        uv[0].x = uv[3].x = 0.0f;
        uv[0].y = uv[1].y = 0.5f;
        uv[1].x = uv[2].x = 0.787109375f;
        uv[2].y = uv[3].y = 1.0f;
        pos[1].x = pos[2].x = DAT_0056aea8 * 403.0f + x;
        pos[2].y = pos[3].y = DAT_0056aeac * 32.0f + y;
        DAT_0056b0ac->vf66(pos, uv, -1, 1.0f, 1.0f);
        TextRenderer* text2 = SMH_FONT2(m_game);
        float ty = DAT_0056aeac * 580.0f;
        col.value = 0xff000000;
        text2->color = col.value;
        text2->Print(DAT_0056b0ac, DAT_0056aea8 * 607.0f, ty, "0%", 0, 1.0f, 1.0f);
        float tx = DAT_0056aea8 * 995.0f - text2->GetWidth("100%", 0);
        text2->Print(DAT_0056b0ac, tx, ty, "100%", 0, 1.0f, 1.0f);
        tx = DAT_0056aea8 * 801.0f - text2->GetWidth(FUN_00405380(0x74), 0) * 0.5f;
        text2->Print(DAT_0056b0ac, tx, ty, FUN_00405380(0x74), 0, 1.0f, 1.0f);
        text->scaleX = DAT_0056aea8 * 0.75f;
        text->scaleY = DAT_0056aeac * 0.75f;
        text->color = DAT_00508c04;
        float hy = DAT_0056aeac * 192.0f;
        text->Print(DAT_0056b0ac, DAT_0056aea8 * 804.0f, hy, FUN_00405380(0x73), 0, 1.0f, 1.0f);
        text->Print(DAT_0056b0ac, DAT_0056aea8 * 604.0f, hy, FUN_00405380(0x76), 0, 1.0f, 1.0f);
        text->color = DAT_00508c04;
        text->scaleX = DAT_0056aea8 * 1.5f;
        text->scaleY = DAT_0056aeac * 1.5f;
        int cost = repairTime[unk2c];
        itoa(cost, buf, 10);
        float ry = DAT_0056aeac * 200.0f;
        text->Print(DAT_0056b0ac, DAT_0056aea8 * 785.0f - text->GetWidth(buf, 0), ry, buf, 0, 1.0f, 1.0f);
        int left = timeLeft;
        if (left < cost && !repair[unk2c]) {
            text->SetColor(0xffd14a4f);
        } else {
            text->color = DAT_00508c04;
        }
        itoa(left, buf, 10);
        text->Print(DAT_0056b0ac, DAT_0056aea8 * 985.0f - text->GetWidth(buf, 0), ry, buf, 0, 1.0f, 1.0f);
        text->scaleX = DAT_0056aea8;
        text->color = DAT_00508c04;
        text->scaleY = DAT_0056aeac;
    } else {
        col.value = -1;
        DAT_0056b0ac->vf35(&repairSprite);
        ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, col, 1.0f, 1.0f);
    }
}

// FUNCTION: RALLYTROPHY 0x44acf0
// Evidence: ev_6fc0b140bd6335fa62960afcee5a4cf1fbfee81964a056e964e6bfc884dfecfb.
// Color lifetime and UV store order preserve the target instruction schedule.
void ServiceMenu::DrawSavePage()
{
    HudPoint pos[4];
    HudPoint uv[4];
    Color white;
    uv[0].x = uv[3].x = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[0].x = pos[3].x = DAT_0056aea8 * 684.0f;
    pos[1].x = pos[2].x = DAT_0056aea8 * 943.0f;
    pos[0].y = pos[1].y = DAT_0056aeac * 266.0f;
    pos[2].y = pos[3].y = DAT_0056aeac * 522.0f;
    white.value = -1;
    DAT_0056b0ac->vf35(&loadSprite);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, white, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x44ae60
// Evidence: ev_a8f6e0372fbee2ac1dccb990627abad0c70a0937029c31261834cc13f03597b4.
// Color lifetime and UV store order preserve the target instruction schedule.
void ServiceMenu::DrawContinuePage()
{
    HudPoint pos[4];
    HudPoint uv[4];
    Color white;
    uv[0].x = uv[3].x = 0.0f;
    uv[1].x = uv[2].x = 1.0f;
    uv[0].y = uv[1].y = 0.0f;
    uv[2].y = uv[3].y = 1.0f;
    pos[0].x = pos[3].x = DAT_0056aea8 * 684.0f;
    pos[1].x = pos[2].x = DAT_0056aea8 * 943.0f;
    pos[0].y = pos[1].y = DAT_0056aeac * 266.0f;
    pos[2].y = pos[3].y = DAT_0056aeac * 522.0f;
    white.value = -1;
    DAT_0056b0ac->vf35(&startRace);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad((Vec2*)pos, (Vec2*)uv, white, 1.0f, 1.0f);
}

// FUNCTION: RALLYTROPHY 0x44afd0
// Evidence: ev_8d4506e624020115e9343d63b221610b94babd90b3c05083705beb2b24128ca2.
// Reuse one counter and reset it before n += 7 to preserve target slots and scheduling.
void ServiceMenu::BuildRects()
{
    float top;
    float shift;
    int i;
    int n;
    UIRect* rect;
    SMD_Point a;
    HudPoint b;
    top = SMD_TOP - 10.0f;
    n = 0;
    shift = 0;
    for (i = 0; i < 5; i++) {
        b.x = SMD_TABX + 260.0f;
        a.x = SMD_TABX;
        a.y = i * SMD_PITCH + shift + top;
        b.y = (i + 1) * SMD_PITCH + shift + top;
        rect = (UIRect*)new SMD_Rect(a, b);
        if (((i == 1 || i == 2) && DAT_00501458 && !DAT_00501454) || ((DAT_00501464 || DAT_00501454) && i == 3) ||
            (DAT_00501454 && i == 2 && !DAT_0050145c) || (i == 2 && (DAT_00501464 == 3 || DAT_00501464 == 2))) {
            rect->enabled = 0;
            shift -= SMD_PITCH;
        }
        m_items.push_back(rect);
        n++;
    }
    b.x = SMD_TABX + 420.0f;
    a.x = SMD_TABX;
    a.y = 607.0f;
    b.y = SMD_PITCH * 607.0f;
    rect = (UIRect*)new SMD_Rect(a, b);
    m_items.push_back(rect);
    n++;
    unk119c = n;
    i = 0;
    n += 7;
    for (; i < 7; i++) {
        b.x = SMD_ROWX + 240.0f;
        a.x = SMD_ROWX;
        a.y = i * SMD_PITCH + top;
        b.y = (i + 1) * SMD_PITCH + top;
        rect = (UIRect*)new SMD_Rect(a, b);
        m_items.push_back(rect);
    }
    unk1198 = n;
    for (i = 0; i < 5; i++) {
        a.x = i * 80.8f + SMD_TUNEX;
        a.y = SMD_TUNEY;
        b.x = (i + 1) * 80.8f + SMD_TUNEX;
        b.y = SMD_TUNEY + 32.0f;
        rect = (UIRect*)new SMD_Rect(a, b);
        std::vector<UIRect*>& v = m_items;
        v.insert(v.end(), rect);
    }
}

// NONMATCHING: RALLYTROPHY 0x44b510
// 1008 vs 983 bytes: control flow, jump table and call order match. Open: the original loads the colour globals into
// eax where mine picks edx, and keeps DAT_00501464 in eax / DAT_00501454 in ecx across the tab loop (mine swapped).
// original: FUN_0044b510 (vtable slot 3, Draw; the jump table 0x44b8e8 is its switch)
void ServiceMenu::Slot3(int skipPresent)
{
    TextRenderer* text = m_game->text;
    if (DAT_0056b0ac->BeginScene()) {
        DAT_0056b0ac->vf38(0);
        overlay->Draw(DAT_0056b0ac, 0);
        text->scaleX = DAT_0056aea8 * 2.0f;
        text->color = DAT_00508bfc;
        text->scaleY = DAT_0056aeac * 2.0f;
        text->Print(DAT_0056b0ac, SMI_TITLEX * DAT_0056aea8, SMI_TITLEY * DAT_0056aeac, FUN_00405380(0x75), 0, 1.0f,
                    1.0f);
        text->scaleX = DAT_0056aea8 * 3.0f;
        text->color = DAT_00508bfc;
        text->scaleY = DAT_0056aeac * 3.0f;
        int rid = rally + 0xb5;
        float rx = DAT_0056b0ac->GetWidth() * 0.5f - text->GetWidth(FUN_00405380(rid), 0) * 0.5f;
        text->Print(DAT_0056b0ac, rx, SMI_RALLYY * DAT_0056aeac, FUN_00405380(rid), 0, 1.0f, 1.0f);
        text->SetScale(DAT_0056aea8, DAT_0056aeac);
        float x = DAT_0056aea8 * 54.0f;
        float y = DAT_0056aeac * 262.0f;
        for (int i = 0; i < 6; i++) {
            if (DAT_00501458 && !DAT_00501454 && (i == 1 || i == 2)) {
                continue;
            }
            if (DAT_00501464 || DAT_00501454) {
                if (i == 3) {
                    continue;
                }
                if (DAT_00501454 && i == 2 && !DAT_0050145c) {
                    continue;
                }
                if ((DAT_00501464 == 3 || DAT_00501464 == 2) && i == 2) {
                    continue;
                }
            }
            if (i == 5) {
                y = DAT_0056aeac * 610.0f;
            }
            Color col;
            bool selected = m_selected == i;
            col.value = selected ? DAT_00508c0c : DAT_00508c04;
            text->color = col.value;
            text->Print(DAT_0056b0ac, x, y, FUN_00405380(DAT_004f9ca8[i]), 0, 1.0f, 1.0f);
            y = DAT_0056aeac * 42.0f + y;
        }
        switch (m_selected) {
        case 0:
            DrawInfo();
            break;
        case 1:
            DrawTune();
            break;
        case 2:
            DrawRepair();
            break;
        case 3:
            DrawSavePage();
            break;
        case 4:
            DrawContinuePage();
            break;
        }
        if (DAT_00501334 > 1) {
            text->scaleX = DAT_0056aea8 * 0.75f;
            text->scaleY = DAT_0056aeac * 0.75f;
            text->color = 0x99000000;
            text->Printf(DAT_0056b0ac, DAT_0056aea8 * 58.0f, DAT_0056aeac * 184.0f, "%s %i: %s", FUN_00405380(0x94),
                         unk30 + 1, DAT_00501338[unk30].text);
            text->SetScale(DAT_0056aea8, DAT_0056aeac);
        }
        DrawCursor(*((MouseInput*)DAT_0056b0c0)->GetPosition());
        DAT_0056b0ac->EndScene();
        if (skipPresent != 1) {
            DAT_0056b0ac->Present();
        }
    }
}

// FUNCTION: RALLYTROPHY 0x44b900
// SYMBOL: _$E23
typedef void Generated_44b900();

// FUNCTION: RALLYTROPHY 0x44b920
// SYMBOL: _$E22
typedef void Generated_44b920();
