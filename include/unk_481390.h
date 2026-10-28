/* Network race mode (0x481390-0x4843df): the race-mode class used for
 * network games (vtable 0x4e2440). It derives from the race-mode base class
 * of the module at 0x485bb0 (ctor) / 0x485da0 (dtor), whose layout is only
 * known as far as this module uses it. Only src/unk_481390.cpp includes
 * this header. */
#ifndef UNK_481390_H
#define UNK_481390_H

#include <windows.h>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <string.h>

// A car's rigid body (only the vectors this module passes on).
struct CarBody {
    BYTE unk0[0x14];
    float unk14[3];                    // 0x14
    float unk20[3];                    // 0x20
    BYTE unk2c[0x40 - 0x2c];
    float forward[3];                  // 0x40
    BYTE unk4c[0x60 - 0x4c];
    float matrix[16];                  // 0x60: 4x4 world matrix (row vectors, translation in row 3)
    float unka0[3];                    // 0xa0
    float unkac;                       // 0xac
    float unkb0;                       // 0xb0
    float unkb4;                       // 0xb4
    float unkb8[3];                    // 0xb8
    BYTE unkc4[0x1cc - 0xc4];
    float velocity[3];                 // 0x1cc
    class Vec3 FUN_004204f0(const class Vec3& point); // 0x4204f0: world point -> body frame (out of line, hidden return pointer)
};
int FUN_004ad1d0(float* a0, float* b0, float* a1, float* b1, float* a2, float* b2); // boxes overlap

class Stage;
class File;
struct CarSetup {                      // the first 0x14 bytes of a Car
    int value[5];
};
class Player;
class PlayerSample;
class AiPath;
class Vec3;
struct RotMatrix;                      // 4x4 rotation about an axis (ctor 0x4b5760)
class Vec3Slot {
public:
    Vec3Slot() {}
    ~Vec3Slot() {}
    float Spline(float p0, float p1, float p2, float p3); // 0x4a7f30: Catmull-Rom, x,y,z hold t,t^2,t^3
    float x;
    float y;
    float z;
};

// One tracked obstacle (another car) of the AI driver, 0xbc bytes (ctor 0x49b960, dtor 0x49b9b0;
// both compiler generated: the unwind tables list Vec3Slot members at 0x00, 0x28, 0x34, 0x40, 0x4c). See analysis/range-49b630.md section 2.
struct Obstacle {
    Vec3Slot unk0;                     // 0x00
    BYTE unkc[0x14 - 0x0c];
    int valid;                         // 0x14: 1 = tracked this tick (FUN_0049ffa0)
    BYTE unk18[0x1c - 0x18];
    int leftDone;                      // 0x1c: left-hand pass already steered around (0x4a0420); cleared by FUN_0049ffa0
    int rightDone;                     // 0x20: right-hand pass already steered around
    int side;                          // 0x24: 1 = in path, 2 = left, 3 = right of the lateral axis
    Vec3Slot unk28, unk34, unk40, unk4c; // 0x28, 0x34, 0x40, 0x4c
    BYTE unk58[0x5c - 0x58];
    Vec3Slot corners[8];               // 0x5c
};
// The computer-driver of a Player (formerly PlayerSound), 0x574 bytes, owned by Player::ai (0x1a80).
// Members of Vec3 type carry an empty ctor/dtor; their offsets come from the EH unwind tables.
class AIDriver {
public:
    AIDriver(Player* player);       // 0x49b8e0
    void FUN_0049c020(float dt, int local);
    void FUN_0049ba70();            // Reset
    void FUN_0049f790(int stage);   // SetSkill
    ~AIDriver();                    // 0x49ba00
    void AddListener(Player* player) { listeners.push_back(player); }
    void Init();                    // 0x49bb80: defaults, then data/extras/ai_<car>.ini
    void FUN_0049bf50();            // per-tick clear
    void FUN_0049bfa0();            // heading from the car body
    // Path tracking and small helpers (0x49c020-0x49f7d0). docs: analysis/range-49b630.md.
    float FUN_0049c100();
    void FUN_0049d480(float steer);
    void FUN_0049d4b0();
    float FUN_0049d510(float steer);
    void FUN_0049d560(int full);
    float FUN_0049d720(Vec3* point, class AiSegment* segment);
    float FUN_0049d950(float unused);
    void FUN_0049f1e0();
    int FUN_0049f1f0(Player* other);
    void FUN_0049f400();
    void FUN_0049f560(float dt);
    void FUN_0049f7d0();
    // Defined in the other parts of the AI driver module.
    float FUN_0049c130();
    void FUN_0049d060();
    float FUN_0049d970();
    float FUN_0049e3d0();
    void FUN_0049fee0();
    // Opponent scan (0x49fee0-0x4a14af).
    void FUN_0049ffa0();
    void FUN_004a0420();
    float FUN_004a0e60(Obstacle* ob, Vec3* unused);
    float FUN_004a1050(Obstacle* ob, Vec3* dir, int side);
    void FUN_004a1390(Obstacle* ob, struct AiBody* src);
    int unk0;
    std::vector<Player*> listeners;    // 0x04
    Player* player;                    // 0x14
    float unk18;                       // 0x18: signed distance to the left edge (FUN_0049c130)
    float unk1c;                       // 0x1c: signed distance to the right edge
    float unk20;                       // 0x20
    float unk24;                       // 0x24: node-interpolated value
    float unk28;                       // 0x28
    BYTE unk2c[0x30 - 0x2c];
    float unk30;                       // 0x30
    float unk34;                       // 0x34: steering offset
    BYTE unk38[0x3c - 0x38];
    Vec3Slot unk3c, unk48, unk54, unk60, unk6c; // 0x3c..0x78
    float unk78;                       // 0x78
    float unk7c;                       // 0x7c: interpolated offset (FUN_0049c130)
    float unk80;                       // 0x80: interpolated path width
    int unk84;                         // 0x84
    float unk88;                       // 0x88: -1000.0 after a reset
    BYTE unk8c[0x90 - 0x8c];
    float unk90;                       // 0x90
    float unk94, unk98, unk9c, unka0;  // 0x94: cleared every tick
    BYTE unka4[0xa8 - 0xa4];
    float throttle;                    // 0xa8
    float brake;                       // 0xac
    int unkb0;                         // 0xb0
    float unkb4, unkb8;                // 0xb4
    int unkbc, unkc0;                  // 0xbc
    int unkc4;                         // 0xc4
    Vec3Slot unkc8, unkd4;             // 0xc8, 0xd4
    Vec3Slot forward;                  // 0xe0: normalised heading of the car (y = 0)
    int unkec;                         // 0xec
    int unkf0;                         // 0xf0
    int unkf4;                         // 0xf4
    BYTE unkf8[0xfc - 0xf8];
    int unkfc;                         // 0xfc
    int unk100;                        // 0x100
    int unk104;                        // 0x104: opponent index of the scan
    int unk108;                        // 0x108: counter
    int unk10c;                        // 0x10c
    float unk110;                      // 0x110
    int unk114;                        // 0x114
    float unk118;                      // 0x118: dt of the last update
    float unk11c;                      // 0x11c
    float unk120;                      // 0x120
    float unk124, unk128, unk12c, unk130; // 0x124: steering wander
    Vec3Slot unk134, unk140, unk14c;   // 0x134, 0x140, 0x14c
    int unk158;                        // 0x158: stuck state 1..4 (FUN_0049f560)
    float unk15c;                      // 0x15c: time in the stuck state (ms)
    float unk160;                      // 0x160: stuck cooldown (ms)
    int unk164;                        // 0x164
    int unk168;                        // 0x168
    int unk16c;                        // 0x16c
    int unk170;                        // 0x170
    int unk174;                        // 0x174
    int unk178;                        // 0x178
    int unk17c;                        // 0x17c
    void* unk180;                      // 0x180: current AiSegment
    float tuning[6];                   // 0x184: BrakingEnd, BrakingHardness, BrakingStart, OversteeringStrength, SlowDownHardness, SlowDownStart
    float skill[3];                    // 0x19c: set by SetSkill
    Obstacle obstacles[5];             // 0x1a8
    int unk554;                        // 0x554
    int unk558;                        // 0x558
    BYTE unk55c[0x560 - 0x55c];
    int unk560;                        // 0x560
    float unk564, unk568;              // 0x564
    float unk56c;                      // 0x56c
    BYTE unk570[0x574 - 0x570];        // object size 0x574
};
// One wheel of a car (0x330 bytes; Car+0x2c0).
struct CarWheel {
    int active;                        // 0x00
    int touching;                      // 0x04
    BYTE unk8[0x68 - 0x8];
    float value;                       // 0x68
    BYTE unk6c[0x330 - 0x6c];
};
// A car (0x465xxx module); only the fields this module reads.
class Car {
public:
    Car(void* a, int b, int c, int d, int e, int f, int g); // 0x45e480
    void FUN_00464800();
    void FUN_0045da60(float a);
    ~Car();                            // 0x461a40
    void FUN_004654f0(int mode);
    void FUN_00469e20();
    void FUN_00468980(int lap);
    void GetState(struct CarState* state);    // 0x465570
    void GetState2(struct CarState2* state);  // 0x465c40
    void FUN_0048e660();               // empty (folded)
    void FUN_0045a850();               // 0x45a850
    void FUN_0045b2c0(void* camera, int time, int dt);
    void FUN_0045b900(int dt);
    int FUN_00458590(Car* other);     // collide with another car
    void FUN_00458690();
    void FUN_0045b390();
    void FUN_004688f0(int mode);
    void FUN_004693a0(int mode);       // car setup passes (slots 18/22)
    void FUN_004695c0(float fade);
    void FUN_00468c70(int a);
    void FUN_00469630(int a, int b, int c, Car* viewer); // slot 19
    void FUN_004697c0(int a, int b);   // slots 20/23
    void FUN_00469a00(int a, int b);   // slots 21/24
    void FUN_00464530(Stage* stage, int b);
    struct CarSnapshot* FUN_0046bc90(int a);
    void FUN_0046be50(int a);
    void FUN_00468c40(int a);
    void FUN_004670c0();
    void FUN_004672c0();
    void FUN_00467440(int a);
    void FUN_0045cc30(struct StartPosition* start, void* world);
    void FUN_0045cc30(const class Vec3& start, struct IdentityMatrix* world); // same function, start given as a temporary Vec3
    void FUN_00464c10(class ViewManager* view);
    void FUN_00464ef0(float dt);
    void FUN_0046a200();
    void FUN_0046a850();
    void FUN_00465530(const char* name);   // load the replay file
    void FUN_00465550(const char* name);   // save the replay file
    void FUN_00465e90(int value);
    void FUN_00466500(int value);
    void FUN_00466520(int value);
    void FUN_00466590(int value);
    void FUN_00466610(int value);
    void FUN_00468bd0();               // 0x468bd0
    void FUN_004658a0(PlayerSample* sample); // 0x4658a0: apply a network sample
    void FUN_00468c00();               // 0x468c00
    void FUN_00465d80(CarSetup* setup); // 0x465d80
    void FUN_0045a890();               // 0x45a890

