/* ServiceMenu (0x445e80-0x44b92f): the rally service-area screen embedded in the Game object at +4 (0x11a0 bytes,
 * vtable 0x4e18e4). Map: analysis/range-445e80.md section 1. Include after unk_481390.h, rallytrophy.h and
 * unk_450ee0.h (MenuScreen, MenuLabel, UIRect, MenuGame). */
#ifndef UNK_445E80_H
#define UNK_445E80_H

class Car;
extern ConfigKey DAT_004f9d68[];      // info.ini keys (66 entries)

#ifndef HITPOINT_DEFINED
#define HITPOINT_DEFINED
class HitPoint {
public:
    HitPoint(const HudPoint& other);   // 0x424300
    float x, y;
};
#endif

class ServiceMenu : public MenuScreen {
public:
    ServiceMenu();                     // 0x445e80
    virtual ~ServiceMenu();            // 0x4464f0 (deleting form 0x4464d0)
    virtual void Setup(MenuGame* game); // 0x4466b0
    virtual void Release();            // 0x446a90 (own implementation)
    virtual void Slot3(int skipPresent); // 0x44b510: Draw
    void LoadTextures(MenuGame* game); // 0x446b00 (argument unused; ret 4)
    void UnloadTextures();             // 0x446fc0
    void Init(int rallyNumber, int stageNumber, int count); // 0x4470a0
    int CanTune(int i);                // 0x447ba0
    int CanRepair(int i);              // 0x447bd0
    void RecalcTimeLeft();             // 0x447c20
    int Run(MenuGame* game, int playerIndex, Car* car); // 0x447c70
    int Frame();                       // 0x447e50: blocking screen loop
    void DrawInfo();                   // 0x448f80: page 0
    void DrawTune();                   // 0x449860: page 1
    void DrawRepair();                 // 0x44a420: page 2
    void DrawSavePage();               // 0x44acf0: page 3
    void DrawContinuePage();           // 0x44ae60: page 4
    void BuildRects();                 // 0x44afd0
    int HitTest(HitPoint position, int first, int last); // 0x438f90 (MenuScreen method)
    void FUN_00438c40(int* value, int delta, int lo, int hi); // 0x438c40 (MenuScreen method)
    void FUN_00438d00();               // 0x438d00 (MenuScreen method)

    int mode;                          // 0x20: right-hand panel sub-state
    int unk24;                         // 0x24: selected row / stage index
    int unk28;                         // 0x28
    int unk2c;                         // 0x2c
    int unk30;                         // 0x30: player index (Run argument 2)
    int tune[5];                       // 0x34: tyres, gearbox, suspension, brake balance, steering
    int tuneOrig[5];                   // 0x48
    int tuneTime[5];                   // 0x5c: repairtimes.ini TuneTime_*
    Sprite tuneSprite;                 // 0x70: tune.tga
    int repair[7];                     // 0x144: parts selected for repair
    int repairTime[7];                 // 0x160: repairtimes.ini RepairTime_*
    float damage[7];                   // 0x17c
    int totalTime;                     // 0x198 (60 minutes)
    int timeLeft;                      // 0x19c
    OverlayImage* overlay;             // 0x1a0: data/menu/ImageRepair.tga
    Sprite damageIcons;                // 0x1a4
    Sprite damageBar;                  // 0x278
    Sprite tuneSelectors;              // 0x34c
    Sprite repairSprite;               // 0x420
    Sprite startRace;                  // 0x4f4
    Sprite loadSprite;                 // 0x5c8
    Sprite partSprites[8];             // 0x69c: tyres, body, gearbox, engine, driveshaft, brakes, suspension, steering
    int rally;                         // 0xd3c
    int stage;                         // 0xd40
    int stageCount;                    // 0xd44
    Sprite infoMap;                    // 0xd48
    Sprite stageSprites[3];            // 0xe1c
    std::string rallyName;             // 0x1098
    std::string stageNames[3];         // 0x10a8
    std::string stageLengths[3];       // 0x10d8
    std::string stageTimes[3];         // 0x1108
    std::string stageSurfaces[3];      // 0x1138: both surfaces, "NN% name\nNN% name"
    std::string stageCaptions[3];      // 0x1168: "Stage N" caption
    int unk1198;                       // 0x1198: first tune-selector rect index
    int unk119c;                       // 0x119c: first row rect index
};

#endif
