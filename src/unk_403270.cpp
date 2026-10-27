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
class NetRace {                               // see include/unk_481390.h
public:
    BYTE pad[0x24];
    BYTE* session;                            // +0x24: progress percent at +0x17ec
    int unk28, unk2c;                         // replay header +0x24, +0x28
    void SendByte(BYTE value);                // 0x483040
    void FUN_00485f60(class ReplayCar* car);
    void FUN_0048f600(class ReplayFile* f);
};
extern NetRace* DAT_00501450;
class TextRenderer {                          // see include/unk_481390.h
public:
    BYTE unk0[0x1c];
    float scaleX, scaleY;                     // 0x1c, 0x20
    BYTE unk24[0xfc];
    int color;                                // 0x120
    void Print(RenderDevice* device, float x, float y, const char* text, int flags, float sx, float sy); // 0x4a97a0
};
extern Game* DAT_0056b0c8;
class GameView {                              // the game object seen as its view; see include/unk_481390.h
public:
    void FUN_00432c00();                      // 0x432c00: per-frame update
};
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

// The path string as the original builds it (VC6 std::string layout): the constructor calls the out-of-line
// _Tidy (0x402700) and the destructor is _Tidy(true) expanded inline. Each append/assign site is pinned either
// out of line (0x403ba0) or expanded around _Grow (0x402980) and _Eos (0x402960), as the target has them.
class PathText {
public:
    PathText(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    PathText(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        size_t n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~PathText() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)              // std::string::_Tidy, expanded inline
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);             // 0x402700
    bool Grow(size_t n, bool trim);    // 0x402980
    void Eos(size_t n);                // 0x402960
    void Append(const char* s, size_t n); // 0x403ba0
    void AppendCall(const char* s) { Append(s, strlen(s)); }
    void AssignGrow(const char* s)     // assign with _Grow and _Eos out of line
    {
        size_t n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    void AssignInline(const char* s)   // assign with _Eos expanded
    {
        size_t n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            len = n, ptr[n] = 0;
        }
    }
    void AppendInline(const char* s)   // append expanded around _Grow and _Eos
    {
        size_t n = strlen(s);
        if (std::string::npos - len <= n)
            std::_Xlen();
        if (0 < n) {
            size_t m = len + n;
            if (Grow(m, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                Eos(m);
            }
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    size_t len;
    size_t reserve;
};

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

// NONMATCHING: RALLYTROPHY 0x4033f0
// Same size; only the scheduling of the wrapU/wrapV stores differs (the target interleaves them with the
// colour stores). Statement order, loops, unrolled stores and typed fields all emit the same order here.
void LoadScreen::Init(int level)
{
    _itoa(level, DAT_005070b8, 10);
    PathText path;
    path.AssignGrow("data/menu/LoadingT");
    path.AppendCall(DAT_005070b8);
    path.AppendCall(".tga");
    back = new SpriteB(DAT_0056b0ac, path.c_str(), 1, 1);
    path.AssignInline("data/menu/LoadingT");
    path.AppendInline(DAT_005070b8);
    path.AppendInline("C.tga");
    front = new SpriteB(DAT_0056b0ac, path.c_str(), 1, 1);
    front->SetBlend(5, 6);
    quad = new SpriteA;
    quad->res = new Res403("data/menu/loadpattern.tga", 0x841, 1);
    quad->res->Load(DAT_0056b0ac);
    SpriteA* q = quad;
    q->f9c = 2;
    q->blendSrc = 5;
    q->blendDst = 6;
    q->wrapU = 2;
    q->colorA[0] = 1.0f;
    q->colorA[1] = 1.0f;
    q->colorA[2] = 1.0f;
    q->colorA[3] = 1.0f;
    q->wrapV = 2;
    q->colorB[0] = 1.0f;
    q->colorB[1] = 1.0f;
    q->colorB[2] = 1.0f;
    q->colorB[3] = 1.0f;
}

struct Col403 {                                      // packed ARGB colour with an empty destructor
    DWORD argb;
    Col403(DWORD c) : argb(c) {}
    ~Col403() {}
};

struct Pt403 {
    float x, y;
    Pt403();
    ~Pt403();
};

// FUNCTION: RALLYTROPHY 0x403890
void LoadScreen::Draw(float t)
{
    int now = DAT_0059b2f8();
    if ((unsigned)(now - lastDraw) >= 0x28 || t >= 1.0f) {
        if ((unsigned)(now - lastLevel) > 0x96) {
            if (DAT_00501454) {
                int v = mode == 0 ? RoundF(t * 50.0f) : RoundF(t * 50.0f) + 0x32;
                if (v < 0)
                    v = 0;
                else if (v > 100)
                    v = 100;
                *(int*)(DAT_00501450->session + 0x17ec) = v;
                DAT_00501450->SendByte((BYTE)v);
            }
            lastLevel = now;
        }
        ((GameView*)DAT_0056b0c8)->FUN_00432c00();
        lastDraw = now;
        if ((char)DAT_0056b0ac->m_active) {
            Pt403 pos[4];
            Pt403 uv[4];
            Col403 white(0xffffffff);
            float sx = DAT_0056b0ac->GetWidth() * 0.0009765625f;
            float sy = DAT_0056b0ac->GetHeight() * 0.0013020833721384406f;
            uv[2].x = t;
            uv[1].x = t;
            pos[0].x = 0;
            pos[3].x = 0;
            pos[1].y = 687.0f;
            pos[0].y = 687.0f;
            pos[3].y = 719.0f;
            pos[2].y = 719.0f;
            uv[1].y = 0;
            uv[0].y = 0;
            uv[3].y = 1.0f;
            uv[2].y = 1.0f;
            uv[3].x = 0;
            pos[1].x = pos[2].x = t * 1024.0f;
            uv[0].x = 0;
            for (int i = 0; i < 4; i++) {
                pos[i].x *= sx;
                pos[i].y *= sy;
            }
            if (DAT_0056b0ac->BeginScene()) {
                DAT_0056b0ac->SetRenderState(5, 0);
                DAT_0056b0ac->vf42(8);
                DAT_0056b0ac->vf38(0);
                back->Draw(DAT_0056b0ac, 0);
                ((float*)front)[0xf] = t * scale + offset;
                front->Draw(DAT_0056b0ac, 0);
                DAT_0056b0ac->vf35((Sprite*)quad);
                DAT_0056b0ac->vf66((HudPoint*)pos, (HudPoint*)uv, white.argb, 1.0f, 1.0f);
                TextRenderer* tr = (TextRenderer*)font;
                tr->color = white.argb;
                tr->scaleX = sx;
                tr->scaleY = sy;
                tr->Print(DAT_0056b0ac, DAT_005070d8 * sx, DAT_005070dc * sy, text.c_str(), 0, 1.0f, 1.0f);
                DAT_0056b0ac->EndScene();
                DAT_0056b0ac->Present();
            }
        }
    }
}

// Opaque types for the replay loader (savegame/replay<n>.dat).
class ReplayFile {                                    // 0x4078 bytes; 0x4a4e00 ctor(path, mode 0x81), 0x4a4d90 dtor
public:
    ReplayFile(const char* path, int mode);
    ~ReplayFile();
    void Read(void* dst, int bytes);                  // 0x4a5e00
    void Close();                                     // 0x4a5d00
    BYTE data[0x4078];
};
extern bool FileExists(const char* path);             // 0x4a6050

// One car's record in the replay file (0x1d4 bytes).
struct CarRecord {
    BYTE setup[0x20];                                 // read by ReplayCar::ReadSetup
    int a20, a24, a28;
    float a2c;
    int blk[90];
    int blk2[10];
    int a1c0, a1c4, id, a1cc;
};
class ReplayCar {                                     // 0x1a88 bytes
public:
    ReplayCar(int first, int unk);                    // 0x4909b0 (first car)
    ReplayCar(const char* name);                      // 0x490b90 (other cars)
    void ReadSetup(CarRecord* r);                     // 0x490fb0
    void Finish();                                    // 0x491130
    BYTE unk0[0x20];
    int unk20, unk24;
    BYTE* body;                                       // +0x28: car body
    int unk2c;
    BYTE unk30[0x1888 - 0x30];
    int blk[90];                                      // +0x1888
    BYTE unk19f0[0x1a14 - 0x19f0];
    int id;                                           // +0x1a14
    BYTE unk1a18[8];
    float unk1a20;
    int unk1a24;
    int blk2[10];                                     // +0x1a28
    int unk1a50, unk1a54, unk1a58;
    BYTE unk1a5c[0x1a6c - 0x1a5c];
    int unk1a6c;
    BYTE unk1a70[0x1a88 - 0x1a70];
};
struct GhostTrack {                                   // car body +0x1c874
    BYTE unk0[0x74];
    int count[3];                                     // +0x74
    int* data[3];                                     // +0x80
    void Reset();                                     // 0x46bbc0
};
struct GhostLog { void Read(ReplayFile* f); };        // car body +0x1bed0, 0x401b00
class CarBody { public: void FUN_0046be50(int mode); };

// NONMATCHING: RALLYTROPHY 0x403ca0
// Calling convention (thiscall on the header, bool result), object sizes, constructor arguments, the PathText
// string calls and block order follow the target. Open: the frame is 4 bytes smaller, so `this` lands in ebx
// instead of ebp; and the target destroys the string in the shared failure exit with an out-of-line
// _Tidy(true) call (here expanded inline).
bool ReplayHeader::Load(int index)
{
    PathText name("savegame/replay");
    char num[2];                                      // the target reserves only two bytes for _itoa
    _itoa(index, num, 10);
    name.AppendCall(num);
    name.AppendCall(".dat");
    if (FileExists(name.c_str())) {
        ReplayFile* f = new ReplayFile(name.c_str(), 0x81);
        int magic2, magic1;
        f->Read(&magic1, 4);
        f->Read(&magic2, 4);
        if (magic1 == 0x198bf3ae && magic2 == 0x20020109) {
            f->Read(this, 0x70);
            DAT_0050132c = *(int*)(raw + 0x2c);
            DAT_00501464 = *(int*)(raw + 0x30);
            DAT_00501458 = raw[0x6e];
            DAT_00501468 = *(int*)(raw + 0x34);
            DAT_0050145c = raw[0x6c];
            NetRace* race = DAT_00501450;
            race->unk28 = *(int*)(raw + 0x24);
            race->unk2c = *(int*)(raw + 0x28);
            int cars = *(int*)(raw + 0x68) + 1;
            for (int i = 0; i < cars; i++) {
                ReplayCar* car = i == 0 ? new ReplayCar(1, 0) : new ReplayCar("dummy");
                CarRecord r;
                int j;
                f->Read(&r, 0x1d4);
                car->ReadSetup(&r);
                car->unk20 = r.a20;
                car->unk1a20 = r.a2c;
                for (j = 0; j < 90; j++)
                    car->blk[j] = r.blk[j];
                car->unk24 = r.a24;
                car->unk2c = r.a28;
                for (j = 0; j < 10; j++)
                    car->blk2[j] = r.blk2[j];
                car->unk1a50 = r.a1c0;
                car->unk1a58 = r.a1c4;
                car->id = r.id;
                car->unk1a6c = r.a1cc;
                car->Finish();
                race->FUN_00485f60(car);
                BYTE* body = car->body;
                GhostTrack* g = (GhostTrack*)(body + 0x1c874);
                g->Reset();
                f->Read(g, 0x8c);
                if (g->count[0]) {
                    g->data[0] = (int*)PoolAlloc(g->count[0] * 4);
                    f->Read(g->data[0], g->count[0] * 4);
                }
                if (g->count[1]) {
                    g->data[1] = (int*)PoolAlloc(g->count[1] * 4);
                    f->Read(g->data[1], g->count[1] * 4);
                }
                if (g->count[2]) {
                    g->data[2] = (int*)PoolAlloc(g->count[2] * 4);
                    f->Read(g->data[2], g->count[2] * 4);
                }
                ((CarBody*)body)->FUN_0046be50(2);
                int has;
                f->Read(&has, 4);
                if (has)
                    ((GhostLog*)(body + 0x1bed0))->Read(f);
            }
            race->FUN_0048f600(f);
            f->Close();
            delete f;
            return true;
        }
        f->Close();
        delete f;
    }
    return false;
}
