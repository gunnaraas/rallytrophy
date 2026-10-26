/* CarInput (0x458700-0x4591f4): the control state of the followed car (0x7c bytes), fed by the game
 * controllers (T_56b0b4::Poll, include/unk_495640.h calls it `Controls`) and read by the race session.
 * Map: analysis/range-4563e0.md section 3. Included by unk_481390.h and unk_495640.h; the force-feedback
 * effects are only declared (ForceEffect is defined in unk_495640.h). */
#ifndef UNK_458700_H
#define UNK_458700_H

#include <windows.h>

class Car;
class ForceEffect;
class T_56b0b4;

class CarInput {
public:
    CarInput(HWND window, void* unused); // 0x458700 (ret 8)
    ~CarInput();                       // 0x458820
    void Reset();                      // 0x458880
    void FUN_004588d0();               // 0x4588d0: StopEffects
    void FUN_00458940();               // 0x458940: ClearOneShots
    // Set by T_56b0b4::Poll (this class is its `Controls`).
    void SetSteering(float value);     // 0x458950: analog
    void SetThrottle(float value);     // 0x458970: analog
    void SetBrake(float value);        // 0x458990: analog
    void FUN_004589b0(int pressed);    // 0x4589b0: steer left (digital)
    void FUN_004589d0(int pressed);    // 0x4589d0: steer right (digital)
    void FUN_004589f0(int pressed);    // 0x4589f0: throttle (digital)
    void FUN_00458a10(int pressed);    // 0x458a10: brake (digital)
    void FUN_00458a30(int pressed);    // 0x458a30: handbrake
    void FUN_00458a40(int pressed);    // 0x458a40
    void FUN_00458a50();               // 0x458a50: one-shot flag 0x28
    void FUN_00458a60();               // 0x458a60: camera button
    void FUN_00458a70();               // 0x458a70: toggles DAT_00501308 (mirror)
    void FUN_00458a90();               // 0x458a90: toggles DAT_0050130c
    void FUN_00458aa0();               // 0x458aa0: toggles DAT_00501310 (HUD gauges)
    void FUN_00458ab0();               // 0x458ab0: reset button
    void FUN_00458ac0(int step);       // 0x458ac0: +1 / -1
    void FUN_00458ad0();               // 0x458ad0 (replay)
    void FUN_00458b30(Car* car, int dt, int flag); // 0x458b30
    void FUN_004590d0(Car* car, float dt); // 0x4590d0: gear shifts of the human path
    void FUN_004591c0(Car* car, float dt); // 0x4591c0: gear shifts requested by the replay
    int unk0;                          // 0x00: 1 after the constructor
    int steerAnalog;                   // 0x04: 1 = analog (SetSteering), 0 = digital
    int throttleAnalog;                // 0x08
    int brakeAnalog;                   // 0x0c
    int brakeDigital;                  // 0x10
    int throttleDigital;               // 0x14
    int steerLeft;                     // 0x18
    int steerRight;                    // 0x1c
    int handbrakeDigital;              // 0x20
    int button24;                      // 0x24: second button (also: the gear-shift path is skipped when 0)
    int button28;                      // 0x28: one-shot
    int cameraButton;                  // 0x2c
    int resetButton;                   // 0x30
    int gearRequest;                   // 0x34: -10 = none
    int step;                          // 0x38: followed-player step requested this frame
    float unk3c;                       // 0x3c: 1.0
    float throttle;                    // 0x40
    float brake;                       // 0x44
    float handbrake;                   // 0x48
    float steering;                    // 0x4c
    int unk50;                         // 0x50
    float shiftTimerUp;                // 0x54
    float shiftTimerDown;              // 0x58
    int timerA;                        // 0x5c: force-feedback timers (ms), 100 ms period
    int timerB;                        // 0x60: starts at -50
    T_56b0b4* device;                  // 0x64: deleted by the destructor (never assigned here)
    float feedback;                    // 0x68: force-feedback intensity 0..1
    ForceEffect* effect6c;             // 0x6c
    ForceEffect* effect70;             // 0x70
    ForceEffect* effect74;             // 0x74
    ForceEffect* effect78;             // 0x78
};

#endif