    union {
        CarSetup setup;                // 0x00: five setup values (replay header, message 1/3)
        struct {
            int unk0;
            int unk4;
            int unk8;
            int unkc;
            int unk10;
        };
    };
    BYTE unk14[0x18 - 0x14];
    float unk18;                       // 0x18: steering range (FUN_0049d510)
    float unk1c;                       // 0x1c
    BYTE unk20[0x24 - 0x20];
    struct CarBody* body;              // 0x24
    BYTE unk28[0x2c0 - 0x28];
    struct CarWheel wheels[4];         // 0x2c0 (stride 0x330)
    float unkf80;                      // 0xf80
    BYTE unkf84[0xff4 - 0xf84];
    int unkff4;                        // 0xff4: viewport values copied to DAT_00508d94..
    int unkff8;                        // 0xff8
    int unkffc;                        // 0xffc
    int unk1000;                       // 0x1000
    int unk1004;                       // 0x1004
    int unk1008;                       // 0x1008
    BYTE unk100c[0x1334 - 0x100c];     // 0x100c: passed to ViewManager::FUN_00458150
    int unk1334;                       // 0x1334
    float unk1338;                     // 0x1338
    BYTE unk133c[0x1340 - 0x133c];
    float unk1340;                     // 0x1340
    BYTE unk1344[0x134c - 0x1344];
    int unk134c;                       // 0x134c
    BYTE unk1350;                      // 0x1350
    BYTE unk1351[0x142c - 0x1351];
    int unk142c;                       // 0x142c
    BYTE unk1430[0x14f8 - 0x1430];
    int unk14f8;                       // 0x14f8
    struct CarSub* sub;                // 0x14fc
    BYTE unk1500[0x1e44 - 0x1500];
    int wheelSurface[4];               // 0x1e44: surface index under each wheel
    BYTE unk1e54[0x1becc - 0x1e54];
    Player* player;                    // 0x1becc
    BYTE unk1bed0[0x1bef4 - 0x1bed0];
    int unk1bef4;                      // 0x1bef4
    BYTE unk1bef8[0x1c907 - 0x1bef8];
    BYTE unk1c907;                     // 0x1c907: set when the car has left the stage/finished (read by the session loop)
    int unk1c908;                      // 0x1c908: message id shown to the player
    int unk1c90c;                      // 0x1c90c: race time of that message
    BYTE unk1c910[0x1f120 - 0x1c910];
    int unk1f120;                      // 0x1f120
    BYTE unk1f124[0x1f138 - 0x1f124];
    int unk1f138;                      // 0x1f138
    int unk1f13c;
    float unk1f140;                    // 0x1f140: distance driven
    BYTE unk1f144[0x1f30c - 0x1f144];  // object size 0x1f30c

    // Work group W1 (F1 sounds, unk_4666a0.cpp); the fields are read through CarSoundView.
    void InitSounds(const std::string& dir); // 0x4666a0: load <dir>Sounds.ini and set up the sound layers
    void ReleaseSounds();              // 0x466fe0
    void UpdateSurfaceSound();         // 0x468550
    // Work group S4 (src/unk_468720.cpp, include/unk_468720.h)
    void FUN_00468720(float dt);       // 0x468720: stuck detection
    void FUN_004689b0(int ms);         // 0x4689b0: checkpoint / lap tracking
    // work group S5 (unk_468c40.cpp, model parts)
    void FUN_004691e0(int cockpit);    // 0x4691e0
    // Work group S6/S8 (0x469630-0x46a89f): render transforms, lighting, shadow, surface mix, gauges.
    void FUN_004699d0(int a, int b);   // 0x4699d0
    void FUN_00469ea0(float k);        // 0x469ea0
    // Work group S7 (unk_469630.cpp): particle emission and tyre tracks.
    void EmitEffects(float a, float b, float c); // 0x469a20
    void UpdateTyreTracks();           // 0x46a000
    // Work group X2 (unk_4654f0.cpp, replay and network state glue)
    void FUN_004581f0();               // 0x4581f0: called by SetState after the wheels are placed
    // Work group WG2 (unk_45e480.cpp, car.ini loader)
    void LoadCarIni(const char* path, unsigned int checksum); // 0x462350
    // Work group WG3 (unk_45e480.cpp, tire.ini loader and wiring helpers)
    void LoadTiresIni(const char* path, unsigned int checksum); // 0x4634e0
    void SetLightIntensity(float level); // 0x4644b0
    void ResetControls();              // 0x464680
    void AttachLights();               // 0x4646c0
    void SetObjectField(int value);    // 0x464940
    // Work group WG4 (src/unk_45e480.cpp, car textures)
    void LoadTextures(const std::string& dir); // 0x463940: flare sprites, light/window/overlay textures
    // Work group W1 (src/unk_459890.cpp, damage helpers; FUN_0045a850 = RepairAll, FUN_0045a890 = RepairBody)
    void AddContact(class Vec3* a, class Vec3* b, float strength); // 0x459890
    void ApplyWheel(int n);            // 0x45a7a0
    void RebuildLights();              // 0x45a980
    void ResetZone1();                 // 0x45ab30
    void ResetZone2();                 // 0x45ab40
    void ResetZone3();                 // 0x45ab50
    void ResetBrakeDamage();           // 0x45ab60
    void ResetWheelDamage();           // 0x45ab90
    void ResetSteeringDamage();        // 0x45abc0
    float GetBrakeDamage();            // 0x45abf0
    float GetWheelDamage();            // 0x45ac40
    float GetSteeringDamage();         // 0x45ac90
    // Work group W2 (src/unk_459890.cpp, car damage and contacts)
    void ProcessContacts();            // 0x459910: apply the buffered contact records to the damage state
    // Work group W8 (src/unk_45dac0.cpp, src/unk_45de90.cpp)
    float SunFactor(void* models);     // 0x45dac0: shade of the ground under the car (SunModels of include/unk_45dac0.h)
    void UpdateLights();               // 0x45de90
    // Work groups W3-W7 (src/unk_45ad10.cpp, car update and physics step)
    void UpdateSteering(float dt);     // 0x45ad40: steering smoothing, wheel orientation matrices
    void PhysicsStep(float dt);        // 0x45bc20
    void ApplySuspensionForces();      // 0x45c400
    void UpdateWheelContact();         // 0x45c700
    void SetupBrakes();                // 0x45cb80
    void FUN_004590b0();               // 0x4590b0: replay hook
    void FUN_00452b20();               // 0x452b20
    void FUN_00452eb0(float dt);       // 0x452eb0
    void FUN_00453940(float k);        // 0x453940
    // Work group F/H/I (src/unk_452b20.cpp, Car animation and driver models)
    void FUN_00454d10();               // 0x454d10: tilt of the driver and co-driver parts from the acceleration
    void LoadDriver(const char* dir);  // 0x455070
    // Work group G7 (src/unk_459230.cpp, environment map)
    void Draw459270(class ViewManager* view, class DrawContext* context); // 0x459270
};
struct CarSub {                        // object at Car+0x14fc
    BYTE unk0[0x33c];
    int surface[1];                    // 0x33c: per surface index (read by FUN_0049f560)
    BYTE unk340[0x1368 - 0x340];
    class AiPath* aiPath;              // 0x1368
};

