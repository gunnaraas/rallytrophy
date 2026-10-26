/* Game (0x4303e0-0x437e9f): the 0x139894-byte front-end/game singleton at DAT_0056b0c8 that embeds every menu
 * screen. Map: analysis/range-4303e0.md. Nonpolymorphic. Members are declared in construction order: the ctor's
 * unwind states (0x4e7840) number them 0x00..0x18 (HudPoint scale and the rects vector have no ctor call but an
 * unwind entry). Other views of the same object: GameView (unk_481390.h) and MenuGame (unk_450ee0.h).
 * Include after unk_481390.h, unk_4843e0.h, rallytrophy.h, unk_450ee0.h, unk_445e80.h, unk_43f9e0.h, unk_439170.h,
 * unk_43c590.h, unk_43a390.h, unk_43d770.h, unk_44ea60.h, unk_44b930.h, unk_441370.h and unk_45e480.h. */
#ifndef UNK_4303E0_H
#define UNK_4303E0_H

// The embedded screens with their own modules (unk_42e930.h needs unk_44ea60.h first).
#include "unk_427600.h"
#include "unk_42c2b0.h"
#include "unk_42e930.h"
#include "unk_4266c0.h"

// Embedded screens whose modules are not reconstructed yet (all MenuScreen subclasses: their ctors call 0x4381c0).
// Only ctor/dtor are named; the rest is padding of the right size.
class GameScreen59E4 : public MenuScreen {  // 0x24c bytes at Game+0x59e4 (the StageTable view of unk_481390.h)
public:
    GameScreen59E4();                  // 0x4205e0
    virtual ~GameScreen59E4();         // 0x420730
    virtual void Setup(MenuGame* game);
    virtual void Release();
    virtual void Slot3(int unused);
    BYTE unk20[0x24c - 0x20];
};

// The two heap objects at Game+0x2dcc / +0x2dd0 (SaveGame / ReplayStore views elsewhere). Both have an empty
// out-of-line ctor (folded to 0x403c90) and dtor (folded to 0x48e660).
class GameSave {                       // 0x614 bytes
public:
    GameSave();                        // 0x403c90 (empty, folded)
    ~GameSave();                       // 0x48e660 (empty, folded)
    BYTE data[0x614];
};
class GameReplays {                    // 0x70 bytes
public:
    GameReplays();                     // 0x403c90 (empty, folded)
    ~GameReplays();                    // 0x48e660 (empty, folded)
    BYTE data[0x70];
};

// One of the 32 text records at Game+0x139490 (0x20 bytes): a std::string and four more dwords. Its ctor/dtor
// are the std::string ones (0x430700 / 0x430720 after folding).
class GameText {
public:
    GameText() {}
    ~GameText() {}
    std::string text;                  // 0x00
    int unk10[4];                      // 0x10
};

class Game {
public:
    Game();                            // 0x4303e0
    ~Game();                           // 0x430760
    void FUN_004309d0();               // 0x4309d0: runs the game (menus and races)
    void FUN_00434300();               // 0x434300: alternative entry (network)
    void FUN_004309b0();               // 0x4309b0: save the highscore table when dirty
    void FUN_00430eb0();               // 0x430eb0: release the five TextRenderers
    void FUN_00430cf0();               // 0x430cf0: create the five TextRenderers
    void FUN_00431c50();               // 0x431c50: release the UI/resource state
    void FUN_00435430(float x, float y); // 0x435430: draw the mouse cursor
    void FUN_004355a0(float x, float y); // 0x4355a0
    void FUN_00435710(float x, float y); // 0x435710
    void FUN_00435880(float x, float y); // 0x435880
    void FUN_004359f0(float x, float y); // 0x4359f0
    void FUN_00435b60(int a, int b, int c, int d, int e, float f); // 0x435b60
    void FUN_00435ca0(int a, int b, int c, int d, int e, float f); // 0x435ca0
    void FUN_00436ed0();               // 0x436ed0: background/overlay draw helper
    int FUN_00437220(int a);           // 0x437220: confirmation dialog, -1 while open (ret 4)

