// Image format conversion (0x4ab010): decodes to 32-bit via 0x4ab910, repacks to the requested format.
#include <string>
#include <stdexcept>
#include <string.h>
#include "unk_4ab010.h"

extern char* PTR_s_Out_of_memory_0050216c;
extern unsigned int* __fastcall FUN_004ab910(ImgW5* img, int unused);
extern void* __cdecl PoolAlloc(unsigned int bytes); // 0x4c1276 (operator new)
extern void __cdecl PoolFree(void* p);              // 0x4bd000 (operator delete)

struct AStr {
    std::allocator<char> alloc; char* ptr; unsigned int len; unsigned int res;
    void Assign(const char* s, unsigned int n);
    AStr(char* const& s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { ptr = 0; len = 0; res = 0; Assign(s, strlen(s)); }
    ~AStr() {}
};
#pragma comment(linker, "/alternatename:?Assign@AStr@@QAEXPBDI@Z=?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z")
struct CStr {
    std::allocator<char> alloc; char* ptr; unsigned int len; unsigned int res;
    bool Grow(unsigned int n, bool trim);
    void Eos(unsigned int n);
    CStr(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al) {
        ptr = 0; len = 0; res = 0;
        unsigned int n = strlen(s);
        if (Grow(n, true)) { memcpy(ptr, s, n); Eos(n); }
    }
    ~CStr() {}
};
#pragma comment(linker, "/alternatename:?Grow@CStr@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Eos@CStr@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")
static inline int Bal(int a) { return a + 1; }
#define ALLOC0(d, T, n) d = (T*)PoolAlloc(n); img->pixels = (unsigned char*)d; if (!d) throw std::logic_error(*(std::string*)&CStr(PTR_s_Out_of_memory_0050216c))
#define ALLOC(d, T, n) d = (T*)PoolAlloc(n); img->pixels = (unsigned char*)d; if (!d) throw std::logic_error(*(std::string*)&AStr(PTR_s_Out_of_memory_0050216c))

// NONMATCHING: RALLYTROPHY 0x4ab010
// original: FUN_004ab010
// reason: first draft; inlined throw/string construction and loop scheduling unverified
// Tried 2026-10-31: per-site ALLOC macro (inline logic_error throw; AStr = inline zeroing + out-of-line assign for sites 1-6/fmt10, CStr = out-of-line _Grow/_Eos for the fmt0 site), 8 discarded Bal(1) at function end (inline budget), n computed per branch, PoolFree(src) not advanced s (fixes a behavioural bug), copy pointers dd/s; 572->~380 insns, size 2314 vs 2302. Bit-mask expansions were already right. Still differs: loop pointer/temp registers (original keeps s in ecx, d in edx, v in eax) and prologue eax/ecx choices.
// Tried 2026-11-01 re-pass: ~35 variants (Bal(1) start x end grid saturates, s declared per branch/after ALLOC, src as fmt-slot reference, volatile src/PoolFree(src), s via &src[0]/casts/second local, indexed src loops); none beat baseline 381 insns differ. Original keeps src in ebp only to copy it into s (ecx, d copy in edx) then reuses ebp as a temp and reloads src from its slot; VC6 coalesces s with src (s=ebp) in every form tried.
// Tried 2026-11-13 pass (Sonnet): 381 -> 200 insns differ (2314 -> 2320 bytes, 1622 -> 1476 differing): fmt3 copy loop as three *dd++ = *sb++ plus sb++ with a separate unsigned char* sb (s copy now in ecx like the original); AStr takes char* const& (global read lazily, byte copy before the load); explicit fmt==7 branch before fmt==10 (VC6 tail-merges the bpp/pixels stores); fmt10 test via local v; fmt0 loop via sp copy and v temp, palette stores in order 0,1,2; fmt1 sp copy before ALLOC. Rejected: inline Pk helpers for loops (all arg orders), sp copies in fmt2/5/6, sp after ALLOC (src reloaded from slot, n in ebp), for-init copies. Remaining: original keeps src in ebp until the per-branch copy (mov ecx,ebp) then reuses ebp as loop temp; register choice (al/cl, eax/ecx) in the 8 throw blocks; fmt1 site 3 bytes longer.
void __fastcall ConvertImageFormat(ImgW5* img, int edx, unsigned int fmt)
{
    if (fmt == (unsigned int)img->format) return;
    unsigned int* src = FUN_004ab910(img, edx);
    PoolFree(img->pixels); // guess: extraout_ECX args; likely the old pixel/palette buffers
    PoolFree(img->palette);
    img->palette = 0;
    img->pixels = 0;
    unsigned int* s = src;
    if (fmt == 1) {
        unsigned int* sp = s;
        int n = img->height * img->width;
        img->format = 1; img->bytesPerPixel = 2; img->bpp = 16;
        unsigned short* d; ALLOC(d, unsigned short, n * 2);
        unsigned short* dd = d;
        for (; n > 0; n--) {
            unsigned int v = *sp++;
            *dd++ = (unsigned short)((v >> 9) & 0x7c00 | (v >> 6) & 0x3e0 | (v >> 3) & 0x1f);
        }
        PoolFree(src);
    } else if (fmt == 2) {
        int n = img->height * img->width;
        img->format = 2; img->bytesPerPixel = 2; img->bpp = 16;
        unsigned short* d; ALLOC(d, unsigned short, n * 2);
        unsigned short* dd = d;
        for (; n > 0; n--) {
            unsigned int v = *s++;
            *dd++ = (unsigned short)((v >> 8) & 0xf800 | (v >> 5) & 0x7e0 | (v >> 3) & 0x1f);
        }
        PoolFree(src);
    } else if (fmt == 3) {
        int n = img->height * img->width;
        img->format = 3; img->bytesPerPixel = 3; img->bpp = 24;
        unsigned char* d; ALLOC(d, unsigned char, n * 3);
        unsigned char* dd = d; unsigned char* sb = (unsigned char*)s;
        for (; n > 0; n--) { *dd++ = *sb++; *dd++ = *sb++; *dd++ = *sb++; sb++; }
        PoolFree(src);
    } else if (fmt == 5) {
        int n = img->height * img->width;
        img->format = 5; img->bytesPerPixel = 2; img->bpp = 16;
        unsigned short* d; ALLOC(d, unsigned short, n * 2);
        unsigned short* dd = d;
        for (; n > 0; n--) {
            unsigned int v = *s++;
            *dd++ = (unsigned short)((v >> 6) & 0x3e0 | (v >> 3) & 0x1f | (v >> 16) & 0x8000 | (v >> 9) & 0x7c00);
        }
        PoolFree(src);
    } else if (fmt == 6) {
        int n = img->height * img->width;
        img->format = 6; img->bytesPerPixel = 2; img->bpp = 16;
        unsigned short* d; ALLOC(d, unsigned short, n * 2);
        unsigned short* dd = d;
        for (; n > 0; n--) {
            unsigned int v = *s++;
            *dd++ = (unsigned short)((v >> 16) & 0xf000 | (v >> 12) & 0xf00 | (v >> 8) & 0xf0 | (v >> 4) & 0xf);
        }
        PoolFree(src);
    } else if (fmt == 7) {
        img->format = 7;
        img->bytesPerPixel = 4;
        img->bpp = 32;
        img->pixels = (unsigned char*)src;
    } else if (fmt == 10) {
        int h = img->height;
        int rowBytes = (img->width + 7) >> 3;
        img->format = 10; img->bytesPerPixel = 1; img->bpp = 1;
        unsigned char* d; ALLOC(d, unsigned char, h * rowBytes);
        memset(d, 0, h * rowBytes);
        for (int y = img->height; y > 0; y--) {
            for (int x = 0; x < img->width; x++) {
                unsigned int v = *s++;
                if (v & 0x80000000)
                    d[x >> 3] |= (unsigned char)(1 << (x & 7));
            }
            d += rowBytes;
        }
        PoolFree(src);
    } else if (fmt == 0) {
        int n = img->height * img->width;
        img->format = 0; img->bytesPerPixel = 1; img->bpp = 8;
        unsigned char* d; ALLOC0(d, unsigned char, n);
        unsigned char* dd = d; unsigned int* sp = s;
        for (; n > 0; n--) { unsigned int v = *sp++; *dd++ = (unsigned char)(v >> 8); }
        unsigned char* p = (unsigned char*)PoolAlloc(0x300);
        img->palette = p;
        for (int i = 0; i < 256; i++, p += 3) { p[0] = (unsigned char)i; p[1] = (unsigned char)i; p[2] = (unsigned char)i; }
        PoolFree(src);
    } else {
        img->format = 4;
        img->bytesPerPixel = 4;
        img->bpp = 32;
        img->pixels = (unsigned char*)src;
    }
    Bal(1);Bal(1);Bal(1);Bal(1);Bal(1);Bal(1);Bal(1);Bal(1);
    img->stride = (img->bpp * img->width + 7) >> 3;
}
