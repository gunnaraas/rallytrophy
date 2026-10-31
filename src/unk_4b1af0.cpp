// BMF model chunk readers: batch (0x2c / 0x24 byte vertices), vertex array and polylist.
#include <string>
#include <stdexcept>
#include "unk_4b1af0.h"
#include "unk_476490.h"

// Layout view of File: stream pointer then name.
struct FileNameView { void* stream; std::string name; };
static __forceinline const std::string& FName(File* f) { return ((FileNameView*)f)->name; }

struct Tmp3 { Tmp3() {} ~Tmp3() {} unsigned int r[11]; };
struct V11 { unsigned int f[11]; };
struct BStr {
    std::allocator<char> alloc; char* ptr; unsigned int len; unsigned int res;
    void Tidy(bool b);
    bool Grow(unsigned int n, bool trim);
    BStr(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al) {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) { memcpy(ptr, s, n); len = n; ptr[n] = 0; }
    }
    ~BStr() { Tidy(true); }
};
#pragma comment(linker, "/alternatename:?Tidy@BStr@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?Grow@BStr@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")

struct PStr {
    std::allocator<char> alloc; char* ptr; unsigned int len; unsigned int res;
    void AssignCall(const char* s, unsigned int n);
    PStr(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { ptr = 0; len = 0; res = 0; AssignCall(s, strlen(s)); }
    ~PStr() { if (ptr != 0) { if (((unsigned char*)ptr)[-1] != 0 && ((unsigned char*)ptr)[-1] != 255) --((unsigned char*)ptr)[-1]; else alloc.deallocate(ptr - 1, res + 2); } }
};

#define BMF_BATCH_BODY(REC, TRI)                                                           \
    int mat = FUN_004af230(file);                                                          \
    int nverts = FUN_004af230(file);                                                       \
    int ntris = FUN_004af230(file);                                                        \
    DynamicMesh* mesh = new DynamicMesh;                                                   \
    BmfVertex* v = new BmfVertex[nverts];                                                  \
    unsigned short* idx = (unsigned short*)operator new(ntris * 6);                        \
    V11* dv = (V11*)v;                                                                   \
    Tmp3 tmp; unsigned int* r = tmp.r;                                                     \
    int i;                                                                                 \
    for (i = nverts; i > 0; i--) {                                                     \
        file->Read(r, REC);                                                                \
        dv->f[0] = r[0]; dv->f[1] = r[1]; dv->f[2] = r[2]; dv->f[3] = r[3]; dv->f[4] = r[4]; dv->f[5] = r[5];      \
        dv->f[6] = r[6] | 0xff000000; dv->f[7] = r[7]; dv->f[8] = r[8]; dv->f[9] = r[7]; dv->f[10] = r[8];     \
        dv++;                                                                              \
    }                                                                                      \
    unsigned short* p = idx;                                                               \
    for (i = 0; i < ntris; i++) {                                                      \
        unsigned short t[4];                                                               \
        file->Read(t, TRI);                                                                \
        *p = t[0]; p++; *p = t[1]; p++; *p = t[2]; p++;                                             \
        if ((*(volatile int*)&t[0] & 0xffff) >= nverts || (*(volatile int*)&t[1] & 0xffff) >= nverts || (*(volatile int*)&t[2] & 0xffff) >= nverts)                          \
            throw file_error(FName(file), *(std::string*)&BStr("BMF batch chunk failed due to illegal vertex index")); \
    }                                                                                      \
    *(short*)((char*)mesh + 0xe) = ntris;                                                  \
    *(short*)((char*)mesh + 0xc) = nverts;                                                 \
    *(void**)((char*)mesh + 8) = idx;                                                      \
    *(void**)((char*)mesh + 4) = v;                                                        \
    *(void**)mesh = materials[mat];                                                        \
    out->batches.push_back(mesh);

// NONMATCHING: RALLYTROPHY 0x4b1af0 (inlined std::string/throw construction differs from original)
// Tried 2026-10-31: BStr (hand ctor: out-of-line _Tidy/_Grow via alternatename, inline copy; matches the original's string construction and EH states), Tmp3 empty-dtor record local (state 2), *(volatile int*)&t[k] & 0xffff compares (t slot), p++ steps, Bal budget calls (no effect), volatile stores/decl-order variants; 132->91 insns. Still differs: file/ntris in ebp/ebx swapped, vertex-loop base offset (esi+0x14 vs +8) and r[7]/r[8] reload, tri-loop load/store interleave, tail mesh-field store order.
void ModelFile::ReadBatch(File* file, ModelOut* out)
{
    BMF_BATCH_BODY(0x2c, 8)
}

// NONMATCHING: RALLYTROPHY 0x4b1ec0 (inlined std::string/throw construction differs from original)
void ModelFile::ReadBatchSmall(File* file, ModelOut* out)
{
    BMF_BATCH_BODY(0x24, 6)
}

// FUNCTION: RALLYTROPHY 0x4b2290
void ModelFile::ReadVertices(File* file, ModelOut* out)
{
    int n = FUN_004af230(file);
    BmfVec12* a = new BmfVec12[n];
    out->verts = a;
    out->vertCount = n;
    file->Read(a, n * 12);
}

// NONMATCHING: RALLYTROPHY 0x4b2330 (loop/size() shape and register assignment differ from original)
// Tried 2026-10-30 night: ~35 variants (explicit begin/end compare, hoisted begin, for/while/do forms, copy of p, at(), volatile reload); size() ternary does not fold to a direct null->throw jump and this/p swap ebx/ebp; improved 96->35 via PStr (out-of-line assign + allocator byte), forceinline FName, vector size()/operator[].
void ModelFile::ReadPolylist(File* file, ModelOut* out)
{
    int n = FUN_004af230(file);
    out->polyCount = n;
    int bytes = n << 4;
    unsigned int* p = (unsigned int*)operator new(bytes);
    out->polys = p;
    file->Read(p, bytes);
    int i = 0;
    if (n > 0) for (;;) {
        if (materials.size() <= *p) {
            PStr msg("BMF polylist chunk failed: illegal material index");
            throw file_error(FName(file), *(std::string*)&msg);
        }
        *p = (unsigned int)materials[*p];
        i++; p += 4;
        if (i >= n) break;
    }
}
