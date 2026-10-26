/* Ballistic car particle emitter CarEmitterC (0x47b4c0-0x47c3cf), embedded in a
 * Car at +0x2488 (vtable 0x4e2260, 0x160 bytes). Map: analysis/range-478870.md.
 * Include after rallytrophy.h, unk_481390.h and unk_47c460.h. */
#ifndef UNK_47B4C0_H
#define UNK_47B4C0_H

// A 4x4 float matrix with an (empty) destructor; Render builds the view matrix of a camera
// lens in one (unwind entries at ebp-0x1ec and ebp-0xe8).
class ViewMat {
public:
    ViewMat() {}
    ~ViewMat() {}
    void SetInverseOf(const ViewMat& src); // 0x472c90: transposes the 3x3 part, copies the rest
    void Multiply(const float* a, const ViewMat& b); // 0x4b5540: this = a * b
    void Transform(const Vec3* v, Vec3* out) const // point * matrix (row vectors, translation in m[12..14])
    {
        *out = Vec3(v->x * m[0] + v->y * m[4] + v->z * m[8] + m[12],
                    v->x * m[1] + v->y * m[5] + v->z * m[9] + m[13],
                    v->x * m[2] + v->y * m[6] + v->z * m[10] + m[14]);
    }
    float m[16];
};

class CarEmitterC : public Effect {
public:
    CarEmitterC();                     // 0x47b4c0
    ~CarEmitterC();                    // 0x47b550 (scalar deleting dtor 0x47b530)
    void Update(float dt);             // 0x47b560
    virtual void Render(RenderDevice* device, Camera* camera); // 0x47b730
    void Emit(const Vec3* origin, const Vec3* dir, float scale, float age); // 0x47bf80
    void SpawnOne(float cost, float scale, float age); // 0x47c1a0

    Vec3 axisA;                        // 0x12c: unit vector perpendicular to dir
    Vec3 axisB;                        // 0x138: dir x axisA
    Vec3 dir;                          // 0x144: emission direction
    Vec3 origin;                       // 0x150: emission origin
    float budget;                      // 0x15c: emission budget, 0.1 after Emit
};

#endif
