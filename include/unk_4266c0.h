/* GameScreen2DD4 (0x4266c0-0x4275ff): the 0x20-byte menu screen embedded at Game+0x2dd4 (vtable 0x4e1548). It adds no
 * members to MenuScreen (ctor, dtor and unwind table show only the base). Map: analysis/range-4266c0.md. Include
 * after unk_481390.h, rallytrophy.h and unk_450ee0.h. */
#ifndef UNK_4266C0_H
#define UNK_4266C0_H

class GameScreen2DD4 : public MenuScreen {
public:
    GameScreen2DD4();                  // 0x4266c0
    virtual ~GameScreen2DD4();         // 0x426700 (deleting form 0x4266e0)
    virtual void Setup(MenuGame* game); // 0x426750
    virtual void Release();            // 0x43ff80 (folded)
    virtual void Slot3(int unused);    // 0x4414c0 (folded)
    int Run(MenuGame* game);           // 0x426770
    int Frame();                       // 0x4267a0
    void LoadLayout();                 // 0x4270c0: menu hit rectangles
};

#endif
