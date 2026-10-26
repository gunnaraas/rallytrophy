/* Drivetrain (0x46a8a0-0x46bb5f): engine, clutch, gearbox and differential of a Car, embedded
 * at Car+0xdb8 (0x1e8 bytes, no vtable). Map: analysis/range-46a8a0.md section 2.
 * Include after unk_481390.h (Vec3) and rallytrophy.h (Config). */
#ifndef UNK_46A8A0_H
#define UNK_46A8A0_H

// One wheel of a Car as the drivetrain sees it: Car+0x28+n*0x330 (the CarWheel of unk_481390.h
// is the tail of this object, starting at +0x298).
struct DriveWheel {
    BYTE unk0[0x264];
    float unk264;                      // 0x264: Car+0x28c for wheel 0 (radius, read by SelectGear)
    BYTE unk268[0x270 - 0x268];
    float unk270;                      // 0x270: Car+0x298 for wheel 0 (read by IntegrateEngineSpeed)
    BYTE unk274[0x284 - 0x274];
    float spin;                        // 0x284: spin speed
    BYTE unk288[0x32c - 0x288];
    float driveShare;                  // 0x32c: share of the drive torque (DistributeDifferential)
};

// The Car as the drivetrain sees it (view; the real class is Car in unk_481390.h).
struct DriveCar {
    BYTE unk0[0x24];
    struct CarBody* body;              // 0x24
    DriveWheel wheel[4];               // 0x28, stride 0x330
    BYTE unkce8[0x1c74c - 0xce8];
    float damage[3];                   // 0x1c74c: engine / engine torque (+0x1c750) / drive torque (+0x1c754) wear
    float shiftLimit;                  // 0x1c758: compared with 0.9 by ShiftBy
};

// One point of the engine torque curve.
struct TorquePoint {
    float rpm;
    float torque;
};

class Drivetrain {
public:
    Drivetrain();                                  // 0x46a8a0
    ~Drivetrain();                                 // 0x46a900
    void LoadConfig(const char* path, unsigned int checksum); // 0x46a920: car engine ini
    void Reset();                                  // 0x46adc0: defaults
    void ResetState();                             // 0x46ae30
    float TorqueAtRpm(float rpm);                  // 0x46ae70
    float GetDriveTorque();                        // 0x46afb0
    float GetReactionTorque();                     // 0x46aff0
    int GetEngagedGear();                          // 0x46b000
    int SelectGear();                              // 0x46b020
    float GetRpmFraction();                        // 0x46b1a0
    float GetShiftProgress();                      // 0x46b1d0
    void SetThrottle(float throttle);              // 0x46b250
    void ShiftBy(int delta);                       // 0x46b280
    void ShiftTo(int gear);                        // 0x46b360
    void ComputeEngineTorque();                    // 0x46b3f0
    void DistributeDifferential(int driveType);    // 0x46b4b0
    void Update(int driveType, float dt);          // 0x46b5b0
    float IntegrateEngineSpeed(float dt, float wheelSpeed); // 0x46b940
    float GetSmoothedRpm();                        // 0x46ba60
    void SetGearing(int setting);                  // 0x46baa0

    DriveCar* car;                     // 0x00: owner
    float engineFriction;              // 0x04
    float differentialBias;            // 0x08
    float differentialLock;            // 0x0c
    float inertiaEngine;               // 0x10
    float inertiaDriveShaft;           // 0x14
    int numGears;                      // 0x18
    float gearInertia[8];              // 0x1c: indexed by gear + 1
    float ratio[8];                    // 0x3c: effective ratio by gear + 1 (SetGearing)
    Vec3 rawRatio[8];                  // 0x5c: ini ratios by gear + 1
    float shiftUp[8];                  // 0xbc: by gear + 1
    float shiftDown[8];                // 0xdc: by gear + 1
    float endRatio;                    // 0xfc
    int gearing;                       // 0x100: SetGearing setting
    float idleRpm;                     // 0x104
    float torqueMax;                   // 0x108
    TorquePoint curve[20];             // 0x10c
    int curvePoints;                   // 0x1ac
    float clutchReleaseTime;           // 0x1b0
    float clutchEngageTime;            // 0x1b4
    float gearShiftTime;               // 0x1b8
    float exhaustPeakRpm;              // 0x1bc
    float throttle;                    // 0x1c0
    float throttleEff;                 // 0x1c4
    float clutch;                      // 0x1c8
    float engineTorque;                // 0x1cc
    float engineSpeed;                 // 0x1d0
    int gear;                          // 0x1d4: -1 reverse, 0 neutral
    float reactionTorque;              // 0x1d8
    int targetGear;                    // 0x1dc
    int shiftState;                    // 0x1e0: 0 idle, 1 clutch releasing, 2 changing, 3 engaging
    float shiftTimer;                  // 0x1e4
};                                     // 0x1e8
typedef char Drivetrain_size_check[sizeof(Drivetrain) == 0x1e8 ? 1 : -1];
typedef char DriveCar_size_check[sizeof(DriveCar) == 0x1c75c ? 1 : -1];

#endif
