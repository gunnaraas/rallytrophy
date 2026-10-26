// Types for src/unk_4097a0.cpp (BinkVideo constructor 0x4097a0).
#ifndef UNK_4097A0_H
#define UNK_4097A0_H
#include <windows.h>
#include "unk_481390.h"

// Raw pixel-buffer helper (0x20 bytes; ctor 0x4aad10, dtor 0x4aafa0).
class VidImage {
public:
    VidImage(int width, int height, int format); // 0x4aad10
    ~VidImage();                                 // 0x4aafa0
    BYTE unk[0x20];
};

// Device object found at RenderDevice+0x20; only the vtable slots used by the video ctor are named.
class VidDevice {
public:
    virtual void s0();
    virtual void s1();
    virtual int PickFormat(int a, int b);                                        // +0x08
    virtual void s3();
    virtual Texture* MakeTextureFromImage(Texture* t, VidImage* img, int fmt, int z); // +0x10
    virtual void s5();
    virtual void s6();
    virtual Texture* MakeTexture(Texture* t, int fmt, int w, int h, int usage); // +0x1c
    virtual void* MakeSurface(Texture* t, int fmt, int w, int h, int usage);    // +0x20
};
#endif
