/* RaceOptionsMenu (0x43f9e0-0x44136f): the "RACE OPTIONS" screen (Units / Ghost Car / Wildlife / Pace Notes / Back),
 * a MenuScreen subclass embedded in the menu manager (vtable 0x4e17c8 object, ctor 0x43c590) at +0x11c, size 0x18c,
 * vtable 0x4e183c. Map: analysis/range-43d770.md section 2. Include after unk_481390.h, rallytrophy.h and
 * unk_450ee0.h (MenuScreen, MenuLabel, MenuGame). */
#ifndef UNK_43F9E0_H
#define UNK_43F9E0_H

extern ConfigKey DAT_004f8f58[];       // racemenu.ini keys (26 entries, LoadLayout)

class RaceOptionsMenu : public MenuScreen {
public:
    RaceOptionsMenu();                 // 0x43f9e0
    virtual ~RaceOptionsMenu();        // 0x43fac0 (deleting form 0x43faa0)
    virtual void Setup(MenuGame* game); // 0x43fba0
    virtual void Release();            // 0x43ff80 (surviving copy of the Release every screen folds onto)
    virtual void Slot3(int unused);    // 0x4414c0 (empty, folded with the other screens' copies)
    int Run(MenuGame* game, NetRace* net); // 0x43ffa0
    int Frame();                       // 0x43ffd0
    void LoadLayout();                 // 0x440890
    void FUN_00438d00();               // 0x438d00 (MenuScreen: clear the item-rect vector)

    std::string title;                 // 0x20: text 0x46 "RACE OPTIONS"
    int unk30;                         // 0x30
    int unk34;                         // 0x34
    float textScaleX;                  // 0x38: screen width / 1024 * 32
    float textScaleY;                  // 0x3c: screen height / 768 * 16
    int unk40;                         // 0x40: -1 after Setup
    MenuLabel items[5];                // 0x44: texts 0x48 "Units", 0x49 "Ghost Car", 0x4c "Wildlife", 0x19a, 0x18 "Back"
    MenuLabel units[2];                // 0xf8: texts 0x1f "Metric", 0x20 "Imperial"
    MenuLabel onOff[2];                // 0x140: texts 2 "Disabled", 1 "Enabled"
    NetRace* net;                      // 0x188: network session serviced every frame (may be null)
};

#endif
