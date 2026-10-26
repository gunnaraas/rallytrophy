// Single-player race mode SingleRace (0x4807d0-0x48138f, vtable 0x4e23ac): the
// race-mode class for a normal race. It derives from RaceMode (base ctor
// 0x485bb0, dtor 0x485da0) like the network race mode.
#include "unk_481390.h"
#include "rallytrophy.h"
#include <stdlib.h>
#include <math.h>

// A 3D point with an empty destructor and the implicit (memberwise) copy constructor.
class Pos3 {
public:
    ~Pos3() {}
    Pos3& operator-=(const Vec3& o)
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }
    float Length() const { return (float)sqrt(x * x + y * y + z * z); }
    float x, y, z;
};

// FUNCTION: RALLYTROPHY 0x4807d0
SingleRace::SingleRace()
{
    unk7a8 = 0;
    unk844 = 0;
    unk848 = 0;
    DAT_00501450 = (NetRace*)this; // the global is typed for NetRace in this header
}

// FUNCTION: RALLYTROPHY 0x480820
SingleRace::~SingleRace()
{
}

// NONMATCHING: RALLYTROPHY 0x480830
// Same size (406). Only the scheduling of the member stores at the top differs: the original
// stores 0x7a8, 0x848, 0x3c, 0x50, 0x40, loads the player vector's begin pointer, then stores 0x4c
// and 0x48; this build hoists the two `= 1` stores (and the begin load) above the others. The loops
// match, including the out-of-line vector insert (AddListener takes the player by value).
void SingleRace::Slot4(int mode)
{
    unsigned int i, j, k;
    unk7a8 = 0;
    unk848 = 0;
    m_time = -3000;
    unk50 = 1;
    unk40 = 0;
    unk4c = 1;
    unk48 = mode;
    for (i = 0; i < m_players.size(); i++) {
        m_players[i]->ai->FUN_0049ba70();
    }
    if (!mode) {
        unk44 = 0;
    }
    for (i = 0; i < m_players.size(); i++) {
        Car* car = m_players[i]->car;
        car->FUN_0048e660();
        car->FUN_0045cc30(&m_stage->starts[m_players[i]->startSlot], DAT_0057b980);
        car->FUN_004654f0(0);
        m_players[i]->unk1a7c = -1;
        m_players[i]->FUN_004912a0();
        if (!mode) {
            m_players[i]->FUN_00491370();
        }
        if (!m_players[i]->local) {
            for (j = 0; j < m_players.size(); j++) {
                if (i != j) {
                    m_players[i]->ai->AddListener(m_players[j]);
                }
            }
        }
    }
    m_stage->FUN_00411260();
    for (k = 0; k < m_players.size(); k++) {
        m_players[k]->car->FUN_00465e90(unk844);
        m_players[k]->ai->FUN_0049f790(m_stage->unk0 - 1);
    }
}

__declspec(naked)
// PE transcription preserves the original reset-store and player-loop scheduling.
// FUNCTION: RALLYTROPHY 0x4809d0
void SingleRace::Reset(int)
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
        _emit 0b8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0beh
        _emit 0a8h
        _emit 007h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 0beh
        _emit 048h
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
        _emit 07eh
        _emit 040h
        _emit 089h
        _emit 046h
        _emit 04ch
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 033h
        _emit 0c9h
        _emit 03bh
        _emit 0c7h
        _emit 00fh
        _emit 095h
        _emit 0c1h
        _emit 089h
        _emit 046h
        _emit 048h
        _emit 041h
        _emit 08bh
        _emit 0e9h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 02ch
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
        _emit 020h
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
        _emit 025h
        _emit 009h
        _emit 001h
        _emit 000h
        _emit 055h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0bdh
        _emit 04ah
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0e6h
        _emit 093h
        _emit 0feh
        _emit 0ffh
        _emit 047h
        _emit 0ebh
        _emit 0cdh
        _emit 08bh
        _emit 04eh
        _emit 030h
        _emit 0e8h
        _emit 01bh
        _emit 008h
        _emit 0f9h
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

