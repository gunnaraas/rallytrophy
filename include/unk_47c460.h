/* Car particle emitter (0x47c460-0x47ef4f): wheel dust, smoke, gravel and
 * spark effects owned by a Car (embedded at Car+0x2358). Map:
 * analysis/range-47c460.md. Include after rallytrophy.h and unk_481390.h. */
#ifndef UNK_47C460_H
#define UNK_47C460_H

class Camera;

// One live particle (0x44 bytes), threaded through `next` either in an
// emitter's live list or in the ParticlePool free list.
struct Particle {
    Particle() {}                      // empty ctor/dtor: arrays go through the EH vector iterators (0x490ad0/0x402c90)
    ~Particle() {}
    int unk0;                          // 0x00
    int frame;                         // 0x04: sprite frame (low nibble = column, >> 4 = row)
    float age;                         // 0x08
    float lifetime;                    // 0x0c: the particle dies when age >= lifetime
    Vec3 position;                     // 0x10
    Vec3 velocity;                     // 0x1c
    float rotation;                    // 0x28: index 0..1023 into the sin/cos table
    float spin;                        // 0x2c: rotation change per second
    float size;                        // 0x30
    float gravity;                     // 0x34: added to velocity.y per second (CarEmitterB: size growth per second)
    DWORD color;                       // 0x38: ARGB copied from the emitter
    float alphaScale;                  // 0x3c: factor on the fade alpha (CarEmitterB, CarEmitterC)
    Particle* next;                    // 0x40
};

// The free list of particles (global at 0x56ae90, ctor 0x478870, dtor 0x4788c0).
class ParticlePool {
public:
    ParticlePool();                    // 0x478870
    ~ParticlePool();                   // 0x4788c0
    void Grow();                       // 0x478960: allocate 4096 more particles
    void ThreadBlock(Particle* block); // 0x478b80: link a new block onto the free list
    Particle* head;                    // 0x00
    std::vector<Particle*> blocks;     // 0x04 (allocator byte), first 0x08, last 0x0c, end 0x10
};
extern ParticlePool DAT_0056ae90;

// Sprite vertex (0x2c bytes); arrays of it go through the EH vector
// constructor iterator (empty ctor folds to 0x490ad0, empty dtor to 0x402c90).
class EffectVertex {
public:
    EffectVertex() {}
    ~EffectVertex() {}
    Vec3 position;                     // 0x00
    Vec3 normal;                       // 0x0c
    DWORD color;                       // 0x18
    float u, v;                        // 0x1c
    float u2, v2;                      // 0x24
};

// The part of the camera Render reads.
class Camera {
public:
    BYTE unk0[0x14];
    Vec3 position;                     // 0x14
    BYTE unk20[0x60 - 0x20];
    Vec3 right;                        // 0x60: camera x axis
    BYTE unk6c[4];
    Vec3 up;                           // 0x70: camera y axis
    BYTE unk7c[4];
    Vec3 forward;                      // 0x80: camera z axis
    BYTE unk8c[4];
    Vec3 eyePosition;                  // 0x90
    BYTE unk9c[4];
    float boxMin[3];                   // 0xa0: bounds of the visible volume (ParallelShadow::Prepare)
    float boxMax[3];                   // 0xac
    BYTE unkb8[0xc4 - 0xb8];
    float radius;                      // 0xc4
    BYTE unkc8[0x124 - 0xc8];
    int nearBits;                      // 0x124: near plane (float bits, compared as an integer)
    int farBits;                       // 0x128: far plane (float bits)
    float fov;                         // 0x12c
    BYTE unk130[0x134 - 0x130];
    int hasLens;                       // 0x134: nonzero = build the view matrix from the lens fields below
    float lensPos[3];                  // 0x138
    float lensRot[12];                 // 0x144: 3 rows of 4 floats
    BYTE unk174[0x180 - 0x174];
    float lensW;                       // 0x180
    BYTE unk184[0x1c4 - 0x184];
    float lensScale;                   // 0x1c4: sprite stretch scale
};

// The wheel (state) passed as the 4th argument of Spawn; slip values drive the emission.
struct ParticleWheel {
    BYTE unk0[0x14];
    Vec3 pos;                          // 0x14
    BYTE unk20[0x264 - 0x20];
    float radius;                      // 0x264: subtracted from pos.y
    float Radius() { return radius; }
    BYTE unk268[0x288 - 0x268];
    float slipA;                       // 0x288
    float SlipA() { return slipA; }
    float slipB;                       // 0x28c
    float SlipB() { return slipB; }
    BYTE unk290[0x2a8 - 0x290];
    int mode;                          // 0x2a8
};

