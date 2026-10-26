/* GameScreen11A4 (0x427600-0x4295bf): the menu screen embedded in the Game object at +0x11a4 (0x1d4 bytes,
 * vtable 0x4e157c = {deleting dtor 0x427a50, Setup 0x427b10, Release 0x427b30, Slot3 0x4414c0}).
 * Map: analysis/range-4266c0.md module 2. Members from the ctor 0x427630 and its unwind table: three MenuLabel arrays
 * (eh vector ctor 0x4c11f4 with MenuLabel ctor/dtor 0x430700/0x430720). Include after unk_481390.h, unk_4843e0.h,
 * rallytrophy.h and unk_450ee0.h (MenuScreen, MenuLabel, MenuGame). */
#ifndef UNK_427600_H
#define UNK_427600_H

extern int DAT_00501330;               // read by Run: index into DAT_004f6af8 when DAT_00501458
extern int DAT_004f6af8[];             // per-index value Run stores into DAT_00501468

class GameScreen11A4 : public MenuScreen {
public:
    GameScreen11A4();                  // 0x427630
    virtual ~GameScreen11A4();         // 0x427a70 (deleting form 0x427a50)
    virtual void Setup(MenuGame* game); // 0x427b10: Begin once
    virtual void Release();            // 0x427b30
    virtual void Slot3(int unused);    // 0x4414c0 (empty, folded with the other screens' copies)
    int Run(MenuGame* game);           // 0x427b40
    int Frame();                       // 0x427c10
    void LoadLayout();                 // 0x4286d0: data/menu/gamesettingsmenu.ini hit rectangles

    MenuLabel values6[6];              // 0x20: "1".."6"
    MenuLabel values3[3];              // 0xf8: "3", "6", "9"
    MenuLabel texts[3];                // 0x164: language texts 9, 10, 11
    int unk1d0;                        // 0x1d0: 1, or 2 once the one-shot flag 0x5014c9 / 0x5014c8 is set
};

#endif