// A 3D vector with an (empty) destructor, which gives its users an EH frame.
class Vec3 {
public:
    Vec3() {}
    Vec3(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    Vec3(const Vec3& o) { x = o.x; y = o.y; z = o.z; }
    ~Vec3() {}
    float Length() const { return (float)sqrt(x * x + y * y + z * z); }
    Vec3 operator+(const Vec3& o) const; // 0x419f00 (out of line, hidden return pointer)
    Vec3 operator*(const struct RotMatrix& m) const; // 0x4b5850 (out of line, returns through a hidden pointer)
    Vec3 operator-(const Vec3& o) const; // 0x414330 (out of line, returns through a hidden pointer)
    Vec3 operator*(float k) const;     // 0x40b0c0 (out of line)
    Vec3 operator-() const;            // 0x4203b0 (out of line, negated copy through a hidden pointer)
    Vec3& operator+=(const Vec3& o)
    {
        x += o.x;
        y += o.y;
        z += o.z;
        return *this;
    }
    Vec3& operator-=(const Vec3& o)
    {
        x -= o.x;
        y -= o.y;
        z -= o.z;
        return *this;
    }
    float x, y, z;
};

class AiNode;
class AiSegment;

// A car body (Car+0x24) as the AI driver's opponent scan reads it.
struct AiBody {
    BYTE unk0[0x14];
    Vec3 position;                     // 0x14
    float matrix[16];                  // 0x20: rotation rows 0x20/0x30/0x40 (forward = 0x40), translation 0x50
    BYTE unk60[0xa0 - 0x60];
    float boxMin[3];                   // 0xa0
    float boxMax[3];                   // 0xac
    BYTE unkb8[0x1cc - 0xb8];
    float vx, vy, vz;                  // 0x1cc
    Vec3& Forward() { return *(Vec3*)&matrix[8]; }
};

// The same body seen with its position as a Vec3.
struct CarBodyV {
    BYTE unk0[0x14];
    Vec3 position;                     // 0x14
};

// An object with a position at 0x14 (the camera/viewpoint passed to the car setup).
struct ViewPoint {
    BYTE unk0[0x14];
    Vec3 position;                     // 0x14
};

// The stage being raced (another module).
struct StartPosition {                 // 12 bytes
    float position[3];
};

// A checkpoint of the stage (only the position).
struct Checkpoint {
    BYTE unk0[0x30];
    Vec3 position;                     // 0x30
    int unk3c;
    float distance;                    // 0x40: distance along the stage
    int index;                         // 0x44
};

class Stage {
public:
    Checkpoint* FUN_00414fb0(int index);
    Stage(int a, int b, int c, int d);  // 0x40c9b0
    ~Stage();                          // 0x40ebc0
    void FUN_00411320(int a);
    void FUN_00411260();
    void FUN_004112a0(class RaceMode* race);
    void FUN_00416ec0(void* data);     // 0x416ec0
    void FUN_00416ef0(int mode);       // 0x416ef0
    void FUN_00417540(class Player* player, int time, int dt); // 0x417540
    Checkpoint* FUN_00414dc0(Vec3* position, int flag); // nearest checkpoint
    void FUN_004112e0(int dt);
    void FUN_004160c0(int dt);         // 0x4160c0: advance the stage by dt ms
    void FUN_00416010();               // 0x416010: begin the race loop
    void FUN_00416050();               // 0x416050: end the race loop
    void FUN_00411170(class ViewManager* view, class DrawContext* context); // 0x411170
    void FUN_00411350(class RaceDeviceSlots* device, class ViewManager* view); // 0x411350
    void FUN_00411460(class RaceDeviceSlots* device, class ViewManager* view, class DrawContext* context, float dt); // 0x411460
    void FUN_00411490(class RaceDeviceSlots* device, class ViewManager* view, class DrawContext* context, float dt); // 0x411490
    int unk0;
    int stageCount;                    // 0x04
    BYTE unk08[0x1a4 - 0x08];
    int unk1a4;                        // 0x1a4: copied to the draw context (+0x184) when DAT_00501434
    int unk1a8;                        // 0x1a8 .. 0x1b4: copied to the draw context (+0x174..+0x180)
    int unk1ac;
    int unk1b0;
    int unk1b4;
    BYTE unk1b8[0x1c4 - 0x1b8];
    int unk1c4;                        // 0x1c4 .. 0x1cc: copied to the draw context (+0x188..+0x190)
    int unk1c8;
    int unk1cc;
    BYTE unk1d0[0xa04 - 0x1d0];
    int unka04;                        // 0xa04
    BYTE unka08[0x130c - 0xa08];
    int unk130c;                       // 0x130c: splits per section
    StartPosition starts[8];           // 0x1310
    BYTE unk1370[0x24d8 - 0x1370];
    float unk24d8[2];                  // 0x24d8: speech thresholds (seconds? scaled by 1000 against the split time)
    BYTE unk24e0[0x24e4 - 0x24e0];
    int unk24e4[10];                   // 0x24e4
    int unk250c[10];                   // 0x250c
};

// An ARGB colour with an (empty) destructor; its unwind entry calls the
// folded empty function at 0x402c90.
class Color {
public:
    Color() {}
    Color(DWORD argb) { value = argb; }
    ~Color() {}
    union {
        DWORD value;
        struct {
            BYTE b, g, r, a;
        };
    };
};

// An ARGB colour passed by value without a destructor (no EH state at the
// call, unlike Color).
class ColorArg {
public:
    ColorArg(DWORD argb) { value = argb; }
    ColorArg(const ColorArg& o) { value = o.value; }
    ~ColorArg() {}
    DWORD value;
};

// A 2D point with empty out-of-line constructor/destructor (arrays of it
// go through the EH vector constructor iterator; ctor folds to 0x490ad0,
// dtor to 0x402c90).
// A 3D point with empty out-of-line constructor/destructor (arrays of it go
// through the EH vector constructor iterator, like Vec2).

class Vec2 {
public:
    Vec2() {}
    ~Vec2() {}
    Vec2(const Vec2& o) { x = o.x; y = o.y; }
    Vec2(float px, float py) { x = px; y = py; }
    Vec2 operator+(const Vec2& o) const; // 0x476490 (out of line, hidden return pointer)
    Vec2 operator-(const Vec2& o) const; // 0x46fdf0 (out of line, hidden return pointer)
    Vec2 operator*(float s) const;       // 0x46fe20 (out of line)
    Vec2 operator/(float s) const;       // 0x46fe50 (out of line)
    float x;
    float y;
};

// A HUD rectangle in 1024x768 screen units (floats at 0x565470 and 0x565460).

// A by-value screen coordinate: a float wrapper with an (empty) destructor,
// so the callee registers it for unwinding (0x48e4c0).
class Coord {
public:
    Coord(float v_) { v = v_; }
    ~Coord() {}
    operator float() const { return v; }
    float v;
};

inline void SetQuad(Vec2* v, float left, float top, float right, float bottom)
{
    v[0].x = v[3].x = left;
    v[0].y = v[1].y = top;
    v[1].x = v[2].x = right;
    v[2].y = v[3].y = bottom;
}

class SessionObject;
// The renderer's vtable slots this module uses (module-local view).
#define SLOTS(n) virtual void slot##n();
// The 4-int by-value argument of the renderer's slot 22 (0x58): a class with an empty
// destructor, so the caller registers it for unwinding.
class RectArg {
public:
    RectArg(int a, int b, int c, int d)
    {
        v[0] = a;
        v[1] = b;
        v[2] = c;
        v[3] = d;
    }
    ~RectArg() {}
    int v[4];
};
class RaceDeviceSlots {
public:
    SLOTS(0)
    SLOTS(1)
    SLOTS(2)
    SLOTS(3)
    virtual void Slot4();  // 0x10
    SLOTS(5)
    SLOTS(6)
    SLOTS(7)
    SLOTS(8)
    SLOTS(9)
    SLOTS(10)
    SLOTS(11)
    SLOTS(12)
    virtual void Present(); // 0x34
    SLOTS(14)
    SLOTS(15)
    SLOTS(16)
    SLOTS(17)
    virtual SessionObject* GrabFrame(); // 0x48: the back buffer as an image (0 if unavailable)
    virtual void ReleaseFrame(); // 0x4c
    SLOTS(20)
    SLOTS(21)
    virtual void SetViewport(class RectArg rect); // 0x58
    virtual void Slot23(float value); // 0x5c
    virtual void Slot24(int value); // 0x60
    SLOTS(25)
    SLOTS(26)
    virtual int BeginScene(); // 0x6c
    virtual void EndScene(); // 0x70
    SLOTS(29)
    SLOTS(30)
    SLOTS(31)
    virtual void Slot32(int mode); // 0x80
    virtual void SetRenderState(int state, int value); // 0x84
    SLOTS(34)
    virtual void SetTexture(void* texture); // 0x8c
    SLOTS(36)
    SLOTS(37)
    virtual void Slot38(int value); // 0x98
    SLOTS(39)
    SLOTS(40)
    virtual void Slot41(int mode, float a, float b); // 0xa4
    virtual void Slot42(int value); // 0xa8
    SLOTS(43)
    virtual void Slot44(int value); // 0xb0
    SLOTS(45)
    SLOTS(46)
    SLOTS(47)
    virtual void SetMatrix(float* matrix, int flag); // 0xc0
    SLOTS(49)
    SLOTS(50)
    virtual void SetClear(float z, float one, float x, float y, int flag); // 0xcc
    SLOTS(52)
    SLOTS(53)
    SLOTS(54)
    SLOTS(55)
    SLOTS(56)
    SLOTS(57)
    SLOTS(58)
    SLOTS(59)
    SLOTS(60)
    SLOTS(61)
    SLOTS(62)
    SLOTS(63)
    SLOTS(64)
    SLOTS(65)
    virtual void DrawQuad(Vec2* position, Vec2* uv, Color color, float sx, float sy); // 0x108
    virtual void DrawRect(Vec2* position, Color color, float sx, float sy); // 0x10c: untextured quad
};
#undef SLOTS

inline void SetRect(Vec2* v, float x, float y, float w, float h)
{
    SetQuad(v, x, y, x + w, y + h);
}

// A texture loaded from a file (0x68 bytes; 0x4a8030 module).
class Texture {
public:
    Texture(const char* file, int format, int mipmaps); // 0x4a8030
    Texture(const char* name);         // 0x4a7fd0: empty texture with a name (ParallelShadow)
    virtual ~Texture();
    void Upload(class RenderDevice* device); // 0x4a8350
    BYTE unk04[0x68 - 4];
};

// A 2D sprite/material (0xd4 bytes; ctor 0x4a85e0, dtor 0x4a86d0).
class Sprite {
public:
    Sprite();                          // 0x4a8580
    Sprite(const char* name);
    ~Sprite();
    Texture* texture;                  // 0x00
    BYTE unk04[0x0c];
    int unk10;                         // 0x10
    int unk14;                         // 0x14
    int unk18;                         // 0x18
    int unk1c;                         // 0x1c
    int unk20;
    int unk24;                         // 0x24
    int unk28;                         // 0x28
    int unk2c;
    int unk30;                         // 0x30
    BYTE unk34[4];
    int unk38;                         // 0x38
    int unk3c;                         // 0x3c
    float unk40[4];                    // 0x40
    float unk50[4];                    // 0x50
    float unk60[4];                    // 0x60
    float unk70[4];                    // 0x70
    BYTE unk80[4];
    int unk84;                         // 0x84
    BYTE unk88[0x10];
    int unk98;                         // 0x98
    int unk9c;                         // 0x9c
    int unka0;                         // 0xa0
    BYTE unka4[0xd4 - 0xa4];
};

// The game's text renderer (another module).
class TextRenderer {
public:
    float GetWidth(const char* text, int flags); // 0x4aa430
    void SetScaleX(float x) { scaleX = x; }
    void SetScale(float x, float y)
    {
        scaleX = x;
        scaleY = y;
    }
    void DrawNameTag(class RenderDevice* device, struct ViewPoint* view, class Vec3Slot* position, class Player* player, float scale); // 0x4a9b00
    void Printf(class RenderDevice* device, float x, float y, const char* format, ...); // 0x4a9740 (cdecl)
    void Print(class RenderDevice* device, float x, float y, const char* text, int flags, float sx, float sy); // 0x4a97a0
    void SetColor(ColorArg color);     // 0x4243b0: stores the argb at +0x120 (ret 4)
    BYTE unk0[0x1c];
    float scaleX;                      // 0x1c
    float scaleY;                      // 0x20
    BYTE unk24[8];
    int lineHeight;                    // 0x2c
    BYTE unk30[0x120 - 0x30];
    DWORD color;                       // 0x120
};

// The global object at DAT_0056b0c8 (another module); only its text renderer.
// A sound channel of the game object (0x150 bytes each).
class GameSound {
public:
    void FUN_0049b730();
    BYTE unk0[0x150];
};
class RecordTable;
class StageTable;
class GameView {
public:
    void* FUN_00432a00();              // returns the field at 0x1390d4
    int FUN_00437220(int a);           // 0x437220: confirmation dialog, -1 while open
    void FUN_00432b30(int a, int b);   // 0x432b30
    void FUN_00432b00();               // 0x432b00
    void FUN_004309b0();
    void FUN_00432c00();               // 0x432c00: per-frame update
    int FUN_00452380(int rank, Player* player); // 0x452380: stores the stage result; bit 0/1 = new record
    int FUN_00452470(int rank, Player* player); // 0x452470
    void ShowCountryVideo(int n);      // 0x4526e0: plays data/video/country<n>.bik with captions
    BYTE unk0[0x3aec];
    BYTE stageTable[0x5c30 - 0x3aec];  // 0x3aec: StageTable
    BYTE records[0x139074 - 0x5c30];   // 0x5c30: RecordTable
    RecordTable* remoteRecords;        // 0x139074 (when DAT_00501471)
    BYTE unk139078[0x1390b8 - 0x139078];
    void FUN_004360c0(float x, float y, float w, float h, int id); // 0x4360c0: draws a car icon
    TextRenderer* bigText;             // 0x1390b8
    BYTE unk1390bc[8];
    TextRenderer* smallText;           // 0x1390c4
    TextRenderer* text;                // 0x1390c8
    BYTE unk1390cc[0x139224 - 0x1390cc];
    GameSound sounds[2];               // 0x139224
};
extern GameView* DAT_0056b0c8;
// An entry of the records table at DAT_0056b0c8 + 0x5c30 (another module).
struct RecordEntry {
    char name[0x1e];                   // 0x00
    BYTE unk1e;
    char unk1f;                        // 0x1f: car class
    BYTE unk20[0x2c - 0x20];
    int splits[11];                    // 0x2c: cumulative stage times, [0] = start, [10] = total
    int time;                          // 0x58
};
// A high-score / record entry as the results code builds it (0x60 bytes).
struct ScoreRecord {
    char name[0x1e];                   // 0x00
    BYTE unk1e;                        // 0x1e: player+0x20
    char unk1f;                        // 0x1f: player+0x2c
    SYSTEMTIME date;                   // 0x20
    int splits[10];                    // 0x30
    int time;                          // 0x58
    float unk5c;                       // 0x5c
    void SetUnk5c(float value) { unk5c = value; }
};
// Highscore tables (0x133441 bytes, embedded in the game object at +0x5c30). Records are laid out as
// [group (rally class) + stage][car class 0..22][rank 0..9]; see analysis/range-48fe20.md.
class RecordTable {
public:
    RecordTable();                     // 0x48fe20
    ~RecordTable();                    // 0x48e660 (empty, folded; called out of line)
    void FUN_0048fe40();               // 0x48fe40: fill every record with its default
    RecordEntry* FUN_0048ffd0(int a, int b, int c, int d);   // 0x48ffd0: record (group a, stage b, rank c, class d) + 3
    RecordEntry* FUN_00490010(int a, int b, int c, int d);   // 0x490010: the same without the +3
    int FUN_00490050(ScoreRecord* record, int a, int b);   // insert by the +0x54 time
    int FUN_004901b0(ScoreRecord* record, int a, int b);   // insert by the +0x58 time
    int FUN_00490310(ScoreRecord* record, int a, int mode);
    void FUN_00490470(const char* file);   // load
    void FUN_00490820(const char* file);   // save when dirty

