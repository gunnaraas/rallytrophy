/* Screen1378 (0x43a390-0x43c58f): a menu screen embedded in the Game object at +0x1378 (at most 0x154 bytes), vtable
 * 0x4e1764. Map: analysis/range-439080.md section 3. Include after unk_481390.h, rallytrophy.h, unk_450ee0.h and
 * unk_43c590.h (MenuScreen, MenuGame). The network lobby screen (player list, session description, ready state). */
#ifndef UNK_43A390_H
#define UNK_43A390_H

#ifndef HITPOINT_DEFINED
#define HITPOINT_DEFINED
class HitPoint {
public:
    HitPoint(const HudPoint& other);   // 0x424300
    float x, y;
};
#endif

extern int DAT_005014e0[];             // allowed-car-class masks, indexed by T_56b0b8::m_cars
extern unsigned char DAT_0057ba35, DAT_0057ba43, DAT_0057ba4e, DAT_0057ba50;

// One lobby row (0x30 bytes, six of them), filled from the NetRace player vector by Draw.
struct LobbyEntry {
    char name[0x20];                   // 0x00: Player::name
    int unk20;                         // 0x20: Player::unk20 (car icon index)
    int state;                         // 0x24: Player::state
    int unk28;                         // 0x28: Player::unk2c, copied when state == 2
    int ping;                          // 0x2c: NetRace latency
};

class Screen1378 : public MenuScreen {
public:
    Screen1378();                      // 0x43a3c0
    virtual ~Screen1378();             // 0x43a440 (deleting form 0x43a420)
    virtual void Setup(MenuGame* game); // 0x43a490
    virtual void Release();            // 0x43a640
    virtual void Slot3(int noPresent); // 0x43b1d0: Draw (Present unless the argument is 1)
    int Open(MenuGame* game, NetRace* net); // 0x43a6b0
    int Frame();                       // 0x43a6e0
    void BuildRects();                 // 0x43ad80
    void Layout();                     // 0x43afa0
    int HitTest(HitPoint position, int first, int last); // 0x438f90 (MenuScreen method)
    void FUN_00438d00();               // 0x438d00: clear the item rectangles

    NetRace* net;                      // 0x20
    Sprite* unk24;                     // 0x24
    Sprite* unk28;                     // 0x28
    int unk2c;                         // 0x2c: set once a start/ready message was sent
    LobbyEntry entries[6];             // 0x30
    int count;                         // 0x150: valid entries (player count)
};

#endif
