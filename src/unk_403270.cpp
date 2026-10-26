// Loading screen object (0x403270-0x403ca0): background quads, "LoadingT<n>.tga" textures, progress bar,
// plus the replay file loader at 0x403ca0 and the std::string::append(const char*, size_t) instantiation.
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "rallytrophy.h"
#include "unk_403270.h"

extern const char* FUN_00405380(int index);   // text by index
extern char DAT_005070b8[];                   // scratch buffer filled by _itoa
extern float DAT_005070d8, DAT_005070dc;
extern int (*DAT_0059b2f8)();                 // Bink/clock import slot: millisecond counter
extern int DAT_0050132c, DAT_00501464, DAT_00501458, DAT_00501468, DAT_0050145c;
extern void* DAT_00501450;                    // NetRace-like singleton
extern Game* DAT_0056b0c8;
extern void* __cdecl PoolAlloc(unsigned int bytes);   // 0x4c1276 (operator new)

static __forceinline int RoundF(float v)
{
    int r;
    __asm { fld v }
    __asm { fistp r }
    return r;
}

// FUNCTION: RALLYTROPHY 0x403ba0
// SYMBOL: ?append@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@QAEAAV12@PBDI@Z
// std::string::append(const char*, size_t)
typedef void Inst_403ba0();
template class std::basic_string<char>;

// FUNCTION: RALLYTROPHY 0x403270
LoadScreen::LoadScreen()
{
    back = 0;
    quad = 0;
    lastDraw = 0;
    lastLevel = 0;
    text = FUN_00405380(0x136);
    font = 0;
    offset = 0;
    scale = 0;
    mode = 0;
}

// FUNCTION: RALLYTROPHY 0x403380
LoadScreen::~LoadScreen()
{
    Cleanup();
}

// FUNCTION: RALLYTROPHY 0x403770
void LoadScreen::Cleanup()
{
    SpriteB* b = back;
    if (b)
        delete b;
    SpriteB* f = front;
    if (f)
        delete f;
    SpriteA* q = quad;
    if (q) {
        delete q->res;
        q = quad;
        delete q;
    }
    back = 0;
    front = 0;
    quad = 0;
}

// FUNCTION: RALLYTROPHY 0x4037e0
void LoadScreen::SetText(const char* s)
{
    text = s;
}

// FUNCTION: RALLYTROPHY 0x403c90
void* LoadScreen::Self()
{
    return this;
}

// NONMATCHING: RALLYTROPHY 0x4033f0 (draft, not yet compared)
void LoadScreen::Init(int level)
{
    _itoa(level, DAT_005070b8, 10);
    std::string path = "data/menu/LoadingT";
    path += DAT_005070b8;
    path.append(".tga", 4);
    back = new SpriteB(path.c_str());
    path = "data/menu/LoadingT";
    path.append(DAT_005070b8, strlen(DAT_005070b8));
    path.append("C.tga", 5);
    front = new SpriteB(path.c_str());
    front->SetBlend(5, 6);
    quad = new SpriteA;
    quad->res = new Res403("data/menu/loadpattern.tga", 0x841, 1);
    quad->res->Load(DAT_0056b0ac);
    int* q = (int*)quad;
    q[0x27] = 2;
    q[6] = 5;
    q[7] = 6;
    q[8] = 2;
    q[9] = 2;
    for (int i = 0; i < 4; i++) {
        ((float*)q)[0x18 + i] = 1.0f;
        ((float*)q)[0x10 + i] = 1.0f;
    }
}

struct Pt403 {
    float x, y;
    Pt403();
    ~Pt403();
};

// NONMATCHING: RALLYTROPHY 0x403890 (draft, not yet compared)
void LoadScreen::Draw(float t)
{
    int now = DAT_0059b2f8();
    if ((unsigned)(now - lastDraw) > 0x27 || t >= 1.0f) {
        if ((unsigned)(now - lastLevel) > 0x96) {
            if (DAT_00501454) {
                float f = t * 50.0f;
                int v = mode == 0 ? (int)RoundF(f) : (int)RoundF(f) + 0x32;
                if (v < 0)
                    v = 0;
                else if (v > 100)
                    v = 100;
                *(int*)(*(int*)((char*)DAT_00501450 + 0x24) + 0x17ec) = v;
                extern void FUN_00483040(int);
                FUN_00483040(v);
            }
            lastLevel = now;
        }
        extern void FUN_00432c00(Game*);
        FUN_00432c00(DAT_0056b0c8);
        lastDraw = now;
        if ((char)DAT_0056b0ac->m_active) {
            Pt403 pos[4];
            Pt403 uv[4];
            float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
            float sy = DAT_0056b0ac->GetHeight() * 0.0013020833721384406f;
            pos[0].x = 0;       pos[0].y = 687.0f;
            pos[1].x = t * 1024.0f; pos[1].y = 687.0f;
            pos[2].x = t * 1024.0f; pos[2].y = 719.0f;
            pos[3].x = 0;       pos[3].y = 719.0f;
            uv[0].x = 0;  uv[0].y = 0;
            uv[1].x = t;  uv[1].y = 0;
            uv[2].x = t;  uv[2].y = 1.0f;
            uv[3].x = 0;  uv[3].y = 1.0f;
            for (int i = 0; i < 4; i++) {
                pos[i].x *= sx;
                pos[i].y *= sy;
            }
            if (DAT_0056b0ac->BeginScene()) {
                DAT_0056b0ac->SetRenderState(5, 0);
                DAT_0056b0ac->vf42(8);
                DAT_0056b0ac->vf38(0);
                back->Draw(DAT_0056b0ac);
                ((float*)front)[0xf] = t * scale + offset;
                front->Draw(DAT_0056b0ac);
                DAT_0056b0ac->vf35((Sprite*)quad);
                DAT_0056b0ac->vf66((HudPoint*)pos, (HudPoint*)uv, 0xffffffff, 1.0f, 1.0f);
                ((int*)font)[0x48] = -1;
                ((float*)font)[7] = sx;
                ((float*)font)[8] = sy;
                extern void FUN_004a97a0(void*, RenderDevice*, unsigned int, float, const char*);
                FUN_004a97a0(font, DAT_0056b0ac, (unsigned int)(DAT_005070d8 * sx), DAT_005070dc * sy,
                             text.c_str());
                DAT_0056b0ac->EndScene();
                DAT_0056b0ac->Present();
            }
        }
    }
}