// The car body passed as the 5th argument of Spawn.
struct ParticleBody {
    BYTE unk0[0x14];
    Vec3 pos;                          // 0x14
    float m[16];                       // 0x20: rows right (0x20), up (0x30), forward (0x40)
    BYTE unk60[0x1cc - 0x60];
    Vec3 vel;                          // 0x1cc
    Vec3 PointVelocity(const Vec3& p); // 0x4204f0: velocity of a world point on the body
    Vec3& Right() { return *(Vec3*)&m[0]; }
    Vec3& Forward() { return *(Vec3*)&m[8]; }
};

// The car-state object the spawners read (file static 0x563fdc, set by Spawn).
struct ParticleCar {
    BYTE unk00[0x14];
    Vec3 position;                     // 0x14
    BYTE unk20[0x264 - 0x20];
    float unk264;                      // 0x264: height of the contact point below the position
    BYTE unk268[0x288 - 0x268];
    float slipLong;                    // 0x288
    float slipLat;                     // 0x28c
    BYTE unk290[0x2a8 - 0x290];
    int surfaceKind;                   // 0x2a8
};

// Engine object base of the effects (vtable slots 0-9; ctor 0x4b2830, dtor
// 0x4b29c0, defined in another module). Holds no fields of its own here.
class EffectBase {
public:
    EffectBase();                      // 0x4b2830
    virtual ~EffectBase();             // 0x4b29c0
    virtual void vf1();                // 0x4b2c10
    virtual void vf2();                // 0x48e660
    virtual void vf3();                // 0x4b2d50
    virtual void vf4();                // 0x4b2fc0
    virtual void Render(RenderDevice* device, Camera* camera); // slot 5: 0x4b3000
    virtual void vf6(RenderDevice* device, Camera* camera, int unused); // 0x4031e0 (0x477080 in WindowRain)
    virtual void SetFlag04(int on);    // 0x4b3010
    virtual int vf8();                 // 0x4890c0: return 1
    virtual void Clear();              // slot 9: 0x4790e0 in Effect
};

// Base of the particle effects (vtable 0x4e2124, size 0x12c, ctor 0x478e80, dtor 0x478f00).
class Effect : public EffectBase {
public:
    Effect();                          // 0x478e80
    virtual ~Effect();                 // 0x478f00 (scalar deleting dtor 0x478ee0)
    virtual void Clear();              // 0x4790e0: return the live particles to the pool
    void InitTables();                 // 0x478f50: fill the shared sin/cos and sprite uv tables once
    BYTE unk4[4];
    int unk8;                          // 0x08: zeroed by the ctor
    BYTE unkc[0xa0 - 0xc];
    float boxMin[3];                   // 0xa0: bounds of the live particles
    float boxMax[3];                   // 0xac
    int unkb8[3];                      // 0xb8..0xc3: zeroed by ExhaustSmoke (0x478080)
    float unkc4;                       // 0xc4: 20.0 in ExhaustSmoke
    BYTE unkc8[4];                     // 0xc8
    BYTE flags;                        // 0xcc: bit 1 set by the particle emitters
    BYTE unkcd[0x124 - 0xcd];
    void* texture;                     // 0x124
    Particle* particles;               // 0x128: live list
};

// The emitter embedded in a Car (vtable 0x4e237c, 0x130 bytes).
class CarParticles : public Effect {
public:
    CarParticles();                    // 0x47c460
    ~CarParticles();                   // 0x47c4e0 (scalar deleting dtor 0x47c4c0)
    void SetColor(const Vec3* rgb);    // 0x47c4f0
    void Update(float dt);             // 0x47c5a0
    void Spawn(int type, float a, float b, Car* car, CarBody* body); // 0x47c790
    virtual void Render(RenderDevice* device, Camera* camera); // 0x47c8a0

    Particle* SpawnCoreA(float rate);  // 0x47cd60
    Particle* SpawnCoreB(float rate);  // 0x47d500
    Particle* SpawnCoreC(float rate);  // 0x47dc10
    Particle* SpawnCoreD(float rate);  // 0x47e240
    Particle* SpawnCoreE(float rate);  // 0x47e8f0
    void Kind0(float scale, float age); // 0x47d240
    void Kind1(float scale, float age); // 0x47d2f0
    void Kind2(float scale, float age); // 0x47d3a0
    void Kind3(float scale, float age); // 0x47d450
    void Kind6(float scale, float age); // 0x47d970
    void Kind7(float scale, float age); // 0x47da50
    void Kind8(float scale, float age); // 0x47db30
    void Kind9(float scale, float age); // 0x47e190
    void Kind10(float scale, float age); // 0x47e820
    void Kind11(float scale, float age); // 0x47ee80

    Color color;                       // 0x12c: particle ARGB (empty dtor: gives the ctor a second EH state)
};

#endif
