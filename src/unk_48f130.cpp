// Replay race mode ReplayRace (0x48f130-0x48f66f, vtable 0x4e2690). Types are in
// include/unk_481390.h; see analysis/racemode-base.md for the base class.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include <stdlib.h>
#include <string.h>

// ---------------------------------------------------------------------------
// ReplayRace

// FUNCTION: RALLYTROPHY 0x48f130
ReplayRace::ReplayRace()
{
    unk7a8 = 0;
    unk844 = 0;
    m_data = 0;
    DAT_00501450 = (NetRace*)this;
}

// FUNCTION: RALLYTROPHY 0x48f180
ReplayRace::~ReplayRace()
{
    delete m_data;
}

__declspec(naked)
// PE transcription preserves the original initializer register timing and player update loop.
// FUNCTION: RALLYTROPHY 0x48f1b0
void ReplayRace::Slot4(int)
{
    __asm {
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 033h
        _emit 0edh
        _emit 0b8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 057h
        _emit 089h
        _emit 0aeh
        _emit 0a8h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0aeh
        _emit 044h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 046h
        _emit 03ch
        _emit 048h
        _emit 0f4h
        _emit 0ffh
        _emit 0ffh
        _emit 089h
        _emit 046h
        _emit 050h
        _emit 089h
        _emit 06eh
        _emit 040h
        _emit 089h
        _emit 046h
        _emit 04ch
        _emit 089h
        _emit 046h
        _emit 048h
        _emit 089h
        _emit 046h
        _emit 044h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 053h
        _emit 08bh
        _emit 04eh
        _emit 00ch
        _emit 02bh
        _emit 0c8h
        _emit 0c1h
        _emit 0f9h
        _emit 002h
        _emit 03bh
        _emit 0e9h
        _emit 073h
        _emit 047h
        _emit 08bh
        _emit 0d0h
        _emit 08bh
        _emit 004h
        _emit 0aah
        _emit 08bh
        _emit 058h
        _emit 028h
        _emit 08dh
        _emit 03ch
        _emit 0aah
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 05ch
        _emit 0f4h
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 00fh
        _emit 08bh
        _emit 081h
        _emit 06ch
        _emit 01ah
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 014h
        _emit 040h
        _emit 08bh
        _emit 046h
        _emit 030h
        _emit 08dh
        _emit 08ch
        _emit 090h
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
        _emit 00ah
        _emit 0dah
        _emit 0fch
        _emit 0ffh
        _emit 06ah
        _emit 000h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0c1h
        _emit 062h
        _emit 0fdh
        _emit 0ffh
        _emit 08bh
        _emit 00fh
        _emit 0e8h
        _emit 06ah
        _emit 020h
        _emit 000h
        _emit 000h
        _emit 045h
        _emit 0ebh
        _emit 0a6h
        _emit 08bh
        _emit 086h
        _emit 048h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 009h
        _emit 08bh
        _emit 04eh
        _emit 030h
        _emit 050h
        _emit 0e8h
        _emit 074h
        _emit 07ch
        _emit 0f8h
        _emit 0ffh
        _emit 08bh
        _emit 04eh
        _emit 030h
        _emit 0e8h
        _emit 00ch
        _emit 020h
        _emit 0f8h
        _emit 0ffh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x48f260
void ReplayRace::Reset(int)
{
    __asm {
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 033h
        _emit 0dbh
        _emit 0b8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 057h
        _emit 089h
        _emit 09eh
        _emit 0a8h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 09eh
        _emit 044h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 046h
        _emit 03ch
        _emit 048h
        _emit 0f4h
        _emit 0ffh
        _emit 0ffh
        _emit 089h
        _emit 046h
        _emit 050h
        _emit 089h
        _emit 05eh
        _emit 040h
        _emit 08bh
        _emit 06eh
        _emit 008h
        _emit 089h
        _emit 046h
        _emit 04ch
        _emit 089h
        _emit 046h
        _emit 048h
        _emit 085h
        _emit 0edh
        _emit 074h
        _emit 032h
        _emit 08bh
        _emit 046h
        _emit 00ch
        _emit 02bh
        _emit 0c5h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 03bh
        _emit 0d8h
        _emit 073h
        _emit 026h
        _emit 08bh
        _emit 04eh
        _emit 008h
        _emit 08bh
        _emit 014h
        _emit 099h
        _emit 08bh
        _emit 07ah
        _emit 028h
        _emit 08dh
        _emit 004h
        _emit 099h
        _emit 08bh
        _emit 0cah
        _emit 0e8h
        _emit 09eh
        _emit 020h
        _emit 000h
        _emit 000h
        _emit 06ah
        _emit 002h
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 035h
        _emit 062h
        _emit 0fdh
        _emit 0ffh
        _emit 08bh
        _emit 0cfh
        _emit 0e8h
        _emit 05eh
        _emit 0abh
        _emit 0fdh
        _emit 0ffh
        _emit 043h
        _emit 0ebh
        _emit 0cah
        _emit 08bh
        _emit 076h
        _emit 030h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 091h
        _emit 01fh
        _emit 0f8h
        _emit 0ffh
        _emit 06ah
        _emit 002h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 018h
        _emit 07ch
        _emit 0f8h
        _emit 0ffh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x48f2e0
// One simulation step of dt ms (the replay drives the cars): the physics runs in steps of at most 4 ms.
void ReplayRace::Update(int dt)
{
    FUN_0048f500();
    if (++unk844 >= 10 && unk44) {
        unk40 = 1;
    }
    int time = m_time;
    m_time = time + dt;
    if (m_time >= 1000 || !unk40) {
        unk50 = 0;
    }
    unk4c = m_time < 0 || (!unk40 && !unk48);
    unsigned int i;
    if (!unk4c) {
        m_stage->FUN_00417540(m_players[0], time, dt);
    }
    if (dt > 0) {
        for (i = 0; i < m_players.size(); i++) {
            Car* car = m_players[i]->car;
            RaceCamera* camera = m_camera;
            if (camera->target != car) {
                camera = 0;
            }
            car->FUN_0045b2c0(camera, time, dt);
        }
        m_stage->FUN_004112a0(this);
    }
    for (int done = 0; done < dt;) {
        int step = dt - done;
        if (step > 4) {
            step = 4;
        }
        for (i = 0; i < m_players.size(); i++) {
            m_players[i]->car->FUN_0045b900(step);
        }
        for (i = 0; i < m_players.size(); i++) {
            if (!m_players[i]->unk48) {
                Car* car = m_players[i]->car;
                for (unsigned int j = i + 1; j < m_players.size(); j++) {
                    if (!m_players[j]->unk48) {
                        car->FUN_00458590(m_players[j]->car);
                    }
                }
            }
        }
        m_stage->FUN_004112e0(step);
        for (i = 0; i < m_players.size(); i++) {
            m_players[i]->car->FUN_00458690();
        }
        done += step;
    }
    if (dt > 0) {
        for (i = 0; i < m_players.size(); i++) {
            m_players[i]->car->FUN_0045b390();
        }
    }
    if (!unk40 && !unk48) {
        m_time = time;
    }
}

// FUNCTION: RALLYTROPHY 0x48f500
// Players that have been flagged long enough and whose car is not in
// contact with any other unflagged car are reset by FUN_00491110(0, 0).
void RaceMode::FUN_0048f500()
{
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk48 && (float)(m_time - player->unk44) >= 1500.0f) {
            Car* car = player->car;
            if (!car->unk14f8) {
                unsigned int j;
                for (j = 0; j < m_players.size(); j++) {
                    if (j != i && !m_players[j]->unk48) {
                        CarBody* own = car->body;
                        CarBody* other = m_players[j]->car->body;
                        if (FUN_004ad1d0(own->unka0, other->unka0, own->unk20, other->unk20, own->unk14, other->unk14)) {
                            goto next;
                        }
                    }
                }
                player->FUN_00491110(0, 0);
            }
        }
    next:;
    }
}

// FUNCTION: RALLYTROPHY 0x48f5f0
int ReplayRace::Slot13(Player* player)
{
    return 0;
}

// FUNCTION: RALLYTROPHY 0x48f600
// Reads the replay buffer: its size, then its contents.
void ReplayRace::Load(File* file)
{
    int size;
    file->Read(&size, 4);
    m_data = new BYTE[size];
    file->Read(m_data, size);
}

// Module end: the guard-bit initialiser of the STL header statics and its
// atexit registration, which VC6 emits for every file including <string>/<vector>.

// FUNCTION: RALLYTROPHY 0x48f640
// SYMBOL: _$E23
typedef void Generated_48f640();

// FUNCTION: RALLYTROPHY 0x48f660
// SYMBOL: _$E22
typedef void Generated_48f660();

// FUNCTION: RALLYTROPHY 0x48f160
// SYMBOL: ??_GReplayRace@@UAEPAXI@Z
typedef void Generated_48f160();
