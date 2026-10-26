// Helper types for Stage::Stage (0x40c9b0). Layouts inferred from pseudocode/asm.
#ifndef UNK_40C9B0_H
#define UNK_40C9B0_H
#include <windows.h>
#include <vector>
#include <string>

class Sprite;
struct StTexCache {                    // Stage+0xd4 (0x20 bytes)
    StTexCache();                      // 0x4aee00
    void SetDetail(int level);         // 0x4aeec0
    BYTE d[0x20];
};
struct StModelCache {                  // Stage+0xf4 (0x3c bytes)
    StModelCache();                    // 0x4ade40
    void Preload(std::vector<void*> objs); // 0x4ae070 (vector passed by value, callee pops)
    void Release(void* obj);           // 0x4ae0e0
    BYTE d[0x3c];
};
struct StModelFile {                   // loaded .bmf (0x80 bytes, ctor 0x4af950, dtor 0x4afe20)
    StModelFile(const char* path, const char* dir, StTexCache* cache, void* materials, int flag);
    ~StModelFile();
    BYTE a[0x28];
    std::vector<void*> v28;            // 0x28 parts
    std::vector<void*> v38;            // 0x38
    std::vector<void*> v48;            // 0x48 objects
    float bounds[9];                   // 0x58 .. 0x7c
};
struct StCityLight {                   // 0x16c bytes
    StCityLight(void* src, StTexCache* tc, Sprite* a, Sprite* b); // 0x46eec0
    BYTE d[0x16c];
};
struct StColTree { void Build(int minTris); };   // 0x41b730
struct StColModel {                    // 0x34 bytes
    StColModel(void* part, int a);     // 0x41b7f0
    void Update();                     // 0x41b9e0
    StColTree* tree;
    BYTE d[0x30];
};
struct StBody {                        // car rigid body (0x284 bytes)
    StBody(void* proto, float mass, const float* scale, const float* up); // 0x41bab0
    void SetCollision2(void* part);    // 0x41c990
    void SetCollision3(void* part);    // 0x41ca00
    BYTE pad[0x14];
    float pos[3];                      // 0x14
};
struct StSign {                        // 0x68 bytes: sign / checkpoint-block object
    StSign();                          // 0x40a680
    void Sync();                       // 0x40a7f0
    float pos[3];                      // 0x00
    float mat[16];                     // 0x0c
    float f4c;                         // 0x4c
    StBody* body;                      // 0x50
    BYTE pad[0x68 - 0x54];
};
struct StGridEntry {                   // 8 bytes
    StGridEntry(void* node);           // 0x4147f0
    int d[2];
};
struct StVirt {                        // scene object: slot 0 dtor, slot 7 prepare
    virtual ~StVirt();
    virtual void s1(); virtual void s2(); virtual void s3(); virtual void s4();
    virtual void s5(); virtual void s6();
    virtual void Prepare();            // slot 7 (+0x1c)
};
struct StMeshList { void* At(int i); };           // 0x4a6800
struct StSceneObj { void Place(float* m); };      // 0x4a7b80
struct StSceneObj2 { void Refresh(); };           // 0x4a6d30
struct StShow { void Show(); };                   // 0x4b2b90
struct StStageRaw {                    // Stage methods of other modules
    void LoadConfig(const char* path); // 0x411600
    void* Cell(float* pos);            // 0x411080
    void AddSplit(void* p);            // 0x4150b0
    void AddStart(void* p);            // 0x414fe0
    void F414b20();
    void F416260();
    void Build();                      // 0x40fcc0
    void Fill();                       // 0x40f390
    void ScanEffects();                // 0x410380
    void F415370();
    void SetTextures(const std::string* dir); // 0x4137c0
    void LoadAnimals();                // 0x416f30
    void Finish();                     // 0x417430
};
struct StAiPath { StAiPath(); void Load(const char* path); };   // 0x49f910 / 0x49faf0
#endif
