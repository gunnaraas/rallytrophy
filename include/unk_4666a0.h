/* Car sounds (0x4666a0-0x46871f, stub 0x4686f0). Map: analysis/range-4666a0.md section 2.
 * Include after unk_481390.h and rallytrophy.h (sound.h comes with rallytrophy.h).
 * The Car members of this module are written against CarSoundView, a raw view of the Car
 * fields the sound code touches (Car itself is not restructured). */
#ifndef UNK_4666A0_H
#define UNK_4666A0_H

// std::string layouts (VC6: allocator byte, pointer, length, reserve) for the file names built in
// InitSounds. The original destroys the temporaries through the out-of-line _Tidy (0x402700) but
// expands the destructor of the local inline; TmpString/NameString reproduce that.
class TmpString {
public:
    ~TmpString() { Tidy(true); }
    void Tidy(bool built);             // 0x402700
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};
TmpString SoundPath(const std::string& dir, const char* name); // 0x409130: operator+(string, const char*)

class NameString {
public:
    NameString(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~NameString() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)              // std::string::_Tidy, expanded inline
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    void Assign(const TmpString& s, unsigned int pos, unsigned int n); // 0x4024f0
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};

struct RawVec3 {
    float x, y, z;
};

// A range-fade record (0xc bytes): full volume between low and high.
struct SoundRange {
    float low;
    float unk4;                        // the rate divisor of the layer (pitch = rate * rpm / unk4)
    float high;
};

// A surface sound of the stage (0x2c bytes at least): the looped sample and its two curves over the car speed.
struct SurfaceSound {
    Sample* sample;                    // 0x00
    float lo2;                         // 0x04 (second curve, 0x468500)
    float hi2;                         // 0x08
    float val2a;                       // 0x0c
    float val2b;                       // 0x10
    float lo1;                         // 0x14 (first curve, 0x4684b0)
    float hi1;                         // 0x18
    float val1a;                       // 0x1c
    float val1b;                       // 0x20
    float nearDistance;                // 0x24
    float farDistance;                 // 0x28
};

// A sound layer of a Car (0x8c bytes): a sample, its 3D source and the per-layer settings
// read from Sounds.ini. Car+0x1c2b4 + k*0x8c: engine idle, engine 1-3, exhaust idle, exhaust 1-3.
struct SoundLayer {
    Sample* sample;                    // 0x00
    SoundSource source;                // 0x04 (0x70 bytes)
    SoundRange range;                  // 0x74: EngineSound<n>Range (low, rate divisor, high)
    float fade;                        // 0x80: SoundFadeRange
    float volume;                      // 0x84: SoundVolume
    float acc;                         // 0x88: SoundVolumeAcc
};
typedef char SoundLayer_size_check[sizeof(SoundLayer) == 0x8c ? 1 : -1];

// A SoundSource followed by the surface sound it plays (0x74 bytes): the looped sources of the own car.
struct SoundSlot {
    SoundSource source;                // 0x00; source.unk54 is the fade level (0..0x100)
    struct SurfaceSound* surface;      // 0x70 (set by UpdateSounds, cleared by the start/stop code)
};

// A queued speech/effect entry of the own car (8-slot ring at Car+0x1f24c; 12 bytes). id = speech sample index, 0 = free;
// aux = priority; timeout = queue time.
struct CrashSlot {
    int id;
    int aux;
    int timeout;
};

// A surface zone with up to four sound ids, found under the car by Stage::0x416980 (0x28+ bytes).
struct SurfaceZone {
    signed char sound[4];              // 0x00: sample ids, -1 = none
    int done[4];                       // 0x04: the id has been queued (0 = still to flush)
    float x0;                          // 0x14
    float unk18;                       // 0x18
    float z0;                          // 0x1c
    float dirX;                        // 0x20
    float unk24;                       // 0x24
    float dirZ;                        // 0x28
};

// The stage as the sound code sees it.
class SoundStage {
public:
    SurfaceZone* FindZone(const float* position);  // 0x416980: zone under a point, or null
    int FUN_00417e60(const Vec3* position, float radius);   // 0x417e60: a stage object within the radius
    SurfaceSound* SoundForSpeed(int surface);               // 0x4161c0
    SurfaceSound* SoundForSpeed2(int surface);              // 0x4161e0
    SurfaceSound* SoundForSlip(int surface);                // 0x416200
    SurfaceSound* SoundForRoll(int surface);                // 0x416240
    SurfaceSound* SoundForRoll2(int surface);               // 0x416220
};