// Opaque types for the replay loader (savegame/replay<n>.dat).
class ReplayFile {                                    // 0x4a4e00 ctor(path, mode 0x81), 0x4a4d90 dtor
public:
    ReplayFile(const char* path, int mode);
    ~ReplayFile();
    void Read(void* dst, int bytes);                  // 0x4a5e00
    void Close();                                     // 0x4a5d00
};
extern bool FileExists(const char* path);             // 0x4a6050
class ReplayCar {                                     // 0x4909b0 / 0x490b90 ctors
public:
    ReplayCar(int);                                   // 0x4909b0 (first car)
    ReplayCar(int, int);                              // 0x490b90 (other cars)
    void ReadSetup(void* buf);                        // 0x490fb0
    void Finish(int v);                               // 0x491130
};
extern void FUN_00485f60(void* race, ReplayCar* car);
extern void FUN_0046bbc0(void* p);
extern void FUN_0046be50(void* car, int mode);
extern void FUN_00401b00(void* p, ReplayFile* f);
extern void FUN_0048f600(void* race, ReplayFile* f);

struct CarRecord {                                    // 0x1d4 bytes per car in the replay file
    int tag; char name[32]; int a20, a24, a2c, a1a20; int blk[90]; int blk2[10]; int a1a50, a1a58, id, a1a6c;
};

// NONMATCHING: RALLYTROPHY 0x403ca0 (draft; faithful port, not byte-matched)
unsigned int LoadReplay(ReplayHeader* hdr, int index)
{
    std::string name = "savegame/replay";
    char num[16];
    _itoa(index, num, 10);
    name.append(num, strlen(num));
    name.append(".dat", 4);
    if (!FileExists(name.c_str()))
        return 0;
    ReplayFile* f = new ReplayFile(name.c_str(), 0x81);
    int magic1, magic2;
    f->Read(&magic1, 4);
    f->Read(&magic2, 4);
    if (magic1 != 0x198bf3ae || magic2 != 0x20020109) {
        f->Close();
        delete f;
        return 0;
    }
    unsigned char* h = (unsigned char*)hdr;
    f->Read(h, 0x70);
    void* race = DAT_00501450;
    DAT_0050132c = *(int*)(h + 0x2c);
    DAT_00501464 = *(int*)(h + 0x30);
    DAT_00501458 = h[0x6e];
    DAT_00501468 = *(int*)(h + 0x34);
    DAT_0050145c = h[0x6c];
    *(int*)((char*)race + 0x28) = *(int*)(h + 0x24);
    *(int*)((char*)race + 0x2c) = *(int*)(h + 0x28);
    int cars = *(int*)(h + 0x68) + 1;
    for (int i = 0; i < cars; i++) {
        ReplayCar* car = i == 0 ? new ReplayCar(0) : new ReplayCar(i, 0);
        CarRecord r;
        f->Read(&r, 0x1d4);
        car->ReadSetup(r.name);
        char* c = (char*)car;
        *(int*)(c + 0x20) = r.a20;
        *(int*)(c + 0x1a20) = r.a1a20;
        memcpy(c + 0x1888, r.blk, sizeof(r.blk));
        *(int*)(c + 0x24) = r.a24;
        *(int*)(c + 0x2c) = r.a2c;
        memcpy(c + 0x1a28, r.blk2, sizeof(r.blk2));
        *(int*)(c + 0x1a50) = r.a1a50;
        *(int*)(c + 0x1a58) = r.a1a58;
        *(int*)(c + 0x1a14) = r.id;
        *(int*)(c + 0x1a6c) = r.a1a6c;
        car->Finish(r.id);
        FUN_00485f60(race, car);
        char* body = *(char**)((char*)car + 0x28);
        FUN_0046bbc0(body + 0x1c874);
        f->Read(body + 0x1c874, 0x8c);
        for (int k = 0; k < 3; k++) {
            int n = *(int*)(body + 0x1c8e8 + k * 4);
            if (n) {
                int* arr = (int*)PoolAlloc(n * 4);
                *(int**)(body + 0x1c8f4 + k * 4) = arr;
                f->Read(arr, n * 4);
            }
        }
        FUN_0046be50(body, 2);
        int has;
        f->Read(&has, 4);
        if (has)
            FUN_00401b00(body + 0x1bed0, f);
    }
    FUN_0048f600(race, f);
    f->Close();
    delete f;
    return 1;
}
