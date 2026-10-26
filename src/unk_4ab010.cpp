// Image format conversion (0x4ab010): decodes to 32-bit via 0x4ab910, repacks to the requested format.
#include <string>
#include <stdexcept>
#include <string.h>
#include "unk_4ab010.h"

extern char* PTR_s_Out_of_memory_0050216c;
extern unsigned int* __fastcall FUN_004ab910(ImgW5* img, int unused);
extern void* __cdecl PoolAlloc(unsigned int bytes); // 0x4c1276 (operator new)
extern void __cdecl PoolFree(void* p);              // 0x4bd000 (operator delete)

static void* AllocOrThrow(unsigned int n)
{
    void* p = PoolAlloc(n);
    if (!p) throw std::logic_error(std::string(PTR_s_Out_of_memory_0050216c));
    return p;
}

// NONMATCHING: RALLYTROPHY 0x4ab010
// original: FUN_004ab010
// reason: first draft; inlined throw/string construction and loop scheduling unverified
void __fastcall ConvertImageFormat(ImgW5* img, int edx, unsigned int fmt)
{
    if (fmt == (unsigned int)img->format) return;
    unsigned int* src = FUN_004ab910(img, edx);
    PoolFree(img->pixels); // guess: extraout_ECX args; likely the old pixel/palette buffers
    PoolFree(img->palette);
    img->palette = 0;
    img->pixels = 0;
    int n = img->height * img->width;
    unsigned int* s = src;
    if (fmt == 1) {
        img->format = 1; img->bytesPerPixel = 2; img->bpp = 16;
        unsigned short* d = (unsigned short*)AllocOrThrow(n * 2);
        img->pixels = (unsigned char*)d;
        for (; n > 0; n--) {
            unsigned int v = *s++;
            *d++ = (unsigned short)((v >> 9) & 0x7c00 | (v >> 6) & 0x3e0 | (v >> 3) & 0x1f);
        }
        PoolFree(s);
    } else if (fmt == 2) {
        img->format = 2; img->bytesPerPixel = 2; img->bpp = 16;
        unsigned short* d = (unsigned short*)AllocOrThrow(n * 2);
        img->pixels = (unsigned char*)d;
        for (; n > 0; n--) {
            unsigned int v = *s++;
            *d++ = (unsigned short)((v >> 8) & 0xf800 | (v >> 5) & 0x7e0 | (v >> 3) & 0x1f);
        }
        PoolFree(s);
    } else if (fmt == 3) {
        img->format = 3; img->bytesPerPixel = 3; img->bpp = 24;
        unsigned char* d = (unsigned char*)AllocOrThrow(n * 3);
        img->pixels = d;
        for (; n > 0; n--) {
            d[0] = ((unsigned char*)s)[0];
            d[1] = ((unsigned char*)s)[1];
            d[2] = ((unsigned char*)s)[2];
            d += 3; s++;
        }
        PoolFree(s);
    } else if (fmt == 5) {
        img->format = 5; img->bytesPerPixel = 2; img->bpp = 16;
        unsigned short* d = (unsigned short*)AllocOrThrow(n * 2);
        img->pixels = (unsigned char*)d;
        for (; n > 0; n--) {
            unsigned int v = *s++;
            *d++ = (unsigned short)((v >> 6) & 0x3e0 | (v >> 3) & 0x1f | (v >> 16) & 0x8000 | (v >> 9) & 0x7c00);
        }
        PoolFree(s);
    } else if (fmt == 6) {
        img->format = 6; img->bytesPerPixel = 2; img->bpp = 16;
        unsigned short* d = (unsigned short*)AllocOrThrow(n * 2);
        img->pixels = (unsigned char*)d;
        for (; n > 0; n--) {
            unsigned int v = *s++;
            *d++ = (unsigned short)((v >> 16) & 0xf000 | (v >> 12) & 0xf00 | (v >> 8) & 0xf0 | (v >> 4) & 0xf);
        }
        PoolFree(s);
    } else if (fmt == 10) {
        int h = img->height;
        int rowBytes = (img->width + 7) >> 3;
        img->format = 10; img->bytesPerPixel = 1; img->bpp = 1;
        unsigned char* d = (unsigned char*)AllocOrThrow(h * rowBytes);
        img->pixels = d;
        memset(d, 0, h * rowBytes);
        for (int y = img->height; y > 0; y--) {
            for (int x = 0; x < img->width; x++) {
                if (*s++ & 0x80000000)
                    d[x >> 3] |= (unsigned char)(1 << (x & 7));
            }
            d += rowBytes;
        }
        PoolFree(src);
    } else if (fmt == 0) {
        img->format = 0; img->bytesPerPixel = 1; img->bpp = 8;
        unsigned char* d = (unsigned char*)AllocOrThrow(n);
        img->pixels = d;
        for (; n > 0; n--) *d++ = (unsigned char)(*s++ >> 8);
        unsigned char* p = (unsigned char*)PoolAlloc(0x300);
        img->palette = p;
        for (int i = 0; i < 256; i++, p += 3) p[0] = p[1] = p[2] = (unsigned char)i;
        PoolFree(src);
    } else {
        img->format = (fmt == 7) ? 7 : 4;
        img->bytesPerPixel = 4;
        img->bpp = 32;
        img->pixels = (unsigned char*)src;
    }
    img->stride = (img->bpp * img->width + 7) >> 3;
}
