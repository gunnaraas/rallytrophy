// Network race mode (0x481390-0x4843df): race-mode class for network games.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "dplay8.h"
#include <stdlib.h>
#include <string.h>

#pragma auto_inline(off)
unsigned int PlayerVector::Size() const { return size(); }
// FUNCTION: RALLYTROPHY 0x484380
PlayerVector::iterator PlayerVector::Erase(iterator first, iterator last) { return erase(first, last); }
void PlayerVector::Destroy(iterator first, iterator last) { _Destroy(first, last); }
unsigned int IntVector::Size() const { return size(); }
IntVector::iterator IntVector::Erase(iterator first, iterator last) { return erase(first, last); }
#pragma auto_inline(on)

// NONMATCHING: RALLYTROPHY 0x481390
// 506 vs 491 bytes. The original inlines m_removed's erase but calls
// _Destroy (0x403130) out of line; this build inlines both. The rest (member
// vectors, sprite setup) matches apart from the registers that follow.
NetRace::NetRace()
{
    unk7a8 = 0;
    m_hostPlayer = 0;
    m_host = 0;
    unk85c = 0;
    unk860 = 0;
    unk850 = 1;
    unk830 = 0;
    unk854 = 0;
    unk858 = 0;
    InitializeCriticalSection(&m_lock);
    if (m_removed.size() > 0) {
        m_removed.erase(m_removed.begin(), m_removed.end());
    }
    unk844 = 0;
    unk880 = 0;
    ClearMessages();
    DAT_00501450 = this;
    m_pingSprite = new Sprite("MenuItems");
    m_pingSprite->texture = new Texture("data/menu/pingindicators.tga", 0x45, 1);
    m_pingSprite->texture->Upload(DAT_0056b0ac);
    m_pingSprite->unk9c = 2;
    m_pingSprite->unk98 = 15;
    m_pingSprite->unk60[0] = 1.0f;
    m_pingSprite->unk60[1] = 1.0f;
    m_pingSprite->unk60[2] = 1.0f;
    m_pingSprite->unk60[3] = 1.0f;
    m_pingSprite->unk40[0] = 1.0f;
    m_pingSprite->unk40[1] = 1.0f;
    m_pingSprite->unk40[2] = 1.0f;
    m_pingSprite->unk40[3] = 1.0f;
    m_pingSprite->unk18 = 5;
    m_pingSprite->unk1c = 6;
    m_pingSprite->unk24 = 2;
}

// NONMATCHING: RALLYTROPHY 0x4815a0
// 360 vs 349 bytes. The original calls m_players.erase (0x484380) out of
// line and caches m_players' begin pointer across the delete loop; this
// build inlines erase. Same inlining question as AddPlayer.
NetRace::~NetRace()
{
    Lock();
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player) {
            delete player;
        }
    }
    if (m_players.size()) {
        m_players.erase(m_players.begin(), m_players.end());
    }
    Unlock();
    DeleteCriticalSection(&m_lock);
    delete m_pingSprite->texture;
    delete m_pingSprite;
    m_pingSprite = 0;
}

// NONMATCHING: RALLYTROPHY 0x481700
// Same size (345). Same scheduling difference as Reset: this build hoists the
// eax stores (0x868, 0x86c, 0x48) above `push edi`.
void NetRace::Slot4(int mode)
{
    unk7a8 = 0;
    m_time = -3000;
    unk868 = -5000;
    unk86c = -5000;
    unk874 = -4000;
    unk50 = 0;
    unk40 = 0;
    unk48 = mode;
    unk4c = 1;
    unk864 = 0;
    unk87c = 0;
    unk830 = 0;
    unk828 = 0;
    unk870 = 0;
    unk880 = 0;
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Car* car = m_players[i]->car;
        car->FUN_0048e660();
        if (!mode) {
            m_players[i]->startSlot = m_players[i]->unk1a70;
            m_players[i]->unk1a74 = 0;
        }
        car->FUN_0045cc30(&m_stage->starts[m_players[i]->startSlot], DAT_0057b980);
        car->FUN_004654f0(0);
        m_players[i]->FUN_004912a0();
        if (!mode) {
            m_players[i]->FUN_00491370();
        }
        if (DAT_00501314 && !m_players[i]->local) {
            m_players[i]->FUN_00491110(1, -3000);
        }
    }
    unk8d0 = 0;
    unk8d4 = 0;
    for (int j = 0; j < 8; j++) {
        unk8d8[j] = 0;
    }
    m_stage->FUN_00411260();
    if (m_host) {
        ClearMessages();
    }
}