// The message queue at Car+0x1f160 (0x40b580 pushes an event with its time).
class CarMessageQueue {
public:
    void Push(int id, int time);       // 0x40b580
};

// A wheel as the sound code sees it (stride 0x330, at Car+0x28; the CarWheel of unk_481390.h starts at +0x298).
struct SoundWheel {
    BYTE unk0[0x298];
    int contact;                       // 0x298: non-zero when the wheel is in use
    BYTE unk29c[0x2fc - 0x29c];
    float v64;                         // 0x2fc
    float v68;                         // 0x300 (summed, never used)
    float v6c;                         // 0x304
    float v70;                         // 0x308
    BYTE unk30c[0x330 - 0x30c];
};

// The rigid body as the sound code sees it (CarBody of unk_481390.h).
struct SoundBody {
    BYTE unk0[0x14];
    Vec3 position;                     // 0x14
    float matrix[3][4];                // 0x20: rotation rows (stride 0x10)
    BYTE unk50[0x1cc - 0x50];
    Vec3 velocity;                     // 0x1cc
};

// The Car as the sound code sees it (view; the real class is Car in unk_481390.h).
struct CarSoundView {
    BYTE unk0[0x24];
    SoundBody* body;                   // 0x24
    SoundWheel wheel[4];               // 0x28
    BYTE unkce8[0xff4 - 0xce8];
    int viewOffset;                    // 0xff4: x offset of the cockpit view (replaces the sound locations)
    BYTE unkff8[0x1338 - 0xff8];
    float throttle;                    // 0x1338
    BYTE unk133c[0x14fc - 0x133c];
    SoundStage* stage;                 // 0x14fc
    BYTE unk1500[0x1e44 - 0x1500];
    int wheelSurface[4];               // 0x1e44
    BYTE unk1e54[0x1eb4 - 0x1e54];
    int cockpit;                       // 0x1eb4
    BYTE unk1eb8[0x1becc - 0x1eb8];
    class Player* player;              // 0x1becc
    BYTE unk1bed0[0x1befc - 0x1bed0];
    int state;                         // 0x1befc: 2 = no random sounds
    BYTE unk1bf00[4];
    SoundSlot slot[5];                 // 0x1bf04, stride 0x74 (init order differs)
    SoundSource crash[3];              // 0x1c148, stride 0x70 (hard, window, light)
    Sample* crashSample[7];            // 0x1c298: window_smash1/2, light_smash, hard_crash1/2, soft_crash1/2
    SoundLayer layer[8];               // 0x1c2b4: engine idle, 1-3, exhaust idle, 1-3
    float throttleRpm;                 // 0x1c714
    Vec3 engineLoc;                    // 0x1c718
    Vec3 exhaustLoc;                   // 0x1c724
    BYTE unk1c730[0x1f148 - 0x1c730];
    int raceTime;                      // 0x1f148
    BYTE unk1f14c[0x1f15c - 0x1f14c];
    SurfaceZone* lastZone;             // 0x1f15c
    CarMessageQueue queue;             // 0x1f160
    BYTE unk1f161[0x1f208 - 0x1f161];
    SurfaceZone* ring[16];             // 0x1f208
    int ringHead;                      // 0x1f248
    CrashSlot crashSlot[8];            // 0x1f24c
    int crashHead;                     // 0x1f2ac
    int lastB;                         // 0x1f2b0
    int lastA;                         // 0x1f2b4
    int lastC;                         // 0x1f2b8
    int flagB;                         // 0x1f2bc
    int flagA;                         // 0x1f2c0
    int window;                        // 0x1f2c4
    int light;                         // 0x1f2c8
    int hard;                          // 0x1f2cc
    int soft;                          // 0x1f2d0
    Vec3 windowPos;                    // 0x1f2d4
    Vec3 hardPos;                      // 0x1f2e0
    int hardVolume;                    // 0x1f2ec
    Vec3 softPos;                      // 0x1f2f0
    int softVolume;                    // 0x1f2fc
    Vec3 lightPos;                     // 0x1f300
};

float SoundRangeFade(float x, const SoundRange* r, float fade, int lowSide, int highSide); // 0x468410
float SoundCurveA(float x, const SurfaceSound* c);   // 0x4684b0
float SoundCurveB(float x, const SurfaceSound* c);   // 0x468500

#endif
