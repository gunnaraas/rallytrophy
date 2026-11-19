// Loading screen object (0x403270-0x403ca0): background quads, "LoadingT<n>.tga" textures, progress bar,
// plus the replay file code of 0x403ca0-0x404b9f (loader, SaveReplay/SaveChamp/Write/WriteState/Load; one TU) and the std::string::append(const char*, size_t) instantiation.
#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include "rallytrophy.h"
#include "unk_403270.h"
#include "unk_404100.h"
#include "unk_4018f0.h"
#include "unk_416240.h"
#include <string>

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

// Twin of PathText for ReplayHeader::Load alone: the destructor is _Tidy(true) with _Tidy defined inline, so each exit
// can expand it or call it. A _Tidy that an earlier function of the TU has used loses the `mov state, -1` before the
// out-of-line call (the target has it), hence a class of its own. See study/FINDINGS.md.
class ReplayPath {
public:
    ReplayPath(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ReplayPath(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        size_t n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~ReplayPath() { Tidy(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Tidy(bool built)
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
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

// FUNCTION: RALLYTROPHY 0x4033f0
// colorA[3] stored through volatile pins "pop edi" after the colorA stores, before wrapV.
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
    *(volatile float*)&q->colorA[3] = 1.0f;
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

static inline int Bal(int a) { return a + 1; }  // discarded calls: inline-budget ballast
// NONMATCHING: RALLYTROPHY 0x403ca0
// 2026-11-16: `double t = rec.r.a2c; car->unk1a20 = (float)t;` gives the original's fld/fstp copy, but the int copies then rotate registers (edx for eax), 22 insns. The mov esi,eax placement (car bound before the Read pushes) is the real blocker; ~65 car-form variants leave it.
// Calling convention (thiscall on the header, bool result), object sizes, constructor arguments, the PathText
// string calls and block order follow the target. Direct singleton field stores before taking the `race` alias
// improve the object comparison (158 differing bytes vs. 589 in the prior form), but the frame remains four
// bytes smaller and the target's shared failure cleanup calls out-of-line _Tidy(true) (here expanded inline).
// Tried 2026-10-30 night: record is {int hdr; CarRecord} (0x1d4 read, ReadSetup gets +4), global store order, cars/decl-order/pragma/early-return variants; still differs: name sits 4 bytes higher (target's cars/new temp is above PathText), fld/fstp float copy, failure-path out-of-line _Tidy(true).
// Tried 2026-10-31 re-pass: hoisting cars/f/race/i/num/magic/has/new-temp locals above PathText, nf/nc temp-var forms, volatile cars: the new-temp slot stays at [esp+0x1c] below name (target [esp+0x2c] above it), all unchanged at 79 diff lines; volatile forms worse (164-205); 9 other diffs (fld/fstp float copy, failure-path _Tidy) untouched; too far (>10 insns) for permute.
// 2026-11-16 round 23: ~35 variants, none better (39 insns/100 bytes). FSet(float&,float) forceinline gives the fld/fstp but reorders loads (51 insns); a PathText twin with ~Tidy(true) matches the failure exit but then the success exit is out of line too (1100 bytes, 44 insns; explicit Free(true) first: 1150); Bal(1) padding, early returns, ok-flag single exit, new-expression if/else forms: no effect or worse.
// 2026-11-16 round 27 (exit study): the failure exit now calls _Tidy(true) out of line and the success exit expands it, as the target
// (39 -> 10 insns, 100 -> 27 bytes, size equal). Needed: two plain `return false` exits (the failure exit textually first), ReplayPath (twin
// with an inline _Tidy), ten discarded Bal() calls before the success cleanup. Left: `mov esi,eax` placement after the Read pushes in the
// car loop and the fld/fstp float copy.
// 2026-11-16 round 27b: ~55 more variants, none better (10 insns). A float temp (`float q = rec.r.a2c; car->unk1a20 = q;`) gives the fld/fstp but shifts register choice (22 insns); car as t/car copy, if/else, hoisted car, const car, casts, Read(&rec.r), rec/car order, reorder.py: `mov esi,eax` stays before the Read pushes.
// 2026-11-16 F10: inline helpers (Pick(car) after Read, SetCar, CopyF/CopyI/CopyD for the a2c and a20 copies): 8 variants, car helpers 10 insns unchanged (compiler still hoists mov esi,eax), CopyF(float&,const float&) stays mov/mov, CopyD(double) gives fld/fstp but 22 insns.
// 2026-11-16 F13: group helpers (PickCar ternary, ReadRec/ReadCar/ReadGroup Read+ReadSetup, CopyRec field copies, PickInto out-param, New0/NewD arms), all 2^3 combinations + ~12 more: 10 insns unchanged, mov esi,eax still hoisted.
// 2026-11-18: real std::string members tried, no gain: RpStd derived local with real ctor/append (1162-1075 bytes at every N 0..20, 744+ differing; real ctor calls assign out of line 0x404ba0 where the target expands Grow/memcpy/Eos); real append(s) via cast twin for the two AppendCall sites (N>=10-11 gives the identical 1117 bytes/27 differing, no change).
bool ReplayHeader::Load(int index)
{
    ReplayPath name("savegame/replay");
    char num[2];                                      // the target reserves only two bytes for _itoa
    _itoa(index, num, 10);
    name.AppendCall(num);
    name.AppendCall(".dat");
    ReplayFile* f;
    if (!FileExists(name.c_str())) return false;
    {
        f = new ReplayFile(name.c_str(), 0x81);
        int magic2, magic1;
        f->Read(&magic1, 4);
        f->Read(&magic2, 4);
        if (magic1 != 0x198bf3ae || magic2 != 0x20020109) {
            f->Close();
            delete f;
            return false;
        }
        {
            f->Read(this, 0x70);
            int t6e = raw[0x6e];
            DAT_0050132c = *(int*)(raw + 0x2c);
            DAT_00501464 = *(int*)(raw + 0x30);
            DAT_00501468 = *(int*)(raw + 0x34);
            DAT_00501458 = t6e;
            DAT_0050145c = raw[0x6c];
            int t2c = *(int*)(raw + 0x28);
            DAT_00501450->unk28 = *(int*)(raw + 0x24);
            DAT_00501450->unk2c = t2c;
            NetRace* race = DAT_00501450;
            int cars = *(int*)(raw + 0x68) + 1;
            for (int i = 0; i < cars; i++) {
                ReplayCar* car = i == 0 ? new ReplayCar(1, 0) : new ReplayCar("dummy");
                struct { int hdr; CarRecord r; } rec;
                int j;
                f->Read(&rec, 0x1d4);
                car->ReadSetup(&rec.r);
                car->unk20 = rec.r.a20;
                car->unk1a20 = rec.r.a2c;
                for (j = 0; j < 90; j++)
                    car->blk[j] = rec.r.blk[j];
                car->unk24 = rec.r.a24;
                car->unk2c = rec.r.a28;
                for (j = 0; j < 10; j++)
                    car->blk2[j] = rec.r.blk2[j];
                car->unk1a50 = rec.r.a1c0;
                car->unk1a58 = rec.r.a1c4;
                car->id = rec.r.id;
                car->unk1a6c = rec.r.a1cc;
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
            Bal(0); Bal(1); Bal(2); Bal(3); Bal(4); Bal(5); Bal(6); Bal(7); Bal(8); Bal(9);  // inline-budget ballast (no code)

            f->Close();
            delete f;
            return true;
        }
    }
}

#define H6AT(type, base, off) (*(type*)((char*)(base) + (off)))

// Current race mode DAT_00501450 (RaceMode in unk_481390.h): players vector at +8/+0xc, local player +0x24, stage +0x30.
struct H6Race {
    char pad0[8];
    int* first;                        // 0x08
    int* last;                         // 0x0c
    char pad10[0x14];
    class H6Player* player;            // 0x24
    int unk28;                         // 0x28
    int unk2c;                         // 0x2c
    S10Stage* stage;                   // 0x30
};
class H6Player {
public:
    char name[0x28];                   // 0x00
    char* car;                         // 0x28 (Car*)
};
extern H6Race* DAT_00501450_;
extern int DAT_0050132c, DAT_00501454, DAT_00501458, DAT_0050145c, DAT_00501464, DAT_00501468;
extern char* __cdecl FUN_004c1902(int value, char* buf, int radix); // _itoa

// std::string seen through its layout; each site is pinned to the expansion depth the compiler used there.
struct H6Str {
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
    void Tidy(bool built);                              // 0x402700
    void AssignCall(const char* s, unsigned int n);     // 0x404ba0
    bool Grow(unsigned int n, bool trim);               // 0x402980
    void Eos(unsigned int n);                           // 0x402960
    static void Xlen();                                 // 0x4bd00b
    H6Str(const char* s, char mode, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        if (mode == 'B')
            AssignB(s);
        else
            AssignC(s);
    }
    H6Str(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        ptr = 0;
        len = 0;
        res = 0;
        AssignCall(s, strlen(s));
    }
    ~H6Str() { Free(); }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free()
    {
        if (ptr != 0) {
            if (Refcnt(ptr) != 0 && Refcnt(ptr) != 255) {
                --Refcnt(ptr);
            } else {
                alloc.deallocate(ptr - 1, res + 2);
            }
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    void AssignB(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            Eos(n);
        }
    }
    __forceinline void AssignC(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            std::char_traits<char>::copy(ptr, s, n);
            len = n;
            ptr[n] = 0;
        }
    }
    void AppendD(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            Xlen();
        }
        if (n > 0) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                Eos(total);
            }
        }
    }
    void AppendC(const char* s)
    {
        unsigned int n = strlen(s);
        if (std::string::npos - len <= n) {
            Xlen();
        }
        if (n > 0) {
            unsigned int total = len + n;
            if (Grow(total, false)) {
                std::char_traits<char>::copy(ptr + len, s, n);
                len = total;
                ptr[total] = 0;
            }
        }
    }
};

// FUNCTION: RALLYTROPHY 0x404100
// Exact only because this file also holds the TU's earlier functions: VC6 clones the epilogue at the dec-refcount arm of the
// inlined string dtor in the first function emitted of a TU (any earlier emitted function stops it), so this must not be
// split back into a file of its own (study 2026-11-16 r29).
bool ReplayFiles::SaveReplay(int index, const char* label)
{
    H6Player* player = DAT_00501450_->player;
    if (player == 0)
        return false;
    char num[2];
    H6Str s("savegame/replay");
    FUN_004c1902(index, num, 10);
    s.AppendC(num);
    s.AppendC(".dat");
    return Write(s.c_str(), label);
}

// FUNCTION: RALLYTROPHY 0x4042c0
bool ReplayFiles::SaveChamp(void)
{
    H6Race* race = DAT_00501450_;
    H6Player* pl = DAT_00501450_->player;
    if (pl == 0)
        return false;
    H6Str s("savegame/champ", 'B');
    char num[8];
    FUN_004c1902(race->unk28, num, 10);
    s.AppendD(num);
    s.AppendD("_");
    FUN_004c1902(race->unk2c, num, 10);
    s.AppendC(num);
    s.AppendC(".dat");
    bool r = Write(s.c_str(), "autosave");
    return r;
}

// FUNCTION: RALLYTROPHY 0x404590
bool ReplayFiles::Write(const char* filename, const char* label)
{
    H6Player* player = DAT_00501450_->player;
    if (player == 0)
        return false;
    File* f = new File(filename, 0x82);
    int magic = 0x198bf3ae;
    f->Write(&magic, 4);
    magic = 0x20020109;
    f->Write(&magic, 4);
    memset(this, 0, 0x70);
    strcpy((char*)this, label);
    H6AT(int, this, 0x24) = DAT_00501450_->unk28;
    H6AT(int, this, 0x28) = DAT_00501450_->unk2c;
    H6AT(int, this, 0x2c) = DAT_0050132c;
    H6AT(int, this, 0x30) = DAT_00501464;
    H6AT(int, this, 0x34) = DAT_00501468;
    b[0x6c] = DAT_0050145c != 0;
    GetLocalTime((SYSTEMTIME*)&b[0x38]);
    strcpy((char*)&b[0x48], DAT_00501450_->player->name);
    H6AT(int, this, 0x20) = H6AT(int, player, 0x2c);
    if (DAT_00501454 != 0)
        b[0x6d] = 1;
    if (DAT_00501458 != 0)
        b[0x6e] = 1;
    b[0x6f] = 0;
    if (b[0x6d] != 0 || b[0x6e] != 0) {
        int n;
        if (DAT_00501450_->first == 0)
            n = 0;
        else
            n = DAT_00501450_->last - DAT_00501450_->first;
        H6AT(int, this, 0x68) = n - 1;
    }
    f->Write(this, 0x70);
    H6Player* local = DAT_00501450_->player;
    WriteState(local, f);
    if (H6AT(int, this, 0x68) != 0) {
        for (int i = 0;; i++) {
            int n;
            if (DAT_00501450_->first == 0)
                n = 0;
            else
                n = DAT_00501450_->last - DAT_00501450_->first;
            if (i >= n)
                break;
            // shortcut: volatile read keeps the original's second load of the vector start
            H6Player* p = (H6Player*)(*(int* volatile*)&DAT_00501450_->first)[i];
            if (p != local)
                WriteState(p, f);
        }
    }
    DAT_00501450_->stage->Save(f);
    f->Close();
    delete f;
    return true;
}

// FUNCTION: RALLYTROPHY 0x4047c0
void ReplayFiles::WriteState(H6Player* p, File* f)
{
    // The 0x1d4-byte per-player block, in the order the original fills it.
    struct {
        int size;
        char name[0x20];
        int a, b, c;                   // player +0x20, +0x24, +0x2c
        float d;                       // player +0x1a20
        int block[0x5a];               // player +0x1888
        int tail[10];                  // player +0x1a28
        int e, g, h, i;                // player +0x1a50, +0x1a58, +0x1a14, +0x1a6c
    } rec;
    strcpy(rec.name, (char*)p);
    rec.a = H6AT(int, p, 0x20);
    rec.b = H6AT(int, p, 0x24);
    rec.c = H6AT(int, p, 0x2c);
    memcpy(rec.tail, (char*)p + 0x1a28, 0x28);
    rec.e = H6AT(int, p, 0x1a50);
    memcpy(rec.block, (char*)p + 0x1888, 0x168);
    rec.d = H6AT(float, p, 0x1a20);
    rec.g = H6AT(int, p, 0x1a58);
    rec.h = H6AT(int, p, 0x1a14);
    rec.i = H6AT(int, p, 0x1a6c);
    rec.size = 0x1d4;
    f->Write(&rec, 0x1d4);
    char* car = p->car;
    char* sub = car + 0x1c75c;
    f->Write(sub, 0x8c);
    if (H6AT(int, sub, 0x74) != 0)
        f->Write(H6AT(void*, sub, 0x80), H6AT(int, sub, 0x74) << 2);
    if (H6AT(int, sub, 0x78) != 0)
        f->Write(H6AT(void*, sub, 0x84), H6AT(int, sub, 0x78) * 4);
    if (H6AT(int, sub, 0x7c) != 0)
        f->Write(H6AT(void*, sub, 0x88), H6AT(int, sub, 0x7c) * 4);
    int v = H6AT(int, car, 0x1bef0);
    f->Write(&v, 4);
    ((RecList*)(car + 0x1bed0))->WriteTo((RecFile*)f);
}

// FUNCTION: RALLYTROPHY 0x404930
char ReplayFiles::Load(int index, ReplayFiles* record)
{
    record->b[0x6f] = 1;
    H6Str s("savegame/replay", 'C');
    char num[2];
    FUN_004c1902(index, num, 10);
    s.AppendD(num);
    s.AppendD(".dat");
    if (File::Exists(s.c_str())) {
        File* f = new File(s.c_str(), 0x81);
        int magic1, magic2;
        f->Read(&magic1, 4);
        f->Read(&magic2, 4);
        if (magic1 == 0x198bf3ae && magic2 == 0x20020109)
            f->Read(record, 0x70);
        f->Close();
        delete f;
    }
    return record->b[0x6f];
}
