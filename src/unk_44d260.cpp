// Save-game file code (0x44d260-0x44ea5f). Map: analysis/range-44b930.md section 2; classes in include/unk_44d260.h.
// Work groups: FB-1 (SaveSlot, GetSlotInfo, FillHeader, module end: prefix FB1_), FB-2 (LoadSlot), FB-3 (LoadRelay,
// SaveRelay, SaveStats).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_450ee0.h"
#include "unk_44ea60.h"
#include "unk_44d260.h"

extern void (__cdecl *DAT_0059b2f8)();
#include <string.h>
#include <stdlib.h>
#include <vector>
#include "unk_46bb90.h"

// ---- Group FB-1 ----
// One player's record in a slot file (0x74 bytes, starting with its own size).
struct FB1_PlayerRecord {
    int size;                          // 0x74
    char name[0x20];
    int unk20;
    int unk24;
    int unk2c;
    int results[10];
    int unk1a50;
    int unk1a54;
    int unk1a5c;
    int unk1a60;
    int startSlot;
    int unk1a68;
    int unk1a64;
};
// The statistics block at car + 0x1c874 (0x8c bytes) with the three arrays it owns.
struct FB1_Stats {
    BYTE unk0[0x74];
    int count74;
    int count78;
    int count7c;
    void* array80;
    void* array84;
    void* array88;
};

// The path string as the original builds it: constructor calls the out-of-line _Tidy (0x402700), the destructor is
// std::string::_Tidy(true) expanded inline (the first site in a function may call it instead), and assign/append
// sites are pinned: out of line (0x404ba0/0x403ba0) or expanded around _Grow (0x402980) and _Eos (0x402960).
class FB1_Path {
public:
    FB1_Path(const std::allocator<char>& a = std::allocator<char>()) : alloc(a) { Tidy(false); }
    ~FB1_Path() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);                             // 0x402700
    void AssignCall(const char* s, unsigned int n);   // 0x404ba0
    void AppendCall(const char* s, unsigned int n);   // 0x403ba0
    bool Grow(unsigned int n, bool trim);              // 0x402980
    void Eos(unsigned int n);                         // 0x402960
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    void AppendD(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            std::_Xlen();
        }
        if (n > 0) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                Eos(total);
            }
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

// ---- Group FB-2: LoadSlot ----
extern int DAT_00501334;               // player count of the loaded slot
extern int DAT_00501460;
extern char DAT_00501338[][30];        // player names of the loaded slot
extern int DAT_005013ec[];             // player ids of the loaded slot

