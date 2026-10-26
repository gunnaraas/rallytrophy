/* Car race state (0x468720-0x468c3f): stuck detection, respawn, checkpoint/lap tracking and the
 * tyre-track switch. Map: analysis/range-4666a0.md section 3.
 * Include after unk_481390.h and unk_4741a0.h. The Car members are written against RaceCar, a raw
 * view of the Car fields these functions touch (Car itself is not restructured). */
#ifndef UNK_468720_H
#define UNK_468720_H

// Stuck-detection thresholds in seconds (floats in .data).
extern float DAT_00501518;             // 5.0: state 1 (no wheel touches the ground, not airborne)
extern float DAT_0050151c;             // 1.0: state 2
extern float DAT_00501520;             // 2.0: state 3
extern float DAT_00501524;             // 2.0: state 4 (a wheel sits on a stuck-surface)
extern float DAT_00501528;             // 0.1: state 4 with a blocking surface

// A checkpoint of the stage (0x48 bytes at Stage+0xa0c); the Checkpoint of unk_481390.h shows only
// its tail. A pass needs the car inside the sphere (centre, radius^2) and between the two planes.
struct RaceCheckpoint {
    Vec3 origin;                       // 0x00: plane origin
    BYTE unkc[0x18 - 0xc];
    Vec3 centre;                       // 0x18: sphere centre
    Vec3 normalB;                      // 0x24: second plane normal (distance limited to 5.0)
    Vec3 normalA;                      // 0x30: first plane normal (distance limited by length)
    float length;                      // 0x3c
    BYTE unk40[4];
    float radiusSq;                    // 0x44
};

// The stage as UpdateCheckpoints sees it.
struct RaceStage {
    BYTE unk0[4];
    int stageCount;                    // 0x04
    BYTE unk8[0xa0c - 8];
    RaceCheckpoint checkpoints[1];     // 0xa0c (stride 0x48)
    BYTE unka54[0x130c - 0xa54];
    int sections;                      // 0x130c: checkpoints per stage section
};

// Player fields UpdateCheckpoints writes (Player of unk_481390.h; this is the 0x19f0 array it lacks).
struct RacePlayer {
    BYTE unk0[0x30];
    int finished;                      // 0x30
    BYTE unk34[0x1884 - 0x34];
    int times[(0x19f0 - 0x1884) / 4];  // 0x1884: pass time per checkpoint
    int sectionTimes[(0x1a18 - 0x19f0) / 4]; // 0x19f0
    int last;                          // 0x1a18: checkpoints passed
    int sections;                      // 0x1a1c
    float maxSpeed;                    // 0x1a20: km/h
    int stageTimes[1];                 // 0x1a24
    BYTE unk1a28[0x1a50 - 0x1a28];
    int stagesDone;                    // 0x1a50
};

// The car body (Car+0x24): position and velocity.
struct RaceBody {
    BYTE unk0[0x14];
    Vec3 position;                     // 0x14
    BYTE unk20[0x1cc - 0x20];
    Vec3 velocity;                     // 0x1cc
};

struct RaceCar {
    BYTE unk0[0x20];
    class ViewManager* view;           // 0x20
    struct RaceBody* body;             // 0x24
    BYTE unk28[0x2c0 - 0x28];
    struct CarWheel wheels[4];         // 0x2c0
    BYTE unkf80[0x142c - 0xf80];
    int unk142c;                       // 0x142c
    BYTE unk1430[0x14fc - 0x1430];
    struct RaceStage* stage;           // 0x14fc
    BYTE unk1500[0x1e44 - 0x1500];
    int wheelSurface[4];               // 0x1e44
    BYTE unk1e54[0x1becc - 0x1e54];
    struct RacePlayer* player;         // 0x1becc
    BYTE unk1bed0[0x1f114 - 0x1bed0];
    int unk1f114;                      // 0x1f114
    int stuckState;                    // 0x1f118: 0 none, 1..4 see UpdateStuck
    float stuckTime;                   // 0x1f11c
    int checkpoint;                    // 0x1f120
    BYTE unk1f124[0x1f13c - 0x1f124];
    float speedSq;                     // 0x1f13c
    BYTE unk1f140[0x1f144 - 0x1f140];
    int passed;                        // 0x1f144: checkpoints passed
    int unk1f148;                      // 0x1f148
    BYTE unk1f14c[0x1f158 - 0x1f14c];
    BYTE tracks;                       // 0x1f158: tyre tracks enabled
    BYTE unk1f159[0x1f2bc - 0x1f159];
    int unk1f2bc;                      // 0x1f2bc: respawn counter
    // The four tyre-track objects of the wheels (stride 0x6558) are at 0x2748.
    class SkidMarks* trails() { return (class SkidMarks*)((char*)this + 0x2748); }
};

#endif
