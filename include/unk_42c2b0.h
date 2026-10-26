/* GameScreen1E9C (0x42c2b0-0x42e92f): a MenuScreen subclass embedded in the Game object at +0x1e9c (0x16c bytes,
 * vtable 0x4e1630 {0x42c360, Setup 0x42c400, Release 0x42c860, Slot3 0x4414c0}). It shows the high-score box
 * (data/menu/highscorebox.tga) and country flags. Map: analysis/range-4266c0.md module 4. Include after
 * unk_481390.h, rallytrophy.h and unk_450ee0.h (MenuScreen, MenuLabel, MenuGame, Sprite). Members recovered from the
 * ctor (unwind 0x4e7420: base, items), the dtor (0x4e7450: base, items, toggle) and Setup. */
#ifndef UNK_42C2B0_H
#define UNK_42C2B0_H

class GameScreen1E9C : public MenuScreen {
public:
    GameScreen1E9C();                  // 0x42c2b0
    virtual ~GameScreen1E9C();         // 0x42c380 (deleting form 0x42c360)
    virtual void Setup(MenuGame* game); // 0x42c400
    virtual void Release();            // 0x42c860
    virtual void Slot3(int unused);    // 0x4414c0 (empty, folded with the other screens' copies)
    void Run(MenuGame* game);          // 0x42c8c0: Setup, then the screen loop (result ignored by Game 0x434df0)
    int Frame();                // 0x42c8e0: the screen loop
    void BuildRects();               // 0x42d710: called at the end of Setup
    void DrawRanking();                // 0x42ddc0: the ranking table
    void DrawFlag(float x, float y, int flag); // 0x42e760: one 32x16 cell of Flags.tga (4 columns x 16 rows)

    MenuLabel items[5];               // 0x20: texts 0xb1, 0x29, 0x1b, 0x61 and 0x18 "Back" (positioned in Setup)
    MenuLabel toggle[2];                // 0xd4: texts 0x35 and 0x2a (not positioned)
    Sprite* box;                       // 0x11c: data/menu/highscorebox.tga
    Sprite* flags;                     // 0x120: the Game's flag sheet (Game+0x1390f8)
    int unk124;                        // 0x124: 0 (ctor)
    int count;                        // 0x128: 1 (ctor)
    int value;                        // 0x12c: 1 (ctor)
    int unk130;                        // 0x130: 0 (ctor)
    int mode;                        // 0x134: 0 (Setup)
    int value0;                        // 0x138: 1 (ctor)
    int value1;                        // 0x13c: 1 (ctor)
    int limitA[5];                  // 0x140: TrackSelectMenu count[i].limitA (+0x2c), copied in Setup
    int limitB[5];                  // 0x154: TrackSelectMenu count[i].limitB (+0x30)
    int unk168;                        // 0x168
};

#endif
