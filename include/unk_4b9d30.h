// RenderBody::Update (0x4b9d30): projects the shadow texture coordinates of a body's meshes.
#ifndef UNK_4B9D30_H
#define UNK_4B9D30_H
#include <vector>

struct ShadowVertex {              // 0x2c bytes
    float pos[3];                  // 0x00
    float normal[3];               // 0x0c
    float unk18[3];
    float u, v;                    // 0x24, 0x28
};
struct ShadowMesh {
    unsigned char unk0[4];
    ShadowVertex* verts;           // 0x04
    unsigned char unk8[4];
    unsigned short count;                    // 0x0c
};
// 4x4 matrix with an (empty) destructor, as the original's locals have.
struct Mat4Local {
    Mat4Local() {}
    ~Mat4Local() {}
    void SetInverseOf(const float* src);              // 0x472c90
    void Multiply(const float* a, const Mat4Local& b); // 0x4b5540
    float m[16];
};
// Position and matrices of a body / caster (see RenderCaster in unk_469630.h).
struct ShadowBody {
    unsigned char unk0[0x14];
    float pos[3];                  // 0x14
    float matA[16];                // 0x20
    float matB[16];                // 0x60
};
struct ShadowUpdater {
    std::vector<ShadowMesh*> meshes;   // 0x00 (allocator), begin 0x04, end 0x08
    void Update(ShadowBody* body, ShadowBody* cam, const float* centre, float radius); // 0x4b9d30
};
#endif
