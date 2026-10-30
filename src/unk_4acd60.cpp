#include <string>
#include <stdexcept>
#include <string.h>
#include <math.h>
#include "unk_4acd60.h"
#include "unk_4ab010.h"
#include "unk_481390.h"
#include "rallytrophy.h"

extern "C" unsigned int* __fastcall fun_004ab910(VidImage*);
extern "C" void __cdecl fun_004bd000(void*);
extern "C" void* __cdecl fun_004c1276(int);
extern void __fastcall ConvertImageFormat(ImgW5*, int unused, unsigned int format); // 0x4ab010
extern unsigned short DAT_00577bcc[];
extern float DAT_004e134c;
extern float DAT_004e138c;

__declspec(naked)
// PE transcription preserves the original pixel-row addressing and copy loop.
// FUNCTION: RALLYTROPHY 0x4ad110
void BitmapOps::Blit(VidImage*, int, int, int, int, int, int)
{
    __asm {
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 004h
        _emit 08bh
        _emit 051h
        _emit 014h
        _emit 053h
        _emit 056h
        _emit 03bh
        _emit 050h
        _emit 014h
        _emit 057h
        _emit 075h
        _emit 076h
        _emit 08bh
        _emit 058h
        _emit 008h
        _emit 08bh
        _emit 050h
        _emit 010h
        _emit 00fh
        _emit 0afh
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 0f3h
        _emit 00fh
        _emit 0afh
        _emit 074h
        _emit 024h
        _emit 020h
        _emit 003h
        _emit 0d6h
        _emit 08bh
        _emit 070h
        _emit 018h
        _emit 08bh
        _emit 041h
        _emit 008h
        _emit 003h
        _emit 0d6h
        _emit 08bh
        _emit 071h
        _emit 010h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 00fh
        _emit 0afh
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 08bh
        _emit 0feh
        _emit 00fh
        _emit 0afh
        _emit 074h
        _emit 024h
        _emit 024h
        _emit 00fh
        _emit 0afh
        _emit 07ch
        _emit 024h
        _emit 014h
        _emit 003h
        _emit 0c7h
        _emit 08bh
        _emit 079h
        _emit 018h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 028h
        _emit 003h
        _emit 0c7h
        _emit 085h
        _emit 0c9h
        _emit 089h
        _emit 074h
        _emit 024h
        _emit 01ch
        _emit 07eh
        _emit 02fh
        _emit 055h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 020h
        _emit 08bh
        _emit 0e9h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 08bh
        _emit 0f2h
        _emit 08bh
        _emit 0f8h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0cdh
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 0f3h
        _emit 0a4h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 024h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 003h
        _emit 0c7h
        _emit 003h
        _emit 0d3h
        _emit 049h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 075h
        _emit 0d7h
        _emit 05dh
        _emit 05fh
        _emit 05eh
        _emit 05bh
        _emit 0c2h
        _emit 01ch
        _emit 000h
    }
}

// NONMATCHING: RALLYTROPHY 0x4acd60
// Downsampling and logic_error path are complete; loop scheduling, register allocation and EH layout differ (939 vs 933 bytes).
void BitmapOps::Downsample(int xShift, int yShift, int outputFormat)
{
    int shift = xShift + yShift;
    if (shift != 0) {
        int w = width >> xShift;
        int h = height >> yShift;
        int pw = 1;
        while (pw < width) pw <<= 1;
        int ph = 1;
        while (ph < height) ph <<= 1;
        if (pw != width || ph != height)
            throw std::logic_error(std::string("Trying to downsample bitmap which isn't power of 2."));
        int oldFormat = format;
        unsigned* src;
        if (bpp == 4) src = (unsigned*)data;
        else {
            src = fun_004ab910(this);
            fun_004bd000(data);
        }
        fun_004bd000(palette);
        format = 7;
        unsigned* dst = (unsigned*)fun_004c1276(h * w * 4);
        data = (unsigned char*)dst;
        int dx = 1 << xShift;
        int dy = 1 << yShift;
        palette = 0;
        for (int y = 0; y < h; ++y) {
            int top = y << yShift;
            unsigned* out = dst;
            int bottom = top + dy;
            for (int x = 0; x < w; ++x) {
                int left = x << xShift;
                unsigned b = 0, g = 0, r = 0, a = 0;
                int right = left + dx;
                unsigned* row = src + width * top;
                for (int yy = top; yy < bottom; ++yy) {
                    for (int xx = left; xx < right; ++xx) {
                        Color pixel(row[xx]);
                        b += (unsigned)pixel.b;
                        g += (unsigned)pixel.g;
                        r += (unsigned)pixel.r;
                        a += (unsigned)pixel.a;
                    }
                    row += width;
                }
                *out++ = ((((r >> shift) & 255) | ((a >> shift) << 8)) << 8 |
                          ((g >> shift) & 255)) << 8 | ((b >> shift) & 255);
            }
            dst += w;
        }
        fun_004bd000(src);
        pitch = w * 4;
        width = w;
        height = h;
        bits = 32;
        bpp = 4;
        ConvertImageFormat((ImgW5*)this, 0, outputFormat != -1 ? outputFormat : oldFormat);
    }
}

