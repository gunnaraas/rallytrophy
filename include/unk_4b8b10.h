// Mesh container / deformation model (0x4b8b10-0x4b9b50). Layout notes from the pseudocode; see analysis/range-4b7a40.md.
#ifndef UNK_4B8B10_H
#define UNK_4B8B10_H
#include <vector>
#include "scene_object.h"

struct MeshMaterial {
    unsigned char pad[0x9c];
    unsigned flags;                    // 0x9c (bit 0x100 selected by 0x4b9b50)
};
struct MeshChunk {                     // element of MeshModel::chunks
    MeshMaterial* material;            // 0x00
    SceneVertex* vertices;             // 0x04
    int pad08;
    unsigned short count;              // 0x0c
};
struct DeformVertex {                  // 0x3c bytes
    int dirty;                         // 0x00
    float weight;                      // 0x04
    SceneVertex* vertex;               // 0x08
    ScenePoint posA, posB;             // 0x0c, 0x18
    ScenePoint nrmA, nrmB;             // 0x24, 0x30
};
struct DeformList {
    DeformVertex* verts;               // 0x00
    int count;                         // 0x04
    int dirty;                         // 0x08
};

class ChunkList {                      // vector<MeshChunk**> at +0 (first at +4)
public:
    ~ChunkList();                      // 0x4b9b20
    void AddFlagged(struct MeshModel* model); // 0x4b9b50
    std::vector<MeshChunk**> items;
};

struct MeshModel {
    SceneBounds bounds;                // 0x00 (min, max, center, radius, radius2, flags)
    std::vector<MeshChunk*> chunks;    // 0x30 (first at 0x34)
    int extraCount;                    // 0x40
    int pad44;
    float* extra;                      // 0x48: extraCount x 3 floats
    int pad4c;
    int dirtyCount;                    // 0x50
    std::vector<DeformList*> deform;   // 0x54 (first at 0x58)

    void ComputeBounds();              // 0x4b8b10
    void ComputeBoundsAndSphere();     // 0x4b8c30
    void Translate(const float* d);    // 0x4b8e50
    void Scale(const float* s);        // 0x4b8ee0
    void Transform(const float* m);    // 0x4b8f70
    void TransformNormals(const float* m); // 0x4b90b0
    void LinkDeformVertices();         // 0x4b9170
    void SetDeformSource(MeshModel* other); // 0x4b91c0
    void Repair();                     // 0x4b9530
    void Refresh();                    // 0x4b95b0
    float* Find(const float* p);       // 0x4b9790
    void Push(const float* p, float a, float b, float c); // 0x4b9830
    void Save(float** out, int* count);// 0x4b99c0
    void Load(const float* src, int count); // 0x4b9a60
};
#endif
