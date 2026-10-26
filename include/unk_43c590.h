/* MenuManager (0x43c590-0x43d73f): the top-level menu screen (six root labels), a MenuScreen subclass embedded in the
 * Game object at +0x2df4, size 0xcf8, vtable 0x4e17c8. It embeds five more screens and dispatches to them by the
 * selected index. Map: analysis/range-43c590.md. Include after unk_481390.h, rallytrophy.h, unk_450ee0.h and
 * unk_43f9e0.h and unk_439170.h (MenuScreen, MenuLabel, MenuGame, RaceOptionsMenu, MenuScreenA48). SoundMenu is declared
 * in unk_450ee0.h. */
#ifndef UNK_43C590_H
#define UNK_43C590_H

#include "unk_4295c0.h"     // MenuScreen2A8 (the screen at +0x2a8)

// The embedded screen whose module are not reconstructed yet: only the constructor, destructor and Setup slot
// the manager calls are named; the rest of the object is padding of the right size. Their vtables and members are
// unknown (the Release/Slot3 slots are declared to keep the MenuScreen interface complete).
class MenuScreen8DC : public MenuScreen {   // 0x16c bytes at manager+0x8dc
public:
    MenuScreen8DC();                   // 0x424410
    virtual ~MenuScreen8DC();          // 0x4244d0
    virtual void Setup(MenuGame* game);
    virtual void Release();
    virtual void Slot3(int unused);
    void SetValue120(int v);           // 0x4243b0
    int Reset(MenuGame* game, int arg); // 0x424920
    int Frame();                       // 0x424960
    void LoadLayout();                 // 0x425220
    std::string title;                 // 0x20
    int unk30[5];                      // 0x30
    MenuLabel two[2];                  // 0x44
    MenuLabel six[6];                  // 0x8c
    int unk164;                        // 0x164
    NetRace* net;                      // 0x168: serviced every frame in Frame (set by Reset)
};
class MenuManager : public MenuScreen {
public:
    MenuManager();                     // 0x43c590 (deleting form 0x43c660)
    virtual ~MenuManager();            // 0x43c680
    virtual void Setup(MenuGame* game); // 0x43c770
    virtual void Release();            // 0x43cae0
    virtual void Slot3(int unused);    // 0x4414c0 (empty, folded with the other screens' copies)
    void FUN_0043cb50(int arg);        // 0x43cb50: forwards to 0x42a210 with this + 0x2a8 (ret 4)
    void FUN_00438d00();               // 0x438d00 (MenuScreen method)
    int Run(MenuGame* game, NetRace* net_); // 0x43cb70
    int Frame();                       // 0x43cc50
    void BuildRects();                 // 0x43d1b0

    std::string title;                 // 0x20: text 0x3a
    int unk30;                         // 0x30
    int unk34;                         // 0x34
    float textScaleX;                  // 0x38: screen width / 1024 * 32
    float textScaleY;                  // 0x3c: screen height / 768 * 16
    int unk40;                         // 0x40: -1 after Setup
    MenuLabel items[6];                // 0x44: texts 0x3b .. 0x3f and 0x18 "Back"
    RaceOptionsMenu raceOptions;       // 0x11c
    MenuScreen2A8 screen2a8;           // 0x2a8
    SoundMenu sound;                   // 0x750
    MenuScreen8DC screen8dc;           // 0x8dc
    MenuScreenA48 screena48;           // 0xa48
    NetRace* net;                      // 0xcf4: network session serviced every frame (may be null)
};

#endif