    BYTE records[0x133440];            // 0x00: 23 * 10 records per (group, stage) list
    BYTE dirty;                        // 0x133440
    BYTE pad133441[3];                 // operator new size is 0x133444 (Game ctor 0x4306bf)
};
class StageTable {
public:
    int FUN_00443620(int rally);       // reference time of a rally stage
    int FUN_00443580();
};
extern class NetRace* DAT_00501450;    // the network race mode

// One network sample of a remote car (0x5c bytes; the first dword is its
// timestamp). Arrays of it go through the EH vector constructor iterator; the
// empty ctor folds to 0x490ad0 and the empty dtor to 0x402c90.
class PlayerSample {
public:
    PlayerSample() {}
    ~PlayerSample() {}
    int time;                          // 0x00
    BYTE value[5];                     // 0x04: u8 car state fields (the last one is biased by 128)
    BYTE unk9[3];
    short unkc;                        // 0x0c
    BYTE unke[0x57 - 0xe];
    BYTE unk57;                        // 0x57
    int unk58;                         // 0x58
};

// The two 32-sample rings of a Player (at Player+0x5c, 0x176c bytes);
// docs: analysis/range-48fe20.md.
class SampleBuffer {
public:
    void Reset();                      // 0x490950: write the "empty" sentinels
    int count;                         // 0x00
    int time;                          // 0x04: latest time (Reset: -5000)
    PlayerSample received[32];               // 0x08
    int unkb88;                        // 0xb88: time of the last position fix applied
    CarSetup unkb8c;                   // 0xb8c: that fix (value[0] = time)
    int unkba0;                        // 0xba0: samples in pending
    PlayerSample pending[32];                // 0xba4
    CarSetup unk1724;                  // 0x1724: newest position fix received (value[0] = time)
    int unk1738;                       // 0x1738: time of the sample interpolated towards
    int unk173c;                       // 0x173c: time of the sample interpolated from
    struct {
        float slope;
        float offset;
    } line[5];                         // 0x1740: slope (per ms) and offset of each interpolated field
    BYTE unk1768;                      // 0x1768: interpolating (1) or extrapolating (0)
};

// 0x491c10 (cdecl): derive the interpolation slopes between two samples.
void FUN_00491c10(SampleBuffer* buffer, PlayerSample* from, PlayerSample* to, int copyFrom);

// A race participant (0x1a88 bytes; ctors 0x4909b0, 0x490ae0, 0x490b90,
// 0x490c30, dtor 0x490ee0). Only the fields this module touches are named.
enum PlayerKind { PLAYER_AI };
class Player {
public:
    Player(const WCHAR* name, DWORD id, int host, int local); // 0x490ae0
    Player(int id, int remote);        // 0x4909b0
    Player(int unused);                // 0x490b90: state 1
    Player(PlayerKind kind, int index); // 0x490c30: AI player from data/settings/cpuNN.ini
    ~Player();                         // 0x490ee0
    void Init();                       // 0x490fe0
    void FUN_004913e0();
    void FUN_00491410();
    void FUN_00490fb0(const char* name);
    void FUN_00491110(int retire, int time);
    void FUN_00491e20(Stage* stage);   // 0x491e20
    void FUN_00491df0(float dt, int local); // forwards to AIDriver::FUN_0049c020
    void FUN_00491350();
    void FUN_004912a0();
    void FUN_00491370();
    void FUN_00491430();
    void FUN_00491450();
    class AiPath* FUN_00491e10();
    void FUN_00491620(const void* packet); // 0x491620: queue a received sample
    void FUN_004916a0(int time);
    void FUN_00491950(int time);
    void FUN_00491130();
    int FUN_00491f70();
    void Lock();                       // 0x491470
    void Unlock();                     // 0x491480
    int FUN_00491490(const void* data, int size); // handles one message, returns its size

