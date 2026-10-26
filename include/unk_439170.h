/* MenuScreenA48: the MenuManager embedded menu at +0xa48 (0x2ac bytes). */
#ifndef UNK_439170_H
#define UNK_439170_H

#include "unk_450ee0.h"

class MenuScreenA48 : public MenuScreen {
public:
    MenuScreenA48();
    virtual ~MenuScreenA48();
    virtual void Setup(MenuGame* game);
    virtual void Release();
    virtual void Slot3(int unused);
    int Run(MenuGame* game, NetRace* net);
    int Frame();                  // 0x4398e0
    void BuildRects();            // 0x43a080
    void FUN_00438c40(int* value, int delta, int lo, int hi); // 0x438c40 (MenuScreen method)
    void DrawSetup();

    MenuLabel items[5];        // +0x20
    MenuLabel onOff[2];        // +0xd4
    MenuLabel four[4];        // +0x11c
    MenuLabel seven[7];        // +0x1ac
    NetRace* net;                 // +0x2a8: network session serviced every frame (may be null)
};

#endif
