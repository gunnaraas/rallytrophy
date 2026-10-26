/* WaterAnimator (0x46fa80-0x46fdbb, vtable 0x4e1fac): cycles the texture of a Stage water mesh
 * through the 32-entry texture table of the Stage and scrolls two texture coordinate layers.
 * Map: analysis/range-46d830.md section 7.
 * Include after rallytrophy.h, unk_481390.h, unk_47c460.h and unk_476490.h. */
#ifndef UNK_46FA80_H
#define UNK_46FA80_H

// The part of the Stage the animator reads: the texture handle table at +0x884.
class WaterOwner {
public:
    BYTE unk0[0x884];
    Texture* textures[32];             // 0x884
};

class WaterAnimator : public StageAnimator {
public:
    WaterAnimator(DynamicMesh* mesh, WaterOwner* owner, float rate, Vec2* scale, Vec2* velocity1, Vec2* velocity2); // 0x46fa80 (scalar deleting dtor 0x46fb70)
    ~WaterAnimator();                  // 0x46fb90
    virtual void Update();             // 0x46fc10

    float rate;                        // 0x08: texture frames per 2048 ms-ticks
    Vec2 scale;                        // 0x0c: uv scale of layer 1 (x) and layer 2 (y)
    Vec2 velocity1;                    // 0x14
    Vec2 velocity2;                    // 0x1c
    Vec2* saved;                       // 0x24: the mesh's original (u, v) per vertex
    DynamicMesh* mesh;                 // 0x28
    WaterOwner* owner;                 // 0x2c
};

#endif