    char name[0x20];                   // 0x00
    int unk20;                         // 0x20
    int unk24;                         // 0x24
    Car* car;                          // 0x28
    int unk2c;                         // 0x2c
    int unk30;                         // 0x30
    int unk34;                         // 0x34: is the session host
    int local;                         // 0x38
    DWORD id;                          // 0x3c: DPNID
    int unk40;                         // 0x40: round state: 3 = not racing, 2 = finished
    int unk44;                         // 0x44
    int unk48;                         // 0x48
    int unk4c;                         // 0x4c
    int unk50;                         // 0x50: start slots this player cannot see
    int ping;                          // 0x54: ping class shown in the standings (0..2)
    int state;                         // 0x58
    SampleBuffer samples;              // 0x5c (.time = 0x60: time of the last state received)
    CRITICAL_SECTION section;          // 0x17c8: guards the sample buffers
    union {
        struct {
            int unk17e0;               // 0x17e0
            int unk17e4;               // 0x17e4
        };
        BYTE appearance[8];            // 0x17e0: [2..7] = car appearance bytes
    };
    BYTE unk17e8;                      // 0x17e8
    BYTE unk17e9[3];
    int unk17ec;                       // 0x17ec
    int ring[8];                       // 0x17f0: frame deltas
    int unk1810;                       // 0x1810
    int unk1814;                       // 0x1814
    int unk1818;                       // 0x1818
    int unk181c;                       // 0x181c: clock offset to the host
    int unk1820;                       // 0x1820: clock offset reported by the player
    int active;                        // 0x1824
    int unk1828;                       // 0x1828
    int unk182c;                       // 0x182c: 3 = chat line pending, 15 = kicked?
    int unk1830;                       // 0x1830
    int unk1834;                       // 0x1834
    BYTE unk1838;
    char unk1839[0x4f - 6];            // 0x1839: chat text
    BYTE unk1882[0x1884 - 0x1882];
    int unk1884[(0x1a14 - 0x1884) / 4]; // 0x1884
    int unk1a14;                       // 0x1a14: best lap time (0 = none)
    int unk1a18;                       // 0x1a18: splits passed
    int unk1a1c;                       // 0x1a1c
    float unk1a20;                     // 0x1a20
    int unk1a24;                       // 0x1a24: stage results, indexed by unk1a50
    int unk1a28[10];                   // 0x1a28
    int unk1a50;                       // 0x1a50: entries used in unk1a28
    int unk1a54;                       // 0x1a54
    int unk1a58;                       // 0x1a58: place (1-based) in the sorted standings
    int unk1a5c;                       // 0x1a5c: place (1-based) in the sorted standings
    int unk1a60;                       // 0x1a60: championship rank
    int unk1a64;                       // 0x1a64
    int unk1a68;                       // 0x1a68
    int startSlot;                     // 0x1a6c: index into Stage::starts
    int unk1a70;                       // 0x1a70
    int unk1a74;                       // 0x1a74
    BYTE unk1a78[0x1a7c - 0x1a78];     // 0x1a78
    int unk1a7c;                       // 0x1a7c: -1 after a restart
    class AIDriver* ai;             // 0x1a80: AI driver (computer players)
    float* aiTable;                    // 0x1a84: 50 floats of AI skill data (object size 0x1a88)
};

// std::vector wrappers whose size() and erase() the original calls out of
// line (0x414460, 0x484380) from an inlined resize(0); the members are
// defined out of line in unk_481390.cpp under auto_inline(off).
class PlayerVector : public std::vector<Player*> {
public:
    void Resize(unsigned int n, Player* const& x = (Player*)0)
    {
        if (n && Size() < n) {
            insert(end(), n - Size(), x);
        } else if (n < Size()) {
            Erase(begin() + n, end());
        }
    }
    unsigned int Size() const;
    iterator Erase(iterator first, iterator last);
    void Destroy(iterator first, iterator last);
    // resize(n) with size() inline: the default argument gives the zeroed stack slot before the erase.
    void ResizeA(unsigned int n, Player* const& x = (Player*)0)   // erase out of line (0x484380)
    {
        if (size() < n) {
            insert(end(), n - size(), x);
        } else if (n < size()) {
            Erase(begin() + n, end());
        }
    }
    iterator EraseOut(iterator first, iterator last) // erase(first, last) with _Destroy out of line
    {
        iterator s = std::copy(last, end(), first);
        Destroy(s, end());
        _Last = s;
        return first;
    }
    void Erase1(iterator pos)          // erase(pos) with _Destroy out of line
    {
        std::copy(pos + 1, end(), pos);
        Destroy(_Last - 1, _Last);
        --_Last;
    }
};
class IntVector : public std::vector<int> {
public:
    void Resize(unsigned int n, const int& x = 0)
    {
        if (n && Size() < n) {
            insert(end(), n - Size(), x);
        } else if (n < Size()) {
            Erase(begin() + n, end());
        }
    }
    unsigned int Size() const;
    iterator Erase(iterator first, iterator last);
};

// A line of the race-mode base's standings table at 0x7b0 (0x14 bytes).
struct RaceEntry {
    class Player* player;              // +0x00
    int rank;                          // +0x04
    char time[12];                     // +0x08: "MM:SS:hh" or "+M:SS:hh"
};

struct RaceCamera {
    BYTE unk0[0x1cc];
    class Car* target;                 // 0x1cc
};

// The VC6 std::string layout (allocator byte, pointer, length, reserve) whose
// _Tidy (0x402700) the race-mode base constructor and destructor call out of
// line; with a real std::string member VC6 expands _Tidy inline there.
class TextString {
public:
    TextString(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~TextString() { Tidy(true); }
    void Tidy(bool built);             // 0x402700 (std::string::_Tidy)
    std::allocator<char> alloc;        // +0x00
    char* ptr;                         // +0x04: reference count at ptr[-1]
    unsigned int len;                  // +0x08
    unsigned int reserve;              // +0x0c
};

// A 2D point with an empty destructor and an explicit copy constructor (the
// original passes the second constructor argument through an out-of-line
// copy, 0x424300).
class HudPoint {
public:
    HudPoint() {}
    HudPoint(const HudPoint& other)
    {
        x = other.x;
        y = other.y;
    }
    ~HudPoint() {}
    float x, y;
};

// A HUD rectangle of the race-mode base (0x14 bytes; ctor 0x4390a0).
class UIRect;

// One name slot of the base's name menu (0x70 bytes; filled by the profile list at game+0x2dd0).
struct NameSlot {
    char name[0x6f];                   // +0x00
    char empty;                        // +0x6f: slot unused (drawn as the "empty" text)
};

// The race-mode base class (module at 0x485bb0). 30 virtual slots.
class RaceMode {
public:
    RaceMode();                        // 0x485bb0
    virtual ~RaceMode();               // slot 0; 0x485da0
    virtual void Update(int dt);       // slot 1
    virtual void Lock();               // slot 2
    virtual void Unlock();             // slot 3
    virtual void Slot4(int mode);
    virtual void Reset(int mode);      // slot 5
    virtual void Slot6();
    virtual bool Slot7();
    virtual void Slot8();
    virtual void Slot9();
    virtual void Slot10(int a, int b);
    virtual void Slot11(int a, int b);
    virtual void SendGrid(int newRace); // slot 12
    virtual int Slot13(Player* player);
    virtual int Slot14(Player* player);
    virtual void Draw();               // slot 15
    virtual void Slot16();
    virtual void DrawOverlay();        // slot 17
    virtual void DrawSetup(int a, struct ViewPoint* view); // slot 18
    virtual void DrawCars(int a, int b, int c); // slot 19
    virtual void DrawShadows(int a, int b);    // slot 20
    virtual void DrawLights(int a, int b);     // slot 21
    virtual void DrawSetupExcept(int a, struct ViewPoint* view, Car* skip); // slot 22
    virtual void DrawShadowsExcept(int a, int b, Car* skip); // slot 23
    virtual void DrawLightsExcept(int a, int b, Car* skip); // slot 24
    virtual int IsJoined();            // slot 25
    virtual int AllFinished();         // slot 26
    virtual void SendStartTimes();     // slot 27
    virtual void SetUnk850(int value); // slot 28
    virtual void SendState(int state); // slot 29

    PlayerVector m_players;            // 0x04
    std::vector<Player*> m_localPlayers; // 0x14
    Player* m_localPlayer;             // 0x24
    int unk28;                         // 0x28
    int unk2c;                         // 0x2c
    Stage* m_stage;                    // 0x30
    struct RaceCamera* m_camera;       // 0x34
    int unk38;                         // 0x38
    int m_time;                        // 0x3c
    int unk40;                         // 0x40
    int unk44;                         // 0x44
    int unk48;                         // 0x48
    int unk4c;                         // 0x4c
    int unk50;                         // 0x50
    int unk54;                         // 0x54
    Sprite* m_bars;                    // 0x58: onscreenbars.tga
    Sprite* m_barsLeft;                // 0x5c: onscreenbarleft.tga
    class OverlayImage* m_image;       // 0x60: ImageInGame.tga
    char m_playerName[0x1e];           // 0x64: results block (FUN_004866b0)
    char m_recordName[0x1e];           // 0x82: name on the record
    const char* m_carText;             // 0xa0: GetString(0x138 + player class)
    const char* m_recordCarText;       // 0xa4: GetString(0x138 + record class)
    char m_stageTimes[10][12];         // 0xa8: this run's stage times
    char m_recordStageTimes[10][12];   // 0x120: the record's stage times
    char m_totalTime[12];              // 0x198
    char m_recordTotalTime[12];        // 0x1a4
    char m_stageDeltas[10][12];        // 0x1b0: difference per stage
    char m_totalDelta[12];             // 0x228
    int m_newRecord;                   // 0x234
    int unk238;                        // 0x238
    int unk23c;                        // 0x23c
    int unk240;                        // 0x240
    int unk244;                        // 0x244
    int unk248;                        // 0x248
    int unk24c;                        // 0x24c
    Sprite* m_cursor;                  // 0x250: MouseCursor.tga
    Sprite m_sprite254;                // 0x254
    Sprite m_replaySprite;             // 0x328: replaybuttons.tga
    NameSlot m_slots[8];               // 0x3fc: name menu entries
    TextString m_text;                 // 0x77c: a std::string
    char m_editing;                    // 0x78c: name menu: typing into m_text
    char m_caret;                      // 0x78d: caret visible
    BYTE unk78e[2];
    DWORD m_caretTime;                 // 0x790: timeGetTime of the last caret toggle
    Player* m_ghost;                   // 0x794: replay ghost
    int m_ghostVisible;                // 0x798: ghost faded in this frame
    int unk79c;                        // 0x79c: results are final (all players finished)
    int m_splitDelta;                  // 0x7a0: last split difference to the best other player
    int m_splitTime;                   // 0x7a4: m_time of that split
    int unk7a8;                        // 0x7a8
    int unk7ac;                        // 0x7ac
    RaceEntry m_entries[6];            // 0x7b0
    int unk828;                        // 0x828
    int unk82c;                        // 0x82c: entries at 0x7b0
    int unk830;                        // 0x830
    std::vector<UIRect*> m_ui;         // 0x834: 7 HUD rectangles

