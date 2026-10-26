/* MenuScreen2A8 (0x4295c0-0x42c2af): the graphics/detail options screen embedded in the menu manager at +0x2a8,
 * size 0x4a8, vtable 0x4e15b4 {0x429740, Setup 0x4298c0, Release 0x43ff80, Slot3 0x4414c0}. Map:
 * analysis/range-4266c0.md module 3. Include after unk_481390.h, rallytrophy.h and unk_450ee0.h (MenuScreen,
 * MenuLabel, MenuGame). Member layout from the constructor/destructor and their unwind tables (FuncInfo 0x4e7090,
 * 0x4e70f8: state 1 = title, states 2-9 = the eight label arrays). */
#ifndef UNK_4295C0_H
#define UNK_4295C0_H

class MenuScreen2A8 : public MenuScreen {
public:
    MenuScreen2A8();                   // 0x4295f0
    virtual ~MenuScreen2A8();          // 0x429760 (deleting form 0x429740)
    virtual void Setup(MenuGame* game); // 0x4298c0
    virtual void Release();            // 0x43ff80 (folded with the other screens' copies)
    virtual void Slot3(int unused);    // 0x4414c0 (empty, folded)
    void FUN_0042a210(int arg);        // 0x42a210: apply the detail settings (reached through MenuManager 0x43cb50)
    int Run(MenuGame* game, NetRace* net); // 0x42a2c0
    int Frame();                       // 0x42a330
    void LoadLayout();                 // 0x42aee0

    std::string title;                 // 0x20: text 0x4d
    int unk30;                         // 0x30
    int unk34;                         // 0x34
    float textScaleX;                  // 0x38: screen width / 1024 * 32
    float textScaleY;                  // 0x3c: screen height / 768 * 16
    int unk40;                         // 0x40: -1 after Setup
    MenuLabel items[9];                // 0x44: texts 0x4e, 0x4f, 0x53, 0x55, 0x54, 0x52, 0x51, 0x50, 0x18 "Back"
    MenuLabel values188[3];            // 0x188: texts 4, 5, 6
    MenuLabel values1f4[4];            // 0x1f4: texts 4, 5, 6, 8
    MenuLabel values284[5];            // 0x284: texts 3, 4, 5, 6, 8
    MenuLabel onOff[2];                // 0x338: texts 2 "Disabled", 1 "Enabled"
    MenuLabel values380[2];            // 0x380: texts 4, 6
    MenuLabel values3c8[3];            // 0x3c8: texts 2, 4, 6
    MenuLabel values434[3];            // 0x434: texts 2, 0x58, 0x5a
    NetRace* net;                      // 0x4a0: network session (set by Run)
    int unk4a4;                        // 0x4a4: 2 if DAT_005012e8, else 1 if DAT_005012ec, else 0 (set by Run)
};

#endif
