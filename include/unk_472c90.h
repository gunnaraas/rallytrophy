/* Rain weather effect (0x472c90-0x47419f, vtable 0x4e1ff8, 0x7c bytes), owned by the Stage
 * (case weather 1 of the Stage constructor). Map: analysis/range-46fdf0.md section 3.
 * Include after rallytrophy.h, unk_481390.h, unk_47c460.h and unk_474c50.h. */
#ifndef UNK_472C90_H
#define UNK_472C90_H

#include <vector>

// A vertex of a collision triangle (position at +0x04).
struct CollVertex {
    int unk0;
    Vec3 pos;                          // 0x04
};

// A collision triangle of the track surface, as far as the rain reads it.
struct CollTri {
    BYTE unk0[0x18];
    Vec3 normal;                       // 0x18
    BYTE unk24[4];
    CollVertex* v0;                    // 0x28
    CollVertex* v1;                    // 0x2c
    CollVertex* v2;                    // 0x30
};

// std::vector<CollTri*> whose erase(first, last) the original calls out of line (0x484380).
class TriVector : public std::vector<CollTri*> {
public:
    void Clear() { Erase(begin(), end()); }
    iterator Erase(iterator first, iterator last);
};

// A sphere (centre + radius) handed to the track query.
struct QuerySphere {
    Vec3 center;                       // 0x00
    float radius;                      // 0x0c
};

// The track's cell grid (the Update argument b): collects the surface triangles near a point
// into a vector (0x411520, thiscall, ret 8).
class TrackGrid {
public:
    void CollectNear(TriVector* out, QuerySphere* sphere); // 0x411520
};

// One rain splash on the ground (0x34 bytes; ctor 0x490ad0 is empty).
struct RainSplash {
    RainSplash() {}                    // empty ctor/dtor: the array goes through the EH vector iterators
    ~RainSplash() {}
    int delay;                         // 0x00: ms until the next respawn; the low byte doubles as the alpha
    Vec3 pos;                          // 0x04
    Vec3 axisA;                        // 0x10: normal x tangent
    Vec3 axisB;                        // 0x1c: unit tangent
    Vec3 normal;                       // 0x28
};

// A plain point (no ctor/dtor: Vec3 members would add EH states to the RainEffect dtor).
struct QueryPoint {
    float x, y, z;
};

class RainEffect : public WeatherEffect {
public:
    RainEffect(TextureLoader* loader, int drops, int splashes); // 0x472d00
    virtual ~RainEffect();             // 0x4732f0 (scalar deleting dtor 0x4732d0)
    virtual void Update(RenderDevice* device, Camera* camera, int a, int b, float dt); // 0x473470
    void InitDrops();                  // 0x4733d0

    Sprite* material;                  // 0x04
    Mesh* dropMesh;                    // 0x08
    Mesh* splashMesh;                  // 0x0c
    int dropCount;                     // 0x10
    int dropDrawCount;                 // 0x14: drops processed per update
    int splashCount;                   // 0x18
    int splashDrawCount;               // 0x1c: splashes processed per update
    int* drops;                        // 0x20: 3 ints (16-bit fixed point x, y, z) per drop
    RainSplash* splashes;              // 0x24
    float threshold;                   // 0x28: minimum view depth of a drawn drop (1.0)
    Vec3 velocity;                     // 0x2c: rain velocity (0, 30, 0)
    float boxSize;                     // 0x38: edge of the weather box (40.0)
    float streakWidth;                 // 0x3c: width of a drop streak (0.03)
    float splashSizeOld;               // 0x40: splash half-size at age 255 (0.025)
    float splashSizeNew;               // 0x44: splash half-size at age 0 (0.085)
    std::vector<CollTri*> tris;        // 0x48 (plain member: a derived vector type adds EH states to the dtor; use Tris() for the out-of-line erase): triangles near the camera (allocator byte, begin 0x4c, end 0x50, cap 0x54)
    BYTE unk58[4];
    QueryPoint queryMin;                     // 0x5c: query box around the camera (written by Update)
    QueryPoint queryMax;                     // 0x68
    QuerySphere* querySphere;          // 0x74: points at a stack local of Update
    int queryKind;                     // 0x78: 0x11 after the constructor, 0xf after Update
    TriVector* Tris() { return (TriVector*)&tris; }
};

#endif
