// Scene entry (0x70 bytes; ctor 0x4b8640, operator= 0x4b8680, dtor 0x4b89f0) and an out-of-line vector<int>
// resize (0x4b83a0). Layout from the pseudocode of those functions.
#ifndef UNK_4B83A0_H
#define UNK_4B83A0_H
#include <vector>

class EntryMesh { public: ~EntryMesh(); };           // 0x4baf90 (copy ctor 0x4baf70 is used by operator=)
struct EntryVec3 { float x, y, z; EntryVec3() {} ~EntryVec3() {} };      // 12 bytes
struct EntryMat { float m[15]; EntryMat() {} ~EntryMat() {} };           // 0x3c bytes
struct EntryMatSet { EntryMat* mats; int count; int extra; };            // 12 bytes

class IntVec : public std::vector<int> {
public:
    void Resize(unsigned int n, const int& x);       // 0x4b83a0
};

class MeshVec : public std::vector<EntryMesh*> {
public:
    unsigned int Size() const;                       // 0x414460
    iterator Erase(iterator f, iterator l);          // 0x484380
    void Ins(iterator pos, int n, EntryMesh* const& x); // 0x4bc680
};
class SetVec : public std::vector<EntryMatSet*> {
public:
    unsigned int Size() const;                       // 0x414460
    iterator Erase(iterator f, iterator l);          // 0x484380
};
struct EntryBase { int head[11]; ~EntryBase() {} };
struct EntryMeshList { char flag2c; MeshVec meshes; EntryMeshList() : meshes() { flag2c = 0; }
    ~EntryMeshList() {} };
class SceneEntry : public EntryBase {
public:
    SceneEntry();                                    // 0x4b8640
    SceneEntry& operator=(const SceneEntry& o);      // 0x4b8680
    ~SceneEntry();                                   // 0x4b89f0
    EntryMeshList ml;                                // 0x2c: flag, meshes at 0x30
    int count40, count44;                            // 0x40 (vertex count), 0x44
    EntryVec3* verts;                                // 0x48
    float* data4c;                                   // 0x4c: count44 * 16 bytes
    int pad50;
    SetVec sets;                  // 0x54
};
#endif
