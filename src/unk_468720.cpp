// Car race state (0x468720-0x468c3f, stub 0x468c10): stuck detection, respawn, checkpoint and lap
// tracking, tyre-track switch. Map: analysis/range-4666a0.md section 3.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_4741a0.h"
#include "unk_468720.h"
#include <math.h>

// FUNCTION: RALLYTROPHY 0x468720
void Car::FUN_00468720(float dt)
{
    RaceCar* v = (RaceCar*)this;
    int touching = 0;
    int lt5 = 0;
    int lt15 = 0;
    int ground = 0;
    int soft = 0;
    for (int i = 0; i < 4; i++) {
        if (v->wheels[i].active) {
            touching++;
            int s = v->wheelSurface[i];
            if (s < 5)
                lt5++;
            if (s < 15)
                lt15++;
            int c = ((CarSub*)v->stage)->surface[s];
            if (c) {
                ground++;
                if (c > 1)
                    soft++;
            }
        }
    }
    if (ground > 0) {
        v->stuckState = 4;
    } else if (touching == 0 && v->unk142c == 0) {
        v->stuckState = 1;
    } else {
        Vec3* vel = &v->body->velocity;
        if (vel->x * vel->x + vel->y * vel->y + vel->z * vel->z < 1.0f) {
            if (v->unk142c && lt15 < 4) {
                v->stuckState = 2;
            } else if (lt5 < 4) {
                v->stuckState = 3;
            } else {
                goto check;
            }
        } else {
            v->stuckState = 0;
            v->stuckTime = 0;
            return;
        }
    }
    v->stuckTime = dt + v->stuckTime;
check:
    int st = v->stuckState;
    if ((st == 1 && v->stuckTime > DAT_00501518) || (st == 2 && v->stuckTime > DAT_0050151c)
        || (st == 3 && v->stuckTime > DAT_00501520)
        || (st == 4 && (v->stuckTime > DAT_00501524 || (soft && v->stuckTime > DAT_00501528))))
        DAT_00501450->Slot13((Player*)v->player);
}

// FUNCTION: RALLYTROPHY 0x4688f0
void Car::FUN_004688f0(int mode)
{
    RaceCar* v = (RaceCar*)this;
    Checkpoint* cp;
    if (mode < 0) {
        int index = v->checkpoint - mode;
        if (index < 0)
            index = 0;
        cp = ((Stage*)v->stage)->FUN_00414fb0(index);
        if (!cp)
            cp = ((Stage*)v->stage)->FUN_00414dc0(&v->body->position, mode);
    } else {
        cp = ((Stage*)v->stage)->FUN_00414fb0(v->checkpoint);
        if (!cp)
            cp = ((Stage*)v->stage)->FUN_00414dc0(&v->body->position, mode);
    }
    FUN_0045cc30((StartPosition*)&cp->position, cp);
    if (v->view)
        v->view->FUN_004566e0();
    v->unk1f2bc++;
}

// FUNCTION: RALLYTROPHY 0x468980
void Car::FUN_00468980(int lap)
{
    RaceCar* v = (RaceCar*)this;
    Checkpoint* cp = ((Stage*)v->stage)->FUN_00414fb0(lap);
    FUN_0045cc30((StartPosition*)&cp->position, cp);
}

// NONMATCHING: RALLYTROPHY 0x4689b0
// Right algorithm and operand order; the compiler keeps this/player/passed in registers where the
// original reloads them after the times[] store, and the d/r vector FPU residency differs.
void Car::FUN_004689b0(int ms)
{
    RaceCar* v = (RaceCar*)this;
    int sections = v->stage->sections;
    int total = DAT_00501468 * sections;
    int passed = v->passed;
    if (passed < total) {
        RaceCheckpoint* cp = &v->stage->checkpoints[passed % sections];
        Vec3 d = v->body->position;
        d -= cp->centre;
        if (d.x * d.x + d.y * d.y + d.z * d.z <= cp->radiusSq) {
            Vec3 r = v->body->position;
            r -= cp->origin;
            float a = r.x * cp->normalA.x + r.y * cp->normalA.y + r.z * cp->normalA.z;
            if (a >= 0 && a <= cp->length) {
                float b = d.x * cp->normalB.x + d.y * cp->normalB.y + d.z * cp->normalB.z;
                if (b >= 0 && b <= 5.0f) {
                    passed++;
                    v->passed = passed;
                    if (v->player) {
                        float speed = (float)sqrt(v->speedSq) * 3.6f;
                        if (speed > v->player->maxSpeed)
                            v->player->maxSpeed = speed;
                        int t = ms + v->unk1f148;
                        t -= t % 10;
                        if (passed > v->player->last) {
                            if (passed % sections == 0) {
                                int n = v->player->sections;
                                int e = t;
                                if (n > 0)
                                    e = t - v->player->times[n * sections];
                                v->player->sectionTimes[n] = e;
                                v->player->sections++;
                            }
                            v->player->times[passed] = t;
                            v->player->last = v->passed;
                        }
                        if (v->passed >= total) {
                            int n = v->stage->stageCount;
                            v->player->finished = 1;
                            v->player->stageTimes[n] = t;
                            v->player->stagesDone = n;
                        }
                    }
                }
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x468bd0
void Car::FUN_00468bd0()
{
    RaceCar* v = (RaceCar*)this;
    v->tracks = 1;
    SkidMarks* trail = v->trails();
    for (int i = 0; i < 4; i++) {
        trail->Expire();
        trail++;
    }
}

// FUNCTION: RALLYTROPHY 0x468c00
void Car::FUN_00468c00()
{
    RaceCar* v = (RaceCar*)this;
    v->tracks = 0;
    v->unk1f114 = 0;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x468c10
// SYMBOL: _$E23
typedef void Generated_468c10();

// FUNCTION: RALLYTROPHY 0x468c30
// SYMBOL: _$E22
typedef void Generated_468c30();
