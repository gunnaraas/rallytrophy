/* CarLight (0x46e880-0x46eebf): the glow sprite of a car's head, tail, brake and reverse
 * lights (12 per car, Car+0x1bd40 + 0x1c*i), 0x13c bytes, vtable 0x4e1f64. Map:
 * analysis/range-46d830.md section 5. Include after rallytrophy.h, unk_481390.h and
 * unk_47c460.h. */
#ifndef UNK_46E880_H
#define UNK_46E880_H

class CarLight : public EffectBase {
public:
    CarLight();                        // 0x46e8e0
    virtual ~CarLight();               // 0x46e9f0 (scalar deleting dtor 0x46e9d0)
    void SetSize(float s);             // 0x46ea00
    virtual void Render(RenderDevice* device, Camera* camera); // 0x46ea20 (slot 5)

    Vec3& Right() { return *(Vec3*)right; }
    Vec3& Up() { return *(Vec3*)up; }
    Vec3& Forward() { return *(Vec3*)forward; }
    Vec3& Position() { return *(Vec3*)position; }

    BYTE unk4[4];
    int unk8;                          // 0x08
    BYTE unkc[0x60 - 0xc];
    float right[3];                    // 0x60: the lamp's world axes (rows of its matrix)
    BYTE unk6c[4];
    float up[3];                       // 0x70
    BYTE unk7c[4];
    float forward[3];                  // 0x80: the lamp's axis
    BYTE unk8c[4];
    float position[3];                 // 0x90
    BYTE unk9c[0xb8 - 0x9c];
    int unkb8;                         // 0xb8
    int unkbc;                         // 0xbc
    int unkc0;                         // 0xc0
    float radiusA;                     // 0xc4: culling radius (SetSize)
    float radiusB;                     // 0xc8
    BYTE flags;                        // 0xcc
    BYTE unkcd[0x124 - 0xcd];
    void* texture;                     // 0x124: glow texture of the light kind
    float exponent;                    // 0x128: falloff exponent of the facing term
    float size;                        // 0x12c: billboard half size
    float scale;                       // 0x130: intensity scale
    float bias;                        // 0x134: intensity bias
    int mirror;                        // 0x138: 1 for the right-hand lights
};

#endif
