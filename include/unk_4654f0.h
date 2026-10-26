/* Replay and network state glue of the Car (0x4654f0-0x465e5f, stub 0x465e60).
 * Map: analysis/range-4649f0.md section 2. Include after unk_481390.h and rallytrophy.h.
 * The Car members of this module (FUN_004654f0 SetReplayMode, FUN_00465530 LoadReplay, FUN_00465550 SaveReplay,
 * GetState, FUN_004658a0 SetState, GetState2, FUN_00465d80 SetState2) are declared in unk_481390.h; the fields are
 * read through CarStateView, a raw view of the Car (Car itself is not restructured). */
#ifndef UNK_4654F0_H
#define UNK_4654F0_H

#pragma pack(push, 1)
// One network/replay sample of a car (0x57 bytes, unaligned dwords inside). Layout from the stores of
// Car::GetState; PlayerSample (unk_481390.h) is the same packet seen from the player side.
struct CarStateData {
    int time;                          // 0x00: written by the caller
    BYTE throttle;                     // 0x04: Car+0x1338 * 255
    BYTE brake;                        // 0x05: Car+0x133c * 255
    BYTE clutch;                       // 0x06: Car+0x1340 * 255
    BYTE handbrake;                    // 0x07: Car+0xf80 * 255
    BYTE steer;                        // 0x08: Car+0x134c * 127 + 128
    BYTE gear;                         // 0x09: Car+0xf8c
    unsigned short rpm;                // 0x0a: Car+0xf88
    short steerRate;                   // 0x0c: change of the 8-entry steering ring
    int body[13];                      // 0x0e: body +0x14c..+0x158, +0x14..+0x1c, +0x19c..+0x1b0
    BYTE compression[4];               // 0x42: wheel travel * 255
    short spin[4];                     // 0x46: wheel +0x284 * 255
    short load[4];                     // 0x4e: Car+0xd10 + n*0x34 * 255
    BYTE damage;                       // 0x56: Car+0x209c * 255 when Car+0x20a4
};
// The wear packet (0x14 bytes).
struct CarWearData {
    int time;                          // 0x00
    BYTE a[4];                         // 0x04: Car+0x2c8 + n*0x330
    BYTE b[4];                         // 0x08: Car+0x1c73c + n*4
    BYTE c[4];                         // 0x0c: Car+0x2cc + n*0x330
    BYTE damage[4];                    // 0x10: Car+0x1c750, 0x1c74c, 0x1c754, 0x1c758
};
#pragma pack(pop)

// The replay/ghost recorder embedded in the Car at +0x1bed0 (methods are in the 0x4013xx range).
class ReplayRecorder {
public:
    void Start(int mode);              // 0x401380
    void Stop();                       // 0x4013b0
    void Save(const char* name);       // 0x401920
    void Load(const char* name);       // 0x401a40
    BYTE data[0x2c];
};

// The part of the car body (Car+0x24) the state packet copies.
struct StateBody {
    BYTE unk0[0x14];
    float position[3];                 // 0x14
    float matrix[16];                  // 0x20 (rotation rows 0x10 apart, translation unused here)
    BYTE unk60[0x14c - 0x60];
    int unk14c, unk150, unk154, unk158; // 0x14c
    BYTE unk15c[0x19c - 0x15c];
    int vel[6];                        // 0x19c
    void FUN_0041c390();               // 0x41c390: refresh the derived body state after SetState
};

