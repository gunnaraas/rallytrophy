/* SoundMenu (0x450ee0-0x45237f): the options screen "Engine / SoundEffects / Speech / Music / MenuMusic / Comments /
 * Back" and its MenuScreen base view. Map: analysis/range-450ee0.md section 1. Include after unk_481390.h and
 * rallytrophy.h. */
#ifndef UNK_450EE0_H
#define UNK_450EE0_H

// A HUD rectangle (0x14 bytes): corners and an enabled flag. Same class as the one unk_4856e0.cpp declares locally.
class UIRect {
public:
    UIRect(HudPoint topLeft, HudPoint bottomRight)   // 0x4390a0
    {
        left = topLeft.x;
        top = topLeft.y;
        right = bottomRight.x;
        bottom = bottomRight.y;
        enabled = 1;
    }
    ~UIRect();                                     // 0x48e660 (empty, called out of line)
    HudPoint GetTopLeft();                         // 0x4243d0 (returns through a hidden pointer)
    HudPoint GetBottomRight();                     // 0x4243f0
    int Contains(HudPoint point);                  // 0x4390d0: point in screen pixels
    float left, top, right, bottom;
    int enabled;
};

// One text slot of a menu screen (0x24 bytes): the label and its position. Constructor 0x430700 and destructor
// 0x430720 are the std::string ones.
class MenuLabel {
public:
    MenuLabel() {}
    ~MenuLabel() {}
    void SetY(float py) { y = py; }
    void SetX(float px) { x = px; }
    void SetPos(float px, float py) { unk20 = -1; SetX(px); SetY(py); }
    std::string text;                  // 0x00
    float width;                       // 0x10: text width in pixels (set by the menu manager's Setup)
    float height;                      // 0x14: text line height
    float x;                           // 0x18
    float y;                           // 0x1c
    int unk20;                         // 0x20 (-1 once set up)
};

// The game object (DAT_0056b0c8) as the menu screens use it (module-local view; methods of other modules).
class MenuGame {
public:
    void Update();                     // 0x432c00: per-frame update
    void UpdateVideo();                // 0x432a10: advance the background video
    void FUN_00432a40();               // 0x432a40: draw the background video frame (if one is loaded)
    void DrawVideo(int flag);          // 0x432a60
    HudPoint* GetScale();              // 0x4329e0: sets and returns the (1, 1) pair at +0x1390cc
    void FUN_00435de0(float a, float b); // 0x435de0
    void FUN_00435f50(float a, float b); // 0x435f50
    void FUN_00435b60(int a, int b, int c, int d, int e, float f); // 0x435b60 (on DAT_0056b0c8)
    void FUN_00435430(HudPoint position); // 0x435430: draw the cursor at a point
    BYTE unk0[0x1390c4];
    TextRenderer* text;                // 0x1390c4
    // The sound sources start at +0x139224 with a stride of 0x70: [0] move, [1] select, [2] back, [3] limit.
    void Play(int index) { ((GameSound*)((BYTE*)this + 0x139224 + index * 0x70))->FUN_0049b730(); }
};

class SpriteCell;                      // include/unk_43d770.h

#ifndef HITPOINT_DEFINED
#define HITPOINT_DEFINED
class HitPoint {
public:
    HitPoint(const HudPoint& other);   // 0x424300
    float x, y;
};
#endif

// Menu screen base class (0x20 bytes; ctor 0x4381c0, dtor 0x438210, vtable 0x4e16dc, 17 derived screens). Only the
// members and helpers the SoundMenu reads are named.
class MenuScreen {
public:
    MenuScreen();                      // 0x4381c0
    virtual ~MenuScreen();             // 0x438210 (deleting form 0x4381f0)
    virtual void Setup(MenuGame* game) = 0;  // slot 1
    virtual void Release() = 0;        // slot 2
    virtual void Slot3(int unused) = 0; // slot 3 (ret 4)
    void AdjustVolume(int* value, int delta); // 0x438be0: add delta, clamp to 0..100, play a sound
    void Toggle(int* value, int delta); // 0x438ca0
    void DrawSlider(float x, float y, int value, int selected); // 0x438700
    void DrawToggle(TextRenderer* text, MenuLabel* labels, float x, float y, float w, int selected, int value,
                    int a, int b);     // 0x438d80 (ret 0x24)
    void DrawCursor(HudPoint position); // 0x438f40
    // Drawing and input helpers (src/unk_437ea0.cpp)
    void DrawCellA(float x, float y, float w, float h, Color color, int flag); // 0x438240 (ret 0x18): left arrow cell
    void DrawCellB(float x, float y, float w, float h, Color color, int flag); // 0x4383b0 (ret 0x18): right arrow cell
    void DrawBar(float x, float y, float w, float h, int value); // 0x438520 (ret 0x14): slider bar
    void DrawSpinner(TextRenderer* text, std::string* label, float x, float y, float w, int enabled, Color color,
                     int value, int lo, int hi); // 0x438810: arrows around a centred label
    void DrawIconCell(TextRenderer* text, SpriteCell* cell, float x, float y, int selected, int icon, int a, int b); // 0x438990 (ret 0x20)
    void AdjustValue(int* value, int delta, int lo, int hi); // 0x438c40
    void FUN_00438d00();               // 0x438d00: clear and delete the item rectangles
    void DrawSelector(TextRenderer* text, std::string* value, float x, float y, float w, int selected, int current,
                      int lo, int hi); // 0x438dc0 (ret 0x24): value centred between the two arrows
    int HitTest(HitPoint position, int first, int last); // 0x438f90
    void Begin(MenuGame* game) { m_state = 1; m_game = game; }
    int m_state;                       // 0x04: 1 once Setup has run
    int m_selected;                    // 0x08
    MenuGame* m_game;                  // 0x0c
    std::vector<UIRect*> m_items;      // 0x10
};

// The options screen (0x18c bytes, embedded in the menu manager at +0x750; vtable 0x4e1a7c).
class SoundMenu : public MenuScreen {
public:
    SoundMenu();                       // 0x450ee0
    virtual ~SoundMenu();              // 0x450f90 (deleting form 0x450f70)
    virtual void Setup(MenuGame* game); // 0x451040
    virtual void Release();            // 0x43ff80 (folded with the other screens' copies)
    virtual void Slot3(int unused);    // 0x4414c0 (folded)
    int Run(MenuGame* game, NetRace* net); // 0x451430
    int Frame();                       // 0x451460
    void LoadLayout();                 // 0x451c10

    MenuLabel labels[2];               // 0x20: texts 2 and 1 ("off"/"on")
    MenuLabel head;                    // 0x68: text 0x5b
    MenuLabel items[7];                // 0x8c: texts 0x5c .. 0x18
    NetRace* net;                      // 0x188: network session serviced every frame (may be null)
};

extern float DAT_0057ba00, DAT_0057ba04;
class MouseInput;
extern DWORD DAT_00508bfc, DAT_00508c04, DAT_00508c0c;

#endif