    // Base-class methods (0x487200-0x488420).
    void FUN_0048f500();               // 0x48f500
    void FUN_00487200();
    void SplitDelta(int delta);        // 0x4884e0 (the same function as DrawSplitDelta, called with this in ecx)
    void FUN_004873b0();
    void FUN_0048e4c0(HudPoint position);
    void FUN_0048e840(float x, float y, float w, float h, Color color, int flag);
    void FUN_0048e9b0(float x, float y, float w, float h, Color color, int flag);
    void FUN_0048eb20(float x, float y, float w, float h, int n);
    void FUN_0048ecf0(float x, float y, int a, int b);
    void FUN_00486f80();
    void FUN_00488690();
    int FUN_004890e0(int unused);
    int FUN_004899a0(int unused);      // result screen 1 (rally summary)
    int FUN_0048a070(int recompute);   // stage results, sorted by stage time
    int FUN_0048a920(int sort);        // totals, sorted by total time
    void FUN_00488880(struct ViewPoint* view);
    int FUN_0048b080(int award);       // 0x48b080
    int FUN_0048b620(int reset);       // 0x48b620
    int FUN_0048c7e0(int unused, BYTE flags); // 0x48c7e0
    void FUN_00487080();
    void FUN_00487e00();
    void FUN_004880f0();
    void FUN_00487560();
    void FUN_004877a0();
    void FUN_00487920();
    void FUN_00487b50();
    void FUN_00488420();
    void AddPlayer(Player* player);    // 0x485f60
    void LoadOverlays();               // 0x486240
    void BuildHud();                   // 0x48ee60
    void FUN_00486130(Stage* stage);
    void CreateStage(int a, int b);    // 0x486180
    void FUN_00486200();
    void FreeSprites();                // 0x486590
    void DeleteStage();                // 0x486620
    void FUN_00486650(int a);
    void DeleteGhost();                // 0x48dde0
    void FUN_0048e720();
    void FUN_0048e770();
    void FUN_0048e7b0(int a);
    int GetStageValue();               // 0x48e820
    int PauseMenu(int reset, int noRestart); // 0x48cf80
    int NameMenu(int reset);           // 0x48bd10
    int FUN_0048cca0(int unused, int variant); // 0x48cca0
    int FUN_0048d7f0(bool init);       // 0x48d7f0
    int FUN_00489300(int reset);       // 0x489300
    void FUN_0048ee00(int* value, int delta); // 0x48ee00: adds delta to a 0..100 volume (does not use this)
    void SubmitRecords(BYTE finished); // 0x4866b0
    void SubmitRally();                // 0x486d20
    void LoadReplay();                 // 0x48de10
    void SaveReplay(Player* player);   // 0x48e1f0
};

// The input object at DAT_0056b0c0 (module-local view of two of its methods).
class MouseInput {
public:
    class HudPoint* GetPosition();     // 0x4243c0 (returns &DAT_0057ba00)
    void SetPosition(Vec2* position);  // 0x4b6d00
};

// The 0x48-byte in-game overlay image of the race-mode base.
class OverlayImage {
public:
    OverlayImage(class RenderDevice* device, const char* file, int a, int b); // 0x4a8ae0
    ~OverlayImage();                   // 0x4a8bb0
    void SetGrid(int columns, int rows); // 0x4a9280
    void Draw(class RenderDevice* device, int unused); // 0x4a92c0
    BYTE unk0[0x48];
};

int __stdcall FUN_0048e800(int a, int b);
int FUN_00487b30(const char* format, ...);
void __stdcall DrawSplitDelta(int delta);      // 0x4884e0
void FormatSplitTime(int time, char* text);   // 0x4885a0
void FormatSessionTime(int time, char* text); // 0x486be0
int CompareByFUN_00491f70(const void* a, const void* b); // 0x485a50
int CompareByTotal(const void* a, const void* b);        // 0x485ad0
int CompareBy1a54(const void* a, const void* b);         // 0x485b50
extern int DAT_004ff054[];
extern int DAT_004ff068[];
extern DWORD DAT_00508bf0;
extern int DAT_004ff078[];
extern int DAT_00501464;
extern int DAT_004ff130[];
extern char DAT_0056541c[];

// Race messages (type byte 0 = race, 1 = player; then a sub-type).
#pragma pack(push, 1)
struct MsgState {                      // 0/0, 10 bytes
    BYTE type;
    BYTE sub;
    DWORD id;
    int state;
};
struct MsgPlayerInfo {                 // 0/1, 0x27 bytes
    BYTE type;
    BYTE sub;
    BYTE unk2c;
    BYTE unk24;
    DWORD id;
    BYTE unk20;
    char name[30];
};
struct MsgStartTimes {                 // 0/2, 0x27 bytes
    BYTE type;
    BYTE sub;
    BYTE count;
    DWORD ids[6];
    WORD delays[6];                    // ms to wait before starting
};
struct MsgStage {                      // 1/2, 11 bytes
    MsgStage(BYTE stage_, int time_, float value)
    {
        type = 1;
        sub = 2;
        stage = stage_;
        time = time_;
        unk7 = value;
    }
    BYTE type;
    BYTE sub;
    BYTE stage;
    int time;
    float unk7;
};
struct CarState {                      // Car::GetState, 0x57 bytes
    int time;
    BYTE unk4[0x53];
};
struct CarState2 {                     // Car::GetState2, 0x14 bytes
    int time;
    BYTE unk4[0x10];
};
struct MsgCarState {                   // 1/0, 0x5e bytes (0x76 with the extension)
    BYTE type;
    BYTE sub;
    CarState state;
    BYTE remoteMask;                   // 0x59: bit (startSlot - 1) per other driving player
    int unk44;                         // 0x5a
    BYTE extended;                     // 0x5e
    BYTE unk5f;
    CarState2 state2;                  // 0x60
    short clockOffset;                 // 0x74
};
struct MsgGridSlot {                   // 0/3, 7 bytes
    BYTE type;
    BYTE sub;
    DWORD id;
    BYTE slot;                         // start position (1-based), 0 = any
};
struct MsgSetup {                      // 1/3, 8 bytes
    BYTE type;
    BYTE sub;
    BYTE setup[5];
    BYTE value;
};
struct MsgChatLine {                   // 0/6, 0x4f bytes
    BYTE type;
    BYTE sub;
    DWORD id;
    char text[0x49];
};
struct MsgName {                       // 0/5, 11 bytes
    BYTE type;
    BYTE sub;
    char name[8];
    BYTE end;
};
struct MsgClock {                      // 0/4, 4 bytes
    BYTE type;
    BYTE sub;
    short offset;
};
struct MsgByte {                       // 1/4, 3 bytes
    BYTE type;
    BYTE sub;
    BYTE value;
};
#pragma pack(pop)

// A line of the on-screen message log (4 entries).
struct NetMessage {
    int time;                          // +0
    char active;                       // +4
    char text[0x83];                   // +5
};

class NetRace : public RaceMode {
public:
    NetRace();                         // 0x481390
    virtual ~NetRace();                // 0x4815a0
    virtual void Lock();               // 0x4824b0
    virtual void Unlock();             // 0x4824c0
    virtual int Slot14(Player* player); // 0x483d60
    virtual int Slot13(Player* player); // 0x481f40
    virtual bool Slot7();              // 0x482530
    virtual void DrawOverlay();        // 0x483da0
    virtual int IsJoined();            // 0x482500
    virtual int AllFinished();         // 0x483bb0
    virtual void SetUnk850(int value); // 0x4828d0
    virtual void SendStartTimes();     // 0x483c10
    virtual void SendGrid(int newRace); // 0x4825d0
    virtual void SendState(int state); // 0x482fe0
    virtual void Reset(int mode);      // 0x481860
    virtual void Update(int dt);       // 0x481940
    virtual void Slot4(int mode);      // 0x481700
    virtual void Draw();               // 0x481fa0

    int StartSession();                // 0x4824d0
    void CloseSession();               // 0x4824f0
    void FUN_00482510();
    int GetLatency(Player* player);    // 0x482580
    Player* AddPlayer(const WCHAR* name, DWORD id, int host, int local); // 0x4828e0
    void RemovePlayer(Player* player); // 0x4829e0
    void FUN_00482bf0(Player* player);
    void SendPlayerInfo();             // 0x483090
    void SendSetup(BYTE value);        // 0x483130
    void SendByte(BYTE value);         // 0x483040
    void SetHost(Player* player);      // 0x482c40
    void OnReceive(Player* sender, const BYTE* data, DWORD size); // 0x482c90
    int OnMessage(const BYTE* data, int size); // 0x482ce0
    Player* FindPlayer(DWORD id)
    {
        for (unsigned int i = 0; i < m_players.size(); i++) {
            if (id == m_players[i]->id) {
                return m_players[i];
            }
        }
        return 0;
    }
    int GetUnk844();                   // 0x4831b0
    static char* RandomName(int length); // 0x483930
    void ClearMessages();              // 0x483db0
    void UpdateMessages();             // 0x483de0
    void DrawEntry(float x, float y, int kind); // 0x484190
    void FUN_00482010();
    void ServiceNetwork();             // 0x4831c0
    void FUN_00481e20();
    void AddUnk8c0(int value);         // 0x482a40
    void SendUpdate();                 // 0x4839a0
    void FUN_00484100();
    void DrawMessages();               // 0x483f60

