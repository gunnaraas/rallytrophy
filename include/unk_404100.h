// Replay file writer/reader on the 0x70-byte object at Game+0x2dd0 (0x404100-0x404b9f); this object doubles as the
// scratch header record (layout: ReplayRecord in unk_44b930.h). Round-trips through savegame/replay<N>.dat.
#ifndef UNK_404100_H
#define UNK_404100_H
#include <windows.h>

class File;
class ReplayFiles {
public:
    bool SaveReplay(int index, const char* label);          // 0x404100: savegame/replay<index>.dat
    bool SaveChamp(void);                                   // 0x4042c0: savegame/champ<a>_<b>.dat, label "autosave"
    bool Write(const char* filename, const char* label);    // 0x404590: header + player/car/stage state
    void WriteState(class H6Player* player, File* f);       // 0x4047c0: per-player block
    char Load(int index, ReplayFiles* record);              // 0x404930: read the header of savegame/replay<index>.dat
    BYTE b[0x70];
};
#endif
