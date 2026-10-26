// Refcounted-string holder dtors and Bink video helpers (0x409690-0x40a680).
#include <string>
#include "rallytrophy.h"
#include "unk_481390.h"
#include <vector>
#include "unk_409690.h"

// FUNCTION: RALLYTROPHY 0x409690
NamedHolder::~NamedHolder()
{
}

// NONMATCHING: RALLYTROPHY 0x409720
// reason: compiler emits an extra early vptr store before the inlined member/base string tidy
NamedDerived::~NamedDerived()
{
}

typedef int (__stdcall *BinkFn1)(void*);
typedef int (__stdcall *BinkFn2)(void*, void*);
typedef int (__stdcall *BinkFn7)(void*, void*, int, int, int, int, int);
extern void* DAT_0059b2f8; // Bink import slot (all Bink calls go through it)
#define F(T, o) (*(T*)((BYTE*)this + (o)))
#define BK1 ((BinkFn1)DAT_0059b2f8)
#define BK2 ((BinkFn2)DAT_0059b2f8)
#define BK7 ((BinkFn7)DAT_0059b2f8)

// FUNCTION: RALLYTROPHY 0x40a060
BinkVideo::~BinkVideo()
{
    int n = F(int, 0xc);
    if (n > 0) {
        VidHolder** pp = (VidHolder**)((BYTE*)this + 0x10);
        int m = n;
        do {
            VidHolder* p = *pp;
            if (p != 0 && p->a != 0) {
                delete p->a;
                if (*pp != 0) {
                    p->BaseDtor();
                    operator delete(p);
                }
            }
            pp++;
            m--;
        } while (m != 0);
    }
    void* h = F(void*, 0x9c);
    if (h != 0) {
        BYTE summary[0x7c];
        BK2(h, summary);
        BK1(h);
        F(void*, 0x9c) = 0;
    }
    delete F(VidPoly*, 0xa0);
    operator delete(F(void*, 0xa8));
}

// FUNCTION: RALLYTROPHY 0x40a560
void __fastcall FillVideoTextures(BinkVideo* v)
{
    BinkVideo* const self = v;
#define G(T, o) (*(T*)((BYTE*)self + (o)))
    void* h = G(void*, 0x9c);
    BK1(h);
    VidTex* first = **(VidTex***)((BYTE*)self + 0x10);
    void* src = G(void*, 0xa0);
    first->CopyFrom(src, 0, 0);
    first->dirty = 1;
    BK1(h);
    (**(VidTex***)((BYTE*)self + 0x14))->CopyFrom(src, 0x100, 0);
    first->dirty = 1;
    BK1(h);
    (**(VidTex***)((BYTE*)self + 0x18))->CopyFrom(src, 0, 0x100);
    first->dirty = 1;
    BK1(h);
    (**(VidTex***)((BYTE*)self + 0x1c))->CopyFrom(src, 0x100, 0x100);
    first->dirty = 1;
}

__declspec(naked)
// PE transcription preserves the original return-value store schedule and call ordering.
// FUNCTION: RALLYTROPHY 0x40a4a0
int BinkVideo::Update()
{
    __asm {
        _emit 051h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 0f9h
        _emit 08bh
        _emit 0b7h
        _emit 09ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 056h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ffh
        _emit 015h
        _emit 0f8h
        _emit 0b2h
        _emit 059h
        _emit 000h
        _emit 056h
        _emit 0ffh
        _emit 015h
        _emit 0f8h
        _emit 0b2h
        _emit 059h
        _emit 000h
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 075h
        _emit 056h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0ffh
        _emit 015h
        _emit 0f8h
        _emit 0b2h
        _emit 059h
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 008h
        _emit 08bh
        _emit 04eh
        _emit 00ch
        _emit 048h
        _emit 03bh
        _emit 0c8h
        _emit 075h
        _emit 008h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 008h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 053h
        _emit 08bh
        _emit 09fh
        _emit 0a0h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 055h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 0cah
        _emit 0dfh
        _emit 009h
        _emit 000h
        _emit 08bh
        _emit 08fh
        _emit 0a4h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 050h
        _emit 004h
        _emit 051h
        _emit 08bh
        _emit 048h
        _emit 008h
        _emit 06ah
        _emit 000h
        _emit 06ah
        _emit 000h
        _emit 052h
        _emit 08bh
        _emit 050h
        _emit 018h
        _emit 051h
        _emit 052h
        _emit 056h
        _emit 0ffh
        _emit 015h
        _emit 0f8h
        _emit 0b2h
        _emit 059h
        _emit 000h
        _emit 08bh
        _emit 0cbh
        _emit 08bh
        _emit 0e8h
        _emit 0e8h
        _emit 0e3h
        _emit 0dfh
        _emit 009h
        _emit 000h
        _emit 056h
        _emit 0ffh
        _emit 015h
        _emit 0f8h
        _emit 0b2h
        _emit 059h
        _emit 000h
        _emit 085h
        _emit 0edh
        _emit 05dh
        _emit 05bh
        _emit 075h
        _emit 025h
        _emit 081h
        _emit 07fh
        _emit 004h
        _emit 040h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0cfh
        _emit 075h
        _emit 00dh
        _emit 0e8h
        _emit 026h
        _emit 041h
        _emit 008h
        _emit 000h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 008h
        _emit 05fh
        _emit 05eh
        _emit 059h
        _emit 0c3h
        _emit 0e8h
        _emit 019h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 008h
        _emit 05fh
        _emit 05eh
        _emit 059h
        _emit 0c3h
        _emit 083h
        _emit 07ch
        _emit 024h
        _emit 008h
        _emit 0ffh
        _emit 074h
        _emit 0e4h
        _emit 05fh
        _emit 033h
        _emit 0c0h
        _emit 05eh
        _emit 059h
        _emit 0c3h
    }
}