    int unk844;                        // 0x844
    int unk848;                        // 0x848
    int m_host;                        // 0x84c
    int unk850;                        // 0x850
    int unk854;                        // 0x854
    int unk858;                        // 0x858
    int unk85c;                        // 0x85c
    int unk860;                        // 0x860
    int unk864;                        // 0x864
    int unk868;                        // 0x868
    int unk86c;                        // 0x86c
    int unk870;                        // 0x870
    int unk874;                        // 0x874
    DWORD unk878;                      // 0x878
    int unk87c;                        // 0x87c
    int unk880;                        // 0x880
    Player* m_hostPlayer;              // 0x884
    CRITICAL_SECTION m_lock;           // 0x888
    PlayerVector m_removed;            // 0x8a0
    std::vector<int> unk8b0;           // 0x8b0
    IntVector unk8c0;                  // 0x8c0
    int unk8d0;                        // 0x8d0
    int unk8d4;                        // 0x8d4
    int unk8d8[8];                     // 0x8d8
    Sprite* m_pingSprite;              // 0x8f8: data/menu/pingindicators.tga
    NetMessage m_messages[4];          // 0x8fc
    int m_nextMessage;                 // 0xb1c
};

extern int DAT_00501304;

// Replay race mode (vtable 0x4e2690, module 0x48f130): plays back a recorded
// race. m_data is a buffer read from the replay file (Load).
class ReplayRace : public RaceMode {
public:
    ReplayRace();                      // 0x48f130
    virtual ~ReplayRace();             // 0x48f180
    virtual void Update(int dt);       // 0x48f2e0
    virtual void Slot4(int mode);      // 0x48f1b0
    virtual void Reset(int mode);      // 0x48f260
    virtual int Slot13(Player* player); // 0x48f5f0
    void Load(File* file);             // 0x48f600

    int unk844;                        // 0x844
    BYTE* m_data;                      // 0x848
};

// Race mode against a recorded ghost (vtable 0x4e2708, ctor 0x48f6c0).
class GhostRace : public RaceMode {
public:
    GhostRace();                       // 0x48f6c0
    virtual ~GhostRace();              // 0x48f710
    virtual void Update(int dt);       // 0x48f760
    virtual void Slot4(int mode);      // 0x48f8d0
    virtual void Reset(int mode);      // 0x48f9e0
    virtual void Slot8();              // 0x48fa60
    virtual void Slot10(int a, int b); // 0x48fd30
    virtual void Slot11(int a, int b); // 0x48fd90
    virtual void Draw();               // 0x48faa0
    void FUN_0048fb10();               // 0x48fb10: rebuilds the standings around the local player

    int unk844;                        // 0x844
    int unk848;                        // 0x848: time of the reference split
};
int CompareSplits(const void* a, const void* b); // 0x48fcf0
extern int DAT_005654e8;
// Single-player race mode (0x4807d0-0x48138f, vtable 0x4e23ac).
class SingleRace : public RaceMode {
public:
    SingleRace();                      // 0x4807d0
    virtual ~SingleRace();             // 0x480820
    virtual void Update(int dt);       // 0x480a50
    virtual void Slot4(int mode);      // 0x480830
    virtual void Reset(int mode);      // 0x4809d0
    virtual void Slot6();              // 0x480ce0
    virtual void SendGrid(int newRace); // 0x4811c0
    virtual int Slot13(Player* player); // 0x481160
    virtual void Draw();               // 0x481120
    void SetUnk844(int value);         // 0x4811b0

