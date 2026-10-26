#ifndef UNK_4AF950_H
#define UNK_4AF950_H
#include "unk_4af230.h"
#include <string>
#include <vector>
// ModelFile (.bmf): layout inferred from 0x4af950/0x4afe20; see analysis/range-4a9b60.md
class ModelFile {
public:
    ~ModelFile();                  // 0x4afe20
    void ReadMaterialList(File*);  // 0x4afec0 (not drafted)
    void ReadTrack(File*);         // 0x4b0920
    void ReadHierarchy(File*);     // 0x4b0cd0
    void ReadBlock(File*, unsigned int*); // 0x4b0a30 (not drafted)
    void ReadLight(File*);         // 0x4b1640
    void ReadMesh(File*);          // 0x4b0e90
    void ReadCamera(File*);        // 0x4b18b0
    void ReadCamera2(File*);       // 0x4b19b0
    int m_pad0[2];
    std::string m_str08;           // 0x08 (+0xc ptr)
    std::vector<void*> m_v18;        // 0x18
    std::vector<void*> m_v28;        // 0x28
    std::vector<void*> m_v38;        // 0x38
    std::vector<void*> m_v48;        // 0x48
    Vector3 m_t58, m_t64, m_t70;   // 0x58, 0x64, 0x70
};
#endif
