// BinkVideo constructor (0x4097a0): opens the video, picks a texture split for 320x240 / 512x384 and builds the surface.
#include <string>
#include <stdexcept>
#include "rallytrophy.h"
#include "unk_481390.h"
#include "unk_4097a0.h"

extern "C" {
__declspec(dllimport) int __stdcall BinkSetSoundTrack(int);
__declspec(dllimport) int __stdcall BinkSetIOSize(int);
__declspec(dllimport) void* __stdcall BinkOpen(const char*, int);
__declspec(dllimport) int __stdcall BinkSetVolume(void*, int);
__declspec(dllimport) int __stdcall BinkGetSummary(void*, void*);
}
extern int DAT_00501448;   // volume setting (0..100 scaled)
struct CStr {
    std::allocator<char> alloc; char* ptr; unsigned int len; unsigned int res;
    bool Grow(unsigned int n, bool trim);
    void Eos(unsigned int n);
    CStr(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al) {
        ptr = 0; len = 0; res = 0;
        unsigned int n = strlen(s);
        if (Grow(n, true)) { memcpy(ptr, s, n); Eos(n); }
    }
    CStr(const char* s, int, const std::allocator<char>& al = std::allocator<char>()) : alloc(al) {
        ptr = 0; len = 0; res = 0;
        unsigned int n = strlen(s);
        if (Grow(n, true)) { memcpy(ptr, s, n); len = n; ptr[n] = 0; }
    }
    ~CStr() {}
};
#pragma comment(linker, "/alternatename:?Grow@CStr@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Eos@CStr@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")
struct ErrStr {
    std::allocator<char> alloc; char* ptr; unsigned int len; unsigned int res;
    void Assign(const CStr& x, unsigned int pos, unsigned int n);
    ErrStr(const CStr& x) : alloc(x.alloc) { ptr = 0; len = 0; res = 0; Assign(x, 0, std::string::npos); }
    ~ErrStr();
};
#pragma comment(linker, "/alternatename:?Assign@ErrStr@@QAEXABUCStr@@II@Z=?assign@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@ABV12@II@Z")
class ColLogicError : public std::exception {
public:
    ColLogicError(const CStr& s) : std::exception(""), str(s) {}
    virtual ~ColLogicError();
    ErrStr str;
};
struct EmptyCall { void Run(); };   // folded empty function 0x48e660
#define F(T, o) (*(T*)((BYTE*)this + (o)))

// FUNCTION: RALLYTROPHY 0x4097a0
BinkVideo::BinkVideo(const char* name, RenderDevice* device, int flags)
{
    F(int, 0xa8) = 0;
    F(int, 0xc) = 0;
    F(int, 0xa0) = 0;
    int bf = 0;
    if (flags == 0) {
        BinkSetSoundTrack(-1);
        bf = 0x4000;
    }
    BinkSetIOSize(0x400000);
    void* h = BinkOpen(name, bf | 0x1000000);
    F(void*, 0x9c) = h;
    if (h == 0) {
        throw ColLogicError(CStr("Cannot open video"));
    }
    if (flags != 0) {
        BinkSetVolume(h, (DAT_00501448 << 15) / 100);
    }
    BinkGetSummary(h, (BYTE*)this + 0x20);
    int width = F(int, 0x20);
    int height = F(int, 0x24);
    VidDevice* dev = *(VidDevice**)((BYTE*)device + 0x20);
    F(int, 4) = width;
    F(int, 8) = height;
    F(int, 0) = dev->PickFormat(4, 0);
    if (F(int, 0) != 4) {
        F(int, 0) = dev->PickFormat(2, 6);
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
        s0->texture = dev->MakeTexture(new Texture("avi0"), F(int, 0), 0x100, 0x100, 0x15);
        F(Sprite*, 0x14)->texture = dev->MakeTexture(new Texture("avi1"), F(int, 0), 0x100, 0x100, 0x15);
        F(Sprite*, 0x18)->texture = dev->MakeTexture(new Texture("avi2"), F(int, 0), 0x100, 0x80, 0x15);
        F(Sprite*, 0x1c)->texture = dev->MakeTexture(new Texture("avi3"), F(int, 0), 0x100, 0x80, 0x15);
    } else {
        throw ColLogicError(CStr("Unsupported video resolution", 0));
    }
    Texture* bt = new Texture("bink");
    F(Texture*, 0xa0) = bt;
    F(void*, 0xa0) = dev->MakeSurface(bt, F(int, 0), width, F(int, 0x24), 5);
    switch (F(int, 0)) {
    case 1: case 5: F(int, 0xa4) = 9; break;
    case 2: F(int, 0xa4) = 10; break;
    case 3: F(int, 0xa4) = 1; break;
    case 6: F(int, 0xa4) = 7; break;
    case 4: case 7: F(int, 0xa4) = 3; break;
    default: throw ColLogicError(CStr("Invalid video format", 0));
    }
    for (int i = 0; i < F(int, 0xc); i++) {
        BYTE* s = F(BYTE*, 0x10 + i * 4);
        *(int*)(s + 0x9c) = 0xe;
        *(int*)(s + 0x98) = 0xf;
        *(float*)(s + 0x60) = 1.0f;
        *(float*)(s + 0x64) = 1.0f;
        *(float*)(s + 0x68) = 1.0f;
        *(float*)(s + 0x6c) = 1.0f;
    }
    ((EmptyCall*)this)->Run();
}