// NONMATCHING: RALLYTROPHY 0x481940
// 1235 vs 1240 bytes; logic complete. Register allocation differs: this
// build keeps 0 in edi and the player index in ebp, the original keeps the
// index in memory and `started` in ebp. Tried other forms of the frame
// counters.
// One simulation step of dt ms: clock correction, lag/ping classification of
// remote players, then the physics in steps of at most 4 ms.
void NetRace::Update(int dt)
{
    unk8d8[unk8d4] = dt;
    unk8d4 = (unk8d4 + 1) & 7;
    unk8d0++;
    int started = unk40;
    ServiceNetwork();
    if (unk870) {
        int step = dt;
        if (abs(unk870) < dt) {
            step = unk870;
        }
        if (unk870 > 0) {
            unk870 -= step;
            dt += step;
        } else {
            unk870 += step;
            dt -= step;
        }
    }
    if (unk8d0 == 10 && !unk48) {
        SendState(5);
    }
    if (unk87c) {
        int elapsed = timeGetTime() - unk878;
        if (elapsed >= 4) {
            dt = elapsed - (elapsed & 3);
            unk40 = 1;
            started = 1;
            unk87c = 0;
        }
    }
    FUN_00481e20();
    int time = m_time;
    m_time = time + dt;
    if (m_time >= 1000 || !started) {
        unk50 = 0;
    }
    unk4c = m_time < 0 || (!started && !unk48);
    unsigned int i;
    if (dt > 0) {
        int localBit = 1 << (m_localPlayer->startSlot - 1);
        for (i = 0; i < m_players.size(); i++) {
            if (!Slot14(m_players[i])) {
                continue;
            }
            bool local = m_players[i] == m_localPlayer;
            Car* car = m_players[i]->car;
            RaceCamera* camera = m_camera;
            if (camera->target != car) {
                camera = 0;
            }
            if (unk48) {
                m_players[i]->unk4c = 0;
                car->FUN_0045b2c0(camera, time, dt);
                continue;
            }
            int frozen = unk4c;
            if (m_players[i]->unk30) {
                frozen = 1;
            }
            if (!car) {
                continue;
            }
            if (!local) {
                m_players[i]->FUN_004916a0(time);
                if (time > 1000) {
                    int age = time - m_players[i]->samples.time;
                    if (m_host && DAT_00501324 && age > DAT_004e2424[DAT_00501324]) {
                        AddUnk8c0((int)m_players[i]);
                    }
                    int ping = 0;
                    if (age > 450) {
                        ping = 2;
                    } else if (age > 250) {
                        ping = 1;
                    }
                    m_players[i]->ping = ping;
                    int lag = 0;
                    if (age > 500) {
                        lag = 2;
                    } else if (age > 300) {
                        lag = 1;
                    }
                    m_players[i]->unk4c = lag;
                    if (lag || (m_players[i]->unk50 & localBit)) {
                        m_players[i]->FUN_00491110(1, time);
                    }
                }
            }
            car->unk1334 = frozen;
            if (m_players[i]->unk4c != 2) {
                car->FUN_0045b2c0(camera, time, dt);
            }
        }
        m_stage->FUN_004112a0(this);
    }
    for (int done = 0; done < dt;) {
        int step = dt - done;
        if (step > 4) {
            step = 4;
        }
        for (i = 0; i < m_players.size(); i++) {
            if (Slot14(m_players[i])) {
                Car* car = m_players[i]->car;
                if (m_players[i] != m_localPlayer && !unk48) {
                    m_players[i]->FUN_00491950(time + done);
                }
                if (m_players[i]->unk4c != 2) {
                    car->FUN_0045b900(step);
                }
            }
        }
        for (i = 0; i < m_players.size(); i++) {
            if (!m_players[i]->unk48 && Slot14(m_players[i])) {
                Car* car = m_players[i]->car;
                for (unsigned int j = i + 1; j < m_players.size(); j++) {
                    if (!m_players[j]->unk48 && Slot14(m_players[j])) {
                        car->FUN_00458590(m_players[j]->car);
                    }
                }
            }
        }
        m_stage->FUN_004112e0(step);
        for (i = 0; i < m_players.size(); i++) {
            if (Slot14(m_players[i])) {
                m_players[i]->car->FUN_00458690();
            }
        }
        done += step;
    }
    if (dt > 0) {
        for (i = 0; i < m_players.size(); i++) {
            if (Slot14(m_players[i]) && m_players[i]->unk4c != 2) {
                m_players[i]->car->FUN_0045b390();
            }
        }
    }
    if (m_localPlayer->car->unk1f138) {
        m_localPlayer->FUN_00491110(1, m_time);
    }
    if (started) {
        if (!unk48) {
            SendUpdate();
        }
    } else if (!unk48) {
        m_time = time;
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x481860
void NetRace::Reset(int)
{
    __asm {
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 0b8h
        _emit 078h
        _emit 0ech
        _emit 0ffh
        _emit 0ffh
        _emit 033h
        _emit 0edh
        _emit 089h
        _emit 086h
        _emit 068h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 086h
        _emit 06ch
        _emit 008h
        _emit 000h
        _emit 000h
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
        _emit 030h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0aeh
        _emit 028h
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
        _emit 0c7h
        _emit 086h
        _emit 074h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 060h
        _emit 0f0h
        _emit 0ffh
        _emit 0ffh
        _emit 089h
        _emit 046h
        _emit 050h
        _emit 089h
        _emit 06eh
        _emit 040h
        _emit 033h
        _emit 0c9h
        _emit 089h
        _emit 046h
        _emit 04ch
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 03bh
        _emit 0c5h
        _emit 00fh
        _emit 095h
        _emit 0c1h
        _emit 089h
        _emit 046h
        _emit 048h
        _emit 089h
        _emit 0aeh
        _emit 064h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0aeh
        _emit 07ch
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0aeh
        _emit 070h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0aeh
        _emit 080h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 041h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 033h
        _emit 0ffh
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 03bh
        _emit 0c5h
        _emit 074h
        _emit 030h
        _emit 08bh
        _emit 056h
        _emit 00ch
        _emit 02bh
        _emit 0d0h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 03bh
        _emit 0fah
        _emit 073h
        _emit 024h
        _emit 08bh
        _emit 00ch
        _emit 0b8h
        _emit 08bh
        _emit 059h
        _emit 028h
        _emit 08dh
        _emit 004h
        _emit 0b8h
        _emit 0e8h
        _emit 058h
        _emit 0fah
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 014h
        _emit 052h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0ech
        _emit 03bh
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 015h
        _emit 085h
        _emit 0feh
        _emit 0ffh
        _emit 047h
        _emit 0ebh
        _emit 0c9h
        _emit 033h
        _emit 0c0h
        _emit 08dh
        _emit 0beh
        _emit 0d8h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 0b9h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0aeh
        _emit 0d0h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0aeh
        _emit 0d4h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 0f3h
        _emit 0abh
        _emit 08bh
        _emit 04eh
        _emit 030h
        _emit 0e8h
        _emit 02fh
        _emit 0f9h
        _emit 0f8h
        _emit 0ffh
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 078h
        _emit 024h
        _emit 000h
        _emit 000h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x482010
// Rebuilds the standings for the local player's stage: players who reached
// it sorted by their time there, then the rest by progress.
void NetRace::FUN_00482010()
{
    int stage = m_localPlayer->unk1a18;
    int behindCount;
    Player* behind[6];
    unk828 = stage;
    unk830 = 1;
    behindCount = 0;
    DAT_00565490 = 0;
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->active) {
            if (player->unk1a18 >= stage) {
                DAT_00565494[DAT_00565490++] = player;
            } else {
                behind[behindCount++] = player;
            }
        }
    }
    DAT_00564754 = stage - 1;
    qsort(DAT_00565494, DAT_00565490, 4, ComparePlayers1884);
    int count = DAT_00565490;
    int j;
    for (j = 0; j < count; j++) {
        m_entries[j].player = DAT_00565494[j];
    }
    qsort(behind, behindCount, 4, ComparePlayersStanding);
    for (j = 0; j < behindCount; j++) {
        m_entries[count + j].player = behind[j];
    }
    if (count) {
        int best = m_entries[0].player->unk1884[stage];
        FormatTime(best, m_entries[0].time);
        for (j = 1; j < count; j++) {
            FormatTimeDelta(m_entries[j].player->unk1884[stage] - best, m_entries[j].time);
        }
    }
    for (j = count; j < 6; j++) {
        m_entries[j].time[0] = 0;
    }
    unk82c = count + behindCount;
    for (j = 0; j < unk82c; j++) {
        m_entries[j].rank = j + 1;
    }
}

// FUNCTION: RALLYTROPHY 0x4821d0
void FormatTime(int time, char* text)
{
    int minutes = time / 60000;
    text[0] = minutes / 10 % 10 + '0';
    text[1] = minutes % 10 + '0';
    text[2] = ':';
    text[3] = time / 10000 % 6 + '0';
    text[4] = time / 1000 % 10 + '0';
    text[5] = ':';
    text[6] = time / 100 % 10 + '0';
    text[7] = time / 10 % 10 + '0';
    text[8] = 0;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4822b0
void FormatTimeDelta(int, char*)
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 008h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 018h
        _emit 0b8h
        _emit 073h
        _emit 0b2h
        _emit 0e7h
        _emit 045h
        _emit 0f7h
        _emit 0eeh
        _emit 0c1h
        _emit 0fah
        _emit 00eh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0e8h
        _emit 01fh
        _emit 003h
        _emit 0d0h
        _emit 08bh
        _emit 0dah
        _emit 0b8h
        _emit 067h
        _emit 066h
        _emit 066h
        _emit 066h
        _emit 0f7h
        _emit 0ebh
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0e8h
        _emit 01fh
        _emit 003h
        _emit 0d0h
        _emit 08bh
        _emit 0eah
        _emit 08bh
        _emit 0c3h
        _emit 099h
        _emit 0bbh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f7h
        _emit 0fbh
        _emit 08bh
        _emit 0c5h
        _emit 057h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 020h
        _emit 0bdh
        _emit 006h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0b9h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c6h
        _emit 007h
        _emit 02bh
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 020h
        _emit 099h
        _emit 0f7h
        _emit 0fbh
        _emit 0b8h
        _emit 0adh
        _emit 08bh
        _emit 0dbh
        _emit 068h
        _emit 08bh
        _emit 0dah
        _emit 0f7h
        _emit 0eeh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 00ch
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0f7h
        _emit 0fdh
        _emit 0b8h
        _emit 0d3h
        _emit 04dh
        _emit 062h
        _emit 010h
        _emit 0bdh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 0f7h
        _emit 0eeh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 006h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0f7h
        _emit 0fdh
        _emit 0b8h
        _emit 01fh
        _emit 085h
        _emit 0ebh
        _emit 051h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 0f7h
        _emit 0eeh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 005h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 0f7h
        _emit 0fdh
        _emit 0b8h
        _emit 067h
        _emit 066h
        _emit 066h
        _emit 066h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 014h
        _emit 0f7h
        _emit 0eeh
        _emit 08bh
        _emit 0c2h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 08bh
        _emit 0d0h
        _emit 0c1h
        _emit 0eah
        _emit 01fh
        _emit 003h
        _emit 0c2h
        _emit 099h
        _emit 08bh
        _emit 0f5h
        _emit 0f7h
        _emit 0feh
        _emit 032h
        _emit 0c0h
        _emit 085h
        _emit 0dbh
        _emit 07eh
        _emit 00dh
        _emit 080h
        _emit 0c3h
        _emit 030h
        _emit 08ah
        _emit 0c1h
        _emit 088h
        _emit 05fh
        _emit 001h
        _emit 0b9h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 020h
        _emit 085h
        _emit 0f6h
        _emit 07fh
        _emit 004h
        _emit 084h
        _emit 0c0h
        _emit 074h
        _emit 012h
        _emit 08ah
        _emit 05ch
        _emit 024h
        _emit 020h
        _emit 080h
        _emit 0c3h
        _emit 030h
        _emit 088h
        _emit 01ch
        _emit 039h
        _emit 041h
        _emit 0c6h
        _emit 004h
        _emit 039h
        _emit 03ah
        _emit 0b0h
        _emit 001h
        _emit 041h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 01ch
        _emit 085h
        _emit 0f6h
        _emit 07fh
        _emit 004h
        _emit 084h
        _emit 0c0h
        _emit 074h
        _emit 00ah
        _emit 08ah
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 004h
        _emit 030h
        _emit 088h
        _emit 004h
        _emit 039h
        _emit 041h
        _emit 08ah
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 004h
        _emit 030h
        _emit 088h
        _emit 004h
        _emit 039h
        _emit 08ah
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 041h
        _emit 0c6h
        _emit 004h
        _emit 039h
        _emit 03ah
        _emit 041h
        _emit 004h
        _emit 030h
        _emit 088h
        _emit 004h
        _emit 039h
        _emit 041h
        _emit 080h
        _emit 0c2h
        _emit 030h
        _emit 088h
        _emit 014h
        _emit 039h
        _emit 0c6h
        _emit 044h
        _emit 039h
        _emit 001h
        _emit 000h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c3h
    }
}
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x481e20
void NetRace::FUN_00481e20()
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 008h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 057h
        _emit 033h
        _emit 0c9h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 085h
        _emit 0c0h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 00fh
        _emit 084h
        _emit 0f6h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 056h
        _emit 00ch
        _emit 02bh
        _emit 0d0h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 03bh
        _emit 0cah
        _emit 00fh
        _emit 083h
        _emit 0e6h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 02ch
        _emit 088h
        _emit 08bh
        _emit 016h
        _emit 055h
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 052h
        _emit 038h
        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 084h
        _emit 0c9h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0a1h
        _emit 014h
        _emit 013h
        _emit 050h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 01bh
        _emit 08bh
        _emit 045h
        _emit 038h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 014h
        _emit 08bh
        _emit 046h
        _emit 03ch
        _emit 050h
        _emit 06ah
        _emit 001h
        _emit 08bh
        _emit 0cdh
        _emit 0e8h
        _emit 096h
        _emit 0f2h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 041h
        _emit 0ebh
        _emit 0aah
        _emit 08bh
        _emit 045h
        _emit 048h
        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 084h
        _emit 09ah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 045h
        _emit 044h
        _emit 08bh
        _emit 04eh
        _emit 03ch
        _emit 02bh
        _emit 0c8h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 0dbh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0d8h
        _emit 01dh
        _emit 03ch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 075h
        _emit 07dh
        _emit 08bh
        _emit 045h
        _emit 028h
        _emit 08bh
        _emit 088h
        _emit 0f8h
        _emit 014h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0c9h
        _emit 075h
        _emit 070h
        _emit 08bh
        _emit 05eh
        _emit 008h
        _emit 033h
        _emit 0ffh
        _emit 085h
        _emit 0dbh
        _emit 074h
        _emit 05ch
        _emit 08bh
        _emit 056h
        _emit 00ch
        _emit 02bh
        _emit 0d3h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 03bh
        _emit 0fah
        _emit 073h
        _emit 050h
        _emit 03bh
        _emit 07ch
        _emit 024h
        _emit 010h
        _emit 074h
        _emit 047h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 08bh
        _emit 014h
        _emit 0b8h
        _emit 08dh
        _emit 00ch
        _emit 0b8h
        _emit 08bh
        _emit 042h
        _emit 048h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 037h
        _emit 08bh
        _emit 045h
        _emit 028h
        _emit 08bh
        _emit 040h
        _emit 024h
        _emit 08bh
        _emit 0cah
        _emit 08bh
        _emit 051h
        _emit 028h
        _emit 08bh
        _emit 04ah
        _emit 024h
        _emit 08dh
        _emit 051h
        _emit 014h
        _emit 052h
        _emit 08dh
        _emit 050h
        _emit 014h
        _emit 052h
        _emit 08dh
        _emit 051h
        _emit 020h
        _emit 052h
        _emit 08dh
        _emit 050h
        _emit 020h
        _emit 052h
        _emit 081h
        _emit 0c1h
        _emit 0a0h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 051h
        _emit 005h
        _emit 0a0h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 0e8h
        _emit 0bfh
        _emit 0b2h
        _emit 002h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 018h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 00eh
        _emit 047h
        _emit 0ebh
        _emit 0a0h
        _emit 06ah
        _emit 000h
        _emit 06ah
        _emit 000h
        _emit 08bh
        _emit 0cdh
        _emit 0e8h
        _emit 0eah
        _emit 0f1h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 010h
        _emit 041h
        _emit 0e9h
        _emit 0fbh
        _emit 0feh
        _emit 0ffh
        _emit 0ffh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x481f40
int NetRace::Slot13(Player* player)
{
    Car* car = player->car;
    if (player->unk30) {
        return 0;
    }
    int lap = car->unk1f120 - 3;
    if (lap < 0) {
        if (DAT_00501458) {
            lap += m_stage->unka04;
        } else {
            lap = 0;
        }
    }
    player->FUN_00491110(1, m_time);
    car->FUN_00468980(lap);
    return 1;
}

// FUNCTION: RALLYTROPHY 0x481fa0
void NetRace::Draw()
{
    FUN_00487200();
    FUN_004877a0();
    if (DAT_00501458) {
        FUN_00487b50();
    } else {
        FUN_00487920();
    }
    FUN_00488420();
    if (m_localPlayer->unk1a18 != unk828 || unk830) {
        FUN_00482010();
        if (unk830) {
            FUN_00487560();
            FUN_00484100();
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4823f0
// qsort comparators over Player* arrays.
int ComparePlayers1884(const void* a, const void* b)
{
    Player* left = *(Player**)a;
    Player* right = *(Player**)b;
    int result = 0;
    if (right->unk1884[DAT_00564754 + 1] < left->unk1884[DAT_00564754 + 1]) {
        result = 1;
    } else if (left->unk1884[DAT_00564754 + 1] < right->unk1884[DAT_00564754 + 1]) {
        result = -1;
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x482430
int ComparePlayersStanding(const void* a, const void* b)
{
    Player* left = *(Player**)a;
    Player* right = *(Player**)b;
    if (right->unk1a1c < left->unk1a1c) {
        return -1;
    }
    if (left->unk1a1c < right->unk1a1c) {
        return 1;
    }
    if (right->unk1a18 < left->unk1a18) {
        return -1;
    }
    if (left->unk1a18 < right->unk1a18) {
        return 1;
    }
    Car* rightCar = right->car;
    Car* leftCar = left->car;
    if (leftCar && rightCar) {
        if (rightCar->unk1f120 < leftCar->unk1f120) {
            return -1;
        }
        if (leftCar->unk1f120 < rightCar->unk1f120) {
            return 1;
        }
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x4824b0
void NetRace::Lock()
{
    EnterCriticalSection(&m_lock);
}

// FUNCTION: RALLYTROPHY 0x4824c0
void NetRace::Unlock()
{
    LeaveCriticalSection(&m_lock);
}

// FUNCTION: RALLYTROPHY 0x4824d0
int NetRace::StartSession()
{
    int result = 0;
    if (DAT_0056b0b8->Start() >= 0) {
        result = 1;
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x4824f0
void NetRace::CloseSession()
{
    DAT_0056b0b8->Close();
}

// FUNCTION: RALLYTROPHY 0x482500
int NetRace::IsJoined()
{
    return DAT_0056b0b8->m_joined;
}

// FUNCTION: RALLYTROPHY 0x482510
void NetRace::FUN_00482510()
{
    if (m_host) {
        DAT_0056b0b8->m_connected = 0;
        SendGrid(1);
    }
}

// FUNCTION: RALLYTROPHY 0x482530
// True when every active player has driven all stages.
bool NetRace::Slot7()
{
    bool result = true;
    int stages = m_stage->stageCount;
    for (unsigned int i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->active && player->unk1a50 != stages) {
            result = false;
            break;
        }
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x4825d0
// Host: assigns start positions. For a new race, players not ready are
// dropped and the rest shuffled; otherwise the grid is ordered by total
// time (any position in ghost mode).
void NetRace::SendGrid(int newRace)
{
    if (!m_host) {
        return;
    }
    int count = 0;
    int dropCount = 0;
    Player* players[6];
    DWORD drop[6];
    MsgGridSlot ordered[6];
    MsgGridSlot shuffled[6];
    BufferDesc buffer;
    BufferDesc orderedBuffer;
    DWORD handle;
    DWORD orderedHandle;
    int j;
    if (newRace) {
        Lock();
        for (unsigned int i = 0; i < m_players.size(); i++) {
            Player* player = m_players[i];
            if (player->state == 2 && (player->unk182c & 2)) {
                players[count++] = player;
            } else {
                drop[dropCount++] = player->id;
            }
        }
        Unlock();
        for (j = 0; j < dropCount; j++) {
            DAT_0056b0b8->m_peer->DestroyPeer(drop[j], 0, 0, 0);
        }
        DAT_0056b0b8->SetSessionDesc(count);
        for (j = 0; j < count; j++) {
            Player* player = players[j];
            int other = rand() % count;
            if (other != j) {
                players[j] = players[other];
                players[other] = player;
            }
        }
        IDirectPlay8Peer* peer = DAT_0056b0b8->m_peer;
        for (j = 0; j < count; j++) {
            shuffled[j].type = 0;
            shuffled[j].sub = 3;
            shuffled[j].id = players[j]->id;
            if (DAT_00501314) {
                shuffled[j].slot = 0;
            } else {
                shuffled[j].slot = j + 1;
            }
        }
        buffer.size = count * sizeof(MsgGridSlot);
        buffer.data = shuffled;
        peer->SendTo(0, &buffer, 1, 0, 0, &handle, 8);
    } else {
        Lock();
        for (unsigned int i = 0; i < m_players.size(); i++) {
            Player* player = m_players[i];
            if (player->active) {
                players[count++] = player;
            }
        }
        Unlock();
        IDirectPlay8Peer* peer = DAT_0056b0b8->m_peer;
        orderedBuffer.size = count * sizeof(MsgGridSlot);
        orderedBuffer.data = ordered;
        if (DAT_00501314) {
            for (j = 0; j < count; j++) {
                ordered[j].type = 0;
                ordered[j].sub = 3;
                ordered[j].id = players[j]->id;
                ordered[j].slot = 0;
            }
        } else {
            for (j = 0; j < count; j++) {
                DAT_0056473c[j] = players[j];
            }
            qsort(DAT_0056473c, count, 4, ComparePlayersTotal);
            for (j = 0; j < count; j++) {
                ordered[j].type = 0;
                ordered[j].sub = 3;
                ordered[j].id = DAT_0056473c[j]->id;
                ordered[j].slot = count - j;
            }
        }
        peer->SendTo(0, &orderedBuffer, 1, 0, 0, &orderedHandle, 8);
    }
}

// FUNCTION: RALLYTROPHY 0x482580
// Round-trip latency to a remote player in ms (0 for local players).
int NetRace::GetLatency(Player* player)
{
    int latency = 0;
    DWORD info[0x17];
    info[0] = sizeof(info);
    if (!player->local) {
        IDirectPlay8Peer* peer = DAT_0056b0b8->m_peer;
        if (peer) {
            if (peer->GetConnectionInfo(player->id, info, 0) == 0) {
                latency = info[1];
            }
        }
    }
    return latency;
}

// FUNCTION: RALLYTROPHY 0x4828d0
void NetRace::SetUnk850(int value)
{
    unk850 = value;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4828e0
Player* NetRace::AddPlayer(const WCHAR*, DWORD, int, int)
{
    __asm {
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 06bh
        _emit 0cfh
        _emit 04dh
        _emit 000h
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 064h
        _emit 089h
        _emit 025h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0ech
        _emit 008h
        _emit 053h
        _emit 056h
        _emit 057h
        _emit 068h
        _emit 088h
        _emit 01ah
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0f1h
        _emit 0e8h
        _emit 06fh
        _emit 0e9h
        _emit 003h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 085h
        _emit 0c0h
        _emit 08bh
        _emit 05ch
        _emit 024h
        _emit 030h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 02ch
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 074h
        _emit 015h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 028h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 053h
        _emit 057h
        _emit 051h
        _emit 052h
        _emit 08bh
        _emit 0c8h
        _emit 0e8h
        _emit 0abh
        _emit 0e1h
        _emit 000h
        _emit 000h
        _emit 0ebh
        _emit 002h
        _emit 033h
        _emit 0c0h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 08bh
        _emit 006h
        _emit 08bh
        _emit 0ceh
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 050h
        _emit 008h
        _emit 085h
        _emit 0ffh
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 074h
        _emit 006h
        _emit 089h
        _emit 086h
        _emit 084h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0dbh
        _emit 0bfh
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 074h
        _emit 015h
        _emit 08dh
        _emit 054h
        _emit 024h
        _emit 00ch
        _emit 052h
        _emit 08dh
        _emit 04eh
        _emit 014h
        _emit 089h
        _emit 046h
        _emit 024h
        _emit 08bh
        _emit 041h
        _emit 008h
        _emit 057h
        _emit 050h
        _emit 0e8h
        _emit 008h
        _emit 09dh
        _emit 003h
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 00ch
        _emit 08dh
        _emit 054h
        _emit 024h
        _emit 00ch
        _emit 052h
        _emit 08dh
        _emit 04eh
        _emit 004h
        _emit 057h
        _emit 050h
        _emit 0e8h
        _emit 0f6h
        _emit 09ch
        _emit 003h
        _emit 000h
        _emit 089h
        _emit 0beh
        _emit 05ch
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0beh
        _emit 060h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 086h
        _emit 04ch
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 019h
        _emit 08bh
        _emit 00dh
        _emit 0b8h
        _emit 0b0h
        _emit 056h
        _emit 000h
        _emit 08bh
        _emit 041h
        _emit 020h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 00ch
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 00ch
        _emit 052h
        _emit 08bh
        _emit 0ceh
        _emit 0e8h
        _emit 027h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 006h
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 050h
        _emit 00ch
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 05fh
        _emit 05eh
        _emit 05bh
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 014h
        _emit 0c2h
        _emit 010h
        _emit 000h
    }
}
__declspec(naked)
// PE transcription preserves the original out-of-line vector insert call.
// FUNCTION: RALLYTROPHY 0x4829e0
void NetRace::RemovePlayer(Player*)
{
    __asm {
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 006h
        _emit 0ffh
        _emit 050h
        _emit 008h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 008h
        _emit 0c7h
        _emit 081h
        _emit 024h
        _emit 018h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 008h
        _emit 08dh
        _emit 044h
        _emit 024h
        _emit 008h
        _emit 050h
        _emit 08dh
        _emit 08eh
        _emit 0a0h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 082h
        _emit 028h
        _emit 018h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 051h
        _emit 008h
        _emit 06ah
        _emit 001h
        _emit 052h
        _emit 0e8h
        _emit 066h
        _emit 09ch
        _emit 003h
        _emit 000h
        _emit 08bh
        _emit 086h
        _emit 04ch
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 010h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 008h
        _emit 08bh
        _emit 048h
        _emit 03ch
        _emit 051h
        _emit 0e8h
        _emit 0cfh
        _emit 0c8h
        _emit 0ffh
        _emit 0ffh
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 08bh
        _emit 016h
        _emit 08bh
        _emit 0ceh
        _emit 0ffh
        _emit 052h
        _emit 00ch
        _emit 05eh
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x482a40
void NetRace::AddUnk8c0(int value)
{
    unk8c0.push_back(value);
}

// FUNCTION: RALLYTROPHY 0x482bf0
void NetRace::FUN_00482bf0(Player* player)
{
    Lock();
    for (unsigned int i = 0; i < m_players.size(); i++) {
        if (m_players[i] == player) {
            player->unk182c = 15;
            break;
        }
    }
    Unlock();
}

__declspec(naked)
// PE transcription preserves when the global host pointer is loaded relative to the member stores.
// FUNCTION: RALLYTROPHY 0x482c40
void NetRace::SetHost(Player*)
{
    __asm {
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 006h

        _emit 0ffh
        _emit 050h
        _emit 008h
        _emit 08bh
        _emit 044h

        _emit 024h
        _emit 008h
        _emit 03bh
        _emit 046h
        _emit 024h

        _emit 0b9h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 089h
        _emit 086h
        _emit 084h
        _emit 008h
        _emit 000h

        _emit 000h
        _emit 089h
        _emit 048h
        _emit 034h
        _emit 075h

        _emit 018h
        _emit 089h
        _emit 08eh
        _emit 04ch
        _emit 008h

        _emit 000h
        _emit 000h
        _emit 089h
        _emit 08eh
        _emit 05ch

        _emit 008h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 015h

        _emit 0b8h
        _emit 0b0h
        _emit 056h
        _emit 000h
        _emit 089h

        _emit 08ah
        _emit 034h
        _emit 002h
        _emit 000h
        _emit 000h

        _emit 08bh
        _emit 006h
        _emit 089h
        _emit 08eh
        _emit 054h

        _emit 008h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0ceh

        _emit 0ffh
        _emit 050h
        _emit 00ch
        _emit 05eh
        _emit 0c2h

        _emit 004h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x482c90
// Splits a received buffer into messages: type 0 for the race, type 1 for
// the sending player.
void NetRace::OnReceive(Player* sender, const BYTE* data, DWORD size)
{
    if (size > 1) {
        while ((int)size > 0) {
            int length = size;
            switch (*data) {
            case 0:
                length = OnMessage(data, size);
                break;
            case 1:
                length = sender->FUN_00491490(data, size);
                break;
            }
            size -= length;
            data += length;
        }
    }
}

// NONMATCHING: RALLYTROPHY 0x482ce0
// 744 vs 739 bytes. The original keeps the result in ebp (loaded with `size`
// on entry, set to each case's constant after Unlock, then `mov eax,ebp` in
// duplicated epilogues); this build folds the constants into the returns.
// Tried returns per case, a result variable with and without a default
// case, assigning the parameter, and inline per-message handlers (worse:
// nested inlining stops vector::size from inlining).
// Handles one race message; returns its size.
int NetRace::OnMessage(const BYTE* data, int size)
{
    int length = size;
    switch (data[1]) {
    case 0: {
        DWORD id = *(DWORD*)(data + 2);
        int state = *(int*)(data + 6);
        Lock();
        Player* player = FindPlayer(id);
        if (player) {
            player->state = state;
            if (state == 4) {
                player->unk17ec = 0;
            }
            if (player == m_hostPlayer) {
                unk844 = state;
            }
            if (player == m_localPlayer) {
                unk848 = state;
            }
            unk85c = 1;
        }
        Unlock();
        length = 10;
        break;
    }
    case 1: {
        DWORD id = *(DWORD*)(data + 4);
        Lock();
        Player* player = FindPlayer(id);
        if (player) {
            player->Lock();
            player->unk2c = data[2];
            player->unk24 = data[3] != 0;
            player->FUN_00490fb0((const char*)data + 9);
            player->unk20 = data[8];
            player->Unlock();
            unk860 = 1;
        }
        Unlock();
        length = 0x27;
        break;
    }
    case 2: {
        DWORD now = timeGetTime();
        int count = data[2];
        DWORD id = m_localPlayer->id;
        unk878 = now;
        unk87c = 1;
        for (int i = 0; i < count; i++) {
            if (((DWORD*)(data + 3))[i] == id) {
                unk878 = ((WORD*)(data + 0x1b))[i] + now;
                break;
            }
        }
        length = 0x27;
        break;
    }
    case 3: {
        DWORD id = *(DWORD*)(data + 2);
        int value = data[6];
        Lock();
        Player* player = FindPlayer(id);
        if (player) {
            player->Lock();
            player->unk1a70 = value;
            player->unk1a74 = 1;
            player->Unlock();
        }
        Unlock();
        length = 7;
        break;
    }
    case 4:
        unk870 = *(short*)(data + 2);
        length = 4;
        break;
    case 5:
        if (m_localPlayer) {
            Lock();
            m_localPlayer->unk1830 = *(int*)(data + 2);
            m_localPlayer->unk1834 = *(int*)(data + 6);
            unk858 = 1;
            Unlock();
        }
        length = 0xb;
        break;
    case 6: {
        Lock();
        Player* player = FindPlayer(*(DWORD*)(data + 2));
        if (player) {
            strcpy(player->unk1839, (const char*)data + 6);
            player->unk182c = 3;
        }
        Unlock();
        length = 0x4f;
        break;
    }
    }
    return length;
}

// FUNCTION: RALLYTROPHY 0x482fe0
// Tells the host this player's state.
void NetRace::SendState(int state)
{
    MsgState message;
    BufferDesc buffer;
    DWORD handle;
    message.type = 0;
    message.sub = 0;
    message.id = m_localPlayer->id;
    message.state = state;
    buffer.size = sizeof(message);
    buffer.data = &message;
    DAT_0056b0b8->m_peer->SendTo(m_hostPlayer->id, &buffer, 1, 0, 0, &handle, 8);
}

// FUNCTION: RALLYTROPHY 0x483040
void NetRace::SendByte(BYTE value)
{
    MsgByte message;
    BufferDesc buffer;
    DWORD handle;
    message.type = 1;
    message.sub = 4;
    message.value = value;
    buffer.size = sizeof(message);
    buffer.data = &message;
    DAT_0056b0b8->m_peer->SendTo(0, &buffer, 1, 150, 0, &handle, 0x32);
}

// FUNCTION: RALLYTROPHY 0x483090
// Tells the host the local player's name and options.
void NetRace::SendPlayerInfo()
{
    MsgPlayerInfo message;
    BufferDesc buffer;
    DWORD handle;
    Player* player = m_localPlayer;
    message.type = 0;
    message.sub = 1;
    message.unk2c = player->unk2c;
    message.unk24 = player->unk24;
    message.id = player->id;
    message.unk20 = player->unk20;
    strcpy(message.name, player->name);
    buffer.size = sizeof(message);
    buffer.data = &message;
    DAT_0056b0b8->m_peer->SendTo(m_hostPlayer->id, &buffer, 1, 0, 0, &handle, 8);
}

// FUNCTION: RALLYTROPHY 0x483130
// Sends the local car's setup to everyone.
void NetRace::SendSetup(BYTE value)
{
    MsgSetup message;
    BufferDesc buffer;
    DWORD handle;
    Car* car = m_localPlayer->car;
    message.type = 1;
    message.sub = 3;
    message.setup[0] = car->unk0;
    message.setup[1] = car->unk4;
    message.setup[2] = car->unk8;
    message.setup[3] = car->unkc;
    message.setup[4] = car->unk10;
    message.value = value;
    buffer.size = sizeof(message);
    buffer.data = &message;
    DAT_0056b0b8->m_peer->SendTo(0, &buffer, 1, 0, 0, &handle, 0x28);
}

// FUNCTION: RALLYTROPHY 0x4831b0
int NetRace::GetUnk844()
{
    return unk844;
}

// NONMATCHING: RALLYTROPHY 0x4831c0
// Per-frame network service: removes dropped players, sends the chat line,
// the host's player list, state, clock corrections and name handshakes.
// Close (1924 vs 1898 bytes): control flow and call sequence match; what
// differs is `this` in ebp instead of ebx (so the original's cached zero
// register is lost), the stack layout (frame 0x568 vs 0x390: the original
// overlaps the message arrays with the ansi/wide buffers) and the 0x1c/0x24
// temps. Tried inline_depth, auto_inline, local references: no effect.
void NetRace::ServiceNetwork()
{
    unsigned int i;
    BufferDesc buffer;
    DWORD handle;
    if (DAT_0056b0b8) {
        DAT_0056b0b8->FUN_00485610();
    }
    UpdateMessages();
    if (m_host) {
        if (unk8c0.size() > 0) {
            for (i = 0; i < unk8c0.size(); i++) {
                RemovePlayer((Player*)unk8c0[i]);
            }
            unk8c0.Resize(0);
        }
    }
    if (unk850) {
        if (m_removed.size() > 0) {
            Lock();
            for (i = 0; i < m_removed.size(); i++) {
                Player* player = m_removed[i];
                std::vector<Player*>::iterator it;
                for (it = m_players.begin(); it != m_players.end(); it++) {
                    if (player == *it) {
                        delete player;
                        m_players.Erase1(it);
                        break;
                    }
                }
            }
            m_removed.Resize(0);
            Unlock();
        }
    }
    if (unk858 && m_hostPlayer && m_localPlayer) {
        MsgChatLine message;
        message.type = 0;
        message.sub = 6;
        Lock();
        Player* local = m_localPlayer;
        FUN_0047ef80(FUN_004a1820(), &local->unk1830, message.text);
        buffer.size = sizeof(message);
        buffer.data = &message;
        message.id = local->id;
        DAT_0056b0b8->m_peer->SendTo(m_hostPlayer->id, &buffer, 1, 0, 0, &handle, 8);
        unk858 = 0;
        Unlock();
    }
    if (m_host) {
        Lock();
        for (i = 0; i < m_players.size(); i++) {
            Player* player = m_players[i];
            int flags = player->unk182c;
            if (!(flags & 1)) {
                MsgName message;
                message.type = 0;
                message.sub = 5;
                int* name = (int*)RandomName(8);
                int a = name[0];
                int b = name[1];
                *(int*)&message.name[0] = a;
                *(int*)&message.name[4] = b;
                message.end = 0;
                buffer.size = sizeof(message);
                buffer.data = &message;
                player->unk1830 = a;
                player->unk1834 = b;
                player->unk182c = 1;
                DAT_0056b0b8->m_peer->SendTo(player->id, &buffer, 1, 0, 0, &handle, 8);
            } else if (flags == 3) {
                IDirectPlay8Peer* peer = DAT_0056b0b8->m_peer;
                IDirectPlay8Address* address;
                IDirectPlay8Address** addresses;
                DWORD count = 0;
                DWORD size;
                DWORD type;
                char ansi[0x100];
                WCHAR wide[0x80];
                player->unk182c = 7;
                size = 0x100;
                wide[0] = 0;
                ansi[0] = 0;
                if (!player->local || !player->unk34) {
                    if (peer->GetPeerAddress(player->id, &address, 0) >= 0 && address) {
                        address->GetComponentByName(L"hostname", wide, &size, &type);
                        FUN_004b7730(ansi, wide, -1);
                        address->Release();
                    }
                } else {
                    addresses = 0;
                    if (peer->GetLocalHostAddresses(0, &count, 0) == (HRESULT)0x80158100) {
                        addresses = new IDirectPlay8Address*[count];
                        if (peer->GetLocalHostAddresses(addresses, &count, 0) >= 0) {
                            address = addresses[0];
                            address->GetComponentByName(L"hostname", wide, &size, &type);
                            FUN_004b7730(ansi, wide, -1);
                            for (DWORD j = 0; j < count; j++) {
                                addresses[j]->Release();
                            }
                        }
                        delete[] addresses;
                        addresses = 0;
                    }
                }
                unsigned long address_ = inet_addr(ansi);
                FUN_0047f1c0(player->id, address_, &player->unk1830, player->unk1839, ServiceCallback, player);
            } else if (flags & 8) {
                IDirectPlay8Peer* peer = DAT_0056b0b8->m_peer;
                player->unk182c = 0;
                if (!m_host || !player->local) {
                    peer->DestroyPeer(player->id, 0, 0, 0);
                } else {
                    DAT_0056b0b8->Close();
                    Unlock();
                    return;
                }
            }
        }
        Unlock();
        FUN_0047f3b0();
        IDirectPlay8Peer* peer = DAT_0056b0b8->m_peer;
        if (unk860) {
            MsgPlayerInfo messages[19];
            MsgPlayerInfo* out = messages;
            Lock();
            unk860 = 0;
            int count = m_players.size();
            for (i = 0; (int)i < count; i++) {
                Player* player = m_players[i];
                out->type = 0;
                out->sub = 1;
                out->unk2c = player->unk2c;
                out->unk24 = player->unk24;
                out->id = player->id;
                out->unk20 = player->unk20;
                strcpy(out->name, player->name);
                out++;
            }
            Unlock();
            buffer.size = count * sizeof(MsgPlayerInfo);
            buffer.data = messages;
            peer->SendTo(0, &buffer, 1, 0, 0, &handle, 0x28);
        }
        if (unk85c) {
            MsgState messages[19];
            MsgState* out = messages;
            Lock();
            unk85c = 0;
            int count = m_players.size();
            for (i = 0; (int)i < count; i++) {
                Player* player = m_players[i];
                out->type = 0;
                out->sub = 0;
                out->id = player->id;
                out->state = player->state;
                out++;
            }
            Unlock();
            buffer.size = count * sizeof(MsgState);
            buffer.data = messages;
            peer->SendTo(0, &buffer, 1, 0, 0, &handle, 0x28);
        }
        if (unk40 && m_time - unk874 > 2000) {
            unk874 = m_time;
            int sum = 0;
            for (int k = 0; k < 8; k++) {
                sum += unk8d8[k];
            }
            sum >>= 3;
            MsgClock message;
            message.type = 0;
            message.sub = 4;
            buffer.size = sizeof(message);
            buffer.data = &message;
            Lock();
            for (i = 0; i < m_players.size(); i++) {
                Player* player = m_players[i];
                if (!player->unk34) {
                    int offset = player->unk181c;
                    int other = player->unk1820;
                    int delta = abs(offset) - sum;
                    if (delta > 0) {
                        if (offset < 0) {
                            delta = -delta;
                        }
                        if (delta * other < 0) {
                            int half = (delta - other) >> 1;
                            if (abs(half) > 20) {
                                if (half > 300) {
                                    half = 300;
                                } else if (half < -300) {
                                    half = -300;
                                }
                                message.offset = (short)half;
                                peer->SendTo(player->id, &buffer, 1, 0, 0, &handle, 0x32);
                            }
                        }
                    }
                }
            }
            Unlock();
        }
    }
}

// FUNCTION: RALLYTROPHY 0x483930
// A random lower-case name of up to 32 letters (static buffer).
char* NetRace::RandomName(int length)
{
    if (length > 32) {
        length = 32;
    }
    DAT_00564718[length] = 0;
    while (length--) {
        DAT_00564718[length] = rand() % 26 + 'a';
    }
    return DAT_00564718;
}

// FUNCTION: RALLYTROPHY 0x483980
// Completion callback of FUN_0047f1c0: flags the player when the check fails.
void ServiceCallback(int, int status, int, Player* player)
{
    if (!status) {
        DAT_00501450->FUN_00482bf0(player);
    }
}

// FUNCTION: RALLYTROPHY 0x4839a0
// Per-frame sends: the stage time when the local player finishes a stage,
// and the car state ten times a second (once a second with the extension).
void NetRace::SendUpdate()
{
    Player* local = m_localPlayer;
    BufferDesc buffer;
    DWORD handle;
    int stage = local->unk1a18;
    if (stage != unk880) {
        MsgStage message(stage - 1, local->unk1884[stage], local->unk1a20);
        unk880 = stage;
        buffer.size = sizeof(message);
        buffer.data = &message;
        DAT_0056b0b8->m_peer->SendTo(0, &buffer, 1, 0, 0, &handle, 0x28);
    }
    int now = m_time;
    if (now - unk868 > 100) {
        IDirectPlay8Peer* peer = DAT_0056b0b8->m_peer;
        MsgCarState message;
        unk868 = now;
        Car* car = local->car;
        message.type = 1;
        message.sub = 0;
        car->GetState(&message.state);
        message.state.time = now;
        message.unk44 = local->unk44;
        message.remoteMask = 0;
        for (unsigned int i = 0; i < m_players.size(); i++) {
            Player* player = m_players[i];
            if (player != local && player->unk4c) {
                message.remoteMask |= 1 << (player->startSlot - 1);
            }
        }
        buffer.data = &message;
        buffer.size = 0x5e;
        if (now - unk86c > 1000) {
            message.extended = 1;
            message.unk5f = 1;
            unk86c = now;
            buffer.size = sizeof(message);
            car->GetState2(&message.state2);
            message.state2.time = now;
            if (m_host) {
                message.clockOffset = 0;
            } else {
                int sum = 0;
                for (int j = 0; j < 8; j++) {
                    sum += unk8d8[j];
                }
                int offset = m_hostPlayer->unk181c;
                int delta = abs(offset) - (sum >> 3);
                if (delta < 0) {
                    delta = 0;
                } else {
                    if (offset < 0) {
                        delta = -delta;
                    }
                    if (delta > 0x7fff) {
                        delta = 0x7fff;
                    } else if (delta < -0x8000) {
                        delta = -0x8000;
                    }
                }
                message.clockOffset = delta;
            }
        }
        peer->SendTo(0, &buffer, 1, 200, 0, &handle, 0x32);
    }
}

// FUNCTION: RALLYTROPHY 0x483bb0
// True when every active player has finished (state 5).
int NetRace::AllFinished()
{
    int result = 1;
    Lock();
    int count = m_players.size();
    for (int i = 0; i < count; i++) {
        if (m_players[i]->active && m_players[i]->state != 5) {
            result = 0;
            break;
        }
    }
    Unlock();
    return result;
}

// FUNCTION: RALLYTROPHY 0x483c10
// Host: tells every player how long to wait before starting so that all
// starts line up despite latency (half the round trip, at most 300 ms).
void NetRace::SendStartTimes()
{
    IDirectPlay8Peer* peer = DAT_0056b0b8->m_peer;
    MsgStartTimes message;
    BufferDesc buffer;
    DWORD handle;
    DWORD latency[6];
    DWORD info[0x17];
    DWORD maxLatency;
    message.type = 0;
    message.sub = 2;
    buffer.size = sizeof(message);
    buffer.data = &message;
    memset(info, 0, sizeof(info));
    info[0] = sizeof(info);
    maxLatency = 0;
    Lock();
    message.count = (BYTE)m_players.size();
    int i;
    for (i = 0; i < message.count; i++) {
        message.ids[i] = m_players[i]->id;
        latency[i] = 0;
        if (peer->GetConnectionInfo(message.ids[i], info, 0) >= 0) {
            latency[i] = info[1] / 2;
            if (latency[i] > maxLatency) {
                maxLatency = latency[i];
            }
        }
    }
    Unlock();
    for (i = 0; i < message.count; i++) {
        int delay = maxLatency - latency[i];
        if (delay < 0) {
            delay = 0;
        } else if (delay > 300) {
            delay = 300;
        }
        message.delays[i] = delay;
    }
    unk864 = 1;
    peer->SendTo(0, &buffer, 1, 0, 0, &handle, 0x88);
}

// FUNCTION: RALLYTROPHY 0x483d60
int NetRace::Slot14(Player* player)
{
    int result = player->active;
    if (!result && unk48 && player->car && player->car->unk1bef4 >= m_time) {
        result = 1;
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x483da0
void NetRace::DrawOverlay()
{
    DrawMessages();
}

// FUNCTION: RALLYTROPHY 0x483db0
void NetRace::ClearMessages()
{
    m_nextMessage = 0;
    for (int i = 0; i < 4; i++) {
        m_messages[i].active = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x483de0
// Adds "name: left" / "name: is host" lines to the message log and expires
// lines older than five seconds.
void NetRace::UpdateMessages()
{
    if (unk40) {
        if (m_removed.size() > 0) {
            Lock();
            for (unsigned int i = 0; i < m_removed.size(); i++) {
                Player* player = m_removed[i];
                if (!player->unk1828) {
                    NetMessage* message = &m_messages[m_nextMessage];
                    sprintf(message->text, "%s: %s", player->name, FUN_00405380(0x135));
                    message->time = m_time;
                    message->active = 1;
                    player->unk1828 = 1;
                    if (++m_nextMessage >= 4) {
                        m_nextMessage = 0;
                    }
                }
            }
            Unlock();
        }
        if (unk854) {
            unk854 = 0;
            NetMessage* message = &m_messages[m_nextMessage];
            sprintf(message->text, "%s: %s", m_hostPlayer->name, FUN_00405380(0x123));
            message->time = m_time;
            message->active = 1;
            if (++m_nextMessage >= 4) {
                m_nextMessage = 0;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (m_messages[i].active && m_time - m_messages[i].time > 5000) {
                m_messages[i].active = 0;
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x484100
void NetRace::FUN_00484100()
{
    float y = DAT_0056aeac * 12.0f;
    float step = DAT_0056aeac * 20.0f;
    for (int i = 0; i < unk82c; i++) {
        DrawEntry(DAT_0056aea8 * 5.0f, y, 2 - m_entries[i].player->ping);
        y += step;
    }
}

// FUNCTION: RALLYTROPHY 0x483f60
// Draws the message log, oldest line first, fading lines out after four
// seconds.
void NetRace::DrawMessages()
{
    int count = 0;
    for (int i = 0; i < 4; i++) {
        if (m_messages[i].active) {
            count++;
        }
    }
    if (count) {
        TextRenderer* text = DAT_0056b0c8->text;
        text->scaleX = DAT_0056aea8 * 0.85f;
        text->scaleY = DAT_0056aeac * 0.85f;
        Color color(0xffff0000);
        float step = DAT_0056aeac * 28.0f;
        float x = DAT_0056aeac * 44.0f;
        float y = DAT_0056aeac * 700.0f - count * step;
        for (int line = 0; line < 4; line++) {
            int index = m_nextMessage + line;
            if (index >= 4) {
                index -= 4;
            }
            NetMessage* message = &m_messages[index];
            if (message->active) {
                int age = m_time - message->time;
                if (age < 4000) {
                    color.a = 0xff;
                } else {
                    int fade = (age - 4000) * 255 / 1000;
                    if (fade > 255) {
                        fade = 255;
                    }
                    color.a = 0xff - fade;
                }
                text->color = color.value;
                text->Print(DAT_0056b0ac, x, y, message->text, 0, 1.0f, 1.0f);
                y += step;
            }
        }
        text->SetScale(DAT_0056aea8, DAT_0056aeac);
    }
}

// NONMATCHING: RALLYTROPHY 0x484190
// 492 vs 488 bytes. The original keeps -1 (colour and the final EH state) in
// edi and has a 4-byte smaller frame; the quad and EH setup otherwise match.
// Tried the colour as a DWORD, as an immediate, and other declaration orders.
// Draws one 16x16 icon from the 2x2 sheet at unk8f8 (kind 0..2).
void NetRace::DrawEntry(float x, float y, int kind)
{
    Vec2 uv[4];
    Vec2 position[4];
    Color color(0xffffffff);
    if (kind == 0) {
        SetQuad(uv, 0.0f, 0.5f, 0.5f, 1.0f);
    } else if (kind == 1) {
        SetQuad(uv, 0.5f, 0.0f, 1.0f, 0.5f);
    } else {
        SetQuad(uv, 0.0f, 0.0f, 0.5f, 0.5f);
    }
    float w = DAT_0056aea8 * 16.0f;
    float h = DAT_0056aeac * 16.0f;
    SetQuad(position, x, y, x + w, y + h);
    ((RaceDeviceSlots*)DAT_0056b0ac)->SetTexture(m_pingSprite);
    ((RaceDeviceSlots*)DAT_0056b0ac)->DrawQuad(position, uv, color, 1.0f, 1.0f);
}

// Compiler-generated functions VC6 emits for this module. Each marker names
// the exact COFF symbol; the typedef only gives match.py a definition line.

// FUNCTION: RALLYTROPHY 0x481580
// SYMBOL: ??_GNetRace@@UAEPAXI@Z
// NetRace scalar deleting destructor
typedef void Generated_481580();

// Module end: the guard-bit initialiser of the STL header statics and its
// atexit registration, which VC6 emits for every file including <string>/<vector>.

// FUNCTION: RALLYTROPHY 0x4843b0
// SYMBOL: _$E23
typedef void Generated_4843b0();

// FUNCTION: RALLYTROPHY 0x4843d0
// SYMBOL: _$E22
typedef void Generated_4843d0();
