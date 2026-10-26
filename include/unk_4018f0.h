// Replay/ghost record list and mesh-set helpers (0x4018f0-0x4021b0).
#ifndef UNK_4018F0_H
#define UNK_4018F0_H
#include <windows.h>

// Buffered file object (ctor 0x4a4d40, dtor 0x4a4d90); size from the 0x4078-byte frame of 0x401920.
class RecFile {
public:
    RecFile();                         // 0x4a4d40
    ~RecFile();                        // 0x4a4d90
    bool Open(const char* name, int mode); // 0x4a4f50
    void Close();                      // 0x4a5d00
    void Write(const void* p, int n);  // 0x4a5e90
    void Read(void* p, int n);         // 0x4a5e00
    BYTE b[0x4078];
};

struct RecNode { BYTE b[0x5c]; RecNode* next; };

// Replay record list (embedded in the Car; the existing ReplayRecorder in unk_4654f0.h has the same methods).
class RecList {
public:
    void Stop();                       // 0x4013b0
    void Reset();                      // 0x401880
    RecNode* Alloc();                  // 0x4018d0
    void Append(RecNode* n);           // 0x4018f0
    BOOL Save(const char* name);       // 0x401920
    BOOL WriteTo(RecFile* f);          // 0x4019e0
    BOOL Load(const char* name);       // 0x401a40
    BOOL ReadFrom(RecFile* f);         // 0x401b00
    BYTE unk0[0x10];
    RecNode* head;                     // 0x10
    RecNode* tail;                     // 0x14
    BYTE unk18[8];
    int count;                         // 0x20
    int extra;                         // 0x24
    BYTE unk28[4];
};

// Interpolated mesh set (0x20 bytes): ctor 0x401c10, dtor 0x401c30, loader 0x401cf0, SetAngle 0x4021b0.
struct MeshVec { float f[3]; ~MeshVec() {} };      // empty dtor 0x402c90
struct MeshElem { float* dst; MeshVec* a; MeshVec* b; }; // dst: 6 floats (position, normal) written by the spline
class MeshSet {
public:
    MeshSet();                         // 0x401c10
    ~MeshSet();                        // 0x401c30 (out-of-line, non-virtual)
    void Interp(float t);              // 0x4021b0
    int loop;                          // 0x00
    int frames;                        // 0x04
    int count;                         // 0x08
    struct Owner { virtual ~Owner(); }* owner; // 0x0c
    MeshElem* elems;                   // 0x10
    MeshVec tint;                      // 0x14
};

// Holder with a heap pointer at +4 (dtor-like 0x401bb0).
struct PtrHolder { int pad; void* p; int b, c; void Release(); };  // 0x401bb0
#endif
