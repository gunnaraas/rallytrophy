#pragma once
// Sky ray tracing (cdecl), range 0x4bb260-0x4bbdef. Map: analysis/range-4b7a40.md.
class SkyWorld;
class Vec3Slot;
struct SkyShadeMap {                   // object at (*obj + 0x58): 1-bit-per-texel map
    int width, height, pitch;          // 0x00, 0x04, 0x08
    int unkc[3];
    unsigned char* bits;               // 0x18
};
struct SkyTexOwner {                   // param_1 of the sample functions
    struct { char unk0[0x58]; SkyShadeMap* map; }* obj; // 0x00 (map at +0x58 of the pointee)
    int unk4[0x26];
    unsigned flags;                    // 0x9c (param_1[0x27]): 4 = clamp u, 8 = clamp v
};
float TraceRay(SkyWorld* world, const float* from, const float* to);                          // 0x4bb260
int SkySampleBit(SkyTexOwner* t, const unsigned short* tri, int verts, float* point);        // 0x4bba10
float SkySampleBilinear(SkyTexOwner* t, const unsigned short* tri, int verts, float* point);  // 0x4bbb30
