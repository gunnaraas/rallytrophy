#pragma once
// Range 0x4ba440-0x4bb240: shadow caster hierarchy, view frustum, dynamic mesh, stage animator base.
#include <string>
#include "scene_object.h"

// ShadowCaster hierarchy (SceneNode + 0x54 bytes, vtables 0x4e2a28 / 0x4e2a4c / 0x4e2a70).
struct ShadowDt0 { ~ShadowDt0() {} };
struct ShadowDt1 { ~ShadowDt1() {} };
struct ShadowDt2 { ~ShadowDt2() {} };
class ShadowObject : public SceneNode {
public:
    ShadowObject();                    // original: FUN_004ba440
    void Reset();                      // original: FUN_004ba4a0
    virtual ~ShadowObject();           // original: FUN_004ba5f0 (deleting dtor 0x4ba600)
    int kind;                          // 0x124
    float f128, f12c, f130, f134;
    float f138[4];                     // 0x138
    float f148[4];                     // 0x148
    float f158, f15c, f160, f164, f168, f16c, f170, f174;
    ShadowDt0 d0; ShadowDt1 d1; ShadowDt2 d2;   // empty-destructor members: reproduce the original EH state 3
};
class ShadowObjectB : public ShadowObject {
public:
    ShadowObjectB();                   // original: FUN_004ba580
};
class ShadowCaster : public ShadowObject {
public:
    ShadowCaster();                    // original: FUN_004ba5a0
};

// Render device vtable slots used by the frustum (0xd0 and 0xe8).
class FrustumDevice {
public:
    virtual void V00();
    virtual void V01();
    virtual void V02();
    virtual void V03();
    virtual void V04();
    virtual void V05();
    virtual void V06();
    virtual void V07();
    virtual void V08();
    virtual void V09();
    virtual void V0a();
    virtual void V0b();
    virtual void V0c();
    virtual void V0d();
    virtual void V0e();
    virtual void V0f();
    virtual void V10();
    virtual void V11();
    virtual void V12();
    virtual void V13();
    virtual void V14();
    virtual void V15();
    virtual void V16();
    virtual void V17();
    virtual void V18();
    virtual void V19();
    virtual void V1a();
    virtual void V1b();
    virtual void V1c();
    virtual void V1d();
    virtual void V1e();
    virtual void V1f();
    virtual void V20();
    virtual void V21();
    virtual void V22();
    virtual void V23();
    virtual void V24();
    virtual void V25();
    virtual void V26();
    virtual void V27();
    virtual void V28();
    virtual void V29();
    virtual void V2a();
    virtual void V2b();
    virtual void V2c();
    virtual void V2d();
    virtual void V2e();
    virtual void V2f();
    virtual void V30();
    virtual void V31();
    virtual void V32();
    virtual void V33();
    virtual void SetupFrustum(class ViewFrustum* f);   // slot 0xd0
    virtual void W00();
    virtual void W01();
    virtual void W02();
    virtual void W03();
    virtual void W04();
    virtual float* GetCameraMatrix();                  // slot 0xe8
};

class ViewFrustum {
public:
    void Init(FrustumDevice* device);                            // original: FUN_004ba620
    int TestSphere(const SceneBounds* b, const float* m);        // original: FUN_004ba830
    int TestBounds(const SceneBounds* b);                        // original: FUN_004ba9d0
    int TestBoundsB(const SceneBounds* b);                       // original: FUN_004bab80
    int TestSphereMargin(const SceneBounds* b, const float* m, float margin); // original: FUN_004bad30
    float planes[24];                  // 0x00 normal.xyz, scale at +0xc
    float tplanes[24];                 // 0x60 transformed normal.xyz, d at +0xc
    float* camera;                     // 0xc0
    float radius;                      // 0xc4
    float nearD;                       // 0xc8
    float farD;                        // 0xcc
    int idx[18];                       // 0xd0
};

struct DynMeshObj { virtual ~DynMeshObj(); };
class DynamicMesh {
public:
    DynamicMesh();                                  // original: FUN_004baf40
    DynamicMesh(const DynamicMesh& o);              // original: FUN_004baf70
    DynamicMesh* Copy(const DynamicMesh* o);        // original: FUN_004bafe0
    ~DynamicMesh();                                 // original: FUN_004baf90
    int f0;
    SceneVertex* verts;                // 0x04
    unsigned short* tris;              // 0x08
    unsigned short nVerts;             // 0x0c
    unsigned short nTris;              // 0x0e
    int f10;
    DynMeshObj* obj;                   // 0x14
    float f18, f1c;
    int f20;
    float f24;
    int f28, f2c, f30, f34;
};

class StageAnimatorBase {
public:
    StageAnimatorBase();                            // original: FUN_004bb130
    virtual ~StageAnimatorBase();
    virtual void Slot1() = 0;
    int id;
};

struct SegmentInfo {
    float diff[3];                     // 0x00
    float a[3];                        // 0x0c
    float b[3];                        // 0x18
    float dir[3];                      // 0x24 (normalized b-a)
    float dist;                        // 0x30
    float len;                         // 0x34
    void Update();                                  // original: FUN_004bb170
};