// The VC6 std::string layout with the original's inline-expansion choices: _Tidy, assign(const char*, n) (0x404ba0)
// and append (0x403ba0) are separate functions.
class FB2_Path {
public:
    FB2_Path(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~FB2_Path()
    {
        if (ptr == 0) {
        } else if (((unsigned char*)ptr)[-1] == 0 || ((unsigned char*)ptr)[-1] == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --((unsigned char*)ptr)[-1];
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    void Assign(const char* s, unsigned int n); // 0x404ba0
    void Append(const char* s, unsigned int n); // 0x403ba0
    void Add(const char* s) { Append(s, strlen(s)); }
    void Set(const char* s) { Assign(s, strlen(s)); }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

// One 0x74-byte player record of a slot file.
struct FB2_Record {
    int size;                          // 0x00
    char name[0x20];                   // 0x04
    int id;                            // 0x24
    int unk28;                         // 0x28
    int flags;                         // 0x2c
    int stats[10];                     // 0x30
    int tail[7];                       // 0x58
};


// ---- Group FB-3: LoadRelay, SaveRelay, SaveStats ----
extern int DAT_00501460;
extern int DAT_0050145c;
extern int DAT_00501334;
extern int DAT_005013ec[];             // one dword per player
extern DWORD DAT_004fa2e4[];           // cipher key of relay.sav

// The file cipher (empty class; 0x4010f0 / 0x401180).
class FB3_Cipher {
public:
    FB3_Cipher() {}
    ~FB3_Cipher() {}
    void Encrypt(DWORD* data, DWORD* key, int count);   // 0x4010f0
    void Decrypt(DWORD* data, DWORD* key, int count);   // 0x401180
};

// The career body at SlotStore+0x6c as the relay code sees it.
struct FB3_Body {
    char name[0x1e];                   // 0x6c
    BYTE kind;                         // 0x8a
    BYTE flags;                        // 0x8b
    BYTE unk8c;                        // 0x8c
    BYTE rally;                        // 0x8d
    BYTE stage;                        // 0x8e
    BYTE done;                         // 0x8f
    BYTE level;                        // 0x90
};
struct FB3_Save {
    BYTE header[0x6c];
    FB3_Body body;
};
#define FB3_BODY(self) (&((FB3_Save*)(self))->body)

// A per-player stage result block (10 dwords at Player+0x1a28).
struct FB3_Stats {
    int v[10];
};

// original: FUN_0044d260
// NONMATCHING: RALLYTROPHY 0x44d260
// Structure matches (record = size dword 0x74 + fields, shared by both loops; path via out-of-line assign/append). The
// four-byte `itoa` buffer reduces the candidate diff 737→728; the frame remains 0x90 vs target 0x94. The original keeps
// File* in EDI with a stack home and RaceMode* in EBX; this candidate keeps File* in EBX and spills RaceMode*.
void SaveGame::SaveSlot(int slot)
{
    FillHeader();
    File* file;
    FB1_Path path;
    FB1_PlayerRecord rec;
    if (DAT_00501458) {
        path.AssignCall("savegame/arcade", strlen("savegame/arcade"));
    } else {
        path.AssignCall("savegame/rally", strlen("savegame/rally"));
    }
    {
        char buf[4];
        itoa(slot, buf, 10);
        path.AppendCall(buf, strlen(buf));
    }
    path.AppendCall(".SAV", strlen(".SAV"));
    file = new File(path.c_str(), 0x82);
    int magic = 0x8195;
    file->Write(&magic, 4);
    magic = 0x20011010;
    file->Write(&magic, 4);
    file->Write(this, 0x6c);
    RaceMode* race = (RaceMode*)DAT_00501450;
    rec.size = 0x74;
    if (DAT_00501458) {
        for (int i = 0; i < (int)race->m_players.size(); i++) {
            Player* p = race->m_players[i];
            strcpy(rec.name, p->name);
            rec.unk20 = p->unk20;
            rec.unk24 = p->unk24;
            rec.unk2c = p->unk2c;
            for (int j = 0; j < 10; j++) {
                rec.results[j] = p->unk1a28[j];
            }
            rec.unk1a50 = p->unk1a50;
            rec.unk1a54 = p->unk1a54;
            rec.unk1a5c = p->unk1a5c;
            rec.unk1a60 = p->unk1a60;
            rec.startSlot = p->startSlot;
            rec.unk1a68 = p->unk1a68;
            rec.unk1a64 = p->unk1a64;
            file->Write(&rec, 0x74);
        }
    } else {
        for (int i = 0; i < (int)race->m_players.size(); i++) {
            Player* p = race->m_players[i];
            strcpy(rec.name, p->name);
            rec.unk20 = p->unk20;
            rec.unk24 = p->unk24;
            rec.unk2c = p->unk2c;
            for (int j = 0; j < 10; j++) {
                rec.results[j] = p->unk1a28[j];
            }
            rec.unk1a50 = p->unk1a50;
            rec.unk1a54 = p->unk1a54;
            rec.unk1a5c = p->unk1a5c;
            rec.unk1a60 = p->unk1a60;
            rec.startSlot = p->startSlot;
            rec.unk1a68 = p->unk1a68;
            rec.unk1a64 = p->unk1a64;
            file->Write(&rec, 0x74);
            if (p->local) {
                FB1_Stats* stats = (FB1_Stats*)((BYTE*)p->car + 0x1c874);
                file->Write(stats, 0x8c);
                if (stats->count74) {
                    file->Write(stats->array80, stats->count74 * 4);
                }
                if (stats->count78) {
                    file->Write(stats->array84, stats->count78 * 4);
                }
                if (stats->count7c) {
                    file->Write(stats->array88, stats->count7c * 4);
                }
            }
        }
    }
    file->Close();
    delete file;
}

// original: FUN_0044d6a0
// NONMATCHING: RALLYTROPHY 0x44d6a0
// 2092 vs 2053 bytes; control flow, calls and both player loops are the original's. Open: the frame is 12 bytes
// larger (the original puts the itoa buffer at the bottom of the frame, overlapping the loop pointer slots, and spills
// `this`), the early-exit string destructor is a call to _Tidy(true) shared by the three failure exits (here expanded
// inline per exit), and the 10-dword copy loop uses edx/esi swapped.
int SaveGame::LoadSlot(int slot)
{
    int magic1, magic2;
    FB2_Record rec;
    FB2_Path path;
    char number[10];
    if (DAT_00501458) {
        path.Set("savegame/arcade");
    } else {
        path.Set("savegame/rally");
    }
    itoa(slot, number, 10);
    path.Add(number);
    path.Add(".SAV");
    File* file;
    if (File::Exists(path.c_str())) {
        file = new File(path.c_str(), 0x81);
        file->Read(&magic1, 4);
        if (magic1 == 0x8195) {
            file->Read(&magic2, 4);
            if (magic2 == 0x20011010) {
                goto loaded;
            }
        }
    }
    return 0;
loaded:
    file->Read(this, 0x6c);
    DAT_0050132c = pad2c;
    int n = kind;
    DAT_00501334 = n;
    DAT_00501460 = 0;
    if (DAT_00501458) {
        RaceMode* race = DAT_00501450;
        race->unk28 = index;
        race->unk2c = value;
        rec.size = 0x74;
        for (int i = 0; i < n; i++) {
            file->Read(&rec, 0x74);
            Player* p = new Player(i + 1, 0);
            p->FUN_00490fb0(rec.name);
            p->unk20 = rec.id;
            strcpy(DAT_00501338[i], rec.name);
            DAT_005013ec[i] = rec.id;
            p->unk24 = rec.unk28;
            p->unk2c = rec.flags;
            for (int k = 0; k < 10; k++) {
                p->unk1a28[k] = rec.stats[k];
            }
            p->unk1a50 = rec.tail[0];
            p->unk1a54 = rec.tail[1];
            p->unk1a5c = rec.tail[2];
            p->unk1a60 = rec.tail[3];
            p->startSlot = rec.tail[4];
            p->unk1a68 = rec.tail[5];
            p->unk1a64 = rec.tail[6];
            race->AddPlayer(p);
        }
        for (int j = remaining; j > 0; j--) {
            file->Read(&rec, 0x74);
            Player* p = new Player((int)"data/AI/ai1.ini");
            p->FUN_00490fb0(rec.name);
            p->unk20 = rec.id;
            p->unk24 = rec.unk28;
            p->unk2c = rec.flags;
            for (int k = 0; k < 10; k++) {
                p->unk1a28[k] = rec.stats[k];
            }
            p->unk1a50 = rec.tail[0];
            p->unk1a54 = rec.tail[1];
            p->unk1a5c = rec.tail[2];
            p->unk1a60 = rec.tail[3];
            p->startSlot = rec.tail[4];
            p->unk1a68 = rec.tail[5];
            p->unk1a64 = rec.tail[6];
            p->FUN_00491130();
            race->AddPlayer(p);
        }
    } else {
        RaceMode* race = DAT_00501450;
        race->unk28 = index;
        race->unk2c = value;
        rec.size = 0x74;
        for (int i = 0; i < n; i++) {
            file->Read(&rec, 0x74);
            Player* p = new Player(i + 1, 0);
            p->FUN_00490fb0(rec.name);
            p->unk20 = rec.id;
            strcpy(DAT_00501338[i], rec.name);
            DAT_005013ec[i] = rec.id;
            p->unk24 = rec.unk28;
            p->unk2c = rec.flags;
            if (n == 1 && !(rec.flags & 1)) {
                DAT_00501460 = 1;
            }
            for (int k = 0; k < 10; k++) {
                p->unk1a28[k] = rec.stats[k];
            }
            p->unk1a50 = rec.tail[0];
            p->unk1a54 = rec.tail[1];
            p->unk1a5c = rec.tail[2];
            p->unk1a60 = rec.tail[3];
            p->startSlot = rec.tail[4];
            p->unk1a68 = rec.tail[5];
            p->unk1a64 = rec.tail[6];
            p->FUN_00491130();
            race->AddPlayer(p);
            Car* car = p->car;
            CarSnapshot* s = &((CarSnapView*)car)->snapshot[2];
            s->Clear();
            file->Read(s, 0x8c);
            if (s->size[0]) {
                s->buffer[0] = (BYTE*)operator new(s->size[0] * 4);
                file->Read(s->buffer[0], s->size[0] * 4);
            }
            if (s->size[1]) {
                s->buffer[1] = (BYTE*)operator new(s->size[1] * 4);
                file->Read(s->buffer[1], s->size[1] * 4);
            }
            if (s->size[2]) {
                s->buffer[2] = (BYTE*)operator new(s->size[2] * 4);
                file->Read(s->buffer[2], s->size[2] * 4);
            }
            car->FUN_0046be50(2);
        }
        for (int j = 0; j < remaining; j++) {
            file->Read(&rec, 0x74);
            Player* p = new Player(PLAYER_AI, j + 1);
            p->FUN_00490fb0(rec.name);
            p->unk20 = rec.id;
            p->unk24 = rec.unk28;
            p->unk2c = rec.flags;
            for (int k = 0; k < 10; k++) {
                p->unk1a28[k] = rec.stats[k];
            }
            p->unk1a50 = rec.tail[0];
            p->unk1a54 = rec.tail[1];
            p->unk1a5c = rec.tail[2];
            p->unk1a60 = rec.tail[3];
            p->startSlot = rec.tail[4];
            p->unk1a68 = rec.tail[5];
            p->unk1a64 = rec.tail[6];
            p->FUN_00491130();
            race->AddPlayer(p);
        }
    }
    FUN_0048e660();
    file->Close();
    delete file;
    return 1;
}

// NONMATCHING: RALLYTROPHY 0x44deb0
// The target explicitly closes the file before deleting it on bad headers; this source now preserves that sequence.
// Candidate is 1259/1321 bytes with 947 differing bytes. The target uses a 0x48 frame and local failure epilogues;
// candidate uses a 0x44 frame and merges some exits. Both use the per-player 10-dword stack temporary.
bool SlotStore::LoadRelay()
{
    DAT_00501472 = 0;
    if (!File::Exists("savegame/relay.sav")) {
        return false;
    }
    File* file = new File("savegame/relay.sav", 0x81);
    int magic0;
    int magic1;
    file->Read(&magic0, 4);
    if (magic0 != 0x63777472 || (file->Read(&magic1, 4), magic1 != 0x20020109)) {
        file->Close();
        delete file;
        return false;
    }
    int size = file->GetSize() - 8;
    BYTE* buffer = new BYTE[size];
    file->Read(buffer, size);
    file->Close();
    delete file;
    {
        FB3_Cipher cipher;
        cipher.Decrypt((DWORD*)buffer, DAT_004fa2e4, size >> 2);
    }
    memcpy(FB3_BODY(this), buffer, 0x5a5);
    BYTE* in = buffer + 0x5a5;
    if (FB3_BODY(this)->done) {
        delete[] buffer;
        return false;
    }
    int level = FB3_BODY(this)->level;
    int rally = FB3_BODY(this)->rally;
    DAT_00501464 = 1;
    DAT_0050132c = level;
    int stage = FB3_BODY(this)->stage;
    if (((StageTable*)((char*)DAT_0056b0c8 + 0x3aec))->FUN_00443620(rally) < stage) {
        delete[] buffer;
        return false;
    }
    RaceMode* race = (RaceMode*)DAT_00501450;
    race->unk28 = rally;
    race->unk2c = stage;
    Player* player = new Player(1, 0);
    player->FUN_00490fb0(FB3_BODY(this)->name);
    int kind = FB3_BODY(this)->kind;
    strcpy(DAT_00501338[0], FB3_BODY(this)->name);
    player->unk20 = kind;
    DAT_005013ec[0] = kind;
    player->unk24 = FB3_BODY(this)->unk8c;
    player->unk2c = FB3_BODY(this)->flags & 0xff;
    player->FUN_00491130();
    race->AddPlayer(player);
    if (!(FB3_BODY(this)->flags & 1)) {
        DAT_00501460 = 1;
    }
    int zero = 0;
    if (FB3_BODY(this)->stage == 1) {
        for (int i = 0; i < 0x13; i++) {
            race->AddPlayer(new Player(PLAYER_AI, i + 1));
        }
    } else {
        CarSnapView* car = (CarSnapView*)player->car;
        CarSnapshot* snap = &car->snapshot[2];
        snap->Clear();
        *snap = *(CarSnapshot*)in;
        in += 0x8c;
        if (snap->size[0]) {
            snap->buffer[0] = new BYTE[snap->size[0] * 4];
            memcpy(snap->buffer[0], in, snap->size[0] * 4);
            in += snap->size[0] * 4;
        }
        if (snap->size[1]) {
            snap->buffer[1] = new BYTE[snap->size[1] * 4];
            memcpy(snap->buffer[1], in, snap->size[1] * 4);
            in += snap->size[1] * 4;
        }
        if (snap->size[2]) {
            snap->buffer[2] = new BYTE[snap->size[2] * 4];
            memcpy(snap->buffer[2], in, snap->size[2] * 4);
            in += snap->size[2] * 4;
        }
        ((Car*)car)->FUN_0046be50(2);
        int tmp[10];
        memcpy(tmp, in, 0x28);
        in += 0x28;
        for (int k = 0; k < 10; k++) {
            player->unk1a28[k] = tmp[k];
        }
        int used = FB3_BODY(this)->stage - 1;
        player->unk1a50 = used;
        for (int i = 0; i < 0x13; i++) {
            Player* ai = new Player(PLAYER_AI, i + 1);
            FB3_Stats stats;
            memcpy(&stats, in, 0x28);
            memcpy(ai->unk1a28, &stats, 0x28);
            ai->unk1a50 = used;
            in += 0x28;
            race->AddPlayer(ai);
        }
    }
    delete[] buffer;
    DAT_00501458 = zero;
    DAT_00501454 = zero;
    DAT_0050132c = FB3_BODY(this)->level;
    DAT_00501460 = zero;
    DAT_0050145c = 1;
    DAT_00501468 = 1;
    DAT_00501464 = 1;
    DAT_00501334 = 1;
    DAT_00501472 = 1;
    return true;
}

// NONMATCHING: RALLYTROPHY 0x44e3e0
// The original copies each per-player 10-dword block through a stack temporary (frame 0x3c); VC6 elides the temporary
// here (no source form found), so the frame is 0x28 smaller; the rest of the body is the same shape.
void SlotStore::SaveRelay(char level, char advance)
{
    int magic0 = 0x63777472;
    int magic1 = 0x20020109;
    RaceMode* race = (RaceMode*)DAT_00501450;
    CarSnapView* car = (CarSnapView*)race->m_localPlayer->car;
    int slot = 1;
    if (advance) {
        slot = 2;
    }
    CarSnapshot* snap = &car->snapshot[slot];
    FB3_BODY(this)->stage = level;
    if (!advance) {
        FB3_BODY(this)->done = advance;
    } else {
        FB3_BODY(this)->done++;
    }
    File* file = new File("savegame/relay.sav", 0x82);
    int n0 = snap->size[0];
    int size = (n0 + snap->size[1] + snap->size[2]) * 4 + 0x631;
    size = (size + race->m_players.size() * 0x28 + 3) & ~3;
    file->Write(&magic0, 4);
    file->Write(&magic1, 4);
    BYTE* buffer = new BYTE[size];
    memcpy(buffer, FB3_BODY(this), 0x5a5);
    BYTE* out = buffer + 0x5a5;
    memcpy(out, snap, 0x8c);
    out += 0x8c;
    if (n0) {
        n0 <<= 2;
        memcpy(out, snap->buffer[0], n0);
        out += n0;
    }
    if (snap->size[1]) {
        int bytes = snap->size[1] * 4;
        memcpy(out, snap->buffer[1], bytes);
        out += bytes;
    }
    if (snap->size[2]) {
        int bytes = snap->size[2] * 4;
        memcpy(out, snap->buffer[2], bytes);
        out += bytes;
    }
    for (int i = 0; i < (int)race->m_players.size(); i++) {
        volatile FB3_Stats stats = *(FB3_Stats*)race->m_players[i]->unk1a28;
        memcpy(out, (const void*)&stats, 0x28);
        out += 0x28;
    }
    FB3_Cipher cipher;
    cipher.Encrypt((DWORD*)buffer, DAT_004fa2e4, size >> 2);
    file->Write(buffer, size);
    delete[] buffer;
    file->Close();
    delete file;
}

inline float FB3_PassFloat(float v) { return v; }

// FUNCTION: RALLYTROPHY 0x44e650
void SlotStore::SaveStats()
{
    RaceMode* race = (RaceMode*)DAT_00501450;
    int stage = race->unk28;
    Player* player = race->m_localPlayer;
    int step = race->unk2c;
    int index = stage * 6 - 6;
    if (stage > 1) {
        index += 2;
    }
    step += index;
    *(float*)((char*)this + step * 4 + 0x58d) = FB3_PassFloat(player->unk1a20);
    memcpy((char*)this + step * 0x28 + 0x69, &player->unk1884[1], 0x28);
}

// original: FUN_0044e6a0 (never reads this)
// FUNCTION: RALLYTROPHY 0x44e6a0
void SlotStore::GetSlotInfo(int index, SlotRecord* record)
{
    FB1_Path path;
    if (DAT_00501458) {
        path.AssignB("savegame/arcade");
    } else {
        path.AssignB("savegame/rally");
    }
    char buf[2];
    itoa(index, buf, 10);
    path.AppendD(buf);
    path.AppendD(".SAV");
    if (!File::Exists(path.c_str())) {
        record->kind = 0;
        return;
    }
    File* file = new File(path.c_str(), 0x81);
    int magic1;
    file->Read(&magic1, 4);
    int magic2;
    file->Read(&magic2, 4);
    file->Read(record, 0x6c);
    file->Close();
    delete file;
}

// The game object's career position (Game+0xd40, +0xd44).
struct FB1_GameView {
    BYTE unk0[0xd40];
    int unkd40;
    int unkd44;
};
#define FB1_GAME ((FB1_GameView*)DAT_0056b0c8)

// original: FUN_0044e960
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x44e960
void SaveGame::FillHeader()
{
    __asm {
        push ebx
        mov ebx, dword ptr [DAT_00501450]
        mov edx, ecx
        mov ecx, dword ptr [ebx + 0x18]
        test ecx, ecx
        push esi
        mov esi, dword ptr [DAT_0056b0c8]
        jne save_header_local_players_nonempty
        xor eax, eax
        jmp save_header_local_players_size
    save_header_local_players_nonempty:
        mov eax, dword ptr [ebx + 0x1c]
        sub eax, ecx
        sar eax, 2
    save_header_local_players_size:
        mov ecx, dword ptr [esi + 0xd44]
        mov dword ptr [edx + 0x20], eax
        mov eax, dword ptr [esi + 0xd40]
        inc eax
        inc ecx
        mov dword ptr [edx + 0x28], ecx
        mov dword ptr [edx + 0x24], eax
        mov eax, dword ptr [DAT_0050132c]
        push ebp
        push edi
        mov edi, dword ptr [ebx + 0x24]
        mov ecx, dword ptr [edi + 0x1a5c]
        mov dword ptr [edx + 0x40], ecx
        or ecx, -1
        mov dword ptr [edx + 0x2c], eax
        mov eax, dword ptr [edi + 0x1a60]
        mov dword ptr [edx + 0x44], eax
        xor eax, eax
        repnz scasb
        not ecx
        sub edi, ecx
        mov eax, ecx
        shr ecx, 2
        mov esi, edi
        lea ebp, [edx + 0x48]
        mov edi, ebp
        rep movsd
        mov ecx, eax
        and ecx, 3
        rep movsb
        mov ecx, dword ptr [ebx + 8]
        test ecx, ecx
        pop edi
        pop ebp
        jne save_header_players_nonempty
        xor eax, eax
        jmp save_header_remaining
    save_header_players_nonempty:
        mov eax, dword ptr [ebx + 0xc]
        sub eax, ecx
        sar eax, 2
    save_header_remaining:
        mov ecx, dword ptr [ebx + 0x18]
        test ecx, ecx
        jne save_header_local_remaining_nonempty
        xor ebx, ebx
        sub eax, ebx
        mov dword ptr [edx + 0x68], eax
        add edx, 0x30
        push edx
        call dword ptr [DAT_0059b2f8]
        pop esi
        pop ebx
        ret
    save_header_local_remaining_nonempty:
        mov ebx, dword ptr [ebx + 0x1c]
        sub ebx, ecx
        sar ebx, 2
        sub eax, ebx
        mov dword ptr [edx + 0x68], eax
        add edx, 0x30
        push edx
        call dword ptr [DAT_0059b2f8]
        pop esi
        pop ebx
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x44ea30
// SYMBOL: _$E23
typedef void Generated_44ea30();

// FUNCTION: RALLYTROPHY 0x44ea50
// SYMBOL: _$E22
typedef void Generated_44ea50();
