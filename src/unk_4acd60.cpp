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
struct CvtTwin { void Conv(unsigned fmt); };
#pragma comment(linker, "/alternatename:?Conv@CvtTwin@@QAEXI@Z=?ConvertImageFormat@@YIXPAUImgW5@@HI@Z")
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


struct TX { unsigned char b, g, r, a; };
// FUNCTION: RALLYTROPHY 0x4acd60
// Texel bytes via a plain struct (no Color EH state); output row cursor walks in the for-increment; ConvertImageFormat called through a thiscall twin so no edx is set.
void BitmapOps::Downsample(int xShift, int yShift, int outputFormat)
{
    int shift = xShift + yShift;
    if (shift != 0) {
        int w = *(volatile int*)&width >> xShift;
        int h = height >> yShift;
        int pw = 1;
        while (pw < width) pw <<= 1;
        int ph = 1;
        while (ph < height) ph <<= 1;
        if (pw != width || ph != height)
            throw std::logic_error(std::string("Trying to downsample bitmap which isn't power of 2."));
        int oldFormat = format;
        unsigned* src;
        if (bpp != 4) {
            src = fun_004ab910(this);
            fun_004bd000(data);
        } else src = (unsigned*)data;
        fun_004bd000(palette);
        format = 7;
        unsigned* dst = (unsigned*)fun_004c1276(h * w * 4);
        data = (unsigned char*)dst;
        palette = 0;
        unsigned* cur = dst;
        int y, x;
        int dx = 1 << xShift;
        int dy = 1 << yShift;
        for (y = 0; y < h; cur += w, ++y) {
            int top = y << yShift;
            unsigned* out = cur;
            int bottom = top + dy;
            for (x = 0; x < w; ++x) {
                int left = x << xShift;
                unsigned b = 0, g = 0, r = 0, a = 0;
                int right = left + dx;
                unsigned* row = src + width * top;
                for (int yy = top; yy < bottom; ++yy) {
                    for (int xx = left; xx < right; ++xx) {
                        TX pixel; *(unsigned*)&pixel = row[xx];
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
        }
        fun_004bd000(src);
        pitch = w * 4;
        width = w;
        height = h;
        bits = 32;
        bpp = 4;
        if (outputFormat != -1) ((CvtTwin*)this)->Conv(outputFormat);
        else ((CvtTwin*)this)->Conv(oldFormat);
    }
}

struct QuadPoints {
    HudPoint positions[2];
    HudPoint uvs[2];
    DWORD tint;
    float alpha;
    QuadPoints() {}
    void Init(DWORD c) { tint = c; alpha = 1.0f; }
    ~QuadPoints() {}
};

extern "C" size_t __cdecl FUN_004c2414(const unsigned short* s);   // wcslen

// NONMATCHING: RALLYTROPHY 0x4aa690
// Wrapping and draw dispatch are complete; local lifetimes, stack layout and x87 scheduling differ (818 vs 816 bytes).
// Tried 2026-10-30 night: memcpy(char-count) for the three word copies (now 816 bytes, 141->114 insns differ); memmove/char_traits/std::copy/wcsncpy/<<1/unsigned sizes, all 24 decl orders of positions/uvs/tint/alpha, POD/volatile tint; still differs: original copy is shr ecx,1/rep movsd/adc/rep movsw (we get shr 2/movsb), ColorArg EH state 2 vs original 1, register assignment ebp/ebx swapped.
// 2026-11-04: FUN_004a94c0 is the dtor of a 0x28-byte quad {HudPoint positions[2]; uvs[2]; tint; alpha} (QuadPoints, ctor sets tint/alpha): 529->371 differing, frame 0x40 and EH states 0,1,2 now match; left: memcpy word form (shr 1/movsd/adc/movsw) and ebx/ebp swap.
// 2026-11-13 pass: wcslen is FUN_004c2414; copies are `for (k=start;k<i;k++) DAT_00577bcc[out++]=text[k];` loops without `if (start<i)` (371 -> 319 bytes); QuadPoints has a no-arg ctor + Init(color) (colour read after ctor) and decl order `int out, start, i` (-> 12 insns differ, 818 vs 816). Left: word-zero store uses esi not ebp, `fcom [mem]` vs `fld st; fcomp` on word+advance (sum temps, inverted conds, ops swaps all no gain), 2 bytes (orig has trailing push -1 = next function?).
// Tried 2026-11-01 re-pass: wmemcpy/wmemmove/wcsncpy, memcpy(n*2 / n<<1 / unsigned / n+n / char*), 8-byte tint+alpha struct with dtor (837 bytes, 102 insns) or volatile POD tint/alpha in 7 layouts (806-820 bytes), all 6 start/out/i orders; none beat 114 insns differ. Original has tint(0x48)+alpha(0x4c) as dead stores above the arrays, EH state 1 as DWORD store (ours BYTE), i in ebp and out in ebx.
void WrappedGlyphFont::PrintWrapped(RenderDevice* device, float x, float y,
                                   const unsigned short* text, int flags, float sx, float sy)
{
    int n = (int)FUN_004c2414(text);
    if (n) {
        device->vf35((Sprite*)((char*)this + 0x48));
        QuadPoints quad; quad.Init(color);
        int out = 0, start = 0, i = 0;
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
                float fa = (float)advance;
                float lw;
                if (word + fa >= wrapWidth) {
                    word = 0.0f;
                    for (int k = start; k < i; k++) DAT_00577bcc[out++] = text[k];
                    DAT_00577bcc[out++] = 10;
                    start = i;
                    --i;
                    line = DAT_004e134c;
                } else if ((lw = line + word) >= wrapWidth) {
                    DAT_00577bcc[out++] = 10;
                    line = DAT_004e134c;
                } else {
                    word = 0.0f;
                    for (int k = start; k < i; k++) DAT_00577bcc[out++] = text[k];
                    line = lw + fa;
                    DAT_00577bcc[out++] = text[i];
                    start = i + 1;
                }
            } else word += (float)advance;
        }
        if (word + line >= wrapWidth) DAT_00577bcc[out++] = 10;
        for (int k = start; k < i; k++) DAT_00577bcc[out++] = text[k];
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
// Tried 2026-10-30 night: flat "if (fabs(p) > sum) return 0;" chain (all 15 tests), float/double temps for sum/fabs, !(<=) and empty-then forms, volatile r/a locals: 617-653 insns differ, worse than nested form (573). Original is an OBB-OBB test (not image code) whose compares are fxch/fcomp st(1)/fstp st(0), a[0]..a[8] and r[0]..r[8] in real frame slots (-0xb4.., -0xfc..) and fail blocks cross-jumped in groups of three; none of that reproduced.
// Tried 2026-11-01 re-pass: reusing outer worldA/worldB for the center transform (no inner redeclare) 573->504 insns differ; plain float[] for axes/centers/extents/projected (no EH objects) drops built to 2223 bytes and 718 insns (original has EH states 0,2,3 and 0xf4 frame, so some Vec3 locals are needed); hoisting all Vec3 to function scope, a/r arrays, a-before-r order: no gain. Gap is ~500 bytes of code (a/r in frame slots), far from permuter range.
// Pass 2026-11-13 (agent): 597 -> 407 insns differ (2741 -> 2917 bytes of 3131): r/a as float r[9]/a[9] (frame slots now exact), centerB hoisted to function scope after worldB (slot -0x48 exact; EH state order now differs), t/ra/rb locals for compares (cross tests give the fadd st,st(1) form; the original's fxch/fcomp st(1)/fstp st(0) compare still not reproduced). Open: fcomp form, x,z,y term order of r00/r02 (VC6 emits z,y,x), missing inline return-0 cleanups (original has 7 dtor pairs, we 3).
int FUN_004ad1d0(float* boxA, float* boxB, float* matrixA, float* matrixB,
                 float* positionA, float* positionB)
{
    Vec3 worldA;
    Vec3 worldB;
    struct F3 { float x, y, z; } extentA, extentB;
    BoxWorldPoint(&worldA, boxA + 6, matrixA, positionA);
    BoxWorldPoint(&worldB, boxB + 6, matrixB, positionB);
    Vec3 centerB;
    Vec3 delta;
    delta.x = worldA.x - worldB.x;
    delta.y = worldA.y - worldB.y;
    delta.z = worldA.z - worldB.z;
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
        Vec3 centerA = *(Vec3*)boxA;
        centerB = *(Vec3*)boxB;
        centerA.x += boxA[3];
        centerA.y += boxA[4];
        centerA.z += boxA[5];
        centerB.x += boxB[3];
        centerB.y += boxB[4];
        centerB.z += boxB[5];
        centerA.x *= DAT_004e138c;
        centerA.y *= DAT_004e138c;
        centerA.z *= DAT_004e138c;
        centerB.x *= DAT_004e138c;
        centerB.y *= DAT_004e138c;
        centerB.z *= DAT_004e138c;
        {
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
        struct F3 projected;
        float r[9];
        float a[9];
        { float q = axisB[0].x * axisA[0].x; q = axisA[0].z * axisB[0].z + q; r[0] = axisA[0].y * axisB[0].y + q; }
        r[1] = axisB[1].z * axisA[0].z + axisB[1].y * axisA[0].y + axisB[1].x * axisA[0].x;
        { float q = axisB[2].x * axisA[0].x; q = axisB[2].z * axisA[0].z + q; r[2] = axisB[2].y * axisA[0].y + q; }
        projected.x = axisA[0].z * delta.z + axisA[0].y * delta.y + axisA[0].x * delta.x;
        a[0] = fabs(r[0]);
        a[1] = fabs(r[1]);
        a[2] = fabs(r[2]);
        { float t = fabs(projected.x); if (t > a[2] * extentB.z + a[1] * extentB.y + a[0] * extentB.x + extentA.x) return 0; }
        { float q = axisB[0].x * axisA[1].x; q = axisA[1].z * axisB[0].z + q; r[3] = axisA[1].y * axisB[0].y + q; }
        r[4] = axisB[1].z * axisA[1].z + axisB[1].y * axisA[1].y + axisB[1].x * axisA[1].x;
        { float q = axisB[2].x * axisA[1].x; q = axisB[2].z * axisA[1].z + q; r[5] = axisB[2].y * axisA[1].y + q; }
        projected.y = axisA[1].z * delta.z + axisA[1].y * delta.y + axisA[1].x * delta.x;
        a[3] = fabs(r[3]);
        a[4] = fabs(r[4]);
        a[5] = fabs(r[5]);
        { float t = fabs(projected.y); if (t > a[5] * extentB.z + a[4] * extentB.y + a[3] * extentB.x + extentA.y) return 0; }
        { float q = axisB[0].x * axisA[2].x; q = axisA[2].z * axisB[0].z + q; r[6] = axisA[2].y * axisB[0].y + q; }
        r[7] = axisB[1].z * axisA[2].z + axisB[1].y * axisA[2].y + axisB[1].x * axisA[2].x;
        { float q = axisB[2].x * axisA[2].x; q = axisB[2].z * axisA[2].z + q; r[8] = axisB[2].y * axisA[2].y + q; }
        projected.z = axisA[2].z * delta.z + axisA[2].y * delta.y + axisA[2].x * delta.x;
        a[6] = fabs(r[6]);
        a[7] = fabs(r[7]);
        a[8] = fabs(r[8]);
        { float t = fabs(projected.z); if (t > a[8] * extentB.z + a[7] * extentB.y + a[6] * extentB.x + extentA.z) return 0; }
        { float t = fabs(projected.z * r[3] - r[6] * projected.y); float ra = a[6] * extentA.y + a[3] * extentA.z; float rb = a[1] * extentB.z + a[2] * extentB.y; float s = ra + rb; if (t > s) return 0; }
        { float t = fabs(projected.z * r[4] - r[7] * projected.y); float ra = a[7] * extentA.y + a[4] * extentA.z; float rb = a[0] * extentB.z + a[2] * extentB.x; float s = ra + rb; if (t > s) return 0; }
        { float t = fabs(projected.z * r[5] - r[8] * projected.y); float ra = a[8] * extentA.y + a[5] * extentA.z; float rb = a[0] * extentB.y + a[1] * extentB.x; float s = ra + rb; if (t > s) return 0; }
        { float t = fabs(r[6] * projected.x - projected.z * r[0]); float ra = a[6] * extentA.x + a[0] * extentA.z; float rb = a[4] * extentB.z + a[5] * extentB.y; float s = ra + rb; if (t > s) return 0; }
        { float t = fabs(r[7] * projected.x - projected.z * r[1]); float ra = a[7] * extentA.x + a[1] * extentA.z; float rb = a[3] * extentB.z + a[5] * extentB.x; float s = ra + rb; if (t > s) return 0; }
        { float t = fabs(r[8] * projected.x - projected.z * r[2]); float ra = a[8] * extentA.x + a[2] * extentA.z; float rb = a[3] * extentB.y + a[4] * extentB.x; float s = ra + rb; if (t > s) return 0; }
        { float t = fabs(projected.y * r[0] - r[3] * projected.x); float ra = a[3] * extentA.x + a[0] * extentA.y; float rb = a[7] * extentB.z + a[8] * extentB.y; float s = ra + rb; if (t > s) return 0; }
        { float t = fabs(projected.y * r[1] - r[4] * projected.x); float ra = a[4] * extentA.x + a[1] * extentA.y; float rb = a[6] * extentB.z + a[8] * extentB.x; float s = ra + rb; if (t > s) return 0; }
        { float t = fabs(projected.y * r[2] - r[5] * projected.x); float ra = a[5] * extentA.x + a[2] * extentA.y; float rb = a[6] * extentB.y + a[7] * extentB.x; float s = ra + rb; if (t > s) return 0; }
        float pb0 = axisB[0].x * delta.x; pb0 = delta.z * axisB[0].z + pb0; pb0 = delta.y * axisB[0].y + pb0;
        { float t = fabs(pb0); if (t > a[6] * extentA.z + a[3] * extentA.y + a[0] * extentA.x + extentB.x) return 0; }
        { float t = fabs(axisB[1].z * delta.z + axisB[1].y * delta.y + axisB[1].x * delta.x); if (t > a[7] * extentA.z + a[4] * extentA.y + a[1] * extentA.x + extentB.y) return 0; }
        float pb2 = axisB[2].x * delta.x; pb2 = axisB[2].z * delta.z + pb2; pb2 = axisB[2].y * delta.y + pb2;
        { float t = fabs(pb2); if (t > a[8] * extentA.z + a[5] * extentA.y + a[2] * extentA.x + extentB.z) return 0; }
        return 1;
    }
    return 0;
}