    void FUN_00435de0(float x, float y);
    void FUN_00435f50(float x, float y);
    void FUN_004360c0(float x, float y, float w, float h, unsigned int flag);
    void FUN_00437040(int noPresent);

    int unk0;                          // 0x00
    ServiceMenu service;               // 0x04 (state 0x00)
    GameScreen11A4 screen11a4;         // 0x11a4 (0x01)
    Screen1378 lobby;                  // 0x1378 (0x02)
    PlayerNameMenu playerName;         // 0x14cc (0x03)
    GameScreen1E9C screen1e9c;         // 0x1e9c (0x04)
    SlotMenu slots;                    // 0x2008 (0x05)
    GameScreen24FC screen24fc;         // 0x24fc (0x06)
    ReplayMenu replays;                // 0x29e4 (0x07)
    GameSave* save;                    // 0x2dcc: new'd in the ctor (0x614 bytes)
    GameReplays* replayStore;          // 0x2dd0: new'd in the ctor (0x70 bytes)
    GameScreen2DD4 screen2dd4;         // 0x2dd4 (0x08)
    MenuManager menu;                  // 0x2df4 (0x09)
    TrackSelectMenu trackSelect;       // 0x3aec (0x0a)
    GameScreen59E4 screen59e4;         // 0x59e4 (0x0b)
    RecordTable records;               // 0x5c30 (0x0c)
    RecordTable* remoteRecords;        // 0x139074: only when DAT_00501471
    int selection;                     // 0x139078: front-end state 0..6
    int unk13907c;                     // 0x13907c
    RaceSession session;               // 0x139080 (0x0d)
    TextRenderer* bigText;             // 0x1390b8
    TextRenderer* text1390bc;          // 0x1390bc
    TextRenderer* text1390c0;          // 0x1390c0
    TextRenderer* smallText;           // 0x1390c4
    TextRenderer* text;                // 0x1390c8
    HudPoint scale;                    // 0x1390cc (0x0e: no ctor code, inline empty dtor)
    void* unk1390d4;                   // 0x1390d4
    void* unk1390d8;                   // 0x1390d8
    void* unk1390dc;                   // 0x1390dc
    void* unk1390e0;                   // 0x1390e0
    void* unk1390e4;                   // 0x1390e4
    void* unk1390e8;                   // 0x1390e8
    void* unk1390ec[7];                // 0x1390ec
    void* unk139108;                   // 0x139108
    void* unk13910c;                   // 0x13910c
    MenuLabel labels[7];               // 0x139110 (0x0f)
    int unk13920c;                     // 0x13920c: 2
    int unk139210;                     // 0x139210
    int unk139214;                     // 0x139214
    int unk139218;                     // 0x139218
    int unk13921c;                     // 0x13921c
    int unk139220;                     // 0x139220: 100
    SoundSource soundMove;             // 0x139224 (0x10)
    SoundSource soundSelect;           // 0x139294 (0x11)
    SoundSource soundBack;             // 0x139304 (0x12)
    SoundSource soundLimit;            // 0x139374 (0x13)
    CarTextureCache textures1;         // 0x1393e4 (0x14)
    CarTextureCache textures2;         // 0x139404 (0x15)
    CarModelCache models;              // 0x139424 (0x16)
    std::vector<UIRect*> rects;        // 0x139460 (0x17)
    int unk139470;                     // 0x139470
    int unk139474;                     // 0x139474
    float unk139478;                   // 0x139478: 24.0
    float unk13947c;                   // 0x13947c: 24.0
    int unk139480;                     // 0x139480
    int unk139484;                     // 0x139484: 1
    int unk139488;                     // 0x139488
    int unk13948c;                     // 0x13948c
    GameText names[32];                // 0x139490 (0x18)
    int unk139890;                     // 0x139890
};

#endif
