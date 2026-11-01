// BMF model chunk readers: batch (0x2c / 0x24 byte vertices), vertex array and polylist.
#include <string>
#include <stdexcept>
#include "unk_4b1af0.h"
#include "unk_476490.h"

// Layout view of File: stream pointer then name.
struct FileNameView { void* stream; std::string name; };
static __forceinline const std::string& FName(File* f) { return ((FileNameView*)f)->name; }

template<int N> struct TmpN { TmpN() {} ~TmpN() {} unsigned int r[N]; };
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

// NONMATCHING: RALLYTROPHY 0x4b1af0 (inlined std::string/throw construction differs from original)
// Tried 2026-10-31 re-pass: if(i<n) for(q=...;;) guard loops bloat (+100 bytes) here, only the plain for(i=n,q=v+5;i>0;i--,q+=11) form works; ntris read through an address-taken pointer (ntris spilled like the original) 91->67; still differs: file/ntris-counter ebx/ebp swapped (file gets ebp), mov ecx,esi/lea d placement vs jle, r[7]/r[8] reload rotation, tri-loop store interleave; per-store block temps, p[0..2], p+=3 and PStr/BStr variants no effect.
// Tried 2026-11-01 twin-idiom re-pass: ReadBatchSmall's named temp for vertex word 6 + volatile first two tri stores, plain ntris (no address-taken pn) 67->34 (969 vs 970 bytes); still differs: file/ntris registers swapped (original file=ebx, ntris=ebp; ours reversed; cnt copies, f aliases, countdown loop, volatile file no effect), lea esi/mov ecx,esi before jle, third add esi,2 one slot early; permute -r 40 found nothing better.
void ModelFile::ReadBatch(File* file, ModelOut* out)
{
    int mat = FUN_004af230(file);
    int nverts = FUN_004af230(file);
    int ntris = FUN_004af230(file);
    DynamicMesh* mesh = new DynamicMesh;
    BmfVertex* v = new BmfVertex[nverts];
    unsigned short* idx = (unsigned short*)operator new(ntris * 6);
    TmpN<11> tmp; unsigned int* r = tmp.r;
    int i; unsigned int* q;
    for (i = nverts, q = (unsigned int*)v + 5; i > 0; i--, q += 11) {
        file->Read(r, 0x2c);
        q[-5] = r[0]; q[-4] = r[1]; q[-3] = r[2]; q[-2] = r[3]; q[-1] = r[4]; q[0] = r[5];
        unsigned int c = r[6] | 0xff000000; q[1] = c; q[2] = r[7]; q[3] = r[8]; q[4] = r[7]; q[5] = r[8];
    }
    unsigned short* p = idx;
    for (i = 0; i < ntris; i++) {
        unsigned short t[4];
        file->Read(t, 8);
        *(volatile unsigned short*)p = t[0]; p++; *(volatile unsigned short*)p = t[1]; p++; *p = t[2]; p++;
        if ((*(volatile int*)&t[0] & 0xffff) >= nverts || (*(volatile int*)&t[1] & 0xffff) >= nverts || (*(volatile int*)&t[2] & 0xffff) >= nverts)
            throw file_error(FName(file), *(std::string*)&BStr("BMF batch chunk failed due to illegal vertex index"));
    }
    *(short*)((char*)mesh + 0xe) = ntris;
    *(short*)((char*)mesh + 0xc) = nverts;
    *(void**)((char*)mesh + 8) = idx;
    *(void**)((char*)mesh + 4) = v;
    *(void**)mesh = materials[mat];
    out->batches.push_back(mesh);
}

// NONMATCHING: RALLYTROPHY 0x4b1ec0 (inlined std::string/throw construction differs from original)
// Tried 2026-10-31: record temp sized to the record (TmpN<9>) fixed the frame (0xcc->0xc4, size now equal 963); vertex store through `unsigned int* d = (uint*)v + 5` with d[-5]..d[5] gives the original's esi+0x14 base; ~25 variants (tri-loop volatile/block-temp stores, d init placement, tail volatile mesh). Still differs: lea of d lands before the n<=0 guard, r[7]/r[8] register rotation, tri-loop load/store interleave, tail mesh-field reloads.
// Tried 2026-11-01 re-pass: ntris-as-counter / pn volatile forms (ReadBatch's 91->67 transfer: 55 insns but 964 bytes) no better; vertex word 6 via a named temp (`unsigned c = r[6]|0xff000000; q[1] = c;`) 57->6 and volatile-cast first two tri stores 6->4 (exact size 963); still differs: `mov ecx,esi / lea esi,[ecx+0x14]` and the idx reload land before the n<=0 jle, third `add esi,2` one slot early (tried q/p init placement, volatile v/idx reloads, *p++, p++ in for-increment: no effect).
void ModelFile::ReadBatchSmall(File* file, ModelOut* out)
{
    int mat = FUN_004af230(file);
    int nverts = FUN_004af230(file);
    int ntris = FUN_004af230(file);
    DynamicMesh* mesh = new DynamicMesh;
    BmfVertex* v = new BmfVertex[nverts];
    unsigned short* idx = (unsigned short*)operator new(ntris * 6);
    TmpN<9> tmp; unsigned int* r = tmp.r;
    int i; unsigned int* q;
    for (i = nverts, q = (unsigned int*)v + 5; i > 0; i--, q += 11) {
        file->Read(r, 0x24);
        q[-5] = r[0]; q[-4] = r[1]; q[-3] = r[2]; q[-2] = r[3]; q[-1] = r[4]; q[0] = r[5];
        unsigned int c = r[6] | 0xff000000; q[1] = c; q[2] = r[7]; q[3] = r[8]; q[4] = r[7]; q[5] = r[8];
    }
    unsigned short* p = idx;
    for (i = 0; i < ntris; i++) {
        unsigned short t[4];
        file->Read(t, 6);
        *(volatile unsigned short*)p = t[0]; p++; *(volatile unsigned short*)p = t[1]; p++; *p = t[2]; p++;
        if ((*(volatile int*)&t[0] & 0xffff) >= nverts || (*(volatile int*)&t[1] & 0xffff) >= nverts || (*(volatile int*)&t[2] & 0xffff) >= nverts)
            throw file_error(FName(file), *(std::string*)&BStr("BMF batch chunk failed due to illegal vertex index"));
    }
    *(short*)((char*)mesh + 0xe) = ntris;
    *(short*)((char*)mesh + 0xc) = nverts;
    *(void**)((char*)mesh + 8) = idx;
    *(void**)((char*)mesh + 4) = v;
    *(void**)mesh = materials[mat];
    out->batches.push_back(mesh);
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

// FUNCTION: RALLYTROPHY 0x4b2330
void ModelFile::ReadPolylist(File* file, ModelOut* out)
{
    int n = FUN_004af230(file);
    out->polyCount = n;
    int bytes = n << 4;
    unsigned int* p = (unsigned int*)operator new(bytes);
    out->polys = p;
    file->Read(p, bytes);
    int i = 0; unsigned int* q;
    if (i < n) for (q = p;;) {
        unsigned int k = *q;
        if (k >= materials.size()) {
            PStr msg("BMF polylist chunk failed: illegal material index");
            throw file_error(FName(file), *(std::string*)&msg);
        }
        *q = (unsigned int)materials[k];
        i++; q += 4; if (i >= n) break;
    }
}