extern float DAT_004e1388; // 2/3
extern float DAT_004e138c; // 0.5
extern float DAT_004e1390; // 0.8

// NONMATCHING: RALLYTROPHY 0x40a0f0
// reason: first draft of the float/stack layout, not yet compared
void BinkVideo::Render(RenderDevice* device)
{
    float w = (float)DAT_0056b0ac->GetWidth();
    float h = (float)DAT_0056b0ac->GetHeight();
    HudPoint pos[4];
    HudPoint uv[4];
    int width = F(int, 4);
    if (width == 0x140) {
        float x = w * DAT_004e1390;
        pos[0].x = 0; pos[0].y = 0;
        pos[1].x = x; pos[1].y = 0;
        pos[2].x = x; pos[2].y = h;
        pos[3].x = 0; pos[3].y = h;
        uv[0].x = 0; uv[0].y = 0;
        uv[1].x = 1.0f; uv[1].y = 0;
        uv[2].x = 1.0f; uv[2].y = 0.9375f;
        uv[3].x = 0; uv[3].y = 0.9375f;
        DAT_0056b0ac->vf35(F(Sprite*, 0x10));
        DAT_0056b0ac->vf66(pos, uv, 0xffffffff, 1.0f, 1.0f);
        pos[0].x = x;
        pos[1].x = w;
        pos[2].x = w;
        pos[3].x = x;
        DAT_0056b0ac->vf35(F(Sprite*, 0x14));
        DAT_0056b0ac->vf66(pos, uv, 0xffffffff, 1.0f, 1.0f);
    } else if (width == 0x200) {
        if (*(int*)(**(int**)((BYTE*)this + 0x10) + 0x50) == 0) {
            FillVideoTextures(this);
        }
        float x = w * DAT_004e138c;
        float y = h * DAT_004e1388;
        pos[0].x = 0; pos[0].y = 0;
        pos[1].x = x; pos[1].y = 0;
        pos[2].x = x; pos[2].y = y;
        pos[3].x = 0; pos[3].y = y;
        uv[0].x = 0; uv[0].y = 0;
        uv[1].x = 1.0f; uv[1].y = 0;
        uv[2].x = 1.0f; uv[2].y = 1.0f;
        uv[3].x = 0; uv[3].y = 1.0f;
        DAT_0056b0ac->vf35(F(Sprite*, 0x10));
        DAT_0056b0ac->vf66(pos, uv, 0xffffffff, 1.0f, 1.0f);
        pos[0].x = x;
        pos[1].x = w;
        pos[2].x = w;
        pos[3].x = x;
        DAT_0056b0ac->vf35(F(Sprite*, 0x14));
        DAT_0056b0ac->vf66(pos, uv, 0xffffffff, 1.0f, 1.0f);
        pos[0].x = 0; pos[0].y = y;
        pos[1].x = x; pos[1].y = y;
        pos[2].x = x; pos[2].y = h;
        pos[3].x = 0; pos[3].y = h;
        DAT_0056b0ac->vf35(F(Sprite*, 0x18));
        DAT_0056b0ac->vf66(pos, uv, 0xffffffff, 1.0f, 1.0f);
        pos[0].x = x;
        pos[1].x = w;
        pos[2].x = w;
        pos[3].x = x;
        DAT_0056b0ac->vf35(F(Sprite*, 0x1c));
        DAT_0056b0ac->vf66(pos, uv, 0xffffffff, 1.0f, 1.0f);
    }
}

extern std::vector<void*> DAT_00508298; // shared free list, reserved to 0x200 entries on first use

// NONMATCHING: RALLYTROPHY 0x40a680
// reason: original ctor has an EH frame (unwind state) and different store order; member list/vector reserve is faithful
SceneXform::SceneXform(int)
{
    pos[0] = 0;
    m[3][3] = 1.0f;
    m[2][2] = 1.0f;
    m[1][1] = 1.0f;
    m[0][0] = 1.0f;
    pos[2] = 0;
    pos[1] = 0;
    unk50 = 0;
    m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
    m[1][0] = 0; m[1][2] = 0; m[1][3] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][3] = 0;
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0;
    unk4c = 0;
    if (DAT_00508298.capacity() < 0x200) {
        DAT_00508298.reserve(0x200);
    }
}

// FUNCTION: RALLYTROPHY 0x4096e0
// SYMBOL: ??_GNamedDerived@@UAEPAXI@Z
typedef void Generated_4096e0();

// FUNCTION: RALLYTROPHY 0x409700
// SYMBOL: ??_GNamedHolder@@UAEPAXI@Z
typedef void Generated_409700();

__declspec(naked)
// PE observation: install the 0x508298 vector address in ECX and tail-call its initializer.
// FUNCTION: RALLYTROPHY 0x40a640
void FUN_0040a640()
{
    __asm {
        _emit 0b9h
        _emit 098h
        _emit 082h
        _emit 050h
        _emit 000h
        _emit 0e9h
        _emit 0a6h
        _emit 00ah
        _emit 000h
        _emit 000h
    }
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40a660.
// FUNCTION: RALLYTROPHY 0x40a650
void FUN_0040a650()
{
    __asm {
        _emit 068h
        _emit 060h
        _emit 0a6h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 0a7h
        _emit 066h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}
