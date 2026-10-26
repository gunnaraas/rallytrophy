/* PlayerNameMenu (0x43d770-0x43f9df): the "Enter your name" screen (six name rows with a face icon each, Continue,
 * Back), a MenuScreen subclass embedded in the Game object at +0x14cc (0x9d0 bytes, vtable 0x4e17fc). Map:
 * analysis/range-43d770.md section 1. Include after unk_481390.h, rallytrophy.h and unk_450ee0.h (MenuScreen,
 * MenuLabel, MenuGame). */
#ifndef UNK_43D770_H
#define UNK_43D770_H

// One 64x32 pixel cell of the icon sheet (0x1c bytes): texture and UV corners. Implicit constructor 0x43db40 (vector
// constructor of the two HudPoints, empty ctor 0x490ad0 / dtor 0x402c90) and destructor 0x43db60.
class SpriteCell {
public:
    void* texture;                     // 0x00: Game+0x1390f8
    HudPoint uv[2];                    // 0x04: uv[0] top left, uv[1] bottom right (c * 0.25, r * 0.0625)
    float width;                       // 0x14: 64.0
    float height;                      // 0x18: 32.0
};

// Default player names: six 30-byte slots (0x501338) and the icon index of each (0x5013ec); the row count (0x501334,
// initially 1) and the career flag (0x501458).
extern char DAT_00501338[6][30];
extern int DAT_005013ec[6];
extern int DAT_00501334;
extern int DAT_00501458;

#ifndef HITPOINT_DEFINED
#define HITPOINT_DEFINED
class HitPoint {
public:
    HitPoint(const HudPoint& other);   // 0x424300
    float x, y;
};
#endif

class PlayerNameMenu : public MenuScreen {
public:
    PlayerNameMenu();                  // 0x43d770
    virtual ~PlayerNameMenu();         // 0x43db80 (deleting form 0x43db20)
    virtual void Setup(MenuGame* game); // 0x43dc70
    virtual void Release();            // 0x43de10 (own implementation: frees the plate sprite)
    virtual void Slot3(int unused);    // 0x4414c0 (empty, folded with the other screens' copies)
    int Open(MenuGame* game, NetRace* net); // 0x43de50
    int Run();                         // 0x43df70
    void BuildRects();                 // 0x43ee70
    void DrawTextCursor(RenderDevice* device, float x, float y, Color color); // 0x43f400 (ret 0x10; Color by value)
    // MenuScreen methods, declared here like the sibling screens do
    int HitTest(HitPoint position, int first, int last); // 0x438f90
    void AdjustValue(int* value, int delta, int lo, int hi); // 0x438c40
    void FUN_00438d00();               // 0x438d00: clear the item rectangles
    void DrawIconCell(TextRenderer* text, SpriteCell* cell, float x, float y, int selected, int icon, int a, int b); // 0x438990
    void SetupLabels();                // 0x43f570
    void DrawPlate(float x, float y);  // 0x43f740 (ret 8)
    int CheckCheatName(const char* name); // 0x43f8b0 (ret 4; this unused)

    MenuLabel labels[8];               // 0x20: [0..5] "Player n:", [6] Continue (text 0x17), [7] Back (text 0x18)
    MenuLabel positions[6];            // 0x140: only x/y written by the constructor
    int icon[6];                       // 0x218: copy of the global 0x5013ec
    int editing[6];                    // 0x230: set to 1 by Run
    SpriteCell cells[64];              // 0x248
    int rowCount;                      // 0x948: = DAT_00501334
    void* iconTexture;                 // 0x94c: = game + 0x1390f8
    Sprite* plate;                     // 0x950: data/menu/linename.tga
    DWORD blinkTick;                   // 0x954
    int blinkOn;                       // 0x958
    std::string scratch;               // 0x95c
    std::string name[6];               // 0x96c
    NetRace* net;                      // 0x9cc
};

#endif
