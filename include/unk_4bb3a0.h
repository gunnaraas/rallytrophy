// Swept-sphere trace of a scene node against its model triangles (0x4bb3a0).
#pragma once
#include <string>
#include <vector>
#include "scene_object.h"
#include "unk_4ba440.h"

struct TraceMeshPart { void* model; SceneVertex* verts; unsigned short* tris; int pad; unsigned short pad0c; unsigned short nTris; };
struct TraceMeshSet { BYTE pad[0x34]; TraceMeshPart** parts; };
class TraceMeshOwner { public: TraceMeshSet* At(int i); };   // original: FUN_004a6800
class TraceNode : public SceneNode {
public:
    BYTE pad124[4];                    // 0x124 (mesh owner, passed as 'this' to 0x4a6800)
    std::vector<float*>* triData;      // 0x128
};
float __cdecl TraceNodeSweep(TraceNode* n);   // original: FUN_004bb3a0
