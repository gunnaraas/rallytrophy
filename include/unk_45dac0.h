/* Car::SunFactor (0x45dac0): ray test of the car's reference point against the triangle lists of a model
 * container, shade from the vertex colour bytes. Map: analysis/range-459890.md section 3.
 * Include after unk_481390.h; Car only gets the method declaration. */
#ifndef UNK_45DAC0_H
#define UNK_45DAC0_H

// One 0x20-byte triangle record of a model's triangle list: plane normal, plane offset, bounding sphere.
struct SunTri {
    float nx;                          // 0x00
    float ny;                          // 0x04
    float nz;                          // 0x08
    float d;                           // 0x0c
    float cx;                          // 0x10: bounding sphere centre
    float cy;                          // 0x14
    float cz;                          // 0x18
    float radius;                      // 0x1c
};
// A 0x2c-byte mesh vertex; only the shade byte is used.
struct SunVertex {
    BYTE unk0[0x19];
    BYTE shade;                        // 0x19
    BYTE unk1a[0x2c - 0x1a];
};
// A mesh part: vertices, 16-bit index triples, triangle count.
struct SunPart {
    BYTE unk0[4];
    SunVertex* vertices;               // 0x04
    unsigned short* indices;           // 0x08
    BYTE unkc[2];
    unsigned short count;              // 0x0e
};
struct SunMesh {
    BYTE unk0[0x34];
    SunPart** parts;                   // 0x34
};
// The mesh list at model+0x124: the triangle lists (a std::vector of record arrays) sit at +4.
struct SunMeshList {
    int unk0;
    std::vector<SunTri*>* tris;        // +4
    SunMesh* At(int index);            // 0x4a6800
};
struct SunModel {
    BYTE unk0[4];
    int kind;                          // 0x04
    BYTE unk8[4];
    unsigned flags;                    // 0x0c
    BYTE unk10[0xb8 - 0x10];
    float centreX;                     // 0xb8
    float unkbc;
    float centreZ;                     // 0xc0
    float radius;                      // 0xc4
    BYTE unkc8[4];
    unsigned flagsCc;                  // 0xcc
    BYTE unkd0[0x124 - 0xd0];
    SunMeshList meshes;                // 0x124
};
// The model container passed in (pointer array at 0x124, count at 0x12c).
struct SunModels {
    BYTE unk0[0x124];
    SunModel** models;                 // 0x124
    BYTE unk128[4];
    int count;                         // 0x12c
};

#endif
