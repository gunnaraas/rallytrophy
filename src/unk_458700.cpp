// CarInput (0x458700-0x4591f4, stub 0x459200): the control state of the followed car and the force-feedback
// update. Map: analysis/range-4563e0.md section 3. Class: include/unk_458700.h.
#include <string>
#include "unk_495640.h"
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46a8a0.h"
#include <math.h>

extern int DAT_00501308;               // mirror enabled
extern int DAT_0050130c;
extern int DAT_00501310;               // HUD gauges enabled

// FUNCTION: RALLYTROPHY 0x458700
CarInput::CarInput(HWND window, void* unused)
{
    unk3c = 1.0f;
    handbrake = 0;
    brake = 0;
    throttle = 0;
    unk50 = 0;
    timerA = 0;
    timerB = -50;
    device = 0;
    unk0 = 1;
    effect6c = 0;
    effect70 = 0;
    effect74 = 0;
    effect78 = 0;
    if (DAT_0056b0b4) {
        DAT_0056b0b4->Refresh();
        if (DAT_00501408) {
            int saturation = 10000 - DAT_00501410 * 70;
            DAT_0056b0b4->SetForceGain(0x400, DAT_0050140c * 100);
            DAT_0056b0b4->SetSaturation(saturation);
            DAT_0056b0b4->SetDeadZone(DAT_00501414 * 25);
            effect70 = DAT_0056b0b4->CreateEffect(0x400, 4, 1);
            effect6c = DAT_0056b0b4->CreateEffect(0x400, 8, 1);
            effect74 = DAT_0056b0b4->CreateEffect(0x400, 0x40, 1);
            effect78 = DAT_0056b0b4->CreateEffect(0x400, 0x40, 1);
        }
    }
    Reset();
}

// FUNCTION: RALLYTROPHY 0x458820
CarInput::~CarInput()
{
    if (effect70) {
        effect70->Stop();
    }
    if (effect6c) {
        effect6c->Stop();
    }
    if (effect74) {
        effect74->Stop();
    }
    if (effect78) {
        effect78->Stop();
    }
    if (DAT_0056b0b4) {
        DAT_0056b0b4->Unacquire();
    }
    if (device) {
        delete device;
    }
}

// FUNCTION: RALLYTROPHY 0x458880
void CarInput::Reset()
{
    FUN_004588d0();
    FUN_00458940();
    steerAnalog = 0;
    throttleAnalog = 0;
    brakeAnalog = 0;
    button24 = 0;
    steerRight = 0;
    steerLeft = 0;
    handbrakeDigital = 0;
    brakeDigital = 0;
    throttleDigital = 0;
    handbrake = 0;
    brake = 0;
    throttle = 0;
    steering = 0;
    unk50 = 0;
    step = 0;
    shiftTimerUp = 0;
    shiftTimerDown = 0;
    timerA = 0;
    timerB = -50;
}

// FUNCTION: RALLYTROPHY 0x4588d0
void CarInput::FUN_004588d0()
{
    if (effect70) {
        effect70->SetParameters(0, 0, 0);
    }
    if (effect6c) {
        effect6c->SetParameters(0, 0, 0);
    }
    if (effect74) {
        effect74->SetParameters(0, 0, 0);
    }
    if (effect78) {
        effect78->SetParameters(0, 0, 0);
    }
    feedback = 0;
    timerA = 0;
    timerB = -50;
}

// FUNCTION: RALLYTROPHY 0x458940
void CarInput::FUN_00458940()
{
    button28 = 0;
    cameraButton = 0;
    resetButton = 0;
}

// FUNCTION: RALLYTROPHY 0x458950
void CarInput::SetSteering(float value)
{
    steerAnalog = 1;
    steering = value;
}

// FUNCTION: RALLYTROPHY 0x458970
void CarInput::SetThrottle(float value)
{
    throttleAnalog = 1;
    throttle = value;
}

// FUNCTION: RALLYTROPHY 0x458990
void CarInput::SetBrake(float value)
{
    brakeAnalog = 1;
    brake = value;
}

// FUNCTION: RALLYTROPHY 0x4589b0
void CarInput::FUN_004589b0(int pressed)
{
    steerAnalog = 0;
    steerLeft = pressed;
}

// FUNCTION: RALLYTROPHY 0x4589d0
void CarInput::FUN_004589d0(int pressed)
{
    steerAnalog = 0;
    steerRight = pressed;
}

