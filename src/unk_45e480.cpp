// Car constructor file (0x45e480-0x464aaf): CarObject (the original Car::Car / Car::~Car), the car.ini, tire.ini and
// texture loaders and the small helpers. Map: analysis/range-45e480.md. Layout: include/unk_45e480.h.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_45e480.h"
#include "unk_46a8a0.h"
#include "unk_4666a0.h"
#include "unk_465e90.h"
#include "unk_47c460.h"
#include "unk_4741a0.h"
#include "unk_468c40.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

// ---- declarations for the constructor 0x45e480 ----
//
// Conventions of this file (details in the notes of the constructor):
//  - CF(T, off) reads/writes a car.ini field of the Car by byte offset (the fields that no module names yet).
//  - VC6 spends a small inline budget per function: the later call sites get expanded, the earlier ones stay calls.
//    The constructor is so large that its early std::string / std::vector calls (0x402700 _Tidy, 0x402980 _Grow,
//    0x402960 _Eos, 0x403ba0 append, 0x4bcf30 reserve, 0x409130 operator+) are out of line while the late ones are
//    expanded; CarString, PathString and PtrVec write each site the way the original has it.

extern int DAT_00508f60;               // number of cars created so far (1 << n marks the parts of a car)
extern int DAT_005012f0;
extern int DAT_00501428;
extern int DAT_0050145c;
extern const float DAT_004e1c38;       // 0.008: size passed to the window / lamp glow objects
extern CarClassRow DAT_004fb0f8[];     // the car class table (12 bytes per row)
extern unsigned int DAT_004fb130[];    // car.ini checksum per car model
extern unsigned int DAT_004fb188[];    // engine.ini checksum
extern unsigned int DAT_004fb1e0[];    // tires.ini checksum

// Passes a float through an inline function's parameter (the copy then goes through the FPU).
inline float PassFloat(float v)
{
    return v;
}

#pragma function(strcmp)

char* CarCopy(char* dst, const char* src, size_t n); // 0x461a10 (std::char_traits<char>::copy)