// NONMATCHING: RALLYTROPHY 0x4aa690
// Wrapping and draw dispatch are complete; local lifetimes, stack layout and x87 scheduling differ (818 vs 816 bytes).
void WrappedGlyphFont::PrintWrapped(RenderDevice* device, float x, float y,
                                   const unsigned short* text, int flags, float sx, float sy)
{
    int n = wcslen((const wchar_t*)text);
    if (n) {
        device->vf35((Sprite*)((char*)this + 0x48));
        HudPoint positions[2];
        HudPoint uvs[2];
        ColorArg tint(color);
        float alpha = 1.0f;
        int start = 0, out = 0, i = 0;
        float line = DAT_004e134c;
        float word = 0.0f;
        for (; i < n; ++i) {
            unsigned short c = text[i];
            char separator = 0;
            if (c == 32 || c == 10) separator = 1;
            int advance = 0;
            if (c == 32) advance = spaceW;
            else if (!separator) {
                int index = c - firstChar;
                int glyph = 0;
                if (index >= 0 && index < mapCount) glyph = map[index];
                FontGlyph* g = glyphs + glyph;
                advance = g->b + g->w + spacing + g->a;
            }
            advance = (int)((float)advance * scaleX);
            if (separator || word + (float)advance >= wrapWidth) {
                if (word + (float)advance >= wrapWidth) {
                    word = 0.0f;
                    if (start < i) {
                        wmemcpy((wchar_t*)DAT_00577bcc + out, (const wchar_t*)text + start, i - start);
                        out += i - start;
                    }
                    DAT_00577bcc[out++] = 10;
                    start = i;
                    --i;
                    line = DAT_004e134c;
                } else if (line + word >= wrapWidth) {
                    DAT_00577bcc[out++] = 10;
                    line = DAT_004e134c;
                } else {
                    line = line + word + (float)advance;
                    word = 0.0f;
                    if (start < i) {
                        wmemcpy((wchar_t*)DAT_00577bcc + out, (const wchar_t*)text + start, i - start);
                        out += i - start;
                    }
                    DAT_00577bcc[out++] = text[i];
                    start = i + 1;
                }
            } else word += (float)advance;
        }
        if (word + line >= wrapWidth) DAT_00577bcc[out++] = 10;
        if (start < i) {
            wmemcpy((wchar_t*)DAT_00577bcc + out, (const wchar_t*)text + start, i - start);
            out += i - start;
        }
        DAT_00577bcc[out] = 0;
        wrapping = 0;
        PrintWide(device, x, y, DAT_00577bcc, flags, sx, sy);
        wrapping = 1;
    }
}

// Transform each coordinate separately into a float before subtracting the two
// world-space points. These stores are significant on the x87.
static __forceinline void BoxWorldPoint(Vec3* o, const float* p, const float* m, const float* t)
{
    __asm {
        mov ecx, p
        mov edx, m
        mov eax, o
        mov ebx, t
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x10]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x14]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x18]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x20]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x24]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x28]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fadd dword ptr [ebx]
        fxch st(1)
        fadd dword ptr [ebx+4]
        fxch st(2)
        fadd dword ptr [ebx+8]
        fxch st(1)
        fstp dword ptr [eax]
        fstp dword ptr [eax+8]
        fstp dword ptr [eax+4]
    }
}

