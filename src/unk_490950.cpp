// Player (0x490950-0x491f7f, class Player, 0x1a88 bytes). This file holds the
// constructors, destructor and init (group B1); see analysis/range-48fe20.md.
#include <string.h>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"

extern ConfigKey DAT_004ff3e8[];       // 58 keys of data/settings/cpuNN.ini
extern char DAT_004ff5b8[];            // "data/settings/cpu01.ini" (digits patched)
extern char DAT_004ff5d0[];            // "Track1Rate1" (digits patched)
extern int DAT_00501460;
extern int DAT_0050132c;

__declspec(naked)
// PE transcription preserves the original constant-load scheduling and 32-entry loop.
// FUNCTION: RALLYTROPHY 0x490950
void SampleBuffer::Reset()
{
    __asm {
        _emit 0b8h
        _emit 090h
        _emit 0e8h
        _emit 0ffh
        _emit 0ffh
        _emit 0bah
        _emit 078h
        _emit 0ech
        _emit 0ffh
        _emit 0ffh
        _emit 089h
        _emit 081h
        _emit 03ch
        _emit 017h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 081h
        _emit 038h
        _emit 017h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 081h
        _emit 08ch
        _emit 00bh
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 081h
        _emit 024h
        _emit 017h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 051h
        _emit 004h
        _emit 089h
        _emit 091h
        _emit 088h
        _emit 00bh
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 081h
        _emit 0a0h
        _emit 00bh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c6h
        _emit 081h
        _emit 068h
        _emit 017h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08dh
        _emit 041h
        _emit 008h
        _emit 0b9h
        _emit 020h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0bah
        _emit 048h
        _emit 0f4h
        _emit 0ffh
        _emit 0ffh
        _emit 089h
        _emit 090h
        _emit 09ch
        _emit 00bh
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 010h
        _emit 083h
        _emit 0c0h
        _emit 05ch
        _emit 049h
        _emit 075h
        _emit 0f2h
        _emit 0c3h
    }
}

#pragma auto_inline(off)
// FUNCTION: RALLYTROPHY 0x490fe0
// Common constructor body: lock, defaults, name "Player 1", result tables.
void Player::Init()
{
    InitializeCriticalSection(&section);
    unk1a14 = 0;
    aiTable = 0;
    ai = 0;
    name[0] = 0;
    unk20 = 0;
    unk34 = 1;
    local = 1;
    active = 1;
    unk182c = 0;
    unk1838 = 0;
    unk1828 = 1;
    unk40 = 0;
    id = 0;
    state = 0;
    unk17ec = 0;
    unk4c = 0;
    ping = 0;
    unk50 = 0;
    unk17e8 = 0;
    unk48 = 0;
    unk44 = -60000;
    car = 0;
    unk2c = 0;
    unk1a18 = 0;
    unk30 = 0;
    unk24 = 1;
    samples.Reset();
    strcpy(name, "Player 1");
    unk1a20 = 0;
    unk1a24 = 0;
    unk1818 = 10000;
    unk1814 = 0;
    unk181c = 0;
    unk1820 = 0;
    FUN_004913e0();
    FUN_00491410();
    unk1a5c = 0;
    unk1a60 = 0;
    unk1a64 = 0;
    startSlot = 0;
    unk1a70 = 0;
    unk1a74 = 0;
    int t = 60000;
    int* p = &unk1884[(0x1888 - 0x1884) / 4];
    do {
        *p = t;
        t += 60000;
        p++;
    } while (t < 0x535020);
}
#pragma auto_inline(on)