// FUNCTION: RALLYTROPHY 0x4589f0
void CarInput::FUN_004589f0(int pressed)
{
    throttleAnalog = 0;
    throttleDigital = pressed;
}

// FUNCTION: RALLYTROPHY 0x458a10
void CarInput::FUN_00458a10(int pressed)
{
    brakeAnalog = 0;
    brakeDigital = pressed;
}

// FUNCTION: RALLYTROPHY 0x458a30
void CarInput::FUN_00458a30(int pressed)
{
    handbrakeDigital = pressed;
}

// FUNCTION: RALLYTROPHY 0x458a40
void CarInput::FUN_00458a40(int pressed)
{
    button24 = pressed;
}

// FUNCTION: RALLYTROPHY 0x458a50
void CarInput::FUN_00458a50()
{
    button28 = 1;
}

// FUNCTION: RALLYTROPHY 0x458a60
void CarInput::FUN_00458a60()
{
    cameraButton = 1;
}

// FUNCTION: RALLYTROPHY 0x458a70
void CarInput::FUN_00458a70()
{
    DAT_00501308 = !DAT_00501308;
}

// FUNCTION: RALLYTROPHY 0x458a90
void CarInput::FUN_00458a90()
{
    DAT_0050130c ^= 1;
}

// FUNCTION: RALLYTROPHY 0x458aa0
void CarInput::FUN_00458aa0()
{
    DAT_00501310 ^= 1;
}

// FUNCTION: RALLYTROPHY 0x458ab0
void CarInput::FUN_00458ab0()
{
    resetButton = 1;
}

// FUNCTION: RALLYTROPHY 0x458ac0
void CarInput::FUN_00458ac0(int delta)
{
    step += delta;
}

// FUNCTION: RALLYTROPHY 0x458ad0
// Replay: drop the pending requests and restart the effects when a device was reacquired.
void CarInput::FUN_00458ad0()
{
    step = 0;
    gearRequest = -10;
    if (DAT_0056b0b4 && DAT_0056b0b4->Poll(this, 1)) {
        if (effect70) {
            effect70->Start();
        }
        if (effect6c) {
            effect6c->Start();
        }
        if (effect74) {
            effect74->Start();
        }
        if (effect78) {
            effect78->Start();
        }
    }
}

// The Car as CarInput::FUN_00458b30 sees it (the real class is Car in unk_481390.h / CarObject).
struct IC_Body {
    BYTE unk0[0x12c];
    float mass;                        // 0x12c
    BYTE unk130[0x1cc - 0x130];
    float velocity[3];                 // 0x1cc
};

struct IC_Car {
    BYTE unk0[0x24];
    IC_Body* body;                     // 0x24
    BYTE unk28[0x2cc - 0x28];
    float f2cc;                        // 0x2cc
    BYTE unk2d0[0x338 - 0x2d0];
    float f338;                        // 0x338
    float f33c;                        // 0x33c
    BYTE unk340[0x5fc - 0x340];
    float f5fc;                        // 0x5fc
    BYTE unk600[0x668 - 0x600];
    float f668;                        // 0x668
    float f66c;                        // 0x66c
    BYTE unk670[0x998 - 0x670];
    float f998;                        // 0x998
    BYTE unk99c[0xcc8 - 0x99c];
    float fcc8;                        // 0xcc8
    BYTE unkccc[0xcf4 - 0xccc];
    float fcf4;                        // 0xcf4
    BYTE unkcf8[0xd00 - 0xcf8];
    float fd00;                        // 0xd00
    BYTE unkd04[0xd0c - 0xd04];
    float fd0c;                        // 0xd0c
    BYTE unkd10[0xd14 - 0xd10];
    float fd14;                        // 0xd14
    float fd18;                        // 0xd18
    BYTE unkd1c[0xd28 - 0xd1c];
    float fd28;                        // 0xd28
    BYTE unkd2c[0xd34 - 0xd2c];
    float fd34;                        // 0xd34
    BYTE unkd38[0xd40 - 0xd38];
    float fd40;                        // 0xd40
    BYTE unkd44[0xd48 - 0xd44];
    float fd48;                        // 0xd48
    float fd4c;                        // 0xd4c
    BYTE unkd50[0xdb8 - 0xd50];
    Drivetrain drive;                  // 0xdb8
    BYTE unkdb8end[0x1338 - 0xdb8 - sizeof(Drivetrain)];
    float throttle;                    // 0x1338
    BYTE unk133c[0x1340 - 0x133c];
    float handbrake;                   // 0x1340
    float steerDigital;                // 0x1344
    BYTE unk1348[0x134c - 0x1348];
    float steerAnalog;                 // 0x134c
    BYTE analog;                       // 0x1350
    BYTE unk1351[0x1c910 - 0x1351];
    float f1c910;
};

