/* Save-game file code (0x44d260-0x44ea5f): the 0x614-byte object at Game+0x2dcc. Map: analysis/range-44b930.md
 * section 2. Include after unk_481390.h, rallytrophy.h, unk_450ee0.h and unk_44ea60.h (SlotRecord). */
#ifndef UNK_44D260_H
#define UNK_44D260_H

// The save object: the slot-file header (SlotRecord, 0x6c bytes) followed by the career state.
class SaveGame : public SlotRecord {
public:
    void SaveSlot(int slot);           // 0x44d260: write savegame/{arcade|rally}<slot>.SAV
    int LoadSlot(int slot);            // 0x44d6a0: returns 0/1
    void FillHeader();                 // 0x44e960: describe the running race in the header
    void FUN_0048e660();               // 0x48e660 (empty, folded)
    // LoadRelay 0x44deb0, SaveRelay 0x44e3e0 and SaveStats 0x44e650 are written as SlotStore methods
    // (include/unk_44ea60.h): the same object, viewed through FB3_Body in src/unk_44d260.cpp.

    BYTE body[0x614 - sizeof(SlotRecord)]; // 0x6c: career state (relay.sav body starts here)
};

#endif
