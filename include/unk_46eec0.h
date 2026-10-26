/* CityLight (0x46eec0-0x46fa7f): the street-lamp glow and lens flare of the city stages, one per
 * lamp object of the Stage (0x16c bytes, vtable 0x4e1f88). Map: analysis/range-46d830.md section 6.
 * Include after rallytrophy.h, unk_481390.h, unk_47c460.h, unk_47b4c0.h and unk_474c50.h. */
#ifndef UNK_46EEC0_H
#define UNK_46EEC0_H

// A ViewMat with the composition 0x4b55e0 (thiscall, ret 4): this = this * m, the translation row
// of m added to this's own translation.
class LightView : public ViewMat {
public:
    void Concat(const ViewMat& m);     // 0x4b55e0
};

// A plain float triple (no ctor/dtor, so that it adds no unwind state to the owner).
struct Point3 {
    float x, y, z;
};

// A 4x4 float matrix copied as one block (rep movsd); the translation row is also addressed as a Point3.
struct LightMatrix {
    float m[12];
    Point3 translation;                // 0x30: row 3
    float w;
};

// A scene object record of the Stage's object loader: only what the CityLight constructor reads.
struct LightSource {
    BYTE unk0[0x14];
    Point3 position;                   // 0x14
    LightMatrix matrix;                // 0x20
    BYTE unk60[0x10c - 0x60];
    LightSource* child;                // 0x10c: the object this one hangs on (null = none)
    BYTE unk110[4];
    BYTE nameStorage[0x10];            // 0x114: std::string name
    std::string& Name() { return *(std::string*)nameStorage; }
};

class CityLight : public EffectBase {
public:
    CityLight(LightSource* source, TextureLoader* loader, Sprite* glowSprite, Sprite* flareSprite); // 0x46eec0
    ~CityLight();                      // 0x46f300 (scalar deleting dtor 0x46f2e0)
    virtual void Render(RenderDevice* device, Camera* camera); // 0x46f360

    int kind;                          // 0x04: 5
    int unk8;                          // 0x08: zeroed by the ctor
    BYTE unkc[0x14 - 0xc];
    Point3 position14;                 // 0x14: copy of the matrix translation (m[12..14]) before the ctor clears it
    LightMatrix matrix;                // 0x20: the source's transform, composed with its child's
    BYTE unk60[0x80 - 0x60];
    Point3 axis;                       // 0x80: facing of the lamp (the quad normal and the flare direction)
    BYTE unk8c[4];
    Point3 position;                   // 0x90: world position of the lamp
    BYTE unk9c[0xb8 - 0x9c];
    int unkb8, unkbc, unkc0;           // 0xb8: zeroed by the ctor
    float unkc4, unkc8;                // 0xc4: 3.0
    BYTE flags;                        // 0xcc
    BYTE unkcd[0x114 - 0xcd];
    BYTE nameStorage[0x10];            // 0x114: std::string name (constructed by the base)
    std::string& Name() { return *(std::string*)nameStorage; }
    Sprite* glow;                      // 0x124: the glow material (Stage+0x6b4), shared by all lamps
    Sprite* flare;                     // 0x128: the flare material (Stage+0x788)
    float exponent;                    // 0x12c: power on the facing for the flare brightness
    float size;                        // 0x130: half size of the glow and offset of the depth
    float scale;                       // 0x134
    float bias;                        // 0x138
    Vec3 corner[4];                    // 0x13c: offsets of the four glow quad corners
};

#endif