// A wheel (Car+0x28 + n*0x330) as the state code sees it (the Wheel class is in unk_46c2c0.h).
struct StateWheel {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14: relative to the body; pos[1] is the rest height
    BYTE unk20[0x128 - 0x20];
    float topY;                        // 0x128: upper end of the wheel travel
    BYTE unk12c[0x284 - 0x12c];
    float spin;                        // 0x284
    BYTE unk288[0x298 - 0x288];
    int active;                        // 0x298
    BYTE unk29c[0x2a0 - 0x29c];
    float wearA;                       // 0x2a0
    float wearC;                       // 0x2a4
    float GetWearA() { return wearA; }
    float GetWearC() { return wearC; }
    void SetWearA(float f) { wearA = f; }
    void SetWearC(float f) { wearC = f; }
    BYTE unk2a8[0x2b0 - 0x2a8];
    void* surface;                     // 0x2b0: tyre record of the surface under the wheel
    float normal[3];                   // 0x2b4
    float params[2];                   // 0x2c0
    BYTE unk2c8[0x328 - 0x2c8];
    float friction;                    // 0x328
    BYTE unk32c[4];
};

// Terrain query object at Car+0x1464.
class StateQuery {
public:
    int FUN_00419770(float height, float* pos, float* normal, float* depth, float* point, int* surface); // 0x419770
};

// The terrain surface arrays of the stage (Car+0x14fc).
struct StateStage {
    BYTE unk0[0x24c];
    int tyre[1];                       // 0x24c: tyre record index per surface
    BYTE unk250[0x38c - 0x250];
    float normal[1][2];                // 0x38c
    BYTE unk394[0x47c - 0x394];
    float friction[1];                 // 0x47c
};

// One suspension record (Car+0xd00 + n*0x34).
struct StateSpring {
    float unk0, unk4;
    float length;                      // 0x08: divisor of the compression
    float travel;                      // 0x0c: topY - restY
    float load;                        // 0x10
    BYTE unk14[0x34 - 0x14];
};

// The Car as the state code sees it (view; the real class is Car in unk_481390.h).
struct CarStateView {
    BYTE unk0[0x24];
    StateBody* body;                   // 0x24
    StateWheel wheel[4];               // 0x28
    BYTE unkce8[0xd00 - 0xce8];
    StateSpring spring[4];             // 0xd00
    BYTE unkdd0[0xf80 - 0xdd0];
    float handbrake;                   // 0xf80
    BYTE unkf84[4];
    float rpm;                         // 0xf88
    int gear;                          // 0xf8c
    BYTE unkf90[0x10f8 - 0xf90];
    float rideA;                       // 0x10f8
    float rideB;                       // 0x10fc
    BYTE unk1100[0x1338 - 0x1100];
    float throttle;                    // 0x1338
    float brake;                       // 0x133c
    float clutch;                      // 0x1340
    BYTE unk1344[0x134c - 0x1344];
    float steer;                       // 0x134c
    BYTE steerSet;                     // 0x1350
    BYTE unk1351[3];
    float steerRing[8];                // 0x1354
    int steerHead;                     // 0x1374
    BYTE unk1378[0x1464 - 0x1378];
    StateQuery query;                  // 0x1464
    BYTE unk1465[0x14fc - 0x1465];
    StateStage* stage;                 // 0x14fc
    BYTE unk1500[0x1680 - 0x1500];
    BYTE tyre[30][0x40];               // 0x1680: tyre force records per surface
    BYTE unk1e00[0x1e04 - 0x1e00];
    float hitNormal[4][3];             // 0x1e04
    BYTE unk1e34[0x1e44 - 0x1e34];
    int surface[4];                    // 0x1e44
    float hitPoint[4][3];              // 0x1e54
    float hitDepth[4][3];              // 0x1e84
    BYTE unk1eb4[0x209c - 0x1eb4];
    float damage;                      // 0x209c
    float damageOut;                   // 0x20a0
    int damageSet;                     // 0x20a4
    BYTE unk20a8[0x1c73c - 0x20a8];
    float wear[4];                     // 0x1c73c: second byte group of the wear packet
    float wearEngine[4];               // 0x1c74c: engine / engine torque / drive torque wear, shift limit
};

// The Car as the replay glue sees it.
struct CarReplayView {
    BYTE unk0[0x1bed0];
    ReplayRecorder recorder;           // 0x1bed0
    int mode;                          // 0x1befc: 0 = off, 1 = record, 2 = play
};

#endif
