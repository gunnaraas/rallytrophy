/* GameScreen24FC (0x42e930-0x4303df): the second eight-slot list screen (twin of SlotMenu, include/unk_44ea60.h)
 * embedded in the Game object at +0x24fc (0x4e8 bytes, vtable 0x4e1688). Map: analysis/range-4266c0.md module 5.
 * Members from the ctor (labels: the only unwind state of FuncInfo 0x4e7670) and the record reads of 0x42ec70 /
 * 0x42f5c0 (kind at +0x1a8, count2 at +0x1cc). Include after unk_481390.h, rallytrophy.h, unk_450ee0.h (MenuScreen,
 * MenuLabel, UIRect, MenuGame) and unk_44ea60.h (SlotRecord). */
#ifndef UNK_42E930_H
#define UNK_42E930_H

class GameScreen24FC : public MenuScreen {
public:
    GameScreen24FC();                  // 0x42e930
    virtual ~GameScreen24FC();         // 0x42e9c0 (deleting form 0x42e9a0)
    virtual void Setup(MenuGame* game); // 0x42ea30
    virtual void Release();            // 0x43ff80 (folded with the other screens' copies)
    virtual void Slot3(int skipPresent); // 0x4301b0: Draw
    int Run(MenuGame* game);           // 0x42ec40
    int Frame();                       // 0x42ec70: blocking screen loop
    void BuildRects();                 // 0x42f360
    void DrawField(int slot);          // 0x42f5c0: one slot of the list
    void RefreshSlots();               // 0x450920 (folded with SlotMenu::RefreshSlots)

    MenuLabel labels[10];              // 0x20: title (0x99), back (0x18), eight slot names
    SlotRecord records[8];             // 0x188: stride 0x6c
};

#endif
