/* ReplayMenu (0x44b930-0x44d25f): the replay-list screen embedded in the Game object at +0x29e4. Map:
 * analysis/range-44b930.md section 1. Include after unk_481390.h, rallytrophy.h and unk_450ee0.h (MenuScreen,
 * MenuLabel, UIRect, MenuGame). */
#ifndef UNK_44B930_H
#define UNK_44B930_H

// One replay slot (0x70 bytes): the 0x6c-byte header shape of the profile record plus an "empty" flag byte.
#ifndef HITPOINT_DEFINED
#define HITPOINT_DEFINED
class HitPoint {
public:
    HitPoint(const HudPoint& other);   // 0x424300
    float x, y;
};
#endif

struct ReplayRecord {
    char name[0x20];                   // 0x00: replay name (printed in the list)
    int textIndex;                     // 0x20: text id 0x138 + value (row 0x61)
    int rally;                         // 0x24: text id 0xb4 + value; index into the Game table at +0x3680 (stride 0x5e8)
    int value;                         // 0x28
    BYTE unk2c[0x38 - 0x2c];
    SYSTEMTIME when;                   // 0x38 (the map said 0x30): date and time of the replay
    char text[0x25];                   // 0x48: name shown in the list and in the detail panel
    BYTE flagD;                        // 0x6d: selects text 0x1a2 over 0x1a1
    char flagE;                        // 0x6e: selects text 0x2a over 0x1b and subtracts the rally table entry
    BYTE empty;                        // 0x6f: slot unused
};

// The replay store at Game+0x2dd0 (0x70-byte heap object; methods live in the earliest TU).
class ReplayStore {
public:
    int FUN_00403ca0(int index);       // 0x403ca0: replay file usable
    void Load(int index, ReplayRecord* record); // 0x404930
    char IsUsable(int index);          // 0x403ca0 again, as the char-returning view Frame uses
};

// The replay screen (0x3e8 bytes; vtable 0x4e19c4).
class ReplayMenu : public MenuScreen {
public:
    ReplayMenu();                      // 0x44b930
    virtual ~ReplayMenu();             // 0x44b9c0 (deleting form 0x44b9a0)
    virtual void Setup(MenuGame* game); // 0x44ba30
    virtual void Release();            // 0x43ff80 (folded with the other screens' copies)
    virtual void Slot3(int skipPresent); // 0x44cfb0: Draw
    int Run(MenuGame* game);           // 0x44bc00
    int Frame();                       // 0x44bc30: blocking screen loop
    void BuildRects();                 // 0x44c320
    void DrawField(int k);             // 0x44c580
    void RefreshRecords();             // 0x44cf80
    int HitTest(HitPoint position, int first, int last); // 0x438f90 (MenuScreen method)

    MenuLabel labels[2];               // 0x20: title (text 0x19f), back (text 0x18)
    ReplayRecord records[8];           // 0x68: stride 0x70
};

#endif