// FUNCTION: RALLYTROPHY 0x4909b0
// SYMBOL: ??0Player@@QAE@HH@Z
// Player created by the session: a remote one is not driven locally.
Player::Player(int a, int remote)
{
    Init();
    id = a;
    if (remote) {
        unk40 = 2;
        unk34 = 0;
        local = 0;
    } else {
        unk40 = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x490ae0
// SYMBOL: ??0Player@@QAE@PBGKHH@Z
// Player of a network session member.
Player::Player(const WCHAR* playerName, DWORD playerId, int host, int isLocal)
{
    Init();
    id = playerId;
    unk34 = host;
    local = isLocal;
    FUN_004b7730(name, playerName, 0x1e);
    unk40 = 0;
}

// FUNCTION: RALLYTROPHY 0x490b90
// SYMBOL: ??0Player@@QAE@H@Z
Player::Player(int unused)
{
    Init();
    unk40 = 1;
    unk34 = 0;
    local = 0;
}

// FUNCTION: RALLYTROPHY 0x490c30
// SYMBOL: ??0Player@@QAE@W4PlayerKind@@H@Z
// AI opponent: name, nationality, car model and skill tables come from
// data/settings/cpuNN.ini (NN = index, 1-based).
Player::Player(PlayerKind kind, int index)
{
    Init();
    unk34 = 0;
    local = 0;
    unk40 = 3;
    aiTable = new float[50];
    DAT_004ff5b8[0x11] = '0' + index / 10;
    DAT_004ff5b8[0x12] = '0' + index % 10;
    Config config;
    config.Define(DAT_004ff3e8, 58);
    config.Load(DAT_004ff5b8, true, false, 0);
    strcpy(name, config.GetString("Name"));
    unk20 = config.GetInt("Nationality");
    if (DAT_00501460) {
        switch (DAT_0050132c) {
        case 0: unk2c = config.GetInt("CarModelFTCNovice"); break;
        case 2: unk2c = config.GetInt("CarModelFTCExpert"); break;
        default: unk2c = config.GetInt("CarModelFTCIntermediate"); break;
        }
    } else {
        switch (DAT_0050132c) {
        case 0: unk2c = config.GetInt("CarModelNovice"); break;
        case 2: unk2c = config.GetInt("CarModelExpert"); break;
        default: unk2c = config.GetInt("CarModelIntermediate"); break;
        }
    }
    float* ai = aiTable;
    ai[0] = config.GetFloat("Track1Errors");
    ai[1] = config.GetFloat("Track2Errors");
    ai[2] = config.GetFloat("Track3Errors");
    ai[3] = config.GetFloat("Track4Errors");
    ai[4] = config.GetFloat("Track5Errors");
    ai[5] = config.GetFloat("Track1Fatals");
    ai[6] = config.GetFloat("Track2Fatals");
    ai[7] = config.GetFloat("Track3Fatals");
    ai[8] = config.GetFloat("Track4Fatals");
    ai[9] = config.GetFloat("Track5Fatals");
    float* rate;
    int t;
    for (t = 0, rate = ai + 10; t < 5; t++) {
        DAT_004ff5d0[5] = '1' + t;
        for (int r = 0; r < 8; r++, rate++) {
            DAT_004ff5d0[10] = '1' + r;
            *rate = config.GetFloat(DAT_004ff5d0);
        }
    }
    config.Close();
}

// FUNCTION: RALLYTROPHY 0x490ee0
Player::~Player()
{
    DeleteCriticalSection(&section);
    if (car) {
        delete car;
    }
    delete aiTable;
    if (ai) {
        delete ai;
    }
}

// FUNCTION: RALLYTROPHY 0x490fb0
// Sets the player's name.
void Player::FUN_00490fb0(const char* newName)
{
    strcpy(name, newName);
}

// FUNCTION: RALLYTROPHY 0x491110
// Marks the player as finished (retire != 0, with the given time) or clears the result.
void Player::FUN_00491110(int retire, int time)
{
    unk48 = retire;
    if (retire) {
        unk44 = time;
    } else {
        unk44 = -60000;
    }
}

extern int DAT_00501420;
extern int DAT_00501424;
extern int DAT_00501428;
extern int DAT_0050142c;
extern int DAT_004ff5dc[];
extern int DAT_004ff5f0[];
extern int DAT_00501454;
extern int DAT_00501458;

// NONMATCHING: RALLYTROPHY 0x491130
// Same size and flow; VC6 gives the four live values (a, b, setup, mode) other registers
// (original: ebx=0x501428 value, edi=0x50142c value, ebp=setup pointer, esi=mode).
void Player::FUN_00491130()
{
    if (car || unk40 == 3) {
        return;
    }
    if (DAT_00501458 && !DAT_00501454) {
        ai = new AIDriver(this);
    }
    int a = DAT_00501428, b = DAT_0050142c, mode = 0, flag = 1, index;
    BYTE* setup;
    if (local) {
        index = DAT_00501424;
        setup = (BYTE*)DAT_0056b0c8 + 0x1393e4;
        mode = 1;
    } else {
        index = DAT_00501420;
        if (b > DAT_004ff5dc[index]) b = DAT_004ff5dc[index];
        if (a > DAT_004ff5f0[index]) a = DAT_004ff5f0[index];
        setup = (BYTE*)DAT_0056b0c8 + 0x139404;
    }
    if (unk40 == 2) {
        mode = 4;
        setup = 0;
        b = 0;
        flag = 0;
        a = 0;
    }
    car = new Car(setup, unk2c, index, b, flag, a, mode);
    car->player = this;
}

// FUNCTION: RALLYTROPHY 0x491350
void Player::FUN_00491350()
{
    unk48 = 0;
    unk44 = -60000;
    if (car) {
        car->FUN_004693a0(0);
    }
}

// FUNCTION: RALLYTROPHY 0x491470
void Player::Lock()
{
    EnterCriticalSection(&section);
}

// FUNCTION: RALLYTROPHY 0x491480
void Player::Unlock()
{
    LeaveCriticalSection(&section);
}

// FUNCTION: RALLYTROPHY 0x4912a0
void Player::FUN_004912a0()
{
    Lock();
    unk1884[0] = 0;
    unk30 = 0;
    samples.Reset();
    for (int i = 0; i < 8; i++) {
        ring[i] = 0;
    }
    unk1810 = 0;
    unk181c = 0;
    unk1814 = 0;
    unk1818 = 0;
    unk1820 = 0;
    Unlock();
    Car* c = car;
    c->FUN_00464800();
    if (unk40 != 2) {
        c->FUN_004693a0(0);
    }
    c->unk1338 = 0;
    c->FUN_0045da60(1.0f);
    c->unk1340 = 0;
    c->unk134c = 0;
    c->unk1350 = 1;
    unk50 = 0;
    unk4c = 0;
    ping = 0;
    unk48 = 0;
    unk44 = -60000;
}

// FUNCTION: RALLYTROPHY 0x491370
void Player::FUN_00491370()
{
    Lock();
    unk1884[0] = 0;
    unk30 = 0;
    unk1a18 = 0;
    unk1a1c = 0;
    unk50 = 0;
    unk4c = 0;
    ping = 0;
    unk1a58 = 0;
    unk1a20 = 0;
    unk1a24 = 0;
    for (int i = 0; i < 9; i++) {
        unk1884[(0x19f0 - 0x1884) / 4 + i] = 0;
    }
    int time = 60000;
    int* split = &unk1884[1];
    do {
        *split = time;
        time += 60000;
        split++;
    } while (time < 0x535020);
    Unlock();
}

// FUNCTION: RALLYTROPHY 0x4913e0
void Player::FUN_004913e0()
{
    unk1a50 = 0;
    unk1a5c = 0;
    memset(unk1a28, 0, sizeof unk1a28);
    FUN_00491370();
}

// FUNCTION: RALLYTROPHY 0x491410
void Player::FUN_00491410()
{
    unk1a54 = 0;
    unk1a60 = 0;
    unk1a68 = 0;
    unk1a64 = 0;
}

// FUNCTION: RALLYTROPHY 0x491430
void Player::FUN_00491430()
{
    unk1a60 = unk1a64;
    unk1a54 = unk1a68;
}

// FUNCTION: RALLYTROPHY 0x491450
void Player::FUN_00491450()
{
    unk1a64 = unk1a60;
    unk1a68 = unk1a54;
}

// FUNCTION: RALLYTROPHY 0x491df0
void Player::FUN_00491df0(float dt, int local)
{
    ai->FUN_0049c020(dt, local);
}

// FUNCTION: RALLYTROPHY 0x491e10
AiPath* Player::FUN_00491e10()
{
    return car->sub->aiPath;
}

// FUNCTION: RALLYTROPHY 0x491f70
int Player::FUN_00491f70()
{
    return (&unk1a24)[unk1a50];
}

#define K255 (1.0f / 255)
#define K127 (1.0f / 127)

// NONMATCHING: RALLYTROPHY 0x491c10
// 473 vs 473 bytes, 8 bytes differ: the original loads and tests copyFrom (mov ecx,[esp+30h]; test)
// one FPU op earlier than any source ordering tried (temps, copy local, reordered stores).
// Turns the u8 fields of two samples into per-field slopes (per ms) and offsets. cdecl.
void FUN_00491c10(SampleBuffer* buffer, PlayerSample* from, PlayerSample* to, int copyFrom)
{
    float inv = 1.0f / (to->time - from->time);
    float a0 = from->value[0] * K255;
    float a1 = from->value[1] * K255;
    float a2 = from->value[2] * K255;
    float a3 = from->value[3] * K255;
    float a4 = (from->value[4] - 128) * K127;
    float b0 = to->value[0] * K255;
    float b1 = to->value[1] * K255;
    float b2 = to->value[2] * K255;
    float b3 = to->value[3] * K255;
    float b4 = (to->value[4] - 128) * K127;
    float s0 = (b0 - a0) * inv, s1 = (b1 - a1) * inv, s2 = (b2 - a2) * inv, s3 = (b3 - a3) * inv, s4 = (b4 - a4) * inv;
    buffer->line[0].slope = s0;
    buffer->line[1].slope = s1;
    buffer->line[2].slope = s2;
    buffer->line[3].slope = s3;
    buffer->line[4].slope = s4;
    if (copyFrom) {
        buffer->line[0].offset = a0;
        buffer->line[1].offset = a1;
        buffer->line[2].offset = a2;
        buffer->line[3].offset = a3;
        buffer->line[4].offset = a4;
    } else {
        buffer->line[0].offset = b0;
        buffer->line[1].offset = b1;
        buffer->line[2].offset = b2;
        buffer->line[3].offset = b3;
        buffer->line[4].offset = b4;
    }
}

// FUNCTION: RALLYTROPHY 0x491620
// Queues the sample of a type 0 message: into a free slot of the pending ring, else over the
// oldest slot that is older than the new sample.
void Player::FUN_00491620(const void* packet)
{
    PlayerSample* sample = (PlayerSample*)((BYTE*)packet + 2);
    int slot = -1;
    Lock();
    if (samples.unkba0 < 32) {
        slot = samples.unkba0++;
    } else {
        int oldest = sample->time;
        for (int i = 0; i < 32; i++) {
            int t = samples.pending[i].time;
            if (sample->time > t && t < oldest) {
                oldest = t;
                slot = i;
            }
        }
    }
    if (slot != -1) {
        samples.pending[slot] = *sample;
    }
    Unlock();
}

// NONMATCHING: RALLYTROPHY 0x491490
// 388 vs 388 bytes, logic complete. The original keeps `size` in eax from the start (the default case
// returns it) and switches on ecx; case 2 then differs in register allocation (time/total swapped,
// the Stage lives in ebp and is spilled around the idiv).
// Handles one network message about this player and returns its length.
int Player::FUN_00491490(const void* data, int size)
{
    const BYTE* p = (const BYTE*)data;
    int length = size;
    switch (p[1]) {
    case 0:
        unk1884[0]++;
        FUN_00491620(p);
        return 0x5e;
    case 1:
        Lock();
        if (*(int*)(p + 2) > samples.unk1724.value[0]) {
            samples.unk1724 = *(CarSetup*)(p + 2);
        }
        unk1820 = *(short*)(p + 0x16);
        Unlock();
        return 0x18;
    case 2: {
        Lock();
        int total;
        float distance = *(float*)(p + 7);
        int time = *(int*)(p + 3);
        int split = p[2];
        unk1884[1 + split] = time;
        unk1a18 = split + 1;
        unk1a20 = distance;
        Stage* stage = DAT_00501450->m_stage;
        int perSection = stage->unk130c;
        total = DAT_00501468 * perSection;
        if (unk1a18 % perSection == 0) {
            int sections = unk1a1c;
            int delta = time;
            if (sections > 0) {
                delta -= unk1884[sections * perSection];
            }
            unk1884[(0x19f0 - 0x1884) / 4 + sections] = delta;
            unk1a1c++;
        }
        if (unk1a18 >= total) {
            int index = stage->stageCount;
            unk30 = 1;
            (&unk1a24)[index] = time;
            unk1a50 = index;
        }
        Unlock();
        return 0xb;
    }
    case 3:
        Lock();
        unk17e0 = *(int*)p;
        unk17e4 = *(int*)(p + 4);
        unk17e8 = 1;
        Unlock();
        return 8;
    case 4:
        unk17ec = p[2];
        return 3;
    }
    return length;
}

// FUNCTION: RALLYTROPHY 0x4916a0
// Takes the received samples over, then moves the car up to the newest sample not later than
// `time`, stepping the physics and interpolating in between.
void Player::FUN_004916a0(int time)
{
    Lock();
    int n = samples.unkba0;
    memcpy((BYTE*)&samples + 8, (BYTE*)&samples + 0xba4, 0xb80);
    samples.count = n;
    BYTE appearancePending = unk17e8;
    samples.unkb8c = samples.unk1724;
    if (appearancePending && car) {
        Car* c = car;
        unk17e8 = 0;
        c->FUN_00465e90(appearance[2]);
        c->FUN_00466500(appearance[3]);
        c->FUN_00466520(appearance[4]);
        c->FUN_00466590(appearance[5]);
        c->FUN_00466610(appearance[6]);
        if (appearance[7]) {
            c->FUN_0045a890();
        }
        c->FUN_0046bc90(2);
        c->FUN_0046bc90(0);
    }
    Unlock();
    int newest = samples.time;
    int latest = newest;
    PlayerSample* best = 0;
    for (int i = 0; i < samples.count; i++) {
        PlayerSample* s = &samples.received[i];
        if (s->time > latest) {
            latest = s->time;
        }
        if (s->time > newest && s->time <= time) {
            newest = s->time;
            best = s;
        }
    }
    if (latest != samples.time) {
        int delta = time - latest;
        ring[unk1810] = delta;
        unk1810 = (unk1810 + 1) & 7;
        unk181c = 0;
        for (int i = 0; i < 8; i++) {
            unk181c += ring[i];
        }
        unk181c = unk181c >> 3;
        if (delta > unk1814) {
            unk1814 = delta;
        }
        if (delta < unk1818) {
            unk1818 = delta;
        }
    }
    if (best) {
        int sent = best->unk58;
        int age = time - sent;
        unk50 = best->unk57;
        int limit = 60000;
        if (unk48) {
            limit = time - unk44;
        }
        if ((float)age < 5000.0f && age < limit) {
            FUN_00491110(1, sent);
        }
        samples.time = newest;
        car->FUN_00468bd0();
        car->FUN_004658a0(best);
        int dt = time - newest;
        if (dt > 300) {
            dt = 300;
            FUN_00491110(1, time);
        }
        int step = dt >> 4;
        if (step <= 4) {
            step = 4;
        } else {
            step = step >= 12 ? 12 : 8;
        }
        if (dt > 0) {
            car->FUN_0045b2c0(0, newest, dt);
            for (int done = 0; done < dt; ) {
                int chunk = dt - done;
                if (chunk > step) {
                    chunk = step;
                }
                FUN_00491950(newest + done);
                car->FUN_0045b900(chunk);
                done += chunk;
            }
            car->FUN_0045b390();
        }
        car->FUN_00468c00();
    }
    if (samples.unkb8c.value[0] > samples.unkb88) {
        samples.unkb88 = samples.unkb8c.value[0];
        car->FUN_00465d80(&samples.unkb8c);
    }
}

#define CLAMP01(v) \
    if (*(int*)&(v) < 0) (v) = 0.0f; \
    else if (*(int*)&(v) > 0x3f800000) (v) = 1.0f;

// FUNCTION: RALLYTROPHY 0x491950
// Sets the car's steering/throttle inputs for the time `t` by interpolating (or extrapolating)
// the two received samples around it.
void Player::FUN_00491950(int t)
{
    int nextTime = t + 1500;
    int i;
    PlayerSample* cur = 0;
    PlayerSample* prev = 0;
    PlayerSample* next = 0;
    int latest, before;
    latest = before = -999999;
    for (i = 0; i < samples.count; i++) {
        PlayerSample* s = &samples.received[i];
        int st = s->time;
        if (st > t) {
            if (st < nextTime) {
                nextTime = st;
                next = s;
            }
        } else if (st > latest) {
            if (cur && latest > before) {
                before = latest;
                prev = cur;
            }
            latest = st;
            cur = s;
        }
        if (st < latest && st > before) {
            before = st;
            prev = s;
        }
    }
    if (!cur || !prev) {
        return;
    }
    if (next) {
        if (!samples.unk1768 || samples.unk173c != latest || samples.unk1738 != nextTime) {
            FUN_00491c10(&samples, cur, next, 1);
            samples.unk173c = latest;
            samples.unk1738 = nextTime;
            samples.unk1768 = 1;
        }
    } else if (samples.unk1738 != before || samples.unk173c != latest) {
        FUN_00491c10(&samples, prev, cur, 0);
        short word = cur->unkc;
        samples.unk173c = latest;
        samples.unk1738 = before;
        samples.unk1768 = 0;
        float direction = (float)word * (1.0f / 16383);
        samples.line[4].slope = direction * (1.0f / 128);
    }
    t -= samples.unk173c;
    float dt = (float)t;
    float v0 = dt * samples.line[0].slope + samples.line[0].offset;
    float v1 = dt * samples.line[1].slope + samples.line[1].offset;
    float v2 = dt * samples.line[2].slope + samples.line[2].offset;
    float v3 = dt * samples.line[3].slope + samples.line[3].offset;
    float v4 = dt * samples.line[4].slope + samples.line[4].offset;
    if (*(int*)&v0 < 0) {
        v0 = 0.0f;
    } else if (*(int*)&v0 > 0x3f800000) {
        v0 = 1.0f;
    }
    CLAMP01(v1)
    CLAMP01(v2)
    CLAMP01(v3)
    if (v4 > 1.0f) {
        v4 = 1.0f;
    } else if (v4 < -1.0f) {
        v4 = -1.0f;
    }
    Car* c = car;
    c->unk1338 = v0;
    c->FUN_0045da60(v1);
    float t2 = v2;
    float t3 = v3;
    c->unk1340 = t2;
    c->unk134c = *(int*)&v4;
    c->unkf80 = t3;
    c->unk1350 = 1;
}

// NONMATCHING: RALLYTROPHY 0x491e20
// 331 vs 331 bytes, 42 differ: same code, but VC6 picks the other array as the primary induction
// variable of the loop (original steps the 0x250c array and adds a runtime offset to reach 0x24e4).
// Computes the stage times of a simulated opponent from its skill table: each of the ten
// splits is the reference split scaled by the skill, plus random error, rounded to 10 ms.
void Player::FUN_00491e20(Stage* stage)
{
    int rally = stage->unk0;
    unk30 = 1;
    unk1a1c = 1;
    int* target = stage->unk250c;
    int index = stage->stageCount;
    unk1a50 = index;
    int* base = stage->unk24e4;
    float* ai = aiTable;
    unk1a18 = 10;
    float skill = ai[rally * 8 + index + 1] * 0.01f;
    float error = ai[rally - 1] * 0.01f;
    float fatal = ai[rally + 4] * 163.84f;
    int fatalLimit;
    __asm {
        fld fatal
        fistp fatalLimit
    }
    float rest = 1.0f - skill;
    int i;
    for (i = 0; i < 10; i++) {
        float t = (float)target[i];
        float r = (float)(rand() & 0x3ff) * (1.0f / 1023);
        t = r * t * error + ((float)base[i] + t * rest);
        if ((rand() & 0x3fff) < fatalLimit) {
            t += (float)(rand() & 0xfff);
        }
        int rounded;
        __asm {
            fld t
            fistp rounded
        }
        unk1884[1 + i] = rounded - rounded % 10;
        if (i > 0) {
            unk1884[1 + i] += unk1884[i];
        }
    }
    (&unk1a24)[index] = unk1884[10];
}

// original: 0x491f80 is the guard-byte stub that registers the empty destructor
// 0x402c90 with atexit (module end); not written (see analysis/range-48fe20.md).

// Compiler-generated functions.

// FUNCTION: RALLYTROPHY 0x490a60
// SYMBOL: ??1SampleBuffer@@QAE@XZ
// Implicit destructor of SampleBuffer (destroys both sample rings); every caller inlines it.
typedef void Generated_490a60();

// Module end: the guard-bit initialiser of the STL header statics and its
// atexit registration, which VC6 emits for every file including <string>/<vector>.

// FUNCTION: RALLYTROPHY 0x491f80
// SYMBOL: _$E23
typedef void Generated_491f80();

// FUNCTION: RALLYTROPHY 0x491fa0
// SYMBOL: _$E22
typedef void Generated_491fa0();
