// Ghost race mode GhostRace (0x48f670-0x48fe1f, vtable 0x4e2708). Types are in
// include/unk_481390.h; see analysis/racemode-base.md for the base class.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include <stdlib.h>
#include <string.h>

// File-scope screen position (original: 0x48f670-0x48f6bf, the same four-function
// pair per object as the base's statics).
class ScreenPoint {
public:
    ScreenPoint(float px, float py) { x = px; y = py; }
    ~ScreenPoint() {}
    float x, y;
};
ScreenPoint DAT_005654f0(768.0f, 96.0f);

// ---------------------------------------------------------------------------
// GhostRace

// FUNCTION: RALLYTROPHY 0x48f6c0
GhostRace::GhostRace()
{
    DAT_00501450 = (NetRace*)this;
    unk830 = 0;
}

// FUNCTION: RALLYTROPHY 0x48f710
GhostRace::~GhostRace()
{
    DeleteGhost();
}

// FUNCTION: RALLYTROPHY 0x48f760
// One simulation step of dt ms: the local player's car, then the ghost's.
void GhostRace::Update(int dt)
{
    if (++unk844 >= 10 && unk44) {
        unk40 = 1;
    }
    int time = m_time;
    m_time = time + dt;
    if (m_time >= 1000 || !unk40) {
        unk50 = 0;
    }
    unk4c = m_time < 0 || !unk40;
    Player* local = m_localPlayer;
    Car* car = local->car;
    int frozen = unk4c;
    if (local->unk30) {
        frozen = 1;
    }
    if (!unk4c) {
        m_stage->FUN_00417540(local, time, dt);
    }
    if (car) {
        car->unk1334 = frozen;
        car->FUN_0045b2c0(m_camera, time, dt);
        m_stage->FUN_004112a0(this);
        for (int done = 0; done < dt;) {
            int step = dt - done;
            if (step > 4) {
                step = 4;
            }
            car->FUN_0045b900(step);
            m_stage->FUN_004112e0(step);
            car->FUN_00458690();
            done += step;
        }
        car->FUN_0045b390();
    }
    if (unk40 && m_ghost) {
        Car* ghost = m_ghost->car;
        ghost->unk1334 = 0;
        ghost->FUN_0045b2c0(0, time, dt);
        for (int done = 0; done < dt;) {
            int step = dt - done;
            if (step > 4) {
                step = 4;
            }
            ghost->FUN_0045b900(step);
            done += step;
        }
        ghost->FUN_0045b390();
    }
    if (!unk40) {
        m_time = time;
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x48f8d0
void GhostRace::Slot4(int)
{
    __asm {
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 057h
        _emit 033h
        _emit 0ffh
        _emit 089h
        _emit 0beh
        _emit 0a8h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0beh
        _emit 030h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0beh
        _emit 028h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0f1h
        _emit 0e4h
        _emit 0ffh
        _emit 0ffh
        _emit 039h
        _emit 07ch
        _emit 024h
        _emit 014h
        _emit 075h
        _emit 018h
        _emit 083h
        _emit 03dh
        _emit 064h
        _emit 014h
        _emit 050h
        _emit 000h
        _emit 003h
        _emit 075h
        _emit 00fh
        _emit 039h
        _emit 03dh
        _emit 004h
        _emit 013h
        _emit 050h
        _emit 000h
        _emit 074h
        _emit 007h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 003h
        _emit 0e5h
        _emit 0ffh
        _emit 0ffh
        _emit 089h
        _emit 07eh
        _emit 050h
        _emit 08bh
        _emit 07eh
        _emit 024h
        _emit 08bh
        _emit 05fh
        _emit 028h
        _emit 08bh
        _emit 0cbh
        _emit 0c7h
        _emit 046h
        _emit 03ch
        _emit 048h
        _emit 0f4h
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h
        _emit 03ch
        _emit 0edh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 087h
        _emit 06ch
        _emit 01ah
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 06eh
        _emit 030h
        _emit 08dh
        _emit 004h
        _emit 040h
        _emit 08dh
        _emit 08ch
        _emit 085h
        _emit 010h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 068h
        _emit 080h
        _emit 0b9h
        _emit 057h
        _emit 000h
        _emit 051h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0ech
        _emit 0d2h
        _emit 0fch
        _emit 0ffh
        _emit 06ah
        _emit 000h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0a3h
        _emit 05bh
        _emit 0fdh
        _emit 0ffh
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 04ch
        _emit 019h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 033h
        _emit 0dbh
        _emit 03bh
        _emit 0c3h
        _emit 075h
        _emit 00ah
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 00bh
        _emit 01ah
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 05eh
        _emit 044h
        _emit 08bh
        _emit 0beh
        _emit 094h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 03bh
        _emit 0fbh
        _emit 074h
        _emit 039h
        _emit 08bh
        _emit 05fh
        _emit 028h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0e4h
        _emit 0ech
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 087h
        _emit 06ch
        _emit 01ah
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 014h
        _emit 040h
        _emit 068h
        _emit 080h
        _emit 0b9h
        _emit 057h
        _emit 000h
        _emit 08dh
        _emit 084h
        _emit 095h
        _emit 010h
        _emit 013h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 097h
        _emit 0d2h
        _emit 0fch
        _emit 0ffh
        _emit 06ah
        _emit 002h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 04eh
        _emit 05bh
        _emit 0fdh
        _emit 0ffh
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 0f7h
        _emit 018h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0dbh
        _emit 08bh
        _emit 0cdh
        _emit 0e8h
        _emit 0aeh
        _emit 018h
        _emit 0f8h
        _emit 0ffh
        _emit 089h
        _emit 09eh
        _emit 044h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 05eh
        _emit 040h
        _emit 05fh
        _emit 0c7h
        _emit 046h
        _emit 04ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 086h
        _emit 0ach
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}
__declspec(naked)
// PE transcription preserves the original late edi pop and reset store order.
// FUNCTION: RALLYTROPHY 0x48f9e0
void GhostRace::Reset(int)
{
    __asm {
        _emit 053h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 010h
        _emit 033h
        _emit 0c0h
        _emit 085h
        _emit 0ffh
        _emit 00fh
        _emit 095h
        _emit 0c0h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 04eh
        _emit 024h
        _emit 08bh
        _emit 059h
        _emit 028h
        _emit 0c7h
        _emit 086h
        _emit 0a8h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 046h
        _emit 03ch
        _emit 048h
        _emit 0f4h
        _emit 0ffh
        _emit 0ffh
        _emit 0c7h
        _emit 046h
        _emit 050h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 040h
        _emit 08bh
        _emit 0f8h
        _emit 0e8h
        _emit 03ah
        _emit 019h
        _emit 000h
        _emit 000h
        _emit 057h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0d2h
        _emit 05ah
        _emit 0fdh
        _emit 0ffh
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0fbh
        _emit 0a3h
        _emit 0fdh
        _emit 0ffh
        _emit 08bh
        _emit 05eh
        _emit 030h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 031h
        _emit 018h
        _emit 0f8h
        _emit 0ffh
        _emit 057h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0b9h
        _emit 074h
        _emit 0f8h
        _emit 0ffh
        _emit 0c7h
        _emit 086h
        _emit 044h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 046h
        _emit 040h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 05fh
        _emit 0c7h
        _emit 046h
        _emit 04ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 086h
        _emit 0ach
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 05eh
        _emit 05bh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x48fa60
void GhostRace::Slot8()
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        if (m_players[i]->unk40 == 3) {
            m_players[i]->FUN_00491e20(m_stage);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48faa0
// Draws the HUD, the standings and the split difference to the reference.
void GhostRace::Draw()
{
    FUN_00487200();
    FUN_004877a0();
    FUN_00487920();
    Player* local = m_localPlayer;
    int split = local->unk1a18;
    if (split != unk828) {
        FUN_0048fb10();
    }
    if (split > 0) {
        int time = local->unk1884[split];
        int since = m_time - time;
        if (since >= 0 && since < 3000) {
            SplitDelta(time - unk848);
        }
    }
    if (unk830) {
        FUN_00487560();
    }
}

// NONMATCHING: RALLYTROPHY 0x48fb10
// Same size (471). Only the rank search loop differs: the original emits `xor edx,edx` for its counter after the
// `count > 0` test and `inc edx` after `add edi,4`; this build hoists the xor and swaps the two increments.
// Rebuilds the standings when the local player passes a split: the leader
// and the players around the local one, with their time differences.
void GhostRace::FUN_0048fb10()
{
    Player* local = m_localPlayer;
    int split = local->unk1a18;
    unk828 = split;
    if (DAT_00501464 == 3) {
        RecordTable* table = (RecordTable*)((BYTE*)DAT_0056b0c8 + 0x5c30);
        RecordEntry* record = table->FUN_0048ffd0(unk28 - 1, unk2c - 1, 0, 0);
        unk848 = record->splits[split];
        return;
    }
    unk830 = 1;
    DAT_00565490 = 0;
    unsigned int i;
    for (i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk1a18 >= split) {
            DAT_00565494[DAT_00565490++] = player;
        }
    }
    DAT_005654e8 = split - 1;
    qsort(DAT_00565494, DAT_00565490, 4, CompareSplits);
    int count = DAT_00565490;
    int rank = 1;
    int n;
    for (n = 0; n < count; n++) {
        if (DAT_00565494[n] == m_localPlayer) {
            break;
        }
        rank++;
    }
    int ref = 0;
    if (rank == 1) {
        ref = rank;
    }
    unk848 = DAT_00565494[ref]->unk1884[split];
    unk7ac = rank;
    int shown = count < 6 ? count : 6;
    unk82c = shown;
    int k;
    if (rank <= 4) {
        for (k = 0; k < unk82c; k++) {
            m_entries[k].rank = k + 1;
        }
    } else if (rank <= count - 2) {
        for (k = 1; k < 6; k++) {
            m_entries[k].rank = rank + k - 3;
        }
    } else {
        for (k = 1; k < 6; k++) {
            m_entries[k].rank = count + k - 5;
        }
    }
    Player* leader = DAT_00565494[0];
    m_entries[0].player = leader;
    m_entries[0].rank = 1;
    FormatTime(leader->unk1884[split], m_entries[0].time);
    int best = leader->unk1884[split];
    for (k = 1; k < 6; k++) {
        Player* player = DAT_00565494[m_entries[k].rank - 1];
        m_entries[k].player = player;
        FormatTimeDelta(player->unk1884[split] - best, m_entries[k].time);
    }
}

// FUNCTION: RALLYTROPHY 0x48fcf0
// qsort comparator: by the split time selected by DAT_005654e8.
int CompareSplits(const void* a, const void* b)
{
    Player* left = *(Player**)a;
    Player* right = *(Player**)b;
    int result = 0;
    if (right->unk1884[DAT_005654e8 + 1] < left->unk1884[DAT_005654e8 + 1]) {
        result = 1;
    } else if (left->unk1884[DAT_005654e8 + 1] < right->unk1884[DAT_005654e8 + 1]) {
        result = -1;
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x48fd30
void GhostRace::Slot10(int a, int b)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        Car* car = player->car;
        if (car && player->unk40 != 3 && (b || player->active)) {
            car->FUN_0046bc90(a);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48fd90
void GhostRace::Slot11(int a, int b)
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        Car* car = player->car;
        if (car && player->unk40 != 3 && (b || player->active)) {
            car->FUN_0046be50(a);
        }
    }
}

// File-scope static DAT_005654f0 (original: 0x48f670-0x48f6b0): wrapper (_$E), initialiser,
// atexit registration and the empty destructor thunk.

// FUNCTION: RALLYTROPHY 0x48f670
// SYMBOL: _$E21
typedef void Generated_48f670();

// FUNCTION: RALLYTROPHY 0x48f680
// SYMBOL: _$E18
typedef void Generated_48f680();

// FUNCTION: RALLYTROPHY 0x48f6a0
// SYMBOL: _$E20
typedef void Generated_48f6a0();

// FUNCTION: RALLYTROPHY 0x48f6b0
// SYMBOL: _$E19
typedef void Generated_48f6b0();

// Module end: the guard-bit initialiser of the STL header statics and its
// atexit registration, which VC6 emits for every file including <string>/<vector>.

// FUNCTION: RALLYTROPHY 0x48fdf0
// SYMBOL: _$E28
typedef void Generated_48fdf0();

// FUNCTION: RALLYTROPHY 0x48fe10
// SYMBOL: _$E27
typedef void Generated_48fe10();

// FUNCTION: RALLYTROPHY 0x48f6f0
// SYMBOL: ??_GGhostRace@@UAEPAXI@Z
typedef void Generated_48f6f0();
