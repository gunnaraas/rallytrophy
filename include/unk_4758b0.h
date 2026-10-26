/* Sky (0x4758b0-0x47648f): the cloud/sky layers of a Stage (Stage+0x6ac, 0xf70 bytes, not polymorphic).
 * Map: analysis/range-4741a0.md. Include after rallytrophy.h, unk_481390.h and unk_47c460.h. */
#ifndef UNK_4758B0_H
#define UNK_4758B0_H

// A projected point (x, y, z, w) with an (empty) destructor, which gives its users an EH state.
class Vec4 {
public:
    Vec4() {}
    ~Vec4() {}
    float x, y, z, w;
};

// The renderer vtable slots the sky uses (module-local view; a 4-int Vec4 is the projection result).
#define SKY_SLOT(n) virtual void Slot##n();
class SkyDevice {
public:
    SKY_SLOT(0)
    SKY_SLOT(1)
    SKY_SLOT(2)
    SKY_SLOT(3)
    SKY_SLOT(4)
    SKY_SLOT(5)
    SKY_SLOT(6)
    SKY_SLOT(7)
    SKY_SLOT(8)
    SKY_SLOT(9)
    SKY_SLOT(10)
    SKY_SLOT(11)
    SKY_SLOT(12)
    SKY_SLOT(13)
    SKY_SLOT(14)
    virtual void GetViewport(int* x, int* y, int* w, int* h); // 0x3c
    SKY_SLOT(16)
    virtual float GetAspect();         // 0x44
    SKY_SLOT(18)
    SKY_SLOT(19)
    SKY_SLOT(20)
    SKY_SLOT(21)
    SKY_SLOT(22)
    SKY_SLOT(23)
    SKY_SLOT(24)
    SKY_SLOT(25)
    SKY_SLOT(26)
    SKY_SLOT(27)
    SKY_SLOT(28)
    SKY_SLOT(29)
    SKY_SLOT(30)
    SKY_SLOT(31)
    SKY_SLOT(32)
    SKY_SLOT(33)
    SKY_SLOT(34)
    virtual void SetTexture(void* texture); // 0x8c
    SKY_SLOT(36)
    SKY_SLOT(37)
    virtual void Slot38(int value);    // 0x98
    SKY_SLOT(39)
    SKY_SLOT(40)
    SKY_SLOT(41)
    SKY_SLOT(42)
    SKY_SLOT(43)
    virtual void Slot44(int value);    // 0xb0
    SKY_SLOT(45)
    SKY_SLOT(46)
    SKY_SLOT(47)
    SKY_SLOT(48)
    SKY_SLOT(49)
    SKY_SLOT(50)
    SKY_SLOT(51)
    SKY_SLOT(52)
    SKY_SLOT(53)
    SKY_SLOT(54)
    SKY_SLOT(55)
    SKY_SLOT(56)
    SKY_SLOT(57)
    SKY_SLOT(58)
    SKY_SLOT(59)
    SKY_SLOT(60)
    virtual void Project(Vec4* out, const Vec3& point); // 0xf4: world point to clip space (x, y, z, w)
    virtual void ToScreen(Vec4* point); // 0xf8: clip space to screen, in place
    SKY_SLOT(63)
    SKY_SLOT(64)
    SKY_SLOT(65)
    SKY_SLOT(66)
    virtual void DrawRect(Vec2* position, Color color, float sx, float sy); // 0x10c
    int width;                         // 0x04
    int height;                        // 0x08
};
#undef SKY_SLOT

// The texture loader behind Stage+0xd4 (another module).
class TextureLoader {
public:
    Texture* Load(const char* path, int flags); // 0x4aef30
};

Vec3 operator*(float k, const Vec3& v); // 0x420430 (cdecl, hidden return pointer)

// The world object the sky traces against (another module).
class SkyWorld;
float TraceRay(SkyWorld* world, const Vec3* from, const Vec3* to); // 0x4bb260 (cdecl)

// The five layer parameters from the stage ini, passed by value to AddLayer.
struct SkyParams {
    float scale;                       // 0x00: sprite scale
    float alphaExponent;               // 0x04
    float offsetGain;                  // 0x08: view offset gain
    float parallax;                    // 0x0c: screen parallax
    float rotationGain;                // 0x10
};

// A 4x4 float matrix whose (empty) constructor and destructor live in another translation unit: the
// file-static instance at 0x561b08 gets thunks that jump to the shared empty functions 0x490ad0 / 0x402c90.
class SkyMatrix {
public:
    SkyMatrix();
    ~SkyMatrix();
    float m[16];
};

// One cloud layer: a Material plus its parameters.
class SkyLayer : public Sprite {
public:
    float param0;                      // 0xd4
    float param1;                      // 0xd8
    float param2;                      // 0xdc
    float param3;                      // 0xe0
    float param4;                      // 0xe4
};

class Sky {
public:
    Sky();                             // 0x4758f0
    ~Sky();                            // 0x475a10
    void SetBase(const char* path, TextureLoader* loader, float scale); // 0x475a70
    void AddLayer(const char* path, TextureLoader* loader, float p0, float p1, float p2, float p3, float p4); // 0x475b20
    void Render(SkyDevice* device, Camera* camera); // 0x475c00
    void Update(SkyDevice* device, Camera* camera, SkyWorld* world, float dt); // 0x475eb0

    SkyLayer layers[16];               // 0x000
    int layerCount;                    // 0xe80
    int hasBase;                       // 0xe84
    Sprite base;                       // 0xe88
    float scale;                       // 0xf5c
    Vec3 direction;                    // 0xf60: set by the stage loader
    float visibility;                  // 0xf6c
};

#endif
