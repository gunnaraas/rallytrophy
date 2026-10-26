/* Car rigid body (Car+0x24, vtable 0x4e140c): ctor 0x41bab0, dtor 0x41bd70, integrator 0x41c010. See analysis/range-409690.md. */
#ifndef UNK_41B9E0_H
#define UNK_41B9E0_H
#include <string>
#include <windows.h>
#include "scene_object.h"

class BodyAttach {                      // object owned at body+0x234 (dtor 0x41b990)
public:
    ~BodyAttach();                      // 0x41b990
    void Update();                      // 0x41b530
};

struct BodyAttachRef {                  // holder whose first member points to a BodyAttach
    BodyAttach* p;
    void Update();                      // 0x41b9e0
};

struct BodyElem {                       // 0x50-byte element of the array at body+0x238 (ctor 0x490ad0)
    BYTE data[0x50];
    BodyElem() {}
    ~BodyElem() {}
};

class ModelBase {                       // base initialiser 0x4a6890, destructor body 0x4a6cb0
public:
    ModelBase();
    virtual ~ModelBase();
    void CopyMeshTransform();           // 0x4a6dc0
    int p4;
    int id;                             // 0x08
    BYTE pad[8];
};

struct IV {                             // plain vector with inline ops (no destructor)
    float x, y, z;
    
    void operator+=(const IV& o) { x = o.x + x; y = o.y + y; z = o.z + z; }
    void Zero() { x = 0; y = 0; z = 0; }
};

inline IV Scale(const IV& v, float k) { IV r; r.x = v.x * k; r.y = v.y * k; r.z = v.z * k; return r; }
inline void ScaleBy(IV& v, float k) { v.x = k * v.x; v.y = k * v.y; v.z = k * v.z; }

struct Mat4 {                           // 4x4 float matrix with the empty destructor (EH frame)
    float m[16];
    Mat4() {}
    ~Mat4() {}
    void Mul(const Mat4& o);            // 0x4b5490
};

struct DV3 {                            // three dwords with the empty destructor
    DWORD x, y, z;
    ~DV3() {}
};

struct FV3 {
    float x, y, z;
    ~FV3() {}
};

struct Mat4Src {                        // 0x14c sub-object, built from the rotation (0x4b5140)
    int v[4];
    ~Mat4Src() {}
    void Init(const Mat4* rot);         // 0x4b5140
};

struct MeshList {                       // mesh container at body+0x124
    int a, b;
    void* At(int i);                    // 0x4a6800
    void Insert(void* mesh, float r);   // 0x4a66e0
};

struct PtRot {
    ~PtRot() {}                          // vector at body+0x218, rotated through 0x4b5850
    float x, y, z;
    ScenePoint Rot(const Mat4* m); // 0x4b5850
};

class CarBody : public ModelBase {
public:
    CarBody(const CarBody* proto, float mass, const DWORD* scaleIn, const DWORD* upIn); // 0x41bab0
    virtual ~CarBody();                 // 0x41bd70
    void SetScale(DV3 s);               // 0x420490 (ScaleBody::SetScale(x, y, z); the caller passes a copy)
    void FUN_0041bed0();                // 0x41bed0: inertia matrices from rotation and scale
    void FUN_0041c540();                // 0x41c540
    void Reset();                       // 0x41be00
    void Integrate();                   // 0x41c010
    void Damp(float a, float b);        // 0x41c220
    static unsigned int PlaneMask(const float* planes, const float* p); // 0x41ba60

    ScenePoint pos;                     // 0x14
    Mat4 rot;                           // 0x20
    BYTE unk60[0x124 - 0x60];
    MeshList mesh;                      // 0x124
    float mass;                         // 0x12c
    float invMass;                      // 0x130
    DV3 scale;                          // 0x134
    FV3 scaleInv;                       // 0x140
    Mat4Src src14c;                     // 0x14c
    Mat4 inertia;                       // 0x15c
    ScenePoint linMom;                  // 0x19c
    ScenePoint angMom;                  // 0x1a8
    ScenePoint g1b4, g1c0;              // 0x1b4, 0x1c0
    ScenePoint vel;                     // 0x1cc
    ScenePoint angVel;                  // 0x1d8
    ScenePoint force;                   // 0x1e4
    ScenePoint torque;                  // 0x1f0
    ScenePoint force2;                  // 0x1fc
    ScenePoint torque2;                 // 0x208
    float dt;                           // 0x214
    PtRot up;                           // 0x218
    ScenePoint centre;                  // 0x224
    DWORD d230;
    BodyAttach* attach;                 // 0x234
    BodyElem* elems;                    // 0x238
    DWORD d23c;
    DV3 d240;                           // 0x240
    DWORD d24c;
    ScenePoint g250, g25c;              // 0x250, 0x25c
    DWORD d268;
};
#endif
