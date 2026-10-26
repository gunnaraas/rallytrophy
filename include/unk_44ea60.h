/* SlotMenu (0x44ea60-0x450edf): the eight-slot list screen embedded in the Game object at +0x2008. Map:
 * analysis/range-44ea60.md. Include after unk_481390.h, rallytrophy.h and unk_450ee0.h (MenuScreen, MenuLabel,
 * UIRect, MenuGame). */
#ifndef UNK_44EA60_H
#define UNK_44EA60_H

// One slot record (0x6c bytes) filled by the profile object at Game+0x2dcc (0x44e6a0).
struct SlotRecord {
    char name[0x20];                   // 0x00: slot name (edited text is copied here)
    int kind;                          // 0x20: 0 = unused, 1 = full record, other = short record
    int index;                         // 0x24: text index (+0xb4) / game table index
    int value;                         // 0x28
    int pad2c;                         // 0x2c
    SYSTEMTIME when;                   // 0x30: date and time shown in the last row
    int count1;                        // 0x40
    int count2;                        // 0x44
    char text[0x20];                   // 0x48: pre-built value text (the first player's name in a slot file header)
    int remaining;                     // 0x68: players that are not in the first group (slot file header)
};

// The profile manager at Game+0x2dcc (module 0x44e3e0).
class SlotStore {
public:
    void GetSlotInfo(int index, SlotRecord* record); // 0x44e6a0
    bool LoadRelay();                  // 0x44deb0: savegame/relay.sav -> body at +0x6c, rebuilds the race
    void SaveRelay(char level, char advance); // 0x44e3e0
    void SaveStats();                  // 0x44e650
};

// The hit test takes its point by value; the copy (0x424300) is out of line and the class has no destructor.
#ifndef HITPOINT_DEFINED
#define HITPOINT_DEFINED
class HitPoint {
public:
    HitPoint(const HudPoint& other);   // 0x424300
    float x, y;
};
#endif

// The slot screen (0x4f4 bytes; vtable 0x4e1a4c).
class SlotMenu : public MenuScreen {
public:
    SlotMenu();                        // 0x44ea60
    virtual ~SlotMenu();               // 0x44eaf0 (deleting form 0x44ead0)
    virtual void Setup(MenuGame* game); // 0x44eb60
    virtual void Release();            // 0x43ff80 (folded with the other screens' copies)
    virtual void Slot3(int skipPresent); // 0x450be0: Draw
    int Run(MenuGame* game);           // 0x44ed70
    int Frame();                       // 0x44ed90: blocking screen loop
    void BuildRects();                 // 0x44fad0
    void DrawField(int slot);          // 0x44fd30
    void RefreshSlots();               // 0x450920
    void DrawCaret(RenderDevice* device, float x, float y, Color color); // 0x450a70 (never reads this)
    int HitTest(HitPoint position, int first, int last); // 0x438f90 (MenuScreen method)

    MenuLabel labels[10];              // 0x20: title (0x170), back (0x18), eight slot names
    SlotRecord records[8];             // 0x188: stride 0x6c
    int clock;                         // 0x4e8: timeGetTime of the last caret toggle
    int caretOn;                       // 0x4ec: caret visible
    int editing;                       // 0x4f0: a slot name is being typed
};

#endif