// A std::string (same layout) whose pieces the original's constructor keeps out of line or expands inline
// at fixed places: _Tidy 0x402700, _Grow 0x402980, _Eos 0x402960, append 0x403ba0, destructor 0x430720.
class CarString {
public:
    CarString(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            Eos(n);
        }
    }
    ~CarString();                      // 0x430720
    void Tidy(bool built);             // 0x402700
    bool Grow(unsigned int n, bool trim); // 0x402980
    void Eos(unsigned int n);          // 0x402960
    void AppendOut(const char* s, unsigned int n); // 0x403ba0
    void AppendInline(const char* s)
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && Grow(n = len + m, false)) {
            memcpy(ptr + len, s, m);
            Eos(n);
        }
    }
    // The late copy of the directory name (basic_string(const basic_string&) expanded inline, with max_size() and
    // _Copy left as calls).
    CarString(const CarString& x) : alloc(x.alloc)
    {
        ptr = 0;
        len = 0;
        res = 0;
        unsigned int n = x.len;
        if (std::string::npos < n) {
            n = std::string::npos;
        }
        if (0 < n && n == x.len && Refcnt(x.c_str()) < 254) {
            ptr = (char*)x.c_str();
            len = x.len;
            res = x.res;
            ++Refcnt(ptr);
        } else if (GrowCall(n, true)) {
            memcpy(ptr, x.c_str(), n);
            ptr[len = n] = 0;
        }
    }
    // append(const char*) expanded inline: _Grow with the constant max_size, std::char_traits<char>::copy as a call.
    void AppendLate(const char* s)
    {
        unsigned int m = strlen(s);
        if (std::string::npos - len <= m) {
            std::_Xlen();
        }
        unsigned int n;
        if (0 < m && GrowConst(n = len + m, false)) {
            CarCopy(ptr + len, s, m);
            Eos(n);
        }
    }
    unsigned int MaxSize() const;      // 0x402aa0 (std::string::max_size)
    void Copy(unsigned int n);         // 0x402ab0 (std::string::_Copy)
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
#define CARSTRING_GROW_BODY(TIDY_FALSE)                                                              \
    if (ptr != 0 && Refcnt(ptr) != 0 && Refcnt(ptr) != 255) {                                        \
        if (n == 0) {                                                                                \
            --Refcnt(ptr), TIDY_FALSE;                                                               \
            return false;                                                                            \
        } else {                                                                                     \
            Copy(n);                                                                                 \
            return true;                                                                             \
        }                                                                                            \
    }                                                                                                \
    if (n == 0) {                                                                                    \
        if (trim) {                                                                                  \
            Tidy(true);                                                                              \
        } else if (ptr != 0) {                                                                       \
            Eos(0);                                                                                  \
        }                                                                                            \
        return false;                                                                                \
    } else {                                                                                         \
        if (trim && (31 < res || res < n)) {                                                         \
            Tidy(true);                                                                              \
            Copy(n);                                                                                 \
        } else if (!trim && res < n) {                                                               \
            Copy(n);                                                                                 \
        }                                                                                            \
        return true;                                                                                 \
    }
    // _Grow with max_size() as a call; _Tidy() as a call (rearlight name) or as three stores (the late path copy).
    bool GrowCallTidyCall(unsigned int n, bool trim)
    {
        if (MaxSize() < n) {
            std::_Xlen();
        }
        CARSTRING_GROW_BODY(Tidy(false))
    }
    bool GrowCall(unsigned int n, bool trim)
    {
        if (MaxSize() < n) {
            std::_Xlen();
        }
        CARSTRING_GROW_BODY((ptr = 0, len = 0, res = 0))
    }
    // _Grow with max_size() folded to 0xfffffffd.
    bool GrowConst(unsigned int n, bool trim)
    {
        if (0xfffffffd < n) {
            std::_Xlen();
        }
        CARSTRING_GROW_BODY((ptr = 0, len = 0, res = 0))
    }
    // assign(const char*) expanded inline (the name of the left rear light glow object).
    void AssignInline(const char* s)
    {
        unsigned int n = strlen(s);
        if (GrowCallTidyCall(n, true)) {
            memcpy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    operator const std::string&() const { return *(const std::string*)this; }
    const char* CStrOut() const;       // 0x464980 (std::string::c_str, out of line)
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};
class CtorPathString {
public:
    ~CtorPathString();                     // 0x430720 (std::string destructor, out of line)
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};
CtorPathString JoinPath(const CarString& dir, const char* name); // 0x409130 (operator+)
TmpString CarPath(const CarString& dir, const char* name); // 0x409130 (operator+)

// Byte-offset access to the car.ini fields of the car that no other module names yet.
#define CF(T, off) (*(T*)((BYTE*)this + (off)))

// Methods of objects the constructor wires together (all out of line in the original).
struct EffectView {
    void SetParent(void* parent);      // 0x4b2b30: attach an effect to the car body
};
// std::vector<MaterialSprite*> as the constructor uses it: insert(end(), 1, x) is expanded inline from the VC6 STL
// source, with _Construct, _Ucopy and _Ufill left as calls (0x403140, 0x424320, 0x424350).
struct PtrVec {
    BYTE alloc;
    MaterialSprite** first;            // 0x04
    MaterialSprite** last;             // 0x08
    MaterialSprite** end;              // 0x0c
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    MaterialSprite** Ucopy(MaterialSprite** f, MaterialSprite** l, MaterialSprite** d); // 0x424320
    void Ufill(MaterialSprite** d, unsigned int n, MaterialSprite* const& x);            // 0x424350
};
void ConstructPtr(MaterialSprite** p, MaterialSprite* const& v); // 0x403140 (std::_Construct, cdecl)

struct PtrVectorOps {
    void Insert(MaterialSprite** pos, unsigned int n, MaterialSprite* const* value); // 0x4bc680 (vector::insert(pos, n, x))
};
struct MeshView {
    void Link(SnapMesh* other);        // 0x4b91c0: join a mesh to the body mesh
};
struct BodyView {
    void SetCollision(ModelFilePart* part); // 0x41c920
};

// The car body (Car+0x24, 0x284 bytes, ctor 0x41bab0).
class BodyObject : public PolyObject {
public:
    BodyObject(ModelFilePart* part, int a, RawVec3* b, RawVec3* c); // 0x41bab0
    BYTE data[0x280];
};
// The drive-light caster (Car+0x1f18, 0x178 bytes, ctor 0x4ba5a0): only the fields the constructor sets.
class ShadowObject : public PolyObject {
public:
    ShadowObject();                    // 0x4ba5a0
    BYTE data[0x174];
};
struct ShadowView {
    BYTE unk0[8];
    int bits;                          // 0x08
    BYTE unkc[0x14 - 0xc];
    RawVec3 pos;                       // 0x14
    RawMatrix matrix;                  // 0x20
    BYTE unk60[0xc4 - 0x60];
    float fc4;                         // 0xc4
    BYTE unkc8[0x114 - 0xc8];
    std::string name;                  // 0x114
    BYTE unk124[4];
    float f128, f12c, f130, f134;      // 0x128..
    BYTE unk138[0x158 - 0x138];
    float f158, f15c, f160, f164, f168, f16c, f170, f174;
};
// The reflection renderer (Car+0x20ec, 0x330 bytes, ctor 0x4703c0).
class ReflectionObject {
public:
    ReflectionObject();                // 0x4703c0
    void Create(RenderDevice* device, int size); // 0x470610
    BYTE data[0x330];
};
// The glow / window sprite objects (0x12c bytes, ctor 0x4a6ac0).
class GlowObject : public EffectBase {
public:
    GlowObject(PolyObject* body, MaterialSprite* material, float size); // 0x4a6ac0
    BYTE data[0x12c - 4];
};
struct NamedObject {
    BYTE unk0[0x114];
    std::string name;                  // 0x114
    SnapMeshList meshes;               // 0x124
};
struct LightView {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    RawMatrix matrix;                  // 0x20
    BYTE unk60[0x124 - 0x60];
    void* texture;                     // 0x124
    float exponent;                    // 0x128
    float size;                        // 0x12c
    float scale;                       // 0x130
    float bias;                        // 0x134
    int mirror;                        // 0x138
};
// A material of the model (node data of the three material lists).
struct MatView {
    void* texture;                     // 0x00
    int unk4;                          // 0x04
    void* dirt;                        // 0x08: dirt texture of the glow materials
    BYTE unkc[0x18 - 0xc];
    int f18;                           // 0x18
    int f1c;                           // 0x1c
    BYTE unk20[0x38 - 0x20];
    int f38;                           // 0x38
    BYTE unk3c[0x4c - 0x3c];
    int f4c;                           // 0x4c
    BYTE unk50[0x87 - 0x50];
    BYTE b87;                          // 0x87
    BYTE unk88[0x98 - 0x88];
    int f98;                           // 0x98
    int flags;                         // 0x9c
    BYTE unka0[0xa8 - 0xa0];
    const char* name;                  // 0xa8
    int fac;                           // 0xac
    BYTE unkb0[0xb8 - 0xb0];
    int fb8;                           // 0xb8
    int fbc;                           // 0xbc
    int fc0;                           // 0xc0
    const char* Name() const { return name ? name : ""; }
};
struct TexView {
    BYTE unk0[0x60];
    int f60;                           // 0x60
    void FUN_004a82b0();               // 0x4a82b0
};

// NONMATCHING: RALLYTROPHY 0x45e480
// 13542 vs 13705 bytes. Logic complete (call sequence, constants, strings and all EH state stores match: 463/461
// key lines). Remaining differences: stack frame layout (locals sit 0x1c bytes lower than in the original, so almost
// every [ebp-N] operand differs), register choice in the long constant-store runs, the dead ShadowCaster stores and
// the std::string/vector inline-vs-call decisions of VC6's per-function inline budget, which are reproduced with
// CarString/PtrVec/ModelObject helpers (see the declarations above).
// SYMBOL: ??0CarObject@@QAE@PAVCarTextureCache@@HHHHHI@Z
CarObject::CarObject(CarTextureCache* a, int b, int c, int d, int e, int f, unsigned int g)
{
    // ---- phase B/C: counters, the shared caches, default values ----
    int i;
    DAT_00508f60++;
    slot = f;
    ownsCaches = 0;
    damaged = 0;
    flags = g;
    model = c;
    if (a) {
        modelCache = (CarModelCache*)((BYTE*)DAT_0056b0c8 + 0x139424);
        textureCache = a;
    } else {
        ownsCaches = 1;
        CarModelCache* mc = new CarModelCache;
        modelCache = mc;
        textureCache = new CarTextureCache;
        mc->flag0 = 1;
    }
    int detail = DAT_004fb0f8[c].kind;
    if (detail >= 2 && !(g & 1)) {
        detail = 1;
    }
    textureCache->SetDetail(detail);
    arg1bcec = e;
    kind = d;
    CF(BYTE, 0x1f158) = 0;
    CF(BYTE, 0x1350) = 0;
    CF(int, 0x1c298) = 0;
    CF(int, 0x1c29c) = 0;
    CF(int, 0x1c2a0) = 0;
    CF(int, 0x1c2a4) = 0;
    CF(int, 0x1c2a8) = 0;
    CF(int, 0x1c2ac) = 0;
    CF(int, 0x1c2b0) = 0;
    CF(int, 0x1f2ac) = 0;
    for (i = 0; i < 8; i++) {
        CF(int, 0x1f24c + i * 12) = 0;
    }
    CF(int, 0x8) = 2;
    CF(int, 0x1c730) = 0;
    CF(int, 0x1c734) = 0;
    CF(int, 0x1c73c) = 0;
    CF(int, 0x1c740) = 0;
    CF(int, 0x1c744) = 0;
    CF(int, 0x1c748) = 0;
    CF(float, 0x1f84) = 1.0f;
    CF(float, 0x1f70) = 1.0f;
    CF(float, 0x1fac) = 1.0f;
    CF(float, 0x1f98) = 1.0f;
    CF(float, 0x1fc4) = 1.0f;
    CF(float, 0x1fb0) = 1.0f;
    CF(int, 0x1f78) = 0;
    CF(int, 0x1f74) = 0;
    CF(int, 0x1f80) = 0;
    CF(float, 0x1fec) = 1.0f;
    CF(float, 0x1fd8) = 1.0f;
    CF(int, 0x1f94) = 0;
    CF(int, 0x1fb8) = 0;
    CF(int, 0x1fb4) = 0;
    CF(int, 0x1fc0) = 0;
    CF(int, 0x1f7c) = 0;
    CF(int, 0x1f8c) = 0;
    CF(int, 0x1f88) = 0;
    CF(int, 0x1f9c) = 0;
    CF(int, 0x1f90) = 0;
    CF(int, 0x1fa8) = 0;
    CF(int, 0x1fa4) = 0;
    CF(int, 0x1fa0) = 0;
    CF(int, 0x1fd4) = 0;
    CF(float, 0x1f30) = 250.0f;
    CF(float, 0x1f34) = 3.5f;
    CF(float, 0x1f38) = 0.100000001f;
    CF(int, 0x1f40) = 0;
    CF(float, 0x1f44) = 0.5f;
    CF(int, 0x1c758) = 0;
    CF(int, 0x1c750) = 0;
    CF(int, 0x1c74c) = 0;
    CF(int, 0x1c754) = 0;
    CF(int, 0x1f68) = 0;
    CF(int, 0x1f6c) = 0;
    CF(int, 0x1fbc) = 0;
    CF(int, 0x1fcc) = 0;
    CF(int, 0x1fc8) = 0;
    CF(int, 0x1fdc) = 0;
    CF(int, 0x1fd0) = 0;
    CF(int, 0x1fe8) = 0;
    CF(int, 0x1fe4) = 0;
    CF(int, 0x1fe0) = 0;
    CF(float, 0x1f4c) = 250.0f;
    CF(float, 0x1f50) = 3.5f;
    CF(float, 0x1f54) = 0.100000001f;
    CF(int, 0x1f5c) = 0;
    CF(float, 0x1f60) = 0.5f;
    CF(int, 0x14fc) = 0;
    CF(int, 0x1ff4) = 0;
    CF(int, 0x1ff8) = 0;
    CF(int, 0x1ffc) = 0;
    CF(int, 0x2004) = 0;
    CF(int, 0x2008) = 0;
    CF(int, 0x20e8) = 0;
    CF(int, 0x24) = 0;
    CF(int, 0x1f18) = 0;
    CF(int, 0x1bd30) = 0;
    CF(int, 0x1bd34) = 0;
    CF(int, 0x1f2c) = 0;
    CF(int, 0x1ff0) = 0;
    CF(int, 0x2018) = 0;
    CF(int, 0x201c) = 0;
    CF(int, 0x2020) = 0;
    CF(int, 0x2024) = 0;
    CF(int, 0x2028) = 0;
    CF(int, 0x202c) = 0;
    CF(int, 0x2048) = 0;
    CF(int, 0x200c) = 0;
    CF(int, 0x20cc) = 0;
    CF(int, 0x20c8) = 0;
    CF(int, 0x20c4) = 0;
    CF(int, 0x20d8) = 0;
    CF(int, 0x20d4) = 0;
    CF(int, 0x20d0) = 0;
    CF(int, 0x20b4) = 0;
    CF(int, 0x20b0) = 0;
    CF(int, 0x20ac) = 0;
    CF(int, 0x2058) = 0;
    CF(int, 0x2054) = 0;
    CF(int, 0x2074) = 0;
    CF(int, 0x2078) = 0;
    CF(int, 0x207c) = 0;
    CF(int, 0x2080) = 0;
    CF(int, 0x2084) = 0;
    CF(int, 0x2088) = 0;
    CF(int, 0x208c) = 0;
    CF(int, 0x2090) = 0;
    CF(int, 0x2094) = 0;
    CF(int, 0x20c0) = 0;
    CF(int, 0x20bc) = 0;
    CF(int, 0x20b8) = 0;
    CF(int, 0x20a8) = 0;
    CF(int, 0x2010) = 0;
    CF(int, 0x204c) = 0;
    CF(int, 0x2014) = 0;
    CF(int, 0x2050) = 0;
    CF(int, 0x1ec8) = 0;
    CF(int, 0x1ecc) = 0;
    CF(int, 0x1eb4) = 1;
    CF(int, 0x20ec) = 0;
    CF(int, 0x1becc) = 0;
    CF(int, 0x1bce4) = 0;
    for (i = 0; i < 4; i++) {
        shadowTyre[i] = 0;
    }
    for (i = 0; i < 3; i++) {
        glow[2 + i] = 0;
        glow[5 + i] = 0;
    }
    glow[0] = 0;
    glow[1] = 0;
    unk1be94 = 0;
    unk1be98 = 0;
    sprite1be90 = 0;
    vec1430.Reserve(0x400);
    vec1464.Reserve(0x400);

    // ---- phase D: file names, the loaders, the model files ----
    CarString dir("data/cars/car_");
    char number[0x28];
    _itoa(b, number, 10);
    dir.AppendOut(number, strlen(number));
    dir.AppendInline("/");
    LoadCarIni(CarPath(dir, "car.ini").c_str(), DAT_004fb130[b]);
    drivetrain.LoadConfig(CarPath(dir, "engine.ini").c_str(), DAT_004fb188[b]);
    LoadTiresIni(CarPath(dir, "tires.ini").c_str(), DAT_004fb1e0[b]);
    LoadTextures(dir);
    CtorPathString modelPath = JoinPath(dir, "model.bmf");
    CtorPathString collisionPath = JoinPath(dir, "collision.bmf");
    if (!(flags & 4)) {
        InitSounds(dir);
    }
    int loadFlag = 0;
    if (DAT_005012f0) {
        loadFlag = 1;
    }
    ModelFile modelFile(modelPath.c_str(), dir.c_str(), textureCache, &materials, loadFlag);
    ModelFile collisionFile(collisionPath.c_str(), dir.c_str(), textureCache, &materials, 0);

    // ---- phase E: sort the parts of the model by name ----
    ModelFilePart* tyre[4];
    ModelFilePart* tyreLod[4];
    tyreLod[0] = 0;
    tyre[0] = 0;
    tyreLod[1] = 0;
    tyre[1] = 0;
    tyreLod[2] = 0;
    tyre[2] = 0;
    ModelFilePart* bodyPart = 0;
    ModelFilePart* bodyLod = 0;
    ModelFilePart* trunkPart = 0;
    ModelFilePart* hoodPart = 0;
    ModelFilePart* interiorHiPart = 0;
    ModelFilePart* crashPart = 0;
    ModelFilePart* trunkCrashPart = 0;
    ModelFilePart* hoodCrashPart = 0;
    tyreLod[3] = 0;
    tyre[3] = 0;
    for (i = 0; i < (int)modelFile.parts.size(); i++) {
        ModelFilePart* part = modelFile.parts[i];
        part->bits = 1 << DAT_00508f60;
        const char* name = "";
        if (part->name) {
            name = part->name;
        }
        if (!strcmp(name, "body")) {
            bodyPart = part;
        } else if (!strcmp(name, "body_lod")) {
            bodyLod = part;
        } else if (!strcmp(name, "hood")) {
            hoodMatrix = part->matrix;
            hoodPart = part;
        } else if (!strcmp(name, "trunk")) {
            trunkMatrix = part->matrix;
            trunkPart = part;
        } else if (!strcmp(name, "wipers")) {
            wipers = new ModelObject(part);
        } else if (!strcmp(name, "interior_lo")) {
            interiorLo = new ModelObject(part);
        } else if (!strcmp(name, "interior_hi")) {
            interiorHiPart = part;
        } else if (!strcmp(name, "steering_wheel_lo")) {
            steeringLo = new ModelObject(part);
        } else if (!strcmp(name, "steering_wheel_hi")) {
            steeringHi = new ModelObject(part);
        } else if (!strcmp(name, "indicator_rpm")) {
            indicatorRpm = new ModelObject(part);
            vec2068.x = part->matrix.m[8];
            vec2068.y = part->matrix.m[9];
            vec2068.z = part->matrix.m[10];
        } else if (!strcmp(name, "indicator_speed")) {
            indicatorSpeed = new ModelObject(part);
            vec205c.x = part->matrix.m[8];
            vec205c.y = part->matrix.m[9];
            vec205c.z = part->matrix.m[10];
        } else if (!strcmp(name, "tire_fl")) {
            tyre[0] = part;
        } else if (!strcmp(name, "tire_fr")) {
            tyre[1] = part;
        } else if (!strcmp(name, "tire_rr")) {
            tyre[2] = part;
        } else if (!strcmp(name, "tire_rl")) {
            tyre[3] = part;
        } else if (!strcmp(name, "tire_fl_lod")) {
            tyreLod[0] = part;
        } else if (!strcmp(name, "tire_fr_lod")) {
            tyreLod[1] = part;
        } else if (!strcmp(name, "tire_rl_lod")) {
            tyreLod[3] = part;
        } else if (!strcmp(name, "tire_rr_lod")) {
            tyreLod[2] = part;
        } else if (!strcmp(name, "shadow_tire_fl")) {
            shadowTyre[0] = new ModelObject(part);
        } else if (!strcmp(name, "shadow_tire_fr")) {
            shadowTyre[1] = new ModelObject(part);
        } else if (!strcmp(name, "shadow_tire_rr")) {
            shadowTyre[2] = new ModelObject(part);
        } else if (!strcmp(name, "shadow_tire_rl")) {
            shadowTyre[3] = new ModelObject(part);
        }
#define LIGHT_BRANCH(NAME, SLOT, K, MIRROR)                                                          \
        else if (!strcmp(name, NAME))                                                               \
        {                                                                                           \
            CarLight* light = new CarLight;                                                         \
            LightView* lv = (LightView*)light;                                                      \
            lv->matrix = part->matrix;                                                              \
            lv->pos[0] = part->pivot[0];                                                            \
            lv->pos[1] = part->pivot[1];                                                            \
            lv->pos[2] = part->pivot[2];                                                            \
            lv->texture = flareTex[K];                                                              \
            light->SetSize(flare[K].radius);                                                        \
            lv->bias = PassFloat(flare[K].bias);                                                               \
            if (MIRROR) {                                                                           \
                lv->mirror = 1;                                                                     \
            }                                                                                       \
            lv->exponent = PassFloat(flare[K].exponent);                                                       \
            SLOT.light = light;                                                                     \
        }
        LIGHT_BRANCH("light_front_left01", lightFrontLeft[0], 0, 0)
        LIGHT_BRANCH("light_front_left02", lightFrontLeft[1], 0, 0)
        LIGHT_BRANCH("light_front_left03", lightFrontLeft[2], 0, 0)
        LIGHT_BRANCH("light_front_right01", lightFrontRight[0], 0, 1)
        LIGHT_BRANCH("light_front_right02", lightFrontRight[1], 0, 1)
        LIGHT_BRANCH("light_front_right03", lightFrontRight[2], 0, 1)
        LIGHT_BRANCH("light_rear_left", lightRear[0], 1, 0)
        LIGHT_BRANCH("light_rear_right", lightRear[1], 1, 1)
        LIGHT_BRANCH("light_brake_left", lightBrake[0], 2, 0)
        LIGHT_BRANCH("light_brake_right", lightBrake[1], 2, 1)
        LIGHT_BRANCH("light_reverse01", lightReverse[0], 3, 0)
        LIGHT_BRANCH("light_reverse02", lightReverse[1], 3, 1)
        else if (!strcmp(name, "ext_pipe01"))
        {
            exhaustFlag = 1;
            exhaustPos.x = part->pivot[0];
            exhaustPos.y = part->pivot[1];
            exhaustPos.z = part->pivot[2];
        }
        if (!strcmp(name, "body_crash") || !strcmp(name, "crash")) {
            crashPart = part;
        }
        if (!strcmp(name, "trunk_crash")) {
            trunkCrashPart = part;
        }
        if (!strcmp(name, "hood_crash")) {
            hoodCrashPart = part;
        }
    }

    // ---- phase F: assemble the car from the parts ----
    CarClassRow* row = &DAT_004fb0f8[model];
    if (bodyLod && row->noLod) {
        bodyPart = bodyLod;
    }
    if (interiorHiPart && (row->hasInterior || (flags & 1))) {
        ModelObject* o = new ModelObject(interiorHiPart);
        o->flags |= 8;
        interiorHi = o;
        o->FUN_004a6d30();
    }
    if (row->tyreLod) {
        for (i = 0; i < 4; i++) {
            if (tyreLod[i]) {
                tyre[i] = tyreLod[i];
            }
        }
    }
    if (row->noLod != row->unk7 && bodyLod) {
        obj1ff0 = new ModelObject(bodyLod);
    }
    BodyObject* bo = new BodyObject(bodyPart, CF(int, 0x1034), &bodyVecA, &bodyVecB);
    body = bo;
    ((NameString_46c2c0*)((BYTE*)bo + 0x114))->Assign("Rigidbody of the car");
    {
        for (i = 0; i < 4; i++) {
            ModelFilePart* t = tyre[i];
            SnapMesh* mesh = t->meshes.At(0);
            float* sp = (float*)spring[i].data;
            sp[0] = t->pivot[0];
            sp[1] = t->pivot[1];
            sp[2] = t->pivot[2];
            wheel[i].position[0] = t->pivot[0];
            wheel[i].position[1] = t->pivot[1];
            wheel[i].position[2] = t->pivot[2];
            wheel[i].vec124.x = t->pivot[0];
            wheel[i].vec124.y = t->pivot[1];
            wheel[i].vec124.z = t->pivot[2];
            wheel[i].LoadModel((const char*)mesh);
        }
    }
    for (i = 0; i < (int)collisionFile.parts.size(); i++) {
        ModelFilePart* part = collisionFile.parts[i];
        if (!strcmp(part->name ? part->name : "", "collision")) {
            ((BodyView*)body)->SetCollision(part);
        }
    }
    if (crashPart && !row->noLod && DAT_0050145c) {
        SnapMesh* m = ((SnapObject*)body)->meshes.At(0);
        ((MeshView*)m)->Link(crashPart->meshes.At(0));
    }
    if (!row->noLod) {
        if (hoodPart) {
            hoodCrash = new ModelObject(hoodPart);
        }
        if (trunkPart) {
            trunkCrash = new ModelObject(trunkPart);
        }
        if (trunkCrash && trunkCrashPart && DAT_0050145c) {
            SnapMesh* m = ((SnapObject*)trunkCrash)->meshes.At(0);
            ((MeshView*)m)->Link(trunkCrashPart->meshes.At(0));
        }
        if (hoodCrash && hoodCrashPart && DAT_0050145c) {
            SnapMesh* m = ((SnapObject*)hoodCrash)->meshes.At(0);
            ((MeshView*)m)->Link(hoodCrashPart->meshes.At(0));
        }
    }
    drivetrain.car = (DriveCar*)this;
    for (i = 0; i < 4; i++) {
        float radius = CF(float, 0x10fc);
        wheel[i].unk264 = radius;
        wheel[i].unk26c = 1.0f / radius;
        float k = 1.0f / (radius * 3.14159274f);
        wheel[i].unk2e8 = k;
        wheel[i].unk2ec = k * 10.0f;
        wheel[i].car = (Car*)this;
        wheel[i].side = i;
        *(int*)&wheel[i].unk278 = CF(int, 0x1100);
        float frontC = CF(float, 0x12ac);
        wheel[i].unk318 = 1.0f / frontC;
        *(int*)&wheel[i].unk320 = CF(int, 0x12b0);
        float rearC = CF(float, 0x12b4);
        wheel[i].unk31c = 1.0f / rearC;
        *(int*)&wheel[i].unk324 = CF(int, 0x12b8);
    }
    CF(int, 0x1ec4) = 0;
    if (CF(int, 0x12a4)) {
        wheel[0].touching = 1;
        wheel[1].touching = 1;
        CF(int, 0x1ec4) |= 2;
    }
    if (CF(int, 0x12a8)) {
        wheel[3].touching = 1;
        wheel[2].touching = 1;
        CF(int, 0x1ec4) |= 1;
    }
    wheel[0].inertia = CF(float, 0x112c);
    wheel[0].unk274 = 1.0f / CF(float, 0x112c);
    wheel[1].inertia = CF(float, 0x112c);
    wheel[1].unk274 = 1.0f / CF(float, 0x112c);
    wheel[3].inertia = CF(float, 0x11f0);
    wheel[3].unk274 = 1.0f / CF(float, 0x11f0);
    wheel[2].inertia = CF(float, 0x11f0);
    wheel[2].unk274 = 1.0f / CF(float, 0x11f0);
    CF(float, 0xcf8) = CF(float, 0x11b8 + setup[2] * 4);
    CF(float, 0xcfc) = CF(float, 0x11cc + setup[2] * 4);
    CF(float, 0xd2c) = CF(float, 0x11b8 + setup[2] * 4);
    CF(float, 0xd30) = CF(float, 0x11cc + setup[2] * 4);
    CF(float, 0xd94) = CF(float, 0x127c + setup[2] * 4);
    CF(float, 0xd98) = CF(float, 0x1290 + setup[2] * 4);
    CF(float, 0xd60) = CF(float, 0x127c + setup[2] * 4);
    CF(float, 0xd64) = CF(float, 0x1290 + setup[2] * 4);
    CF(float, 0xcf4) = CF(float, 0x114c + setup[2] * 4);
    CF(float, 0xd28) = CF(float, 0x114c + setup[2] * 4);
    CF(float, 0xd90) = CF(float, 0x1210 + setup[2] * 4);
    CF(float, 0xd5c) = CF(float, 0x1210 + setup[2] * 4);
    CF(float, 0xd00) = CF(float, 0x1138 + setup[2] * 4);
    CF(float, 0xd04) = CF(float, 0x1130);
    CF(float, 0xd08) = CF(float, 0x1134);
    CF(float, 0xd34) = CF(float, 0x1138 + setup[2] * 4);
    CF(float, 0xd38) = CF(float, 0x1130);
    CF(float, 0xd3c) = CF(float, 0x1134);
    CF(float, 0xd9c) = CF(float, 0x11fc + setup[2] * 4);
    CF(float, 0xda0) = CF(float, 0x11f4);
    CF(float, 0xda4) = CF(float, 0x11f8);
    CF(float, 0xd68) = CF(float, 0x11fc + setup[2] * 4);
    CF(float, 0xd6c) = CF(float, 0x11f4);
    CF(float, 0xd70) = CF(float, 0x11f8);
    CF(int, 0xd14) = CF(int, 0x11b0);
    CF(float, 0xd18) = CF(float, 0x11b4);
    CF(float, 0xd4c) = CF(float, 0x11b4);
    CF(int, 0xd48) = CF(int, 0x11b0);
    CF(float, 0xdb4) = CF(float, 0x1278);
    CF(float, 0xd80) = CF(float, 0x1278);
    char driverDir[22] = "data/drivers/driver1/";
    CF(int, 0xdb0) = CF(int, 0x1274);
    CF(int, 0xd7c) = CF(int, 0x1274);
    vec20c4.x = vec20c4.y = vec20c4.z = 0;
    vec20d0.x = vec20d0.y = vec20d0.z = 0;
    driverDir[19] = (char)(CF(BYTE, 0x1030) + '0');
    vec2030.x = vec2030.y = vec2030.z = 0;
    vec203c.x = vec203c.y = vec203c.z = 0;
    if (row->hasInterior || (flags & 1)) {
        FUN_00455070(driverDir);
        if (row->hasInterior) {
            FUN_00455ec0(dir);
            FUN_004560a0(dir);
        }
    }
    SetCockpit(0);
    AttachLights();

    // ---- phase G: free the owned objects of both model files ----
    for (i = 0; i < (int)modelFile.objects.size(); i++) {
        delete modelFile.objects[i];
    }
    for (i = 0; i < (int)collisionFile.objects.size(); i++) {
        delete collisionFile.objects[i];
    }

    // ---- phase H: attach effects and shadow to the body, kind-dependent lamp setup ----
    for (i = 0; i < 4; i++) {
        ((EffectView*)&wheel[i])->SetParent(body);
        ((EffectView*)&skid[i])->SetParent(body);
        skid[i].SetQuality(slot);
    }
    ShadowObject* so = new ShadowObject;
    ShadowView* sv = (ShadowView*)so;
    *(Vec3*)&sv->f128 = *(Vec3*)&CF(float, 0x109c);
    sv->f128 = 1.0f;
    sv->f12c = 1.0f;
    sv->f174 = 1.0f;
    sv->f130 = 0.85f;
    sv->bits = ~(1 << DAT_00508f60) & 0x7f;
    sv->f134 = 0;
    shadow = so;
    sv->f134 = 0;
    sv->f158 = 150.0f;
    sv->f16c = 0.08726646751f;
    sv->f170 = 1.04719758034f;
    sv->f160 = 0;
    sv->f164 = 0.06f;
    sv->f168 = 0;
    sv->f15c = 2.0f;
    ((NameString_46c2c0*)((BYTE*)sv + 0x114))->Assign("drive light");
    sv->pos = *(RawVec3*)&CF(float, 0x1090);
    sv->fc4 = 150.0f;
    sv->matrix.SetIdentity();
    {
        Vec3 axis;
        axis.x = 1.0f;
        axis.y = 0.0f;
        axis.z = 0.0f;
        sv->matrix.FromAxisAngle(axis, CF(float, 0x10a8) * -0.0174532924f);
    }
    if (model != 0 || (flags & 1)) {
        ((EffectView*)so)->SetParent(body);
    }
    ((EffectView*)&dust)->SetParent(body);
    ((EffectView*)&debris)->SetParent(body);
    ((EffectView*)&exhaust)->SetParent(body);
    ((EffectView*)&spray)->SetParent(body);
    ((EffectView*)&smokeB)->SetParent(body);
    if (indicatorRpm) {
        ((EffectView*)indicatorRpm)->SetParent(body);
    }
    if (indicatorSpeed) {
        ((EffectView*)indicatorSpeed)->SetParent(body);
    }
    if (trunkCrash) {
        ((EffectView*)trunkCrash)->SetParent(body);
    }
    if (hoodCrash) {
        ((EffectView*)hoodCrash)->SetParent(body);
    }
    if (wipers) {
        ((EffectView*)wipers)->SetParent(body);
    }
    ResetControls();
    if (!(flags & 2) && DAT_0056b0ac->vf76(0x12) && slot != 0) {
        ReflectionObject* ro = new ReflectionObject;
        reflection = (PolyObject*)ro;
        ro->Create(DAT_0056b0ac, row->shadowSize);
    }
    float skidWidth = CF(float, 0x10f8);
    for (i = 0; i < 4; i++) {
        skid[i].unk144 = skidWidth;
        skid[i].width = skidWidth * 0.5f;
    }
    int lampA = CF(int, 0x1088);
    int lampOpacity = CF(int, 0x108c);
    int lampB = CF(int, 0x1088);
    int v14 = FloatToInt(CF(float, 0x108c) * 256.0f);
    int a18, i54, c1c = 0;
    int t1, t2;
    switch (kind) {
    case 2:
        a18 = 5;
        if (DAT_0056b0ac->vf76(1) && DAT_0056b0ac->vf76(3)) {
            i54 = 1;
            t1 = 3;
            v14 = 0;
        } else {
            i54 = 2;
            t1 = 4;
            lampOpacity = CF(int, 0x1088);
        }
        t2 = DAT_0056b0ac->vf76(0xb) ? 0xb : 0xc;
        break;
    case 1:
        a18 = 10;
        if (DAT_0056b0ac->vf76(6) && DAT_0056b0ac->vf76(8)) {
            i54 = 6;
            t1 = 8;
            v14 = 0;
        } else {
            i54 = 7;
            t1 = 9;
            lampOpacity = CF(int, 0x1088);
        }
        c1c = 0x100;
        t2 = DAT_0056b0ac->vf76(0xd) ? 0xd : 0xe;
        break;
    case 0:
        t1 = 0;
        i54 = 0;
        t2 = 0;
        a18 = 0;
        break;
    }
    window[1] = t1;
    window[2] = t2;
    int w = FloatToInt(CF(float, 0x1088) * 256.0f);
    if (w > 255) {
        w = 255;
    }
    int l64, l5c, l60, l58, l68;
    if (row->hasInterior) {
        l64 = window[1];
        l5c = 5;
        l60 = 6;
        l58 = -1;
        l68 = 2;
    } else {
        l64 = window[2];
        l5c = 2;
        l60 = 1;
        l58 = -3;
        l68 = 0;
    }
    bool nightCar = slot == 2;
    if (!(flags & 1) && DAT_004fb0f8[model].kind == 2 && DAT_00501428 == 2) {
        nightCar = true;
    }
    if (CF(int, 0x1bcf8)) {
        *(int*)(CF(int, 0x1bcf8) + 0x60) = v14;
    }

    // ---- phase I: the materials of the model ----
    void* dirt = 0;
    CarList<MaterialSprite*>::Node* node;
    for (node = materials.m_head; node; node = node->Next()) {
        MatView* m = (MatView*)node->data;
        if (m->fb8 != -1) {
            m->f98 = 15;
        }
        if (m->fac) {
            if (strstr(m->Name(), "#E")) {
                MaterialSprite* tmp = (MaterialSprite*)m;
                ((PtrVectorOps*)&vec1bcd4)->Insert(vec1bcd4.end(), 1, &tmp);
            }
        }
        if (!strcmp(m->Name(), "#E1 rearlight_left")) {
            m->f38 = a18;
            int fl = m->flags;
            m->f4c = lampB;
            m->flags = fl | c1c;
            MaterialSprite* ms = node->data;
            m->b87 = w;
            CF(int, 0x1be2c) = (int)ms;
            CF(int, 0x1be30) = *(int*)ms;
            GlowObject* go = new GlowObject(body, ms, DAT_004e1c38);
            glow[0] = (ModelObject*)go;
            ((CarString*)((BYTE*)go + 0x114))->AssignInline("rearlights left");
            ***(int***)((BYTE*)((NamedObject*)go)->meshes.At(0) + 0x34) = (int)sprite1be90;
            ((EffectView*)go)->SetParent(body);
        } else if (!strcmp(m->Name(), "#E1 rearlight_right")) {
            m->f38 = a18;
            int fl = m->flags;
            m->f4c = lampB;
            m->flags = fl | c1c;
            MaterialSprite* ms = node->data;
            m->b87 = w;
            CF(int, 0x1be48) = (int)ms;
            CF(int, 0x1be4c) = *(int*)ms;
            GlowObject* go = new GlowObject(body, ms, DAT_004e1c38);
            glow[1] = (ModelObject*)go;
            ***(int***)((BYTE*)((NamedObject*)go)->meshes.At(0) + 0x34) = (int)sprite1be90;
            ((EffectView*)go)->SetParent(body);
        } else if (!strcmp(m->Name(), "#E1 headlight_left")) {
            m->f38 = a18;
            int fl = m->flags;
            m->f4c = lampB;
            m->flags = fl | c1c;
            MaterialSprite* ms = node->data;
            m->b87 = w;
            CF(int, 0x1bd4c) = (int)ms;
            CF(int, 0x1bd50) = *(int*)ms;
            GlowObject* go = new GlowObject(body, ms, DAT_004e1c38);
            glow[2] = (ModelObject*)go;
            ***(int***)((BYTE*)((NamedObject*)go)->meshes.At(0) + 0x34) = (int)sprite1be90;
            ((EffectView*)go)->SetParent(body);
        } else if (!strcmp(m->Name(), "#E1 headlight_right")) {
            m->f38 = a18;
            int fl = m->flags;
            m->f4c = lampB;
            m->flags = fl | c1c;
            MaterialSprite* ms = node->data;
            m->b87 = w;
            CF(int, 0x1bda0) = (int)ms;
            CF(int, 0x1bda4) = *(int*)ms;
            GlowObject* go = new GlowObject(body, ms, DAT_004e1c38);
            glow[5] = (ModelObject*)go;
            ***(int***)((BYTE*)((NamedObject*)go)->meshes.At(0) + 0x34) = (int)sprite1be90;
            ((EffectView*)go)->SetParent(body);
        } else if (!strcmp(m->Name(), "#E1 window front")) {
            m->f4c = lampOpacity;
            m->b87 = w;
            m->f18 = l5c;
            m->f38 = l64;
            MaterialSprite* ms = node->data;
            m->f1c = l60;
            m->flags = (m->flags & l58) | l68 | c1c;
            lamp0.target = (int*)ms;
            lamp0.off = *(int*)ms;
            if (v14 && m->texture) {
                ((TexView*)m->texture)->f60 = v14;
            }
        } else if (!strcmp(m->Name(), "#E1 window back")) {
            m->f38 = l64;
            m->b87 = w;
            m->f1c = l60;
            m->f4c = lampOpacity;
            m->f18 = l5c;
            MaterialSprite* ms = node->data;
            lamp1.target = (int*)ms;
            m->flags = (m->flags & l58) | l68 | c1c;
            lamp1.off = *(int*)ms;
            if (v14 && m->texture) {
                ((TexView*)m->texture)->f60 = v14;
            }
        } else if (!strcmp(m->Name(), "#E1 window leftside")) {
            m->b87 = w;
            m->f38 = l64;
            m->f4c = lampOpacity;
            m->f1c = l60;
            m->f18 = l5c;
            m->flags = (m->flags & l58) | l68 | c1c;
            MaterialSprite* ms = node->data;
            lamp2.target = (int*)ms;
            lamp2.off = *(int*)ms;
            if (v14 && m->texture) {
                ((TexView*)m->texture)->f60 = v14;
            }
        } else if (!strcmp(m->Name(), "#E1 window rightside")) {
            m->f4c = lampOpacity;
            m->b87 = w;
            m->f18 = l5c;
            m->f38 = l64;
            MaterialSprite* ms = node->data;
            m->f1c = l60;
            m->flags = (m->flags & l58) | l68 | c1c;
            lamp3.target = (int*)ms;
            lamp3.off = *(int*)ms;
            if (v14 && m->texture) {
                ((TexView*)m->texture)->f60 = v14;
            }
        } else if (strstr(m->Name(), "#E")) {
            m->f38 = i54;
            m->f4c = lampA;
            m->flags = m->flags | c1c;
            m->b87 = w;
        } else if (!strcmp(m->Name(), "windows")) {
            if (m->texture) {
                ((TexView*)m->texture)->FUN_004a82b0();
            }
            m->unk4 = window[0];
            m->flags |= 2;
            m->fc0 = 1;
            m->f38 = 0;
            marker = m;
        }
        if (m->fbc == 1 && nightCar) {
            int lampTex;
            switch (kind) {
            case 0:
                if (DAT_0056b0ac->vf76(0x13)) {
                    lampTex = 0x13;
                } else {
                    goto next;
                }
                break;
            case 1:
                if (DAT_0056b0ac->vf76(0x15)) {
                    lampTex = 0x15;
                } else if (DAT_0056b0ac->vf76(0x17)) {
                    lampTex = 0x17;
                } else {
                    goto next;
                }
                break;
            case 2:
                if (DAT_0056b0ac->vf76(0x14)) {
                    lampTex = 0x14;
                } else if (DAT_0056b0ac->vf76(0x16)) {
                    lampTex = 0x16;
                } else {
                    goto next;
                }
                break;
            default:
                goto next;
            }
            m->f38 = lampTex;
            {
                PtrVec* v = (PtrVec*)&vec1bcc4;
                MaterialSprite* x = node->data;
                MaterialSprite** P = v->last;
                if ((unsigned int)(v->end - v->last) < 1) {
                    unsigned int N = v->Size() + (1 < v->Size() ? v->Size() : 1);
                    int An = (int)N;
                    if (An < 0) {
                        An = 0;
                    }
                    MaterialSprite** S = (MaterialSprite**)operator new(An * sizeof(MaterialSprite*));
                    MaterialSprite** Q = S;
                    MaterialSprite** F;
                    for (F = v->first; F != P; ++Q, ++F) {
                        ConstructPtr(Q, *F);
                    }
                    ConstructPtr(Q, x);
                    Q = Q + 1;
                    for (F = P; F != v->last; ++Q, ++F) {
                        ConstructPtr(Q, *F);
                    }
                    operator delete(v->first);
                    v->end = S + N;
                    v->last = S + v->Size() + 1;
                    v->first = S;
                } else if ((unsigned int)(v->last - P) < 1) {
                    v->Ucopy(P, v->last, P + 1);
                    v->Ufill(v->last, 1 - (v->last - P), x);
                    MaterialSprite** F;
                    for (F = P; F != v->last; ++F) {
                        *F = x;
                    }
                    v->last += 1;
                } else {
                    v->Ucopy(v->last - 1, v->last, v->last);
                    MaterialSprite** B = v->last;
                    MaterialSprite** E = v->last - 1;
                    while (E != P) {
                        *--B = *--E;
                    }
                    MaterialSprite** F;
                    for (F = P; F != P + 1; ++F) {
                        *F = x;
                    }
                    v->last += 1;
                }
            }
            if (!dirt) {
                CarString path(dir);
                path.AppendLate("dirt.tga");
                dirt = textureCache->Load(path.CStrOut(), 0x40);
            }
            m->dirt = dirt;
        }
    next:;
    }

    // ---- phase J: the mesh list, texture release, default setups ----
    meshes.Add(((ModelFilePart*)body)->FirstMesh());
    if (trunkCrash) {
        meshes.Add(((ModelFilePart*)trunkCrash)->FirstMesh());
    }
    if (hoodCrash) {
        meshes.Add(((ModelFilePart*)hoodCrash)->FirstMesh());
    }
    if (kind != 1 && (row->noLod || !DAT_0050145c)) {
        if (body) {
            modelCache->Release(body);
        }
        if (trunkCrash) {
            modelCache->Release(trunkCrash);
        }
        if (hoodCrash) {
            modelCache->Release(hoodCrash);
        }
        for (i = 0; i < 4; i++) {
            modelCache->Release(*(void**)&CF(BYTE, 0x130 + i * 0x330));
            modelCache->Release(shadowTyre[i]);
        }
        if (obj2018) {
            modelCache->Release(obj2018);
        }
        if (obj2028) {
            modelCache->Release(obj2028);
        }
        if (obj201c) {
            modelCache->Release(obj201c);
        }
        if (obj202c) {
            modelCache->Release(obj202c);
        }
        if (interiorLo) {
            modelCache->Release(interiorLo);
        }
        if (interiorHi) {
            modelCache->Release(interiorHi);
        }
        if (steeringLo) {
            modelCache->Release(steeringLo);
        }
        if (steeringHi) {
            modelCache->Release(steeringHi);
        }
        if (indicatorRpm) {
            modelCache->Release(indicatorRpm);
        }
        if (indicatorSpeed) {
            modelCache->Release(indicatorSpeed);
        }
        if (obj1ff0) {
            modelCache->Release(obj1ff0);
        }
    }
    SetLightIntensity(1.0f);
    FUN_00465e90(1);
    FUN_00466500(2);
    FUN_00466520(2);
    FUN_00466590(2);
    FUN_00466610(2);
    Reset();
    FUN_0048e660();
    FUN_0045a850();
    for (node = ((CarList<MaterialSprite*>::Node*)&materials)->Next(); node; node = node->Next()) {
        MaterialSprite* orig = node->Data();
        MaterialSprite* copy = new MaterialSprite;
        copy->Assign(orig);
        materials3.AddTail(copy);
    }
    ResetMessage();
}

#pragma intrinsic(strcmp)

// FUNCTION: RALLYTROPHY 0x461a10
// SYMBOL: ?CarCopy@@YAPADPADPBDI@Z
// The memcpy-style helper (rep movsd / rep movsb, returns dst) that the original's inline std::string::append in
// the constructor calls at 0x4616ce (std::char_traits<char>::copy). This build expands the append differently,
// so the function is written out here with the same body to keep the bytes at 0x461a10 covered.
char* CarCopy(char* dst, const char* src, size_t n)
{
    return (char*)memcpy(dst, src, n);
}

// NONMATCHING: RALLYTROPHY 0x461a40
// SYMBOL: ??1CarObject@@QAE@XZ
// 2274 vs 2276 bytes: everything matches (member destruction order, all 79 EH states, the inline list/array
// destructors) except the first light loop, where the original materialises &lightFrontRight[0] as
// `lea eax,[esi+0x1bd94]; mov ebp,eax` and the build has `lea ebp,[esi+0x1bd94]` (30 source forms tried).
CarObject::~CarObject()
{
    delete shadow;
    int i;
    for (i = 0; i < 4; i++) {
        delete shadowTyre[i];
    }
    delete glow[0];
    delete glow[1];
    for (i = 0; i < 3; i++) {
        delete glow[2 + i];
        delete glow[5 + i];
        delete lightFrontLeft[i].light;
        delete lightFrontRight[i].light;
    }
    delete sprite1be90;
    for (i = 0; i < 2; i++) {
        delete lightRear[i].light;
        delete lightBrake[i].light;
        delete lightReverse[i].light;
    }
    delete set2090;
    delete set2094;
    delete set2080;
    delete set2084;
    delete set2088;
    delete set208c;
    delete obj1f2c;
    delete obj1ff0;
    delete obj2018;
    delete obj201c;
    delete obj2028;
    delete obj202c;
    delete interiorLo;
    delete interiorHi;
    delete steeringLo;
    delete steeringHi;
    delete indicatorSpeed;
    delete indicatorRpm;
    delete hoodCrash;
    delete trunkCrash;
    delete wipers;
    delete set1ff8;
    delete obj2004;
    delete obj2008;
    delete body;
    delete reflection;
    delete obj1bce4;
    CarList<MaterialSprite*>::Node* node;
    for (node = materials.m_head; node; node = materials.m_head) {
        if (node->data) {
            node->data->ScalarDelete(1);
        }
        materials.RemoveNode(node);
    }
    for (node = materials2.m_head; node; node = materials2.m_head) {
        if (node->data) {
            node->data->ScalarDelete(1);
        }
        materials2.RemoveNode(node);
    }
    for (node = materials3.m_head; node; node = materials3.m_head) {
        if (node->data) {
            node->data->ScalarDelete(1);
        }
        materials3.RemoveNode(node);
    }
    if (ownsCaches) {
        delete textureCache;
        delete modelCache;
    }
    if (!(flags & 4)) {
        ReleaseSounds();
    }
}

// FUNCTION: RALLYTROPHY 0x462330
// SYMBOL: ?ScalarDelete@MaterialSprite@@QAEPAXI@Z
// The three material lists are emptied through the scalar deleting destructor of the 0xd4-byte
// sprite: ~Sprite() (0x4a86d0), then operator delete(this) when bit 0 of `flags` is set.
void* MaterialSprite::ScalarDelete(unsigned int flags)
{
    ((Sprite*)this)->~Sprite();
    if (flags & 1) {
        operator delete(this);
    }
    return this;
}


// ===== LoadCarIni (0x462350) =====

// Keys of car.ini (0x4fadd8, 100 entries).
ConfigKey CarIniKeys[100] = {
    { "WheelDisplacement", 2 },
    { "WheelAligningTorqueLimits", 3 },
    { "OverlayMeterLocation", 3 },
    { "OverlaySpeedLocation", 3 },
    { "OverlayGearLocation", 3 },
    { "OverlaySpeedSize", 4 },
    { "OverlayRpmLocation", 3 },
    { "OverlayRpmSize", 4 },
    { "OverlayRpmMax", 2 },
    { "OverlaySpeedMax", 2 },
    { "InteriorRpmMax", 2 },
    { "InteriorSpeedMax", 2 },
    { "DriverType", 1 },
    { "InteriorLoc", 4 },
    { "BumperLoc", 4 },
    { "HoodLoc", 4 },
    { "DriverLoc", 4 },
    { "CoDriverLoc", 4 },
    { "FFFrictionNominalLoad", 2 },
    { "FFFrictionOffset", 2 },
    { "FFCenteringNominalLoad", 2 },
    { "FFCenteringOffset", 2 },
    { "TrunkAngle", 2 },
    { "HoodAngle", 2 },
    { "TrunkMaxAngle", 2 },
    { "HoodMaxAngle", 2 },
    { "WiperAnimSpeed", 2 },
    { "NumWipers", 1 },
    { "WiperRadius", 3 },
    { "Wiper1Angle", 3 },
    { "Wiper2Angle", 3 },
    { "Wiper1Pivot", 4 },
    { "Wiper2Pivot", 4 },
    { "Reflection", 2 },
    { "OpacityBias", 2 },
    { "Mass", 2 },
    { "MomentOfInertia", 4 },
    { "CenterOfMass", 4 },
    { "AeroDrag", 4 },
    { "AeroDragLoc", 4 },
    { "DownForceConst", 2 },
    { "CenterOfDownForce", 4 },
    { "Damping", 3 },
    { "DamageSpeedRange", 3 },
    { "DriveLightLocation", 4 },
    { "DriveLightColor", 4 },
    { "DriveLightAngle", 2 },
    { "FrontFlareRadius", 2 },
    { "FrontFlareSharpness", 2 },
    { "FrontFlareAlphaMin", 2 },
    { "RearFlareRadius", 2 },
    { "RearFlareSharpness", 2 },
    { "RearFlareAlphaMin", 2 },
    { "BrakeFlareRadius", 2 },
    { "BrakeFlareSharpness", 2 },
    { "BrakeFlareAlphaMin", 2 },
    { "ReverseFlareRadius", 2 },
    { "ReverseFlareSharpness", 2 },
    { "ReverseFlareAlphaMin", 2 },
    { "FrontTraction", 1 },
    { "RearTraction", 1 },
    { "BrakeBalance", 4 },
    { "BrakeTorque", 2 },
    { "HandBrakeTorque", 2 },
    { "SteeringSpeeds", 3 },
    { "SteeringLimit", 2 },
    { "SteeringReduction", 2 },
    { "TireTurnAngleIn", 2 },
    { "TireTurnAngleOut", 2 },
    { "TireCenteringSpeed", 2 },
    { "TireWidth", 2 },
    { "TireRadius", 2 },
    { "TireMass", 2 },
    { "FrontTireMomentOfInertia", 2 },
    { "RearTireMomentOfInertia", 2 },
    { "SolidRearAxle", 1 },
    { "FrontSuspensionMinLength", 2 },
    { "FrontSuspensionMaxLength", 2 },
    { "FrontSuspensionRestLength", 4 },
    { "FrontSuspensionConst", 4 },
    { "FrontRollBarStiffness", 4 },
    { "FrontBumpDamp", 4 },
    { "FrontReboundDamp", 4 },
    { "RearSuspensionMinLength", 2 },
    { "RearSuspensionMaxLength", 2 },
    { "RearSuspensionRestLength", 4 },
    { "RearSuspensionConst", 4 },
    { "RearRollBarStiffness", 4 },
    { "RearBumpDamp", 4 },
    { "RearReboundDamp", 4 },
    { "FrontBumperLength", 2 },
    { "FrontBumperConst", 2 },
    { "RearBumperLength", 2 },
    { "RearBumperConst", 2 },
    { "FrontCamberAngle", 2 },
    { "RearCamberAngle", 2 },
    { "FrontCamberTravelUp", 2 },
    { "FrontCamberTravelDown", 2 },
    { "RearCamberTravelUp", 2 },
    { "RearCamberTravelDown", 2 },
};

// The Car fields LoadCarIni writes (0xfe8..0x1f14); the rest of Car is not touched.
struct CarIniView {
    BYTE unk0[0xfe8 - 0x0];
    float wheelDisplacement; // 0xfe8
    Vec2 wheelAligningLimits; // 0xfec
    Vec3 interiorLoc; // 0xff4
    Vec3 bumperLoc; // 0x1000
    Vec3 hoodLoc; // 0x100c
    Vec3 driverLoc; // 0x1018
    Vec3 coDriverLoc; // 0x1024
    int driverType; // 0x1030
    float mass; // 0x1034
    Vec3 momentOfInertia; // 0x1038
    Vec3 centerOfMass; // 0x1044
    Vec3 aeroDrag; // 0x1050
    Vec3 aeroDragLoc; // 0x105c
    Vec2 damping; // 0x1068
    Vec3 centerOfDownForce; // 0x1070
    float downForceConst; // 0x107c
    Vec2 damageSpeedRange; // 0x1080
    float reflection; // 0x1088
    float opacityBias; // 0x108c
    Vec3 driveLightLoc; // 0x1090
    Vec3 driveLightColor; // 0x109c
    float driveLightAngle; // 0x10a8
    float flare[12]; // 0x10ac
    Vec2 steeringSpeeds; // 0x10dc
    float steeringLimit; // 0x10e4
    float steeringReduction; // 0x10e8
    float tireTurnAngleIn; // 0x10ec
    float tireTurnAngleOut; // 0x10f0
    float tireCenteringSpeed; // 0x10f4
    float tireWidth; // 0x10f8
    float tireRadius; // 0x10fc
    float tireMass; // 0x1100
    float invTireMass; // 0x1104
    Vec3 brakeBalance; // 0x1108
    float brakeTorque; // 0x1114
    float handBrakeTorque; // 0x1118
    int unk111c; // 0x111c
    float frontCamberAngle; // 0x1120
    float frontCamberTravelUp; // 0x1124
    float frontCamberTravelDown; // 0x1128
    float frontTireMomentOfInertia; // 0x112c
    float frontSuspMin; // 0x1130
    float frontSuspMax; // 0x1134
    float frontRest[5]; // 0x1138
    float frontConst[5]; // 0x114c
    float frontStatic[5]; // 0x1160
    float frontInvStatic[5]; // 0x1174
    float frontInvTravel[5]; // 0x1188
    float frontRollBar[5]; // 0x119c
    float frontBumperLength; // 0x11b0
    float frontBumperConst; // 0x11b4
    float frontBump[5]; // 0x11b8
    float frontRebound[5]; // 0x11cc
    int solidRearAxle; // 0x11e0
    float rearCamberAngle; // 0x11e4
    float rearCamberTravelUp; // 0x11e8
    float rearCamberTravelDown; // 0x11ec
    float rearTireMomentOfInertia; // 0x11f0
    float rearSuspMin; // 0x11f4
    float rearSuspMax; // 0x11f8
    float rearRest[5]; // 0x11fc
    float rearConst[5]; // 0x1210
    float rearStatic[5]; // 0x1224
    float rearInvStatic[5]; // 0x1238
    float rearInvTravel[5]; // 0x124c
    float rearRollBar[5]; // 0x1260
    float rearBumperLength; // 0x1274
    float rearBumperConst; // 0x1278
    float rearBump[5]; // 0x127c
    float rearRebound[5]; // 0x1290
    int frontTraction; // 0x12a4
    int rearTraction; // 0x12a8
    float ffFrictionNominalLoad; // 0x12ac
    float ffFrictionOffset; // 0x12b0
    float ffCenteringNominalLoad; // 0x12b4
    float ffCenteringOffset; // 0x12b8
    float hoodAxes[6]; // 0x12bc
    float trunkAxes[6]; // 0x12d4
    BYTE unk12ec[0x12f4 - 0x12ec];
    float hoodMaxAngle; // 0x12f4
    float trunkMaxAngle; // 0x12f8
    int numWipers; // 0x12fc
    float wiperAnimSpeed; // 0x1300
    Vec2 wiperRadius; // 0x1304
    Vec2 wiper1Angle; // 0x130c
    Vec2 wiper2Angle; // 0x1314
    Vec3 wiper1Pivot; // 0x131c
    Vec3 wiper2Pivot; // 0x1328
    BYTE unk1334[0x1ed0 - 0x1334];
    Vec2 overlayMeterLocation; // 0x1ed0
    Vec2 overlayRpmLocation; // 0x1ed8
    Vec2 overlaySpeedLocation; // 0x1ee0
    Vec2 overlayGearLocation; // 0x1ee8
    Vec3 overlayRpmSize; // 0x1ef0
    Vec3 overlaySpeedSize; // 0x1efc
    float overlayRpmMax; // 0x1f08
    float overlaySpeedMax; // 0x1f0c
    float interiorRpmMax; // 0x1f10
    float interiorSpeedMax; // 0x1f14
};

// std::string as the car.ini error path builds it: out-of-line _Tidy (0x402700) and _Grow (0x402980), the
// copy and the terminator expanded inline (PathString has _Eos out of line).
class IniString {
public:
    IniString(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al)
    {
        Tidy(false);
        Assign(s);
    }
    ~IniString() { Free(true); }
    unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    void Free(bool built)
    {
        if (!built || ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, reserve + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, reserve = 0;
    }
    void Tidy(bool built);                        // 0x402700
    bool Grow(unsigned int n, bool trim);         // 0x402980
    void Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (Grow(n, true)) {
            memcpy(ptr, s, n);
            ptr[len = n] = 0;
        }
    }
    const char* c_str() const
    {
        const char* r = ptr;
        if (r == 0)
            r = "";
        return r;
    }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};
inline const std::string& AsString(const IniString& s) { return *(const std::string*)&s; }
TmpString MissingText(const IniString& prefix, const char* name); // 0x409130: operator+(string, const char*)

#define LOAD3(dst, src) \
    do { \
        const ConfigVector3& c_ = (src); \
        (dst).x = c_.values[0]; \
        (dst).y = c_.values[1]; \
        (dst).z = c_.values[2]; \
    } while (0)
#define LOAD2(dst, src) \
    do { \
        const ConfigVector2& c_ = (src); \
        (dst).x = c_.values[0]; \
        (dst).y = c_.values[1]; \
    } while (0)
inline void Copy2(Vec2& dst, const ConfigVector2& c)
{
    dst.x = c.values[0];
    dst.y = c.values[1];
}

inline void Copy3(Vec3& dst, const ConfigVector3& c)
{
    dst.x = c.values[0];
    dst.y = c.values[1];
    dst.z = c.values[2];
}

#define COPY2(dst, src) Copy2(dst, src)
#define COPY3(dst, src) Copy3(dst, src)

inline void Scale3(Vec3* p, const float& k)
{
    p->x *= k;
    p->y *= k;
    p->z *= k;
}

const float kDegToRad = 0.017453292f;

// Five-point curves of a Vec3 (suspension tables): (lo, mid, y, mid, hi), or scaled around y.
#define MID5(d, c) \
    do { \
        (d)[0] = (c).x; \
        (d)[1] = ((c).x + (c).y) * 0.5f; \
        (d)[2] = (c).y; \
        (d)[3] = ((c).y + (c).z) * 0.5f; \
        (d)[4] = (c).z; \
    } while (0)

#define PROD5(d, c) \
    do { \
        (d)[0] = (c).y * (c).x; \
        (d)[1] = ((c).x + 1.0f) * (c).y * 0.5f; \
        (d)[2] = (c).y; \
        (d)[3] = ((c).z + 1.0f) * (c).y * 0.5f; \
        (d)[4] = (c).z * (c).y; \
    } while (0)

// original: FUN_00462350
// NONMATCHING: RALLYTROPHY 0x462350
// 4504 built vs 4484 target bytes; 2339 bytes differ outside relocations. The 100 Config reads, five-point
// suspension tables, static-load loop, wiper tail and Config::Close remain structurally close. Sequential Vec3 axis
// locals restore the target's 0x88 frame and EH states 8/9, with state 9 installed after the hood cosine. Open:
// (1) the target pushes 0x4fb77c between the scaled-angle fst and fcos; this candidate pushes 0 after fcos; (2) the
// failure-path string code differs: the target calls _Tidy at 0x402700 and loads the concatenated string data pointer
// directly into esi, while this candidate routes it through eax; (3) the target keeps mass in st(0) in MomentOfInertia.
void Car::LoadCarIni(const char* path, unsigned int checksum)
{
    CarIniView* v = (CarIniView*)this;
    Config config;
    config.Define(CarIniKeys, 100);
    if (!config.Load(path, true, true, checksum)) {
        throw std::runtime_error(AsString(
            IniString(MissingText(IniString("Missing variables in file: "), path).c_str())));
    }
    Vec3 t3;
    Vec2 t2;
    COPY2(v->damageSpeedRange, config.GetVector2("DamageSpeedRange"));
    COPY2(v->overlayMeterLocation, config.GetVector2("OverlayMeterLocation"));
    COPY2(v->overlaySpeedLocation, config.GetVector2("OverlaySpeedLocation"));
    COPY2(v->overlayRpmLocation, config.GetVector2("OverlayRpmLocation"));
    COPY2(v->overlayGearLocation, config.GetVector2("OverlayGearLocation"));
    LOAD3(t3, config.GetVector3("OverlaySpeedSize"));
    v->overlaySpeedSize.x = t3.x;
    v->overlaySpeedSize.y = t3.y;
    v->overlaySpeedSize.z = t3.z;
    LOAD3(t3, config.GetVector3("OverlayRpmSize"));
    v->overlayRpmSize.x = t3.x;
    v->overlayRpmSize.y = t3.y;
    v->overlayRpmSize.z = t3.z;
    v->overlayRpmMax = config.GetFloat("OverlayRpmMax");
    v->overlaySpeedMax = config.GetFloat("OverlaySpeedMax");
    v->interiorRpmMax = config.GetFloat("InteriorRpmMax");
    v->interiorSpeedMax = config.GetFloat("InteriorSpeedMax");
    v->driverType = config.GetInt("DriverType");
    COPY3(v->interiorLoc, config.GetVector3("InteriorLoc"));
    COPY3(v->bumperLoc, config.GetVector3("BumperLoc"));
    COPY3(v->hoodLoc, config.GetVector3("HoodLoc"));
    COPY3(v->driverLoc, config.GetVector3("DriverLoc"));
    COPY3(v->coDriverLoc, config.GetVector3("CoDriverLoc"));
    v->wheelDisplacement = config.GetFloat("WheelDisplacement");
    COPY2(v->wheelAligningLimits, config.GetVector2("WheelAligningTorqueLimits"));
    v->reflection = config.GetFloat("Reflection");
    v->opacityBias = config.GetFloat("OpacityBias");
    v->ffFrictionNominalLoad = config.GetFloat("FFFrictionNominalLoad");
    v->ffFrictionOffset = config.GetFloat("FFFrictionOffset");
    v->ffCenteringNominalLoad = config.GetFloat("FFCenteringNominalLoad");
    v->ffCenteringOffset = config.GetFloat("FFCenteringOffset");
    float mass = config.GetFloat("Mass");
    v->mass = mass;
    COPY3(v->momentOfInertia, config.GetVector3("MomentOfInertia"));
    Scale3(&v->momentOfInertia, mass);
    COPY3(v->centerOfMass, config.GetVector3("CenterOfMass"));
    COPY3(v->aeroDrag, config.GetVector3("AeroDrag"));
    COPY3(v->aeroDragLoc, config.GetVector3("AeroDragLoc"));
    LOAD2(t2, config.GetVector2("Damping"));
    v->damping.x = t2.x;
    v->damping.y = t2.y;
    COPY3(v->centerOfDownForce, config.GetVector3("CenterOfDownForce"));
    v->downForceConst = config.GetFloat("DownForceConst");
    COPY3(v->driveLightLoc, config.GetVector3("DriveLightLocation"));
    COPY3(v->driveLightColor, config.GetVector3("DriveLightColor"));
    v->driveLightAngle = config.GetFloat("DriveLightAngle");
    v->flare[0] = config.GetFloat("FrontFlareRadius");
    v->flare[1] = config.GetFloat("FrontFlareSharpness");
    v->flare[2] = config.GetFloat("FrontFlareAlphaMin");
    v->flare[3] = config.GetFloat("RearFlareRadius");
    v->flare[4] = config.GetFloat("RearFlareSharpness");
    v->flare[5] = config.GetFloat("RearFlareAlphaMin");
    v->flare[6] = config.GetFloat("BrakeFlareRadius");
    v->flare[7] = config.GetFloat("BrakeFlareSharpness");
    v->flare[8] = config.GetFloat("BrakeFlareAlphaMin");
    v->flare[9] = config.GetFloat("ReverseFlareRadius");
    v->flare[10] = config.GetFloat("ReverseFlareSharpness");
    v->flare[11] = config.GetFloat("ReverseFlareAlphaMin");
    Vec2 steer;
    LOAD2(steer, config.GetVector2("SteeringSpeeds"));
    v->steeringSpeeds.x = steer.x;
    v->steeringSpeeds.y = steer.y;
    v->steeringLimit = config.GetFloat("SteeringLimit");
    v->steeringReduction = config.GetFloat("SteeringReduction");
    COPY3(v->brakeBalance, config.GetVector3("BrakeBalance"));
    v->brakeTorque = config.GetFloat("BrakeTorque");
    v->handBrakeTorque = config.GetFloat("HandBrakeTorque");
    v->tireWidth = config.GetFloat("TireWidth");
    v->tireRadius = config.GetFloat("TireRadius");
    v->tireMass = config.GetFloat("TireMass");
    v->invTireMass = 1.0f / v->tireMass;
    v->tireTurnAngleIn = config.GetFloat("TireTurnAngleIn") * kDegToRad;
    v->tireTurnAngleOut = config.GetFloat("TireTurnAngleOut") * kDegToRad;
    v->tireCenteringSpeed = config.GetFloat("TireCenteringSpeed");
    v->unk111c = 0;
    v->frontTireMomentOfInertia = config.GetFloat("FrontTireMomentOfInertia");
    v->frontSuspMin = config.GetFloat("FrontSuspensionMinLength");
    v->frontSuspMax = config.GetFloat("FrontSuspensionMaxLength");
    v->frontBumperLength = config.GetFloat("FrontBumperLength");
    v->frontBumperConst = config.GetFloat("FrontBumperConst");
    LOAD3(t3, config.GetVector3("FrontRollBarStiffness"));
    PROD5(v->frontRollBar, t3);
    LOAD3(t3, config.GetVector3("FrontSuspensionRestLength"));
    MID5(v->frontRest, t3);
    LOAD3(t3, config.GetVector3("FrontSuspensionConst"));
    PROD5(v->frontConst, t3);
    LOAD3(t3, config.GetVector3("FrontBumpDamp"));
    PROD5(v->frontBump, t3);
    LOAD3(t3, config.GetVector3("FrontReboundDamp"));
    PROD5(v->frontRebound, t3);
    v->solidRearAxle = config.GetInt("SolidRearAxle");
    v->rearTireMomentOfInertia = config.GetFloat("RearTireMomentOfInertia");
    v->rearSuspMin = config.GetFloat("RearSuspensionMinLength");
    v->rearSuspMax = config.GetFloat("RearSuspensionMaxLength");
    v->rearBumperLength = config.GetFloat("RearBumperLength");
    v->rearBumperConst = config.GetFloat("RearBumperConst");
    LOAD3(t3, config.GetVector3("RearRollBarStiffness"));
    PROD5(v->rearRollBar, t3);
    LOAD3(t3, config.GetVector3("RearSuspensionRestLength"));
    MID5(v->rearRest, t3);
    LOAD3(t3, config.GetVector3("RearSuspensionConst"));
    PROD5(v->rearConst, t3);
    LOAD3(t3, config.GetVector3("RearBumpDamp"));
    PROD5(v->rearBump, t3);
    LOAD3(t3, config.GetVector3("RearReboundDamp"));
    PROD5(v->rearRebound, t3);
    v->frontTraction = config.GetInt("FrontTraction");
    v->rearTraction = config.GetInt("RearTraction");
    v->frontCamberAngle = config.GetFloat("FrontCamberAngle") * kDegToRad;
    v->rearCamberAngle = config.GetFloat("RearCamberAngle") * kDegToRad;
    v->frontCamberTravelUp = config.GetFloat("FrontCamberTravelUp") * kDegToRad;
    v->frontCamberTravelDown = config.GetFloat("FrontCamberTravelDown") * kDegToRad;
    v->rearCamberTravelUp = config.GetFloat("RearCamberTravelUp") * kDegToRad;
    v->rearCamberTravelDown = config.GetFloat("RearCamberTravelDown") * kDegToRad;
    float load = mass * 2.4525f;
    for (int i = 0; i < 5; i++) {
        v->frontStatic[i] = v->frontRest[i] - load / v->frontConst[i];
        v->rearStatic[i] = v->rearRest[i] - load / v->rearConst[i];
        v->frontInvStatic[i] = 1.0f / v->frontStatic[i];
        v->rearInvStatic[i] = 1.0f / v->rearStatic[i];
        v->frontInvTravel[i] = 1.0f / (v->frontSuspMax - v->frontStatic[i]);
        v->rearInvTravel[i] = 1.0f / (v->rearSuspMax - v->rearStatic[i]);
    }
    float hoodAngle = config.GetFloat("HoodAngle");
    float trunkAngle = config.GetFloat("TrunkAngle");
    Vec3 axisA;
    v->hoodAxes[0] = 0;
    v->hoodAxes[3] = 0;
    v->trunkAxes[0] = 0;
    float hood = hoodAngle * kDegToRad;
    axisA.y = (float)cos(hood);
    Vec3 axisB;
    axisA.x = 0;
    axisB.x = 0;
    v->trunkAxes[3] = 0;
    axisA.z = (float)sin(hood);
    axisB.y = -axisA.z;
    axisB.z = axisA.y;
    v->hoodAxes[1] = axisA.y;
    v->hoodAxes[2] = axisA.z;
    v->hoodAxes[4] = axisB.y;
    v->hoodAxes[5] = axisB.z;
    float trunk = trunkAngle * kDegToRad;
    axisA.y = (float)cos(trunk);
    axisA.z = (float)sin(trunk);
    axisB.y = -axisA.z;
    axisB.z = axisA.y;
    v->trunkAxes[1] = axisA.y;
    v->trunkAxes[2] = axisA.z;
    v->trunkAxes[4] = axisB.y;
    v->trunkAxes[5] = axisB.z;
    v->hoodMaxAngle = config.GetFloat("HoodMaxAngle") * kDegToRad;
    v->trunkMaxAngle = config.GetFloat("TrunkMaxAngle") * kDegToRad;
    v->wiperAnimSpeed = config.GetFloat("WiperAnimSpeed");
    v->numWipers = config.GetInt("NumWipers");
    COPY2(v->wiperRadius, config.GetVector2("WiperRadius"));
    COPY2(v->wiper1Angle, config.GetVector2("Wiper1Angle"));
    COPY2(v->wiper2Angle, config.GetVector2("Wiper2Angle"));
    COPY3(v->wiper1Pivot, config.GetVector3("Wiper1Pivot"));
    COPY3(v->wiper2Pivot, config.GetVector3("Wiper2Pivot"));
    v->wiper1Angle.x *= 0.017453292f;
    v->wiper1Angle.y *= 0.017453292f;
    v->wiper2Angle.x *= 0.017453292f;
    v->wiper2Angle.y *= 0.017453292f;
    config.Close();
}

// ===== LoadTiresIni (0x4634e0) =====

// Keys of tire.ini (0x4fac70, 5 tyre sets x 9 keys).
ConfigKey TireKeys[45] = {
    { "Tire0RollingResistance", 2 },
    { "Tire0InducedDragCoeff", 2 },
    { "Tire0PneumaticTrail", 2 },
    { "Tire0PneumaticOffset", 2 },
    { "Tire0ZStiffness", 4 },
    { "Tire0XStiffness", 4 },
    { "Tire0CStiffness", 3 },
    { "Tire0ZFriction", 3 },
    { "Tire0XFriction", 3 },
    { "Tire1RollingResistance", 2 },
    { "Tire1InducedDragCoeff", 2 },
    { "Tire1PneumaticTrail", 2 },
    { "Tire1PneumaticOffset", 2 },
    { "Tire1ZStiffness", 4 },
    { "Tire1XStiffness", 4 },
    { "Tire1CStiffness", 3 },
    { "Tire1ZFriction", 3 },
    { "Tire1XFriction", 3 },
    { "Tire2RollingResistance", 2 },
    { "Tire2InducedDragCoeff", 2 },
    { "Tire2PneumaticTrail", 2 },
    { "Tire2PneumaticOffset", 2 },
    { "Tire2ZStiffness", 4 },
    { "Tire2XStiffness", 4 },
    { "Tire2CStiffness", 3 },
    { "Tire2ZFriction", 3 },
    { "Tire2XFriction", 3 },
    { "Tire3RollingResistance", 2 },
    { "Tire3InducedDragCoeff", 2 },
    { "Tire3PneumaticTrail", 2 },
    { "Tire3PneumaticOffset", 2 },
    { "Tire3ZStiffness", 4 },
    { "Tire3XStiffness", 4 },
    { "Tire3CStiffness", 3 },
    { "Tire3ZFriction", 3 },
    { "Tire3XFriction", 3 },
    { "Tire4RollingResistance", 2 },
    { "Tire4InducedDragCoeff", 2 },
    { "Tire4PneumaticTrail", 2 },
    { "Tire4PneumaticOffset", 2 },
    { "Tire4ZStiffness", 4 },
    { "Tire4XStiffness", 4 },
    { "Tire4CStiffness", 3 },
    { "Tire4ZFriction", 3 },
    { "Tire4XFriction", 3 },
};

// Raw access to a Car field by offset (the Car is only partly restructured; see unk_481390.h).
#define CAR_AT(type, offset) (*(type*)((BYTE*)this + (offset)))

// The message text of a temporary string (the original tests the pointer after loading the empty literal).
static inline const char* TextOf(const TmpString& s)
{
    const char* text = "";
    if (s.ptr) text = s.ptr;
    return text;
}

// These discarded calls retain the target runtime_error string-copy call boundaries.
static inline int TireInlineBalance(int a) { return a + 1; }

// Link the layout-compatible error-text helpers to their existing standard-string bodies.
#pragma comment(linker, "/alternatename:?Tidy@PathString@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?Grow@PathString@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
#pragma comment(linker, "/alternatename:?Eos@PathString@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")
#pragma comment(linker, "/alternatename:?Tidy@TmpString@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?MissingText@@YA?AVTmpString@@ABVPathString@@PBD@Z=??Hstd@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@0@ABV10@PBD@Z")
typedef char TirePathLayoutCheck[sizeof(PathString) == sizeof(std::string) ? 1 : -1];
typedef char TireTmpLayoutCheck[sizeof(TmpString) == sizeof(std::string) ? 1 : -1];

// original: FUN_004634e0
// FUNCTION: RALLYTROPHY 0x4634e0
// Probe history and relocation audit: docs/analysis/tire-config-loader.md.
void Car::LoadTiresIni(const char* path, unsigned int checksum)
{
    CarSetupView* v = (CarSetupView*)this;
    Config config;
    config.Define(TireKeys, 45);
    if (!config.Load(path, true, true, checksum)) {
        throw std::runtime_error(AsString(
            PathString(TextOf(MissingText(PathString("Missing variables in file: "), path)))));
    }
    char rollingResistance[] = "Tire0RollingResistance";
    char inducedDrag[] = "Tire0InducedDragCoeff";
    char pneumaticTrail[] = "Tire0PneumaticTrail";
    char pneumaticOffset[] = "Tire0PneumaticOffset";
    char zStiffness[] = "Tire0ZStiffness";
    char xStiffness[] = "Tire0XStiffness";
    char cStiffness[] = "Tire0CStiffness";
    char zFriction[] = "Tire0ZFriction";
    char xFriction[] = "Tire0XFriction";
    int i = 0;
    // Anchor the byte cursor at the third z-stiffness word, matching the target induction pointer.
    BYTE* cursor = (BYTE*)&v->tyreScale + 0x18;
    for (; i < 5; i++, cursor += sizeof(TyreSurface)) {
        char digit = (char)(i + '0');
        rollingResistance[4] = digit;
        inducedDrag[4] = digit;
        pneumaticTrail[4] = digit;
        pneumaticOffset[4] = digit;
        zStiffness[4] = digit;
        xStiffness[4] = digit;
        cStiffness[4] = digit;
        zFriction[4] = digit;
        xFriction[4] = digit;
        *(float*)(cursor - 0x18) = config.GetFloat(rollingResistance);
        *(float*)(cursor - 0x14) = config.GetFloat(inducedDrag);
        *(float*)(cursor - 0x10) = config.GetFloat(pneumaticTrail);
        *(float*)(cursor - 0xc) = config.GetFloat(pneumaticOffset);
        {
            const TyreF3& r = *(const TyreF3*)&config.GetVector3(zStiffness);
            TyreF3* z = (TyreF3*)(cursor - 0x8);
            z->i[0] = r.i[0];
            z->i[1] = r.i[1];
            z->i[2] = r.i[2];
        }
        {
            const TyreF3& r = *(const TyreF3*)&config.GetVector3(xStiffness);
            *(int*)(cursor + 0x4) = r.i[0];
            *(int*)(cursor + 0x8) = r.i[1];
            *(int*)(cursor + 0xc) = r.i[2];
        }
        {
            const TyreF2& r = *(const TyreF2*)&config.GetVector2(cStiffness);
            *(int*)(cursor + 0x10) = r.i[0];
            *(int*)(cursor + 0x14) = r.i[1];
        }
        {
            const TyreF2& r = *(const TyreF2*)&config.GetVector2(zFriction);
            *(int*)(cursor + 0x18) = r.i[0];
            *(int*)(cursor + 0x1c) = r.i[1];
        }
        {
            const TyreF2& r = *(const TyreF2*)&config.GetVector2(xFriction);
            *(int*)(cursor + 0x20) = r.i[0];
            *(int*)(cursor + 0x24) = r.i[1];
        }
    }
    TireInlineBalance(1);
    TireInlineBalance(1);
}


// ===== LoadTextures (0x463940) =====

// A file name temporary whose destructor is expanded inline (the later loads of LoadTextures).
class InlineTmp {
public:
    ~InlineTmp()
    {
        char* p = ptr;
        if (p) {
            unsigned char refs = ((unsigned char*)p)[-1];
            --p;
            if (refs == 0 || refs == 255) {
                alloc.deallocate(p, reserve + 2);
            } else {
                *(unsigned char*)p = refs - 1;
            }
        }
    }
    const char* c_str() const { return ptr ? ptr : ""; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int reserve;
};
InlineTmp JoinPath(const std::string& dir, const char* name); // 0x409130: operator+(string, const char*)

// Assigns dir + name to a file name string, the temporary destroyed inline. (A named std::string instead of a
// reference-bound temporary: the latter uses up compiler counters and renumbers the module-end _$E22/_$E23.)
#define ASSIGN_INLINE(str, name) \
    do { \
        std::string tmp_ = dir + (name); \
        (str).Assign(*(const TmpString*)&tmp_, 0, std::string::npos); \
    } while (0)

// A node of the sprite list at Car+0xfc0 (0xc bytes; the nodes are recycled through a free list).
struct SpriteNode {
    SpriteNode* next;                  // 0x0
    SpriteNode* prev;                  // 0x4
    struct FlareSprite* data;          // 0x8
};

// The Sprite of unk_481390.h (0xd4 bytes, ctor 0x4a8580) as the texture loader fills it in.
// The out-of-line std::string::assign(const char*, size_t) (0x404ba0).
class SpriteName : public std::string {
public:
    void AssignN(const char* text, unsigned int n); // 0x404ba0
    void Assign(const char* text) { AssignN(text, strlen(text)); }
};

struct FlareSprite {
    Texture* texture;                  // 0x00
    BYTE unk04[0x18 - 0x04];
    int blendSrc;                      // 0x18
    int blendDst;                      // 0x1c
    int unk20;                         // 0x20
    int unk24;                         // 0x24
    BYTE unk28[0x3c - 0x28];
    int unk3c;                         // 0x3c
    float color40[4];                  // 0x40
    BYTE unk50[0x60 - 0x50];
    float color60[4];                  // 0x60
    float color70[4];                  // 0x70
    BYTE unk80[0x98 - 0x80];
    int unk98;                         // 0x98
    int unk9c;                         // 0x9c
    int unka0;                         // 0xa0
    std::string name;                  // 0xa4
    int unkb4;
    int unkb8;                         // 0xb8
    BYTE unkbc[0xd4 - 0xbc];
};

// The object DAT_0056b0ac + 0x20 (a texture sink with a vtable); slot 5 takes the environment map.
class CarEnvironment {
public:
    virtual void Slot0();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void SetTexture(Texture* texture, int slot, int flag); // 0x14
};
struct CarClock {
    BYTE unk0[0x20];
    CarEnvironment* environment;       // 0x20
};

// The Car as LoadTextures sees it (a view; Car itself is in unk_481390.h).
struct CarTextureView {
    BYTE unk0[0xfa4];
    CarTextureCache* cache;            // 0xfa4
    BYTE unkfa8[0xfc0 - 0xfa8];
    SpriteNode* spriteHead;            // 0xfc0
    SpriteNode* spriteTail;            // 0xfc4
    SpriteNode* spriteFree;            // 0xfc8
    volatile int spriteCount;          // 0xfcc (volatile: keeps VC6 from caching the count between appends; fixes the register choice)
    int spriteFreeCount;               // 0xfd0
    BYTE unkfd4[0x108c - 0xfd4];
    float unk108c;                     // 0x108c: scaled into the interior windows texture
    BYTE unk1090[0x1ec8 - 0x1090];
    FlareSprite* metersKmh;            // 0x1ec8
    FlareSprite* metersMph;            // 0x1ecc
    BYTE unk1ed0[0x1bce4 - 0x1ed0];
    Texture* cubeMap;                  // 0x1bce4
    int shadowMode;                    // 0x1bce8: 2 = environment map
    int unk1bcec;                      // 0x1bcec
    BYTE unk1bcf0[0x1bcf8 - 0x1bcf0];
    Texture* windowsA;                 // 0x1bcf8
    BYTE unk1bcfc[0x1bd08 - 0x1bcfc];
    Texture* windowsB;                 // 0x1bd08
    BYTE unk1bd0c[0x1bd18 - 0x1bd0c];
    Texture* windowsC;                 // 0x1bd18
    BYTE unk1bd1c[0x1bd28 - 0x1bd1c];
    Texture* windowsD;                 // 0x1bd28
    BYTE unk1bd2c[0x1bd34 - 0x1bd2c];
    Texture* interiorWindows;          // 0x1bd34
    BYTE unk1bd38[0x1bd58 - 0x1bd38];
    Texture* lightsA;                  // 0x1bd58
    BYTE unk1bd5c[0x1bdac - 0x1bd5c];
    Texture* lightsB;                  // 0x1bdac
    BYTE unk1bdb0[0x1be38 - 0x1bdb0];
    Texture* lightsC;                  // 0x1be38
    BYTE unk1be3c[0x1be54 - 0x1be3c];
    Texture* lightsD;                  // 0x1be54
    BYTE unk1be58[0x1be90 - 0x1be58];
    FlareSprite* lightSprite;          // 0x1be90
    Texture* lightsLit;                // 0x1be94
    Texture* lightsBraking;            // 0x1be98
    BYTE unk1be9c[0x1bebc - 0x1be9c];
    FlareSprite* flareFront;           // 0x1bebc
    FlareSprite* flareRear;            // 0x1bec0
    FlareSprite* flareBrake;           // 0x1bec4
    FlareSprite* flareReverse;         // 0x1bec8
    BYTE unk1becc[0x1bf00 - 0x1becc];
    BYTE flags;                        // 0x1bf00: bit0 = overlay set, bit1 = no light/window textures
};

// Appends a sprite to the list at Car+0xfc0, reusing a node of the free list when there is one.
#define ADD_SPRITE(v, sprite) \
    do { \
        if ((v)->spriteFree == 0) { \
            node = (SpriteNode*)operator new(sizeof(SpriteNode)); \
            if (node) { \
                node->prev = 0; \
                node->next = 0; \
                node->data = 0; \
            } else { \
                node = 0; \
            } \
        } else { \
            node = (v)->spriteFree; \
            (v)->spriteFree = node->next; \
            __asm dec dword ptr [esi+0xfd0] /* target: dec spriteFreeCount in place */ \
        } \
        tail = (v)->spriteTail; \
        node->data = (sprite); \
        node->prev = tail; \
        node->next = 0; \
        if (tail) { \
            tail->next = node; \
        } else { \
            (v)->spriteHead = node; \
        } \
        (v)->spriteTail = node; \
        ++(v)->spriteCount; \
    } while (0)

// NONMATCHING: RALLYTROPHY 0x463940
// Built 2983 bytes against 2926, 1773 differing bytes. Volatile stores at three flare sites plus the explicit
// free-count DEC reduce the mismatch; target still differs in register/scheduling choices and sprite-list updates.
// Loads the flare sprites, the light, window and overlay textures of the car from its data directory.
void Car::LoadTextures(const std::string& dir)
{
    CarTextureView* v = (CarTextureView*)this;
    NameString a;
    NameString b;
    FlareSprite* s;
    SpriteNode* node;
    SpriteNode* tail;

    a.Assign(SoundPath(dir, "FlareFront.tga"), 0, std::string::npos);
    s = (FlareSprite*)new Sprite;
    v->flareFront = s;
    s->texture = v->cache->Load(a.c_str(), 0x40);
    s->blendSrc = 2;
    s->blendDst = 2;
    // Preserve the target's immediate store before the sprite free-list check.
    *(volatile int*)&s->unk98 = 0xb;
    s->unk9c = 2;
    s->color60[0] = 0;
    s->color60[1] = 0;
    s->color60[2] = 0;
    s->color60[3] = 0;
    s->unkb8 = -1;
    s->color70[0] = 0;
    s->color70[1] = 0;
    s->color70[2] = 0;
    s->color70[3] = 0;
    s->unk3c = 1;
    ADD_SPRITE(v, s);

    a.Assign(SoundPath(dir, "FlareRear.tga"), 0, std::string::npos);
    s = (FlareSprite*)new Sprite;
    v->flareRear = s;
    s->texture = v->cache->Load(a.c_str(), 0x40);
    s->blendSrc = 2;
    s->blendDst = 2;
    *(volatile int*)&s->unk98 = 0xb;
    s->unk9c = 2;
    s->color60[0] = 0;
    s->color60[1] = 0;
    s->color60[2] = 0;
    s->color60[3] = 0;
    s->unkb8 = -1;
    s->color70[0] = 0;
    s->color70[1] = 0;
    s->color70[2] = 0;
    s->color70[3] = 0;
    s->unk3c = 1;
    ADD_SPRITE(v, s);

    a.Assign(SoundPath(dir, "FlareBrake.tga"), 0, std::string::npos);
    s = (FlareSprite*)new Sprite;
    ((SpriteName*)&s->name)->Assign("flare brake");
    v->flareBrake = s;
    s->texture = v->cache->Load(a.c_str(), 0x40);
    s->blendSrc = 2;
    s->blendDst = 2;
    s->color60[0] = 0;
    s->color60[1] = 0;
    s->color60[2] = 0;
    s->color60[3] = 0;
    s->unk98 = 0xb;
    s->color40[0] = 1.0f;
    s->color40[1] = 1.0f;
    s->color40[2] = 1.0f;
    s->color40[3] = 1.0f;
    s->unk9c = 2;
    s->unkb8 = -1;
    s->color70[0] = 0;
    s->color70[1] = 0;
    s->color70[2] = 0;
    s->color70[3] = 0;
    s->unk3c = 1;
    ADD_SPRITE(v, s);

    a.Assign(SoundPath(dir, "FlareReverse.tga"), 0, std::string::npos);
    s = (FlareSprite*)new Sprite;
    v->flareReverse = s;
    s->texture = v->cache->Load(a.c_str(), 0x40);
    s->blendSrc = 2;
    s->blendDst = 2;
    s->color60[0] = 0;
    s->color60[1] = 0;
    s->color60[2] = 0;
    s->color60[3] = 0;
    *(volatile int*)&s->unk98 = 0xb;
    s->color40[0] = 1.0f;
    s->color40[1] = 1.0f;
    s->color40[2] = 1.0f;
    s->color40[3] = 1.0f;
    s->unk9c = 2;
    s->unkb8 = -1;
    s->color70[0] = 0;
    s->color70[1] = 0;
    s->color70[2] = 0;
    s->color70[3] = 0;
    s->unk3c = 1;
    ADD_SPRITE(v, s);

    a.Assign(SoundPath(dir, "LightsLit.tga"), 0, std::string::npos);
    Texture* lit = v->cache->Load(a.c_str(), 0x40);
    v->lightsLit = lit;
    a.Assign(SoundPath(dir, "LightsBraking.tga"), 0, std::string::npos);
    v->lightsBraking = v->cache->Load(a.c_str(), 0x40);
    s = (FlareSprite*)new Sprite;
    s->texture = lit;
    s->blendSrc = 2;
    s->blendDst = 2;
    s->unk98 = 0xf;
    s->unka0 = 0xf;
    s->unk9c = 2;
    s->color60[0] = 1.0f;
    s->color60[1] = 1.0f;
    s->color60[2] = 1.0f;
    s->color60[3] = 1.0f;
    s->unk3c = -1;
    s->color70[0] = 0;
    s->color70[1] = 0;
    s->color70[2] = 0;
    s->color70[3] = 0;
    s->unk20 = 4;
    v->lightSprite = s;
    s->color40[0] = 1.0f;
    s->color40[1] = 1.0f;
    s->color40[2] = 1.0f;
    s->color40[3] = 1.0f;
    s->unk24 = 1;

    if (v->flags & 2) {
        return;
    }

    a.Assign(SoundPath(dir, "lights_damaged.tga"), 0, std::string::npos);
    v->lightsA = v->lightsB = v->lightsC = v->lightsD = v->cache->Load(a.c_str(), 0x40);

    a.Assign(SoundPath(dir, "windows_damaged.tga"), 0, std::string::npos);
    Texture* windows = v->cache->Load(a.c_str(), 0x40);
    v->windowsA = v->windowsB = v->windowsC = v->windowsD = windows;
    if (windows && v->unk1bcec == 0) {
        *(int*)((char*)windows + 0x60) = FloatToInt(v->unk108c * 256.0f);
    }

    ASSIGN_INLINE(b, "interior_windows_damaged.tga");
    v->interiorWindows = v->cache->Load(b.c_str(), 0x40);

    if (v->flags & 1) {
        ASSIGN_INLINE(b, "OverlayMetersKmh.tga");
        s = (FlareSprite*)new Sprite;
        v->metersKmh = s;
        s->texture = v->cache->Load(b.c_str(), 0x441);
        s->blendSrc = 5;
        s->blendDst = 6;
        s->color60[0] = 1.0f;
    s->color60[1] = 1.0f;
    s->color60[2] = 1.0f;
    s->color60[3] = 1.0f;
        s->unk20 = 2;
        s->color40[0] = 1.0f;
    s->color40[1] = 1.0f;
    s->color40[2] = 1.0f;
    s->color40[3] = 1.0f;
        s->unk24 = 2;
        s->unk98 = 0xf;
        s->unk9c = 2;
        ADD_SPRITE(v, s);

        ASSIGN_INLINE(b, "OverlayMetersMph.tga");
        s = (FlareSprite*)new Sprite;
        v->metersMph = s;
        s->texture = v->cache->Load(b.c_str(), 0x441);
        s->blendSrc = 5;
        s->blendDst = 6;
        s->color60[0] = 1.0f;
    s->color60[1] = 1.0f;
    s->color60[2] = 1.0f;
    s->color60[3] = 1.0f;
        s->unk20 = 2;
        s->color40[0] = 1.0f;
    s->color40[1] = 1.0f;
    s->color40[2] = 1.0f;
    s->color40[3] = 1.0f;
        s->unk24 = 2;
        s->unk98 = 0xf;
        s->unk9c = 2;
        ADD_SPRITE(v, s);
    }

    if (v->shadowMode == 2) {
        CarEnvironment* environment = ((CarClock*)DAT_0056b0ac)->environment;
        Texture* cube = (Texture*)new Texture("CubeMap");
        v->cubeMap = cube;
        environment->SetTexture(cube, 0x80, 0);
    }
}

// ===== wiring helpers (0x4644b0 - 0x464940) =====
// Light slot of the car (0x1c bytes; Car+0x1bd40 .. +0x1be90, twelve in groups of 3, 3, 2, 2, 2).
struct CarLightRec {
    ModelPart* light;                  // 0x00: the CarLight (flag byte at +0xc, intensity float at +0x130)
    BYTE flag;                         // 0x04
    BYTE unk5[3];
    struct LightMesh* mesh;            // 0x08: the mesh of the light's name (light+0x14) in the body
    int* target;                       // 0x0c
    int value;                         // 0x10
    BYTE unk14[0x1c - 0x14];
};

// The twelve light slots of the Car (Car+0x1bd40): head/tail/brake/reverse groups A (0-2), B (3-5), C (6-7), D (8-9), E (10-11).
struct CarLightsView {
    BYTE unk0[0x1bd40];
    CarLightRec rec[12];               // 0x1bd40
};

// original: FUN_004644b0
// FUNCTION: RALLYTROPHY 0x4644b0
void Car::SetLightIntensity(float level)
{
    CarLightsView* v = (CarLightsView*)this;
    for (int i = 0; i < 3; i++) {
        if (v->rec[i].light) *(float*)((BYTE*)v->rec[i].light + 0x130) = level;
        if (v->rec[i + 3].light) *(float*)((BYTE*)v->rec[i + 3].light + 0x130) = level;
    }
    for (int j = 0; j < 2; j++) {
        if (v->rec[j + 6].light) *(float*)((BYTE*)v->rec[j + 6].light + 0x130) = level;
        if (v->rec[j + 8].light) *(float*)((BYTE*)v->rec[j + 8].light + 0x130) = level;
        if (v->rec[j + 10].light) *(float*)((BYTE*)v->rec[j + 10].light + 0x130) = level;
    }
}

// The Stage fields that AttachStage hands to the car's parts.
struct StageWiring {
    BYTE unk0[0x1a0];
    float lightLevel;                  // 0x1a0: car light intensity of the stage
    BYTE unk1a4[0x1f8 - 0x1a4];
    Vec3 effectColor;                  // 0x1f8: particle colour
    BYTE unk204[4];
    int shadowColour[4];               // 0x208: copied to the car.ini light at Car+0x1bd30 (+0x88)
    BYTE unk218[0x22c - 0x218];
    int rainy;                         // 0x22c: 1 when the stage is wet
    float wetness;                     // 0x230
    BYTE unk234[0x908 - 0x234];
    BYTE skidMarkData;                 // 0x908: address handed to the four SkidMarks
    BYTE unk909[0x9dc - 0x909];
    int unk9dc;                        // 0x9dc
    int* unk9e0;                       // 0x9e0
    int unk9e4;                        // 0x9e4: object value for the vector at Car+0x1bcd4
    void* Func414360();                // 0x414360: returns the second dword of the object at +0x6b0
};
struct Int4 {
    int a, b, c, d;
};

extern "C" void fun_00464940();
extern "C" void fun_004644b0();
extern "C" void fun_0047c4f0();
extern "C" void fun_00414360();

// original: FUN_00464530
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x464530
void Car::FUN_00464530(Stage*, int)
{
    __asm {
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 0x1bce8]
        test eax, eax
        push edi
        mov edi, dword ptr [esp + 0xc]
        mov dword ptr [esi + 0x14fc], edi
        je car_attach_stage_lights
        cmp eax, 2
        jne car_attach_stage_default_field
        mov eax, dword ptr [esi + 0x1bce4]
        jmp car_attach_stage_set_field
    car_attach_stage_default_field:
        mov eax, dword ptr [edi + 0x9e4]
    car_attach_stage_set_field:
        push eax
        mov ecx, esi
        call fun_00464940
    car_attach_stage_lights:
        fld dword ptr [edi + 0x1a0]
        push ecx
        fstp dword ptr [esp]
        mov ecx, esi
        call fun_004644b0
        lea edx, [edi + 0x908]
        lea eax, [esi + 0x2874]
        mov ecx, 4
    car_attach_stage_skid_loop:
        mov dword ptr [eax], edx
        add eax, 0x6558
        dec ecx
        jne car_attach_stage_skid_loop
        mov eax, dword ptr [esi + 0x1f18]
        test eax, eax
        je car_attach_stage_after_wetness
        fld dword ptr [edi + 0x230]
        fstp dword ptr [eax + 0x174]
    car_attach_stage_after_wetness:
        mov eax, dword ptr [edi + 0x9e0]
        mov ecx, dword ptr [edi + 0x9dc]
        mov dword ptr [esi + 0x247c], eax
        mov dword ptr [esi + 0x2224], eax
        mov dword ptr [esi + 0x2350], eax
        mov dword ptr [esi + 0x25ac], ecx
        lea edx, [edi + 0x1f8]
        push edx
        lea ecx, [esi + 0x2358]
        mov dword ptr [esi + 0x270c], eax
        call fun_0047c4f0
        mov eax, dword ptr [esi + 0x1bd30]
        test eax, eax
        je car_attach_stage_after_shadow
        mov ecx, dword ptr [esi + 0x14fc]
        add ecx, 0x208
        mov edx, dword ptr [ecx]
        add eax, 0x88
        mov dword ptr [eax], edx
        mov edx, dword ptr [ecx + 4]
        mov dword ptr [eax + 4], edx
        mov edx, dword ptr [ecx + 8]
        mov ecx, dword ptr [ecx + 0xc]
        mov dword ptr [eax + 8], edx
        mov dword ptr [eax + 0xc], ecx
    car_attach_stage_after_shadow:
        mov eax, dword ptr [esi + 0x2004]
        test eax, eax
        je car_attach_stage_after_light
        mov edx, dword ptr [edi + 0x9e0]
        mov ecx, dword ptr [edx]
        mov dword ptr [eax + 0x124], ecx
    car_attach_stage_after_light:
        push ebx
        mov ebx, dword ptr [esi + 0x2008]
        test ebx, ebx
        je car_attach_stage_after_light_source
        mov ecx, edi
        call fun_00414360
        mov dword ptr [ebx + 0x124], eax
    car_attach_stage_after_light_source:
        mov eax, dword ptr [esi + 0x14fc]
        xor ecx, ecx
        test eax, eax
        pop ebx
        je car_attach_stage_dry
        cmp dword ptr [eax + 0x22c], 1
        jne car_attach_stage_dry
        mov eax, 1
        pop edi
        mov dword ptr [esi + 0x1ffc], eax
        pop esi
        ret 8
    car_attach_stage_dry:
        pop edi
        mov dword ptr [esi + 0x1ffc], ecx
        pop esi
        ret 8
    }
}

// original: FUN_00464680
// FUNCTION: RALLYTROPHY 0x464680
void Car::ResetControls()
{
    CAR_AT(int, 0x1340) = 0;
    CAR_AT(int, 0x133c) = 0;
    CAR_AT(int, 0x1338) = 0;
    CAR_AT(int, 0x1344) = 0;
    CAR_AT(int, 0x134c) = 0;
    CAR_AT(int, 0x138c) = 0;
    CAR_AT(int, 0x1388) = 0;
    CAR_AT(int, 0x1384) = 0;
    CAR_AT(int, 0x142c) = 0;
    CAR_AT(int, 0x1befc) = 0;
}

// The body's mesh container (Car+0x24 + 0x124): At returns the owner whose Find looks a mesh up by name.
struct MeshOwner {
    LightMesh* Find(const void* name); // 0x4b9790
};
struct BodyMeshList {
    MeshOwner* At(int index);          // 0x4a6800
};

extern "C" void* fun_004a6800();
extern "C" void fun_004b2b30();
extern "C" void* fun_004b9790();

// original: FUN_004646c0
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4646c0
void Car::AttachLights()
{
    __asm {
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov edi, ecx
        mov ecx, dword ptr [edi + 0x24]
        push 0
        add ecx, 0x124
        call fun_004a6800
        mov ebx, eax
        lea esi, [edi + 0x1bd40]
        mov dword ptr [esp + 0x10], 3
    car_attach_lights_loop3:
        mov ebp, dword ptr [esi]
        test ebp, ebp
        je car_attach_lights_b
        mov eax, dword ptr [edi + 0x24]
        push eax
        mov ecx, ebp
        call fun_004b2b30
        add ebp, 0x14
        push ebp
        mov ecx, ebx
        call fun_004b9790
        mov dword ptr [esi + 8], eax
        mov byte ptr [esi + 4], 0
        mov eax, dword ptr [esi + 0xc]
        test eax, eax
        je car_attach_lights_b
        mov ecx, dword ptr [esi + 0x10]
        mov dword ptr [eax], ecx
    car_attach_lights_b:
        mov ebp, dword ptr [esi + 0x54]
        test ebp, ebp
        je car_attach_lights_next3
        mov edx, dword ptr [edi + 0x24]
        push edx
        mov ecx, ebp
        call fun_004b2b30
        add ebp, 0x14
        push ebp
        mov ecx, ebx
        call fun_004b9790
        mov dword ptr [esi + 0x5c], eax
        mov byte ptr [esi + 0x58], 0
        mov eax, dword ptr [esi + 0x60]
        test eax, eax
        je car_attach_lights_next3
        mov ecx, dword ptr [esi + 0x64]
        mov dword ptr [eax], ecx
    car_attach_lights_next3:
        mov eax, dword ptr [esp + 0x10]
        add esi, 0x1c
        dec eax
        mov dword ptr [esp + 0x10], eax
        jne car_attach_lights_loop3
        lea esi, [edi + 0x1be20]
        mov dword ptr [esp + 0x10], 2
    car_attach_lights_loop2:
        mov ebp, dword ptr [esi]
        test ebp, ebp
        je car_attach_lights_c
        mov edx, dword ptr [edi + 0x24]
        push edx
        mov ecx, ebp
        call fun_004b2b30
        add ebp, 0x14
        push ebp
        mov ecx, ebx
        call fun_004b9790
        mov dword ptr [esi + 8], eax
        mov byte ptr [esi + 4], 0
        mov eax, dword ptr [esi + 0xc]
        test eax, eax
        je car_attach_lights_c
        mov ecx, dword ptr [esi + 0x10]
        mov dword ptr [eax], ecx
    car_attach_lights_c:
        mov ebp, dword ptr [esi - 0x38]
        test ebp, ebp
        je car_attach_lights_e
        mov edx, dword ptr [edi + 0x24]
        push edx
        mov ecx, ebp
        call fun_004b2b30
        add ebp, 0x14
        push ebp
        mov ecx, ebx
        call fun_004b9790
        mov dword ptr [esi - 0x30], eax
        mov byte ptr [esi - 0x34], 0
    car_attach_lights_e:
        mov ebp, dword ptr [esi + 0x38]
        test ebp, ebp
        je car_attach_lights_next2
        mov eax, dword ptr [edi + 0x24]
        push eax
        mov ecx, ebp
        call fun_004b2b30
        add ebp, 0x14
        push ebp
        mov ecx, ebx
        call fun_004b9790
        mov dword ptr [esi + 0x40], eax
        mov byte ptr [esi + 0x3c], 0
    car_attach_lights_next2:
        mov eax, dword ptr [esp + 0x10]
        add esi, 0x1c
        dec eax
        mov dword ptr [esp + 0x10], eax
        jne car_attach_lights_loop2
        pop edi
        pop esi
        pop ebp
        pop ebx
        pop ecx
        ret
    }
}

// original: FUN_00464800
// FUNCTION: RALLYTROPHY 0x464800
void Car::FUN_00464800()
{
    for (int n = 0; n < 4; n++) {
        CAR_AT(int, 0x1bcac + n * 4) = 0;
    }
    CAR_AT(int, 0x1bcbc) = 0;
    CAR_AT(int, 0x1bcc0) = 0;
    for (int i = 0; i < 4; i++) {
        ((SkidMarks*)((BYTE*)this + 0x2748 + i * 0x6558))->Reset();
    }
    CAR_AT(int, 0x1f13c) = 0;
    CAR_AT(int, 0x1f148) = 0;
    CAR_AT(int, 0x1f144) = 0;
    CAR_AT(int, 0x1f120) = 0;
    CAR_AT(int, 0x1374) = 0;
    CAR_AT(int, 0x1378) = 0;
    for (int m = 0; m < 8; m++) {
        CAR_AT(int, 0x1354 + m * 4) = 0;
    }
    CAR_AT(int, 0x1f2b0) = 0;
    CAR_AT(int, 0x1f2b4) = 0;
    CAR_AT(int, 0x1f2b8) = 0;
    CAR_AT(int, 0x1f2c0) = 0;
    CAR_AT(int, 0x1f2bc) = 0;
    CAR_AT(int, 0x1f2c4) = 0;
    CAR_AT(int, 0x1f2c8) = 0;
    CAR_AT(int, 0x1f2cc) = 0;
    CAR_AT(int, 0x1f2d0) = 0;
    CAR_AT(int, 0x1f114) = 0;
    std::vector<int>& first = CAR_AT(std::vector<int>, 0x1430);
    first.erase(first.begin(), first.end());
    std::vector<int>& second = CAR_AT(std::vector<int>, 0x1464);
    second.erase(second.begin(), second.end());
    CAR_AT(int, 0x1f2ac) = 0;
    for (int k = 0; k < 8; k++) {
        CAR_AT(int, 0x1f24c + k * 12) = 0;
    }
    CAR_AT(int, 0x1c908) = 0;
    CAR_AT(int, 0x1c90c) = -60000;
}

// An object of the vector at Car+0x1bcd4 (one int field at +4 is set by SetObjectField).
struct ObjectField {
    int unk0;
    int value;                         // 0x04
};

// original: FUN_00464940
// FUNCTION: RALLYTROPHY 0x464940
void Car::SetObjectField(int value)
{
    for (unsigned int i = 0; i < CAR_AT(std::vector<ObjectField*>, 0x1bcd4).size(); i++) {
        CAR_AT(std::vector<ObjectField*>, 0x1bcd4)[i]->value = value;
    }
}


typedef CarList<MaterialSprite*>::Node MaterialNode;

// FUNCTION: RALLYTROPHY 0x464980
// SYMBOL: ?CStrOut@CarString@@QBEPBDXZ
// std::string::c_str(), out of line: the constructor calls it for the late dirt.tga path (0x4616e4).
const char* CarString::CStrOut() const
{
    return ptr ? ptr : "";
}

// FUNCTION: RALLYTROPHY 0x464990
// SYMBOL: ?Next@Node@?$CarList@PAVMaterialSprite@@@@QAEPAU12@XZ
MaterialNode* MaterialNode::Next()
{
    return next;
}

// FUNCTION: RALLYTROPHY 0x4649a0
// SYMBOL: ?Data@Node@?$CarList@PAVMaterialSprite@@@@QAEPAVMaterialSprite@@XZ
MaterialSprite* MaterialNode::Data()
{
    return data;
}

// FUNCTION: RALLYTROPHY 0x4649b0
SnapMesh* ModelFilePart::FirstMesh()
{
    return meshes.At(0);
}

// Module-end guard pair VC6 emits for files that include the STL headers:
// guard bit 0 of 0x5826d5, then atexit(0x402c90).

// FUNCTION: RALLYTROPHY 0x4649c0
// SYMBOL: _$E23
typedef void Generated_4649c0();

// FUNCTION: RALLYTROPHY 0x4649e0
// SYMBOL: _$E22
typedef void Generated_4649e0();

// FUNCTION: RALLYTROPHY 0x4649f0
// SYMBOL: ??1CarEventQueue@@QAE@XZ
// Compiler-generated destructor of the 0xa8-byte queue at Car+0x1f160 (two element arrays with empty
// destructors); the exception funclet for state 0x49 jumps here, the normal path inlines it.
typedef void Generated_4649f0();

// FUNCTION: RALLYTROPHY 0x464a50
void CarObject::ResetMessage()
{
    messageId = 0;
    messageTime = -60000;
}

// FUNCTION: RALLYTROPHY 0x464a70
LightRec::LightRec()
{
    light = 0;
    flag = 0;
    mesh = 0;
    unkc = 0;
    unk10 = 0;
    unk14 = 0;
    unk18 = 0;
}

// FUNCTION: RALLYTROPHY 0x464a90
// SYMBOL: ??0SoundLayer@@QAE@XZ
// Compiler-generated: the SoundLayer constructor the EH vector constructor iterator calls (SoundSource ctor at +4).
typedef void Generated_464a90();

// FUNCTION: RALLYTROPHY 0x464aa0
// SYMBOL: ??1SoundLayer@@QAE@XZ
// Compiler-generated: the SoundLayer destructor the EH vector destructor iterator calls (jumps to ~SoundSource at +4).
typedef void Generated_464aa0();
