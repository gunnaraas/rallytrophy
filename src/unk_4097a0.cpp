// BinkVideo constructor (0x4097a0): opens the video, picks a texture split for 320x240 / 512x384 and builds the surface.
#include <string>
#include <stdexcept>
#include "rallytrophy.h"
#include "unk_481390.h"
#include "unk_4097a0.h"

typedef int (__stdcall *BinkFnA)(int);
typedef int (__stdcall *BinkFnB)(void*, int);
typedef int (__stdcall *BinkFnC)(void*, void*);
extern void* DAT_0059b2f8; // Bink import slot
extern int DAT_00501448;   // volume setting (0..100 scaled)
#define F(T, o) (*(T*)((BYTE*)this + (o)))

// NONMATCHING: RALLYTROPHY 0x4097a0
// reason: first draft; exception handling (std::logic_error vs original string-exception class 0x4e2c78) and new-expression scheduling unverified
BinkVideo::BinkVideo(const char* name, RenderDevice* device, int flags)
{
    F(int, 0xa8) = 0;
    F(int, 0xc) = 0;
    F(int, 0xa0) = 0;
    int bf = 0;
    if (flags == 0) {
        ((BinkFnA)DAT_0059b2f8)(-1);
        bf = 0x4000;
    }
    ((BinkFnA)DAT_0059b2f8)(0x400000);
    void* h = (void*)((BinkFnB)DAT_0059b2f8)((void*)name, bf | 0x1000000);
    F(void*, 0x9c) = h;
    if (h == 0) {
        throw std::logic_error("Cannot open video");
    }
    if (flags != 0) {
        ((BinkFnB)DAT_0059b2f8)(h, (int)(((__int64)DAT_00501448 << 15) / 100));
    }
    ((BinkFnC)DAT_0059b2f8)(h, (BYTE*)this + 0x20);
    int width = F(int, 0x20);
    int height = F(int, 0x24);
    VidDevice* dev = *(VidDevice**)((BYTE*)device + 0x20);
    F(int, 4) = width;
    F(int, 8) = height;
    int fmt = dev->PickFormat(4, 0);
    F(int, 0) = fmt;
    if (fmt != 4) {
        fmt = dev->PickFormat(2, 6);
        F(int, 0) = fmt;
    }
    if (width == 0x140 && height == 0xf0) {
        F(int, 0xc) = 2;
        Sprite* s0 = new Sprite;
        F(Sprite*, 0x10) = s0;
        F(Sprite*, 0x14) = new Sprite;
        VidImage* img = new VidImage(0x100, 0x100, F(int, 0));
        s0->texture = dev->MakeTextureFromImage(new Texture("AviLeft"), img, 0x15, 0);
        if (img) {
            img->~VidImage();
            operator delete(img);
        }
        img = new VidImage(0x40, 0x100, F(int, 0));
        F(Sprite*, 0x14)->texture = dev->MakeTextureFromImage(new Texture("AviRight"), img, 0x15, 0);
        if (img) {
            img->~VidImage();
            operator delete(img);
        }
    } else if (width == 0x200 && height == 0x180) {
        F(int, 0xc) = 4;
        Sprite* s0 = new Sprite;
        F(Sprite*, 0x10) = s0;
        F(Sprite*, 0x14) = new Sprite;
        F(Sprite*, 0x18) = new Sprite;
        F(Sprite*, 0x1c) = new Sprite;
        int f = F(int, 0);
        s0->texture = dev->MakeTexture(new Texture("avi0"), f, 0x100, 0x100, 0x15);
        F(Sprite*, 0x14)->texture = dev->MakeTexture(new Texture("avi1"), f, 0x100, 0x100, 0x15);
        F(Sprite*, 0x18)->texture = dev->MakeTexture(new Texture("avi2"), f, 0x100, 0x80, 0x15);
        F(Sprite*, 0x1c)->texture = dev->MakeTexture(new Texture("avi3"), f, 0x100, 0x80, 0x15);
    } else {
        throw std::logic_error("Unsupported video resolution");
    }
    F(void*, 0xa0) = dev->MakeSurface(new Texture("bink"), F(int, 0), width, F(int, 0x24), 5);
    int surf;
    switch (F(int, 0)) {
    case 1: case 5: surf = 9; break;
    case 2: surf = 10; break;
    case 3: surf = 1; break;
    case 4: case 7: surf = 3; break;
    case 6: surf = 7; break;
    default: throw std::logic_error("Invalid video format");
    }
    F(int, 0xa4) = surf;
    for (int i = 0; i < F(int, 0xc); i++) {
        BYTE* s = F(BYTE*, 0x10 + i * 4);
        *(int*)(s + 0x9c) = 0xe;
        *(int*)(s + 0x98) = 0xf;
        *(float*)(s + 0x60) = 1.0f;
        *(float*)(s + 0x64) = 1.0f;
        *(float*)(s + 0x68) = 1.0f;
        *(float*)(s + 0x6c) = 1.0f;
    }
}