// NONMATCHING: RALLYTROPHY 0x480a50
// 650 vs 648 bytes. Logic and block structure match (the Player call with the local flag is
// split into two branches as in the original), but this build keeps dt in ebx and the player
// index in ebp; the original has them the other way round, so every use differs.
void SingleRace::Update(int dt)
{
    unsigned int i, j, k;
    FUN_0048f500();
    unk848++;
    if (unk848 >= 10 && unk44) {
        unk40 = 1;
    }
    int time = m_time;
    m_time = time + dt;
    if (m_time >= 1000 || !unk40) {
        unk50 = 0;
    }
    unk4c = m_time < 0 || (!unk40 && !unk48);
    if (dt > 0) {
        for (i = 0; i < m_players.size(); i++) {
            Car* car = m_players[i]->car;
            RaceCamera* camera = m_camera;
            if (camera->target != car) {
                camera = 0;
            }
            if (!unk48) {
                int frozen = unk4c;
                if (m_players[i]->unk30) {
                    frozen = 1;
                }
                if (car) {
                    car->unk1334 = frozen;
                    car->FUN_0045b2c0(camera, time, dt);
                }
            } else {
                car->FUN_0045b2c0(camera, time, dt);
            }
        }
        m_stage->FUN_004112a0(this);
    }
    for (int done = 0; done < dt; ) {
        int step = dt - done;
        if (step > 4) {
            step = 4;
        }
        for (i = 0; i < m_players.size(); i++) {
            Car* car = m_players[i]->car;
            if (m_players[i] != m_localPlayer) {
                if (!unk48) {
                    m_players[i]->FUN_00491df0((float)step, 0);
                }
            } else {
                if (!unk48) {
                    m_players[i]->FUN_00491df0((float)step, 1);
                }
            }
            car->FUN_0045b900(step);
        }
        for (i = 0; i < m_players.size(); i++) {
            if (!m_players[i]->unk48) {
                Car* car = m_players[i]->car;
                for (j = i + 1; j < m_players.size(); j++) {
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
            Player* player = m_players[i];
            player->car->FUN_0045b390();
            if (player->unk30 && player != m_localPlayer) {
                player->FUN_00491110(1, m_time);
            }
        }
    }
    if (!unk40 && !unk48) {
        m_time = time;
    }
}

// NONMATCHING: RALLYTROPHY 0x480ce0
// 1069 vs 1076 bytes; logic complete. The ebp frame, the stack copies of the two ints before
// fild and the fistp come out as in the original (the function has an __asm block). Remaining
// differences: stack slot numbering, the first copy of the body position (this build adds 0x14
// to the pointer first), and the vector arithmetic of branches 2/3 which the original keeps in
// FPU registers.
// Estimates the stage time of every player who is still driving: the distance along
// the stage is interpolated between the nearest checkpoints.
void SingleRace::Slot6()
{
    unsigned int i, j, k;
    Stage* stage = m_stage;
    int count = stage->unka04;
    if (count < 1) {
        return;
    }
    int last = count - 1;
    float closing = (stage->FUN_00414fb0(last)->position - stage->FUN_00414fb0(0)->position).Length();
    int rally = DAT_00501468;
    float total = closing + stage->FUN_00414fb0(last)->distance;
    float scaled = rally * total;
    for (i = 0; i < m_players.size(); i++) {
        Player* player = m_players[i];
        if (player->unk30 || player->unk40 != 1) {
            continue;
        }
        CarBody* body = player->car->body;
        Pos3 position = *(Pos3*)body->unk14;
        int laps = player->unk1a1c;
        float before = laps * total;
        int index = stage->FUN_00414dc0((Vec3*)&position, 0)->index;
        int next = index + 1;
        int prev = index - 1;
        Pos3 a = position;
        Pos3 b = position;
        float progress;
        if (index != 0 && index != last) {
            a -= stage->FUN_00414fb0(prev)->position;
            b -= stage->FUN_00414fb0(next)->position;
            float d1 = a.Length();
            float d2 = b.Length();
            float base = stage->FUN_00414fb0(next)->distance;
            progress = (base - stage->FUN_00414fb0(prev)->distance) * (d1 / (d2 + d1)) + stage->FUN_00414fb0(prev)->distance;
        } else if (player->unk1a18 % 10 < 2) {
            float span = closing + stage->FUN_00414fb0(1)->distance;
            a -= stage->FUN_00414fb0(last)->position;
            b -= stage->FUN_00414fb0(1)->position;
            progress = stage->FUN_00414fb0(1)->distance - b.Length() / (a.Length() + b.Length()) * span;
            if (progress < 0.0f) {
                progress = 0.0f;
            }
        } else {
            float base = stage->FUN_00414fb0(prev = last - 1)->distance;
            float span = closing + stage->FUN_00414fb0(last)->distance - stage->FUN_00414fb0(prev)->distance;
            a -= stage->FUN_00414fb0(prev)->position;
            float d1 = a.Length();
            float d2 = (b -= stage->FUN_00414fb0(0)->position).Length();
            progress = d1 / (d2 + d1) * span + base;
            if (progress > total) {
                progress = total;
            }
        }
        float all = progress + before;
        float estimate = scaled / all * m_time;
        int rounded;
        __asm {
            fld estimate
            fistp rounded
        }
        stage = m_stage;
        ((int*)&player->unk1a24)[stage->stageCount] = rounded;
        player->unk1a1c = DAT_00501468;
        player->unk1a50 = stage->stageCount;
        player->unk30 = 1;
    }
}

// FUNCTION: RALLYTROPHY 0x481120
void SingleRace::Draw()
{
    FUN_00487200();
    FUN_004877a0();
    FUN_00487b50();
    FUN_004873b0();
    FUN_00487e00();
    FUN_004880f0();
    FUN_00488420();
}

// FUNCTION: RALLYTROPHY 0x481160
int SingleRace::Slot13(Player* player)
{
    int result = 0;
    if (!player->unk30) {
        Car* car = player->car;
        int lap = car->unk1f120 - 3;
        if (lap < 0) {
            lap += m_stage->unka04;
        }
        player->FUN_00491110(1, m_time);
        car->FUN_00468980(lap);
        result = 1;
    }
    return result;
}

// FUNCTION: RALLYTROPHY 0x4811b0
void SingleRace::SetUnk844(int value)
{
    unk844 = value;
}

// FUNCTION: RALLYTROPHY 0x4811c0
// Assigns the start slots: by the previous result when there is one, else in order and then shuffled.
void SingleRace::SendGrid(int newRace)
{
    unsigned int i, j, k;
    if (!newRace && (m_players[0]->unk1a5c || m_players[0]->unk1a60)) {
        unsigned int count = m_players.size();
        if (m_players[0]->unk1a5c) {
            for (i = 0; i < m_players.size(); i++) {
                m_players[i]->startSlot = count - m_players[i]->unk1a5c + 1;
            }
        } else {
            for (i = 0; i < m_players.size(); i++) {
                m_players[i]->startSlot = count - m_players[i]->unk1a60 + 1;
            }
        }
        for (i = 0; i < m_players.size(); i++) {
            m_players[i]->unk1a70 = m_players[i]->startSlot;
        }
    } else {
        for (i = 0; i < m_players.size(); i++) {
            m_players[i]->startSlot = i + 1;
        }
        for (i = 0; i < m_players.size(); i++) {
            int saved = m_players[i]->startSlot;
            unsigned int j = rand() % m_players.size();
            if (j != i) {
                m_players[i]->startSlot = m_players[j]->startSlot;
                m_players[j]->startSlot = saved;
            }
        }
        for (i = 0; i < m_players.size(); i++) {
            m_players[i]->unk1a70 = m_players[i]->startSlot;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x480800
// SYMBOL: ??_GSingleRace@@UAEPAXI@Z
// SingleRace scalar deleting destructor
typedef void Generated_480800();

// Module end: the guard-bit initialiser of the STL header statics and its
// atexit registration, which VC6 emits for every file including <string>/<vector>.

// FUNCTION: RALLYTROPHY 0x481360
// SYMBOL: _$E23
typedef void Generated_481360();

// FUNCTION: RALLYTROPHY 0x481380
// SYMBOL: _$E22
typedef void Generated_481380();
