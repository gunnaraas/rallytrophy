#pragma once
// VidImage-like raw pixel buffer (0x20 bytes); layout per range-4a9b60.md.
class VidImage {
public:
    int width;      // 0
    int height;     // 4
    int pitch;      // 8
    int bits;       // 0xc
    int bpp;        // 0x10 (bytes per pixel)
    int format;     // 0x14
    unsigned char* data;     // 0x18
    unsigned char* palette;  // 0x1c (0x300 bytes)

    void Sub4abf30(int a);
    void Base();                                  // 0x48e660 (empty, folded)
    VidImage* Init0();                            // 0x4aaca0
    VidImage* Init1(int a);                          // 0x4aacd0
    VidImage* Create(int w, int h, int fmt);      // 0x4aad10
    VidImage* Copy(VidImage* src, char convert);  // 0x4aae80
    void Free();                                  // 0x4aafa0
    void Clear();                                 // 0x4aafc0
};