#define BITS(x) (*(int*)&(x))

// NONMATCHING: RALLYTROPHY 0x458b30
// Tried 2026-10-30: inline-asm fistp for the p2 blend (the original has it) moves size to 1364 but adds 5 differing bytes.
// Same code shape and constants (builds 1358 bytes, original 1393); the original keeps `car` in ebx and zero in edi and
// copies floats through the FPU (fld/fstp), which the source below does not reproduce.
void CarInput::FUN_00458b30(Car* carPtr, int dtMs, int flag)
{
register IC_Car* car = (IC_Car*)carPtr;
    float dt = dtMs * 0.001f;
    if (car->f1c910 > (feedback = feedback - dt * 0.7f)) {
        feedback = car->f1c910;
    }
    if (BITS(feedback) < 0) {
        feedback = 0;
    } else if (BITS(feedback) > 0x3f800000) {
        feedback = 1.0f;
    }
    step = 0;
    gearRequest = -10;
    if (DAT_0056b0b4) {
        bool fireA = false;
        bool fireB = false;
        int a = timerA - dtMs;
        int b = timerB - dtMs;
        timerA = a;
        timerB = b;
        if (a <= 0) {
            fireA = true;
            timerA = 100 - (-a) % 100;
        }
        if (b <= 0) {
            fireB = true;
            timerB = 100 - (-b) % 100;
        }
        if (fireA || fireB) {
            float tmp = (car->f668 + car->f338) * 0.3f + (car->fcc8 + car->f998) * 0.2f;
            tmp = tmp * 10000.0f;
            int w;
            __asm {
                fld tmp
                fistp w
            }
            if (w > 10000) {
                w = 10000;
            } else if (w < 0) {
                w = 0;
            }
            volatile float x = car->fd40;
            float lateral = (car->fd34 - x) * car->fd28;
            if (BITS(x) < BITS(car->fd48)) {
                lateral += (car->fd48 - x) * car->fd4c;
            }
            float y = car->fd0c;
            float lateral2 = (car->fd00 - y) * car->fcf4;
            if (BITS(y) < BITS(car->fd14)) {
                lateral2 += (car->fd14 - y) * car->fd18;
            }
            float d = lateral2 - lateral;
            float q = d / (car->body->mass * 2.4525f);
            float torque = q * q * 10000.0f;
            if (d < 0) {
                torque = -torque;
            }
            tmp = torque + (car->f5fc - car->f2cc) * 5000.0f;
            int t;
            __asm {
                fld tmp
                fistp t
            }
            if (t < -10000) {
                t = -10000;
            } else if (t > 10000) {
                t = 10000;
            }
            if (fireA && effect6c) {
                effect6c->SetParameters(t >> 1, w, w);
            }
            w >>= 1;
            if (fireB && effect70) {
                effect70->SetParameters(w, w, 0);
            }
            float suspension = (car->f66c + car->f33c) * 0.5f;
            float speed = ((Vec3*)car->body->velocity)->Length();
            float level;
            if (speed < 5.0f) {
                level = 0;
            } else if (speed < 20.0f) {
                level = suspension * (speed - 5.0f) * 0.06666667f;
            } else {
                level = suspension;
            }
            tmp = level * 10000.0f;
            int rumble;
            __asm {
                fld tmp
                fistp rumble
            }
            if (rumble < 0) {
                rumble = 0;
            } else if (rumble > 10000) {
                rumble = 10000;
            }
            int period = 0;
            if (rumble != 0) {
                float k = speed * 0.02857143f;
                if (k > 1.0f) {
                    k = 1.0f;
                }
                tmp = (0.2f - k * 0.175f) * 1000000.0f;
                __asm {
                    fld tmp
                    fistp period
                }
            }
            int p1 = rumble;
            int p2 = period;
            if (feedback > 0.001f) {
                float f1 = feedback * 25000.0f;
                float f2 = (feedback * 0.1f + 0.1) * 1000000.0f;
                if (f1 > 10000.0f) {
                    f1 = 10000.0f;
                }
                float wf = (float)rumble;
                p2 = (int)((period * wf + f2 * f1) * (1.0f / (wf + f1)));
                if (f1 > wf) {
                    p1 = (int)f1;
                }
            }
            if (fireA && effect74) {
                effect74->SetParameters(p1, p2, 0);
            }
            if (fireB && effect78) {
                effect78->SetParameters(rumble, period * 2, 0);
            }
        }
        if (DAT_0056b0b4->Poll(this, 1)) {
            if (effect70) {
                effect70->Start();
            }
            if (effect6c) {
                effect6c->Start();
            }
            if (effect74) {
                effect74->Start();
            }
            if (effect78) {
                effect78->Start();
            }
        }
    }
    if (!throttleAnalog) {
        if (throttleDigital) {
            throttle = 4.0f * dt + throttle;
        } else {
            throttle = throttle - 10.0f * dt;
        }
    }
    if (!brakeAnalog) {
        if (brakeDigital) {
            brake = 5.0f * dt + brake;
        } else {
            brake = brake - 5.0f * dt;
        }
    }
    if (handbrakeDigital) {
        handbrake = 2.0f * dt + handbrake;
    } else {
        handbrake = handbrake - 5.0f * dt;
    }
    if (BITS(throttle) < 0) {
        throttle = 0;
    } else if (BITS(throttle) > 0x3f800000) {
        throttle = 1.0f;
    }
    if (BITS(brake) < 0) {
        brake = 0;
    } else if (BITS(brake) > 0x3f800000) {
        brake = 1.0f;
    }
    if (BITS(handbrake) < 0) {
        handbrake = 0;
    } else if (BITS(handbrake) > 0x3f800000) {
        handbrake = 1.0f;
    }
    if (steerAnalog) {
        car->steerAnalog = steering;
        car->analog = 1;
    } else {
        float s = 0;
        if (steerLeft) {
            s = -1.0f;
        }
        if (steerRight) {
            s += 1.0f;
        }
        car->steerDigital = s;
        car->analog = 0;
    }
    car->throttle = throttle;
    ((Car*)car)->FUN_0045da60(brake);
    car->handbrake = handbrake;
    if (flag == 0) {
        if (button24) {
            FUN_004590d0(carPtr, dt);
        } else {
            FUN_004591c0(carPtr, dt);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4590b0
void Car::FUN_004590b0()
{
    IC_Car* car = (IC_Car*)this;
    car->drive.ShiftTo(car->drive.SelectGear());
}

// FUNCTION: RALLYTROPHY 0x4590d0
void CarInput::FUN_004590d0(Car* carPtr, float dt)
{
    IC_Car* car = (IC_Car*)carPtr;
    shiftTimerUp = shiftTimerUp - dt;
    shiftTimerDown = shiftTimerDown - dt;
    if (button24) {
        Vec3* velocity = (Vec3*)car->body->velocity;
        if (velocity->x * velocity->x + velocity->y * velocity->y + velocity->z * velocity->z < 100.0f) {
            car->drive.ShiftTo(-1);
            shiftTimerUp = 0;
            shiftTimerDown = 0;
        }
    }
    if (!button24) {
        int wanted = car->drive.gear;
        int gear = car->drive.SelectGear();
        if (gear != wanted) {
            if (gear > wanted) {
                if (shiftTimerUp <= 0) {
                    car->drive.ShiftTo(gear);
                    shiftTimerDown = 1.4f;
                }
            } else {
                if (shiftTimerDown <= 0) {
                    car->drive.ShiftTo(gear);
                    shiftTimerUp = 1.4f;
                }
            }
        }
    }
    if (*(unsigned*)&shiftTimerUp & 0x80000000) {
        shiftTimerUp = 0;
    }
    if (*(unsigned*)&shiftTimerDown & 0x80000000) {
        shiftTimerDown = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x4591c0
void CarInput::FUN_004591c0(Car* carPtr, float dt)
{
    IC_Car* car = (IC_Car*)carPtr;
    if (step != 0) {
        car->drive.ShiftBy(step);
    }
    if (gearRequest != -10) {
        car->drive.ShiftTo(gearRequest);
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x459200
// SYMBOL: _$E23
typedef void Generated_459200();

// FUNCTION: RALLYTROPHY 0x459220
// SYMBOL: _$E22
typedef void Generated_459220();
