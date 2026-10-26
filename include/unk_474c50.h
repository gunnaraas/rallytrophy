/* SnowFall weather effect (0x474c50-0x4758af, vtable 0x4e2038, 0x202c bytes),
 * owned by the Stage at +0x6b0. Map: analysis/range-4741a0.md section 2.2.
 * Include after rallytrophy.h, unk_481390.h and unk_47c460.h. */
#ifndef UNK_474C50_H
#define UNK_474C50_H

// The texture loader the Stage hands to weather effects (0x4aef30 loads a file with load flags).
class TextureLoader {
public:
    Texture* Load(const char* name, int flags); // 0x4aef30
};

// A renderable batch of quads (0x38 bytes; ctor 0x4baf40, dtor 0x4baf90).
class Mesh {
public:
    Mesh();                            // 0x4baf40
    ~Mesh();                           // 0x4baf90
    Sprite* material;                  // 0x00
    EffectVertex* vertices;            // 0x04
    WORD* indices;                     // 0x08
    WORD vertexCount;                  // 0x0c
    WORD primitiveCount;               // 0x0e: triangles
    BYTE unk10[8];
    int unk18;                         // 0x18: cleared before drawing (ParallelShadow::Render)
    BYTE unk1c[8];
    int unk24;                         // 0x24
    BYTE unk28[0x38 - 0x28];
};

// Abstract base of the Stage's weather effects (vtable 0x4e2000: slot 0 the scalar deleting
// destructor 0x4732b0, slot 1 _purecall). The rain class lives in the previous file.
class WeatherEffect {
public:
    virtual ~WeatherEffect() {}
    virtual void Update(RenderDevice* device, Camera* camera, int a, int b, float dt) = 0;
};

// One snowflake (0x20 bytes). Positions are 16-bit fixed point in the weather box.
struct SnowFlake {
    int x;                             // 0x00: 0..65535
    int y;                             // 0x04
    int z;                             // 0x08
    float fall;                        // 0x0c: fall speed
    float u;                           // 0x10: texture offset, 0 or 0.5
    float v;                           // 0x14
    int phase;                         // 0x18: sway phase, 16.16 index into the sway tables
    int speed;                         // 0x1c: phase change per second
};

// A 4x4 world matrix: the identity (the translation m[12..14] is assigned by the user) (inline ctor, empty dtor: gives the EH frame a state).
struct WorldMatrix {
    WorldMatrix()
    {
        m[1] = m[2] = m[3] = 0.0f;
        m[4] = m[6] = m[7] = 0.0f;
        m[8] = m[9] = m[11] = 0.0f;
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }
    ~WorldMatrix() {}
    float m[16];
};

class SnowFall : public WeatherEffect {
public:
    SnowFall(TextureLoader* loader, int count); // 0x474c50 (scalar deleting dtor 0x474f90)
    virtual ~SnowFall();               // 0x474fb0
    virtual void Update(RenderDevice* device, Camera* camera, int a, int b, float dt); // 0x4751b0
    void InitParticles();              // 0x475030

    Sprite* material;                  // 0x04
    int drawCount;                     // 0x08: flakes processed per update (equal to count)
    int count;                         // 0x0c
    Mesh* mesh;                        // 0x10
    float threshold;                   // 0x14: minimum view depth of a drawn flake (1.0)
    Vec3 box;                          // 0x18: size of the weather box (its empty dtor gives the EH frame state 1)
    SnowFlake* flakes;                 // 0x24
    float size;                        // 0x28: half-size factor of a flake quad (0.05)
    int sway[2][1024];                 // 0x2c: x and z sway tables (65535 * 0.2 / box * cos/sin)
};

#endif
