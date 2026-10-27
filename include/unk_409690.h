// Types for src/unk_409690.cpp.
#ifndef UNK_409690_H
#define UNK_409690_H
#include <string>
#include <windows.h>

// Base with a string (0x14 bytes; dtor 0x409690, deleting dtor 0x409700). vtable 0x4e1384.
class NamedBase {
public:
    virtual ~NamedBase() {}
    std::string m_name; // 0x04
};

// Derived (0x2c bytes; dtor 0x409720, deleting dtor 0x4096e0).
class NamedHolder {
public:
    virtual ~NamedHolder();   // 0x409690
    std::string m_name;
};
class NamedDerived : public NamedBase {
public:
    int m_unk14[2];     // 0x14
    std::string m_str2; // 0x1c
    int m_unk2c;
};

// Polymorphic part owned by a video quad holder; the holder's dtor 0x4a86d0 is called out of line.
class VidPoly {
public:
    virtual ~VidPoly();
};
class VidHolder {
public:
    void BaseDtor();   // 0x4a86d0
    VidPoly* a;
};
// Texture slice target (CopyFrom 0x4a8490, dirty flag at 0x50).
class VidTex {
public:
    void CopyFrom(void* source, int x, int y); // 0x4a8490
    BYTE unk0[0x50];
    int dirty;         // 0x50
};
class VidSurface {
public:
    struct Lock_t { int size, width, pitch; BYTE unkc[0x18 - 0xc]; BYTE* bits; };
    Lock_t* Lock();    // 0x4a84c0
    void Unlock();     // 0x4a8500
};
class VidEmpty {
public:
    void Empty();      // 0x48e660 (empty, folded)
};

// 0x54-byte scene transform block (ctor 0x40a680): a position, an identity 4x4 matrix and two spare fields.
// Both unwind through the shared empty dtor 0x402c90.
struct XformVec { float v[3]; XformVec() {} ~XformVec() {} };
struct XformMat { float m[4][4]; XformMat() {} ~XformMat() {} };
class SceneXform {
public:
    SceneXform();           // 0x40a680
    XformVec pos;           // 0x00
    XformMat m;             // 0x0c
    int unk4c;              // 0x4c
    int unk50;              // 0x50
};
#endif
