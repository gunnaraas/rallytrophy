#pragma once
#include "unk_4aaca0.h"
#include "unk_4aa430.h"

// Method-only views extend existing layouts without changing shared declarations.
class BitmapOps : public VidImage {
public:
    void Downsample(int xShift, int yShift, int outputFormat); // 0x4acd60
    void Blit(VidImage* source, int x, int y, int sourceX, int sourceY, int w, int h); // 0x4ad110
};
class RenderDevice;
class WrappedGlyphFont : public GlyphFont {
public:
    void PrintWrapped(RenderDevice* device, float x, float y,
                      const unsigned short* text, int flags, float sx, float sy); // 0x4aa690
    void PrintWide(RenderDevice* device, float x, float y,
                   const unsigned short* text, int flags, float sx, float sy); // 0x4a97f0
    unsigned color;             // 0x120
    char unknown124[0xc];
    char wrapping;              // 0x130
    char unknown131[3];
    float wrapWidth;            // 0x134
};
int FUN_004ad1d0(float*, float*, float*, float*, float*, float*);