    int unk844;                        // 0x844
    int unk848;                        // 0x848
};

extern char DAT_00564718[];            // RandomName buffer
extern int DAT_00564754;               // column compared by ComparePlayers1884
extern Player* DAT_0056473c[];         // grid order buffer
extern int DAT_00565490;               // players sorted by stage time
extern Player* DAT_00565494[];
void FormatTime(int time, char* text);      // 0x4821d0: "MM:SS:hh"
void FormatTimeDelta(int time, char* text); // 0x4822b0: "+M:SS:hh", leading zeros dropped
int ComparePlayers1884(const void* a, const void* b);
int ComparePlayersStanding(const void* a, const void* b);
int ComparePlayersTotal(const void* a, const void* b);
extern int DAT_00501468;               // stage count of the rally?
extern BYTE DAT_0057b980[];            // passed to Car::FUN_0045cc30
extern int DAT_00501458;               // race option (draws with 0x487b50 instead of 0x487920)
// Race session loop state (0x493650).
extern int DAT_004ffab8[];             // camera view per DAT_00501328
extern int DAT_00501328;               // selected camera (0..5)
extern int DAT_0050132c;               // difficulty class (1 / 2) selecting the award thresholds
extern int DAT_00501434;
extern int DAT_0050146c;               // replay speed: 1 normal, 2 paused, 3 slow, 4 fast
extern BYTE DAT_00501470;              // demo / attract mode
extern int DAT_004ffaac;               // next screenshot number
extern int DAT_005654fc, DAT_00565500, DAT_00565504, DAT_00565508;
extern int DAT_0056b0d0;               // accumulated race-loop time (ms)
extern int DAT_004e2424[];             // lag limit (ms) per DAT_00501324 setting
void FUN_0047f300(DWORD id);           // called when a player leaves a hosted game
void FUN_0047ef80(const char* a, void* name, char* text); // GameSpy gcd_compute_response (gcdkeyc.c, third-party SDK; not decompiled, see analysis/range-47c460.md)
void FUN_0047f1c0(DWORD id, unsigned long address, void* name, char* text, void (*callback)(int, int, int, Player*), Player* player);
void FUN_0047f3b0();
void ServiceCallback(int, int status, int, Player* player); // 0x483980

// A std::string (VC6 layout) of the camera whose _Grow and _Eos the original calls out of line.
class CameraName {
public:
    void Assign(const char* text);     // inline: _Grow(n, true), copy, _Eos(n)
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    BYTE alloc;                        // +0x00
    char* ptr;                         // +0x04
    unsigned int len;                  // +0x08
    unsigned int reserve;              // +0x0c
};

// The view helper (0x1cc bytes; ctor 0x4b77b0, vtable 0x4e29c8, dtor 0x4b78e0, scalar deleting 0x4b78c0): the static at
// 0x509080 of the rear-view mirror (src/unk_464ab0.cpp) and the base of ViewManager. The slots 1-6 are inherited by the
// ViewManager vtable (0x4e1b74) from the view base class of the ctor 0x4b2830. +0x124..+0x12c are the projection
// (near, far, field of view). Moved here from include/unk_464ab0.h so that ViewManager can derive from it.
class MirrorView {
public:
    MirrorView();                      // 0x4b77b0
    virtual ~MirrorView();             // slot 0 (0x4b78c0 is the scalar deleting destructor, 0x4b78e0 the body)
    virtual void Slot1();              // 0x4b2c10
    virtual void Slot2();              // 0x48e660 (shared empty function)
    virtual void Slot3();              // 0x4b2d50
    virtual void Slot4();              // 0x4b2fc0
    virtual void Slot5();              // 0x4b3000
    virtual void Slot6();              // 0x4031e0
    BYTE unk4[0x14 - 4];
    float pos[3];                      // 0x14: camera position
    float unk20[3];                    // 0x20: body frame rows
    float unk2c;
    float unk30[3];
    float unk3c;
    float unk40[3];                    // 0x40: negated body axis
    BYTE unk4c[0x60 - 0x4c];
    float m0[3];                       // 0x60: view matrix, rows 0x10 apart
    float unk6c;
    float m1[3];                       // 0x70
    float unk7c;
    float m2[3];                       // 0x80
    float unk8c;
    float m3[3];                       // 0x90
    BYTE unk9c[0x114 - 0x9c];
    CameraName name;                   // 0x114: "Car camera"
    float nearPlane;                   // 0x124
    float farPlane;                    // 0x128
    float fov;                         // 0x12c
    int zoneValue;                     // 0x130: copied from the camera zone (ViewManager::FUN_004569e0)
    int resetSmoothing;                // 0x134: cleared by SetView / zone changes
    float savedPos[3];                 // 0x138: the car position saved by the camera step 0x456b10
    float prevMatrix[16];              // 0x144: 4x4 matrix (identity by the ctor; the previous frame's matrix)
    BYTE unk184[0x1c4 - 0x184];
    int stepArg;                       // 0x1c4: argument of the camera step 0x456b10
    BYTE unk1c8[0x1cc - 0x1c8];
};

// A 0x34-byte buffer (as CarTrace of include/unk_45e480.h): a vector with the capacity limit 0x11 stored at +0x30.
struct ViewTrace {
    ViewTrace() { limit = 0x11; }
    std::vector<int> v;                 // 0x00
    BYTE unk10[0x30 - 0x10];
    int limit;                         // 0x30
};

// The 0x254-byte camera of the race session, the chase/replay camera of a Car (ctor 0x456490, vtable 0x4e1b74).
// Map: analysis/range-4563e0.md section 1.3. The camera presets (CameraPreset, include/unk_4563e0.h) live in a static
// table. Fields 0x1cc..0x253 are named here; the vector at 0x220 is an inlined std::vector<int>::reserve(0x400) buffer.
class ViewManager : public MirrorView {
public:
    ViewManager();                     // 0x456490
    virtual ~ViewManager();            // 0x456620 (slot 0: scalar deleting destructor 0x456600)
    void FUN_00456780(int view);       // 0x456780: select the camera view (-1 = automatic)
    void FUN_00456660(Car* car, Stage* stage); // 0x456660: SetTarget
    void FUN_004566e0();               // 0x4566e0: PlaceCamera
    void FUN_00458150(void* viewport); // 0x458150: thiscall that ignores this; sets the fixed camera preset
    void FUN_004b78f0();               // 0x4b78f0 (called through the view's base class)
    // The other M1 members (src/unk_4563e0.cpp)
    void FUN_00456820(float dt);       // 0x456820: cheap per-frame follow (Car::Update)
    void FUN_004569e0();               // 0x4569e0: zone scan
    void FUN_00456b10(float dt);       // 0x456b10: full camera step (Car::Update passes the frame time; 0x4566e0 passes 0)
    void FUN_004578c0(float* eye, float* look); // 0x4578c0: preset offsets for the current view
    Car* target;                       // 0x1cc: the followed Car
    Stage* stage;                      // 0x1d0
    int view;                          // 0x1d4: camera preset 0..8 (6 = mounted, 7 = fallback, 8 = automatic)
    int zoneCount;                     // 0x1d8: frames the zone has been confirmed
    BYTE local;                        // 0x1dc: the followed player is local
    BYTE unk1dd[3];
    int reset;                         // 0x1e0: restart the smoothing (ctor 1)
    int autoZone;                      // 0x1e4: automatic zone-driven camera selection
    void* zone;                        // 0x1e8: current camera zone (Stage::0x414870)
    void* prevZone;                    // 0x1ec: candidate zone
    // Note (G2, FUN_00456b10): the camera step uses +0x1f0 as the look-at target and +0x1fc as the camera position, the
    // reverse of the names below; the names are kept because the other functions only copy the pair around.
    Vec3 eye;                          // 0x1f0: eye output (empty destructors: EH states 1-4 of the constructor)
    Vec3 look;                         // 0x1fc: look-at output
    Vec3 smoothEye;                    // 0x208: eye relative to the car, smoothed
    Vec3 smoothLook;                   // 0x214
    ViewTrace buffer;                  // 0x220: reserve(0x400) by the ctor; the limit 0x11 is at 0x250
};

// The growable array of rallytrophy.h (Array<T>), for module files that do not
// include it: data, end, count, capacity, grow step.
struct ViewArray {
    ViewManager** m_data;              // 0x0
    ViewManager** m_end;               // 0x4
    int m_count;                       // 0x8
    int m_capacity;                    // 0xc
    int m_grow;                        // 0x10
    void Add(ViewManager* const& item)
    {
        if (m_count == m_capacity) {
            ViewManager** data = (ViewManager**)operator new((m_capacity + m_grow) * sizeof(ViewManager*));
            memcpy(data, m_data, m_capacity * sizeof(ViewManager*));
            m_capacity += m_grow;
            operator delete(m_data);
            m_data = data;
            m_end = m_data + m_count;
        }
        *m_end = item;
        m_count++;
        m_end++;
    }
};

// The 0x194-byte draw context of the session (ctor 0x4b7a40): a vtable, the list
// of views drawn this frame at +0x124 and the stage's fog/light settings.
class DrawContext {
public:
    DrawContext();                     // 0x4b7a40
    virtual ~DrawContext();            // slot 0 (scalar deleting destructor)
    virtual void Slot1(ViewManager* view);
    void FUN_004b7c80(class RaceDeviceSlots* device, ViewManager* view); // 0x4b7c80
    void FUN_004b8160(class RaceDeviceSlots* device, ViewManager* view); // 0x4b8160
    void FUN_004b82f0(class RaceDeviceSlots* device, ViewManager* view); // 0x4b82f0
    void ClearViews()
    {
        views.m_end = views.m_data;
        views.m_count = 0;
    }
    BYTE unk04[0x124 - 4];
    ViewArray views;                   // 0x124
    BYTE unk138[0x174 - 0x138];
    int unk174;                        // 0x174 .. 0x180: stage +0x1a8..+0x1b4
    int unk178;
    int unk17c;
    int unk180;
    int unk184;                        // 0x184: stage +0x1a4 when DAT_00501434
    int unk188;                        // 0x188 .. 0x190: stage +0x1c4..+0x1cc
    int unk18c;
    int unk190;
};
extern DrawContext* DAT_00581de4;

// The 0x7c-byte control state of the followed car (ctor 0x458700): include/unk_458700.h.
#include "unk_458700.h"


// A progress label shown while the stage loads (0x34 bytes, ctor 0x403270).
class LoadingLabel {
public:
    LoadingLabel();                    // 0x403270
    ~LoadingLabel();                   // 0x403380
    void SetText(const char* text);    // 0x4037e0
    void SetValue(int value);          // 0x4033f0
    void SetProgress(float progress);  // 0x403890
    BYTE unk0[0x24];
    TextRenderer* font;                // 0x24
    float x;                           // 0x28
    float y;                           // 0x2c
    int size;                          // 0x30
};

// A resource group the loading screen steps through (embedded in the stage at
// +0xd4 and in the game object at +0x1393e4 / +0x139404).
class LoadTask {
public:
    int Count();                       // 0x4af1b0
    int Step(class RenderDevice* device); // 0x4af1c0: loads one resource
};

// An object the session owns at +0x28 (destroyed with 0x4aafa0 + delete).
class SessionObject {
public:
    SessionObject(void* surface, int flags); // 0x4aae80
    ~SessionObject();                  // 0x4aafa0
    int Save(const char* file);        // 0x4ac730: nonzero on success
    BYTE unk0[0x20];
};


// The in-race game-loop object embedded in Game at +0x139080 (0x491fb0-0x4952af).
// Name is a guess. original: ctor 0x4304d5 (in Game ctor 0x4303e0).
class RaceSession {
public:
    RaceSession();                     // 0x491fb0
    ~RaceSession();                    // 0x48e660 (empty, folded; called out of line)
    void Shutdown();                   // 0x492060
    void CreateCountdown();            // 0x492090: lazily builds the CountDown sprite
    void DestroyCountdown();           // 0x492190
    void RecordStageProgress();        // 0x494c80
    void UpdateViewport(int keep);     // 0x494f00 (thiscall, ret 4)
    void PickBestPlayer();             // 0x495000
    void DrawCountdown(RaceMode* mode); // 0x495080 (thiscall, ret 4)
    int FUN_00493650(int demo);                 // 0x493650: the per-frame race loop (thiscall, ret 4)
    void HandleCameraKeys(ViewManager* view, int demo); // 0x494d00 (thiscall, ret 8)
    int Run(RaceMode* mode, int demo, BYTE a, BYTE b); // 0x4921d0 (thiscall, ret 0x10)
    int RunAttract(RaceMode* mode);    // 0x492e40 (thiscall, ret 4)

    Sprite* m_countdown;               // 0x00: CountDown.tga overlay
    RaceMode* m_mode;                  // 0x04
    Stage* m_stage;                    // 0x08
    DrawContext* m_unk0c;              // 0x0c: 0x194-byte object (ctor 0x4b7a40)
    ViewManager* m_view;               // 0x10
    int m_followed;                    // 0x14: index of the followed player
    BYTE m_unk18;                      // 0x18
    BYTE m_unk19;                      // 0x19
    BYTE m_unk1a;                      // 0x1a
    BYTE m_unk1b;                      // 0x1b
    Car* m_car;                        // 0x1c
    BYTE m_unk20;                      // 0x20
    BYTE m_pad21[3];
    int m_fps;                         // 0x24: VideoOutFps (data/settings/video.ini)
    SessionObject* m_unk28;            // 0x28
    BYTE m_unk2c;                      // 0x2c
    BYTE m_pad2d[3];
    int m_unk30;                       // 0x30
    CarInput* m_unk34;                 // 0x34
};


// AI racing line (0x49f910-0x49fedf, nodes 0x4a14e0, segments 0x4a1530), loaded
// from "<stage>.ai". Owned by Stage+0x1368. Non-polymorphic.
class AiNode {                         // 0x30 bytes
public:
    AiNode();                          // 0x4a14e0
    ~AiNode();                         // empty; folded with 0x48e660 (called, never inlined)
    float x, y, z;                     // 0x00: position
    BYTE unkc[0x1c - 0xc];
    int unk1c;                         // 0x1c: 200
    int index;                         // 0x20
    float unk24;                       // 0x24: 4.25
    float unk28;                       // 0x28
    float unk2c;                       // 0x2c
};
class AiSegment {                      // 0x98 bytes
public:
    AiSegment(int unused);             // 0x4a1530
    ~AiSegment();                      // 0x4a15e0
    AiNode* a;                         // 0x00: start node
    AiNode* b;                         // 0x04: end node
    int unk8;                          // 0x08
    int unkc;                          // 0x0c: 2
    BYTE unk10[4];
    int unk14;                         // 0x14
    BYTE unk18[4];
    int index;                         // 0x1c
    BYTE unk20[8];
    Vec3Slot unk28[2];                 // 0x28, 0x34
    float unk40;                       // 0x40
    float unk44;                       // 0x44
    Vec2 unk48[2];                     // 0x48
    Vec2 unk58[2];                     // 0x58
    Vec3Slot unk68[4];                 // 0x68..0x98
};
class AiPath {                         // 0x54 bytes
public:
    AiPath();                          // 0x49f910
    ~AiPath();                         // 0x49f9e0
    void Load(const char* path);       // 0x49faf0 (thiscall, ret 4)
    void LinkSegments();               // 0x49fd50
    void ClearSegments();              // 0x49fdf0
    AiSegment* At(int i);              // 0x49fe70 (wraps around)
    std::vector<AiNode*> m_nodes;      // 0x00
    std::vector<AiSegment*> m_segments; // 0x10
    std::vector<int> unk20;            // 0x20: freed in the destructor only
    std::vector<int> unk30;            // 0x30: freed in the destructor only
    int unk40;                         // 0x40
    int unk44;                         // 0x44: -1
    BYTE unk48[0x54 - 0x48];
};

#endif