// NONMATCHING: RALLYTROPHY 0x4ad1d0
// All 15 separating axes are implemented. Xform asm helper (BoxWorldPoint, x4) now gives the original ebp frame and the first 100 bytes exactly; 2853 -> 2526 differing bytes (built 2619 vs 3131; inner-scope worldA/worldB redeclared). Still differs: EH states / local layout (original frame 0xf4) and ~500 bytes of code not yet accounted for.
int FUN_004ad1d0(float* boxA, float* boxB, float* matrixA, float* matrixB,
                 float* positionA, float* positionB)
{
    Vec3 worldA, worldB;
    Vec3 delta, extentA, extentB;
    {
        BoxWorldPoint(&worldA, boxA + 6, matrixA, positionA);
        BoxWorldPoint(&worldB, boxB + 6, matrixB, positionB);
        delta.x = worldA.x - worldB.x;
        delta.y = worldA.y - worldB.y;
        delta.z = worldA.z - worldB.z;
    }
    float radius = boxA[9] + boxB[9];
    if (delta.z * delta.z + delta.y * delta.y + delta.x * delta.x <= radius * radius) {
        Vec3 axisA[3];
        Vec3 axisB[3];
        axisA[0].x = matrixA[0];
        axisA[0].y = matrixA[1];
        axisA[0].z = matrixA[2];
        axisA[1].x = matrixA[4];
        axisA[1].y = matrixA[5];
        axisA[1].z = matrixA[6];
        axisA[2].x = matrixA[8];
        axisA[2].y = matrixA[9];
        axisA[2].z = matrixA[10];
        axisB[0].x = matrixB[0];
        axisB[0].y = matrixB[1];
        axisB[0].z = matrixB[2];
        axisB[1].x = matrixB[4];
        axisB[1].y = matrixB[5];
        axisB[1].z = matrixB[6];
        axisB[2].x = matrixB[8];
        axisB[2].y = matrixB[9];
        axisB[2].z = matrixB[10];
        Vec3 centerA, centerB;
        centerA.x = boxA[0] + boxA[3];
        centerA.y = boxA[1] + boxA[4];
        centerA.z = boxA[2] + boxA[5];
        centerB.x = boxB[0] + boxB[3];
        centerB.y = boxB[1] + boxB[4];
        centerB.z = boxB[2] + boxB[5];
        centerA.x *= DAT_004e138c;
        centerA.y *= DAT_004e138c;
        centerA.z *= DAT_004e138c;
        centerB.x *= DAT_004e138c;
        centerB.y *= DAT_004e138c;
        centerB.z *= DAT_004e138c;
        {
            Vec3 worldA, worldB;
            BoxWorldPoint(&worldA, &centerA.x, matrixA, positionA);
            BoxWorldPoint(&worldB, &centerB.x, matrixB, positionB);
            delta.x = worldA.x - worldB.x;
            delta.y = worldA.y - worldB.y;
            delta.z = worldA.z - worldB.z;
        }
        extentA.x = boxA[3] - boxA[0];
        extentA.y = boxA[4] - boxA[1];
        extentA.z = boxA[5] - boxA[2];
        extentB.x = boxB[3] - boxB[0];
        extentB.y = boxB[4] - boxB[1];
        extentB.z = boxB[5] - boxB[2];
        Vec3 projected;
        float r00, r01, r02, r10, r11, r12, r20, r21, r22;
        float a00, a01, a02, a10, a11, a12, a20, a21, a22;
        r00 = axisB[0].x * axisA[0].x + axisA[0].z * axisB[0].z + axisA[0].y * axisB[0].y;
        r01 = axisB[1].z * axisA[0].z + axisB[1].y * axisA[0].y + axisB[1].x * axisA[0].x;
        r02 = axisB[2].x * axisA[0].x + axisB[2].z * axisA[0].z + axisB[2].y * axisA[0].y;
        projected.x = axisA[0].z * delta.z + axisA[0].y * delta.y + axisA[0].x * delta.x;
        a00 = fabs(r00);
        a01 = fabs(r01);
        a02 = fabs(r02);
        if (a02 * extentB.z + a01 * extentB.y + a00 * extentB.x + extentA.x < fabs(projected.x)) {
          return 0;
        }
        r10 = axisB[0].x * axisA[1].x + axisA[1].z * axisB[0].z + axisA[1].y * axisB[0].y;
        r11 = axisB[1].z * axisA[1].z + axisB[1].y * axisA[1].y + axisB[1].x * axisA[1].x;
        r12 = axisB[2].x * axisA[1].x + axisB[2].z * axisA[1].z + axisB[2].y * axisA[1].y;
        projected.y = axisA[1].z * delta.z + axisA[1].y * delta.y + axisA[1].x * delta.x;
        a10 = fabs(r10);
        a11 = fabs(r11);
        a12 = fabs(r12);
        if (a12 * extentB.z + a11 * extentB.y + a10 * extentB.x + extentA.y < fabs(projected.y)) {
          return 0;
        }
        r20 = axisB[0].x * axisA[2].x + axisA[2].z * axisB[0].z + axisA[2].y * axisB[0].y;
        r21 = axisB[1].z * axisA[2].z + axisB[1].y * axisA[2].y + axisB[1].x * axisA[2].x;
        r22 = axisB[2].x * axisA[2].x + axisB[2].z * axisA[2].z + axisB[2].y * axisA[2].y;
        projected.z = axisA[2].z * delta.z + axisA[2].y * delta.y + axisA[2].x * delta.x;
        a20 = fabs(r20);
        a21 = fabs(r21);
        a22 = fabs(r22);
        if (fabs(projected.z) <= a22 * extentB.z + a21 * extentB.y + a20 * extentB.x + extentA.z) {
          if (a20 * extentA.z + a10 * extentA.y + a00 * extentA.x + extentB.x <
              fabs(axisB[0].x * delta.x + delta.z * axisB[0].z + delta.y * axisB[0].y)) return 0;
          if (a21 * extentA.z + a11 * extentA.y + a01 * extentA.x + extentB.y <
              fabs(axisB[1].z * delta.z + axisB[1].y * delta.y + axisB[1].x * delta.x)) return 0;
          if (fabs(axisB[2].x * delta.x + axisB[2].z * delta.z + axisB[2].y * delta.y) <=
              a22 * extentA.z + a12 * extentA.y + a02 * extentA.x + extentB.z) {
            if ((a20 * extentA.y + a10 * extentA.z) + (a01 * extentB.z + a02 * extentB.y) <
                fabs(projected.z * r10 - r20 * projected.y)) return 0;
            if ((a21 * extentA.y + a11 * extentA.z) + (a00 * extentB.z + a02 * extentB.x) <
                fabs(projected.z * r11 - r21 * projected.y)) return 0;
            if (fabs(projected.z * r12 - r22 * projected.y) <=
                (a22 * extentA.y + a12 * extentA.z) + (a00 * extentB.y + a01 * extentB.x)) {
              if ((a20 * extentA.x + a00 * extentA.z) + (a11 * extentB.z + a12 * extentB.y) <
                  fabs(r20 * projected.x - projected.z * r00)) return 0;
              if ((a21 * extentA.x + a01 * extentA.z) + (a10 * extentB.z + a12 * extentB.x) <
                  fabs(r21 * projected.x - projected.z * r01)) return 0;
              if (fabs(r22 * projected.x - projected.z * r02) <=
                  (a22 * extentA.x + a02 * extentA.z) + (a10 * extentB.y + a11 * extentB.x)) {
                if ((a10 * extentA.x + a00 * extentA.y) + (a21 * extentB.z + a22 * extentB.y) <
                    fabs(projected.y * r00 - r10 * projected.x)) {
                  return 0;
                }
                if ((a11 * extentA.x + a01 * extentA.y) + (a20 * extentB.z + a22 * extentB.x) <
                    fabs(projected.y * r01 - r11 * projected.x)) {
                  return 0;
                }
                if (fabs(projected.y * r02 - r12 * projected.x) <=
                    (a12 * extentA.x + a02 * extentA.y) + (a20 * extentB.y + a21 * extentB.x)) {
                  return 1;
                }
              }
            }
          }
        }
    }
    return 0;
}
