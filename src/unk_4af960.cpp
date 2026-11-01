#include "unk_4af960.h"
#include "unk_4b8b10.h"
#include "unk_40c280.h"
#include <math.h>
#include <string.h>

extern int DAT_0057b95c;
struct BmfFileName { void* stream; std::string name; };
static __forceinline const std::string& Name(File* f) { return ((BmfFileName*)f)->name; }
#define WORD(p,n) (((unsigned int*)(p))[n])
#define FLOAT(p,n) (((float*)(p))[n])
struct BStr {
    std::allocator<char> alloc; char* ptr; unsigned int len; unsigned int res;
    void Tidy(bool b);
    bool Grow(unsigned int n, bool trim);
    void Eos(unsigned int n);
    BStr(const char* s, const std::allocator<char>& al = std::allocator<char>()) : alloc(al) {
        Tidy(false);
        unsigned int n = strlen(s);
        if (Grow(n, true)) { memcpy(ptr, s, n); Eos(n); }
    }
    ~BStr() { Tidy(true); }
};
#pragma comment(linker, "/alternatename:?Tidy@BStr@@QAEX_N@Z=?_Tidy@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEX_N@Z")
#pragma comment(linker, "/alternatename:?Eos@BStr@@QAEXI@Z=?_Eos@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAEXI@Z")
#pragma comment(linker, "/alternatename:?Grow@BStr@@QAE_NI_N@Z=?_Grow@?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@AAE_NI_N@Z")
static inline int Bal(int a) { return a; }
static __forceinline void Ins0(BStr* d, const std::string& x)
{
    unsigned int m = std::string::npos;
    unsigned int n = x.size();
    if (n < m) m = n;
    if (std::string::npos - d->len <= m) std::_Xlen();
    if (0 < m) {
        unsigned int k = d->len + m;
        if (d->Grow(k, false)) {
            memmove(d->ptr + m, d->ptr, d->len);
            memcpy(d->ptr, x.c_str(), m);
            d->Eos(k);
        }
    }
}
struct MatView { char pad[0xa4]; std::string name; };
static __forceinline void Set3(float* d, float x, float y, float z) { d[0] = x; d[1] = y; d[2] = z; }
static __forceinline void Set4(float* d, float x, float y, float z, float w) { d[0] = x; d[1] = y; d[2] = z; d[3] = w; }

// std::string built from a C string in one class: _Tidy (0x402700) and assign(s, n) (0x404ba0) as calls, the
// object's cleanup state set after the assign returns.
struct BmfDirString {
    BmfDirString(const char* s, const std::allocator<char>& a = std::allocator<char>()) : alloc(a)
    {
        Tidy(false);
        AssignText(s, strlen(s));
    }
    ~BmfDirString() { ((std::string*)this)->~basic_string(); }
    void Tidy(bool built);                       // 0x402700
    void AssignText(const char*, unsigned int);  // 0x404ba0
    void Append(unsigned int, char);             // 0x408d10
    std::string& Str() { return *(std::string*)this; }
    std::allocator<char> alloc;
    char* ptr;
    unsigned int size, capacity;
};
// NONMATCHING: RALLYTROPHY 0x4af950
// 1227/1227 bytes, 29 differ: the original schedules `push edx; mov ecx,ebp` and the cleanup-state 7 store of the
// path assign between the b and c zero stores; every statement order and a call-after-stores form were tried.
// Tried 2026-10-30 night: volatile barriers on a/b/c stores, hoisted strlen local, enabled-before-c, permute -r 20; stuck at 3 insns (push edx/mov ecx,ebp/state-7 store land before the a stores instead of between b and c).
// Tried 2026-10-31 re-pass: volatile on single b/a/c stores (field cast, volatile Vector3*, volatile float* run) -> 6-10 insns worse; strlen hoisted to local before a / after b (3, unchanged), AssignText moved after c or before c (4-7 worse); permute -r 40 best stays 3 insns; original pushes strlen then does a,b stores, then push dir/state-7/c stores, which no statement order reproduces.
BmfReader::BmfReader(const char* filename, const char* dir, BmfTextures* cache, List<Sprite*>* mats, int flags)
{
    DAT_0057b95c = flags;
    a.x = a.y = a.z = 0;
    b.x = b.y = b.z = 0;
    c.x = c.y = c.z = 0;
    enabled = true;
    path.AssignText(dir, strlen(dir));
    textures = cache;
    materials = mats;
    File file(filename, 0x81);
    BmfDirString directory(dir);
    if (!directory.Str().empty() && directory.Str()[directory.Str().size()-1] != '/')
        directory.Append(1, '/');
    ModelNamedValue* chunk = FUN_004af560(&file);
    BmfTag tag(*chunk);
    if ((*(std::string*)&tag) != "MAIN") {
        std::string message("BMF main chunk loading failed");
        throw file_error(Name(&file), message);
    }

    while ((unsigned int)FUN_004a5fc0(&file) < tag.value) {
        chunk = FUN_004af560(&file);
        if (chunk->name == "MATERIALLIST")
            ((ModelFile*)this)->ReadMaterialList(&file);
        else if (chunk->name == "HIERARCHY")
            ((ModelFile*)this)->ReadHierarchy(&file);
        else if (chunk->name == "TRACK")
            ((ModelFile*)this)->ReadTrack(&file);
        else
            FUN_004af710(&file, (ModelChunk*)chunk);
    }
}

// FUNCTION: RALLYTROPHY 0x4afec0
void ModelFile::ReadMaterialList(File* file)
{
    int count = FUN_004af230(file);
    BmfPointerVector& refs = *(BmfPointerVector*)&m_v18;
    refs.reserve(count);
    while (count-- > 0) {
        ModelNamedValue* chunk = FUN_004af560(file);
        if (chunk->name != "MATERIAL") {
            std::string message("BMF Material list chunk read failed");
            throw file_error(Name(file), message);
        }
        void* material = ((BmfMaterialMethods*)this)->ReadMaterial(file);
        refs.Insert(refs.end(), 1, material);
    }
}

// NONMATCHING: RALLYTROPHY 0x4b0180
// Static string setup, material color copies, and texture path insertion code generation differ.
// Tried 2026-10-31: 231->46 insns (size 1628 vs 1626): Vector3 c0..c3 locals before the statics (empty dtors give EH state 3/4), Set3/Set4 forceinline row copies (fld z,y,x / fstp x,y,z), Bal(1) x5 at end, Assign args via a pointer local (call before npos load), MatView for the node->name compare, Ins0 hand-written insert with out-of-line BStr::Eos/Grow; still differs: stack slots of the five FUN_004af230 results (orig b,c,d,e ascending 0x10..0x1c, ours permuted; declaration order and q[4] array did not move them), `mov [ebp+0x7c]/0x6c/0x5c,ebx` store placement, `textures` null test form, new-Node else-xor.
// Tried 2026-11-01 re-pass: 46->28 (size 1625 vs 1626): stores into result ordered a,b,c,d,e (stack slots of the FUN_004af230 results follow first-use order), duplicate dead WORD(0x1f)=0 removed, WORD 0x1b/0x17 zero stores placed after their row copies, volatile read of f; 20+ variants for the `textures` null test (locals, volatile, casts, !x else, self ptr) and permute -r 40 all keep `cmp [esi],0` where the original has `mov eax,[esi]; test eax,eax`, which is the only real insn left.
// Tried 2026-11-01 third pass: 30 more null-test forms (named/for/switch/goto/ternary/!!/compare-to-bool, file-scope inline and forceinline accessors, local-class accessors which VC6 will not inline), the whole texture block as a forceinline helper with an early return (inlines but costs the Prefix/assign inline budget, 130+ insns at any Bal count), Ins0 c_str via a local, permute -r 20; every in-place form folds to `cmp [esi],0`. The original also uses eax for the `this` reload inside the Ins0 c_str (`mov eax,[esp+0x1c]; mov eax,[eax+0xc]`, ours ecx), so both look like one register-allocation difference, not a source form of the test.
Sprite* BmfReader::ReadMaterial(File* file)
{
    int a = FUN_004af230(file);
    int b = FUN_004af230(file);
    int c = FUN_004af230(file);
    int d = FUN_004af230(file);
    int e = FUN_004af230(file);
    Vector3 c0, c1, c2, c3;
    static std::string name;
    static BmfPath texture;
    static BmfPath texture2;
    static BmfPath texture3;
    float power;
    int f, g, h, i, j;
    file->Read(&c0, 12);
    file->Read(&c1, 12);
    file->Read(&c2, 12);
    file->Read(&c3, 12);
    file->Read(&power, 4);
    file->Read(&f, 4);
    file->Read(&g, 4);
    file->Read(&h, 4);
    file->Read(&i, 4);
    file->Read(&j, 4);
    const std::string* r;
    r = FUN_004af270(file); ((ModelRecordString*)&name)->Assign(*r, 0, std::string::npos);
    r = FUN_004af270(file); ((ModelRecordString*)&texture)->Assign(*r, 0, std::string::npos);
    r = FUN_004af270(file); ((ModelRecordString*)&texture2)->Assign(*r, 0, std::string::npos);
    r = FUN_004af270(file); ((ModelRecordString*)&texture3)->Assign(*r, 0, std::string::npos);
    for (List<Sprite*>::Node* scan = materials->m_head; scan; scan = scan->next)
    {
        Sprite* sp = scan->data;
        if (name == ((MatView*)sp)->name)
            return sp;
    }
    Sprite* result = new Sprite;
    WORD(result,4) = a;
    WORD(result,5) = b;
    WORD(result,6) = c;
    WORD(result,7) = d;
    WORD(result,3) = e;
    Set3(&FLOAT(result,0x1c), c0.x, c0.y, c0.z);
    WORD(result,0x1f) = 0;
    Set4(&FLOAT(result,0x10), c1.x, c1.y, c1.z, *(float*)&h);
    Set3(&FLOAT(result,0x18), c2.x, c2.y, c2.z);
    WORD(result,0x1b) = 0;
    Set3(&FLOAT(result,0x14), c3.x, c3.y, c3.z);
    WORD(result,0x17) = 0;
    FLOAT(result,0x20) = power * 100.0f;
    if (d != 1) {
        WORD(result,9) = 2;
        WORD(result,12) = 3;
    }
    WORD(result,0x2e) = *(volatile int*)&f;
    WORD(result,0x2f) = g;
    if (FLOAT(result,0x20) < 0.99f) WORD(result,0x20) = 0;
    WORD(result,0x27) = i;
    WORD(result,0x2d) = j;
    *(std::string*)((char*)result + 0xa4) = name.c_str();
    if (textures) {
        if (texture != "") {
            texture.Prefix(0, path.Str(), 0, std::string::npos);
            WORD(result,0) = (unsigned int)textures->Load(texture.c_str(), 0x40);
        }
        if (texture2 != "") {
            texture2.Prefix(0, path.Str(), 0, std::string::npos);
            WORD(result,1) = (unsigned int)textures->Load(texture2.c_str(), 0x40);
        }
        if ((DAT_0057b95c & 1) && texture3 != "") {
            Ins0((BStr*)&texture3, path.Str());
            WORD(result,2) = (unsigned int)textures->LoadSpecial(texture3.c_str(), 0x40);
        }
    }
    List<Sprite*>::Node* node = materials->m_free;
    if (!node) {
        node = new List<Sprite*>::Node;
        if (node) { node->prev = 0; node->next = 0; node->data = 0; }
        else node = 0;
    } else {
        materials->m_free = node->next;
        --materials->m_freeCount;
    }
    List<Sprite*>::Node* tail = materials->m_tail;
    node->data = result;
    node->prev = tail;
    node->next = 0;
    if (tail) tail->next = node;
    else materials->m_head = node;
    materials->m_tail = node;
    ++materials->m_count;
    Bal(1); Bal(1); Bal(1);
    return result;
}

struct BlkVec : StageIntVec { unsigned int SizeCall(); }; // 0x414460 out of line
void __cdecl StageConstruct(int* p, const int& v); // 0x403140
static __forceinline void BlkInsert(BlkVec* v, int* P, unsigned int M, const int& X)
{
    if ((unsigned int)(v->end - v->last) < M) {
        unsigned int m2 = (M < v->Size() ? v->Size() : M);
        unsigned int n = v->SizeCall() + m2;
        int* s = v->allocator.allocate(n, (void*)0);
        int* q = v->Ucopy(v->first, P, s);
        v->Ufill(q, M, &X);
        v->Ucopy(P, v->last, q + M);
        v->Destroy(v->first, v->last);
        StageFree(v->first);
        v->end = s + n;
        v->last = s + v->SizeCall() + M;
        v->first = s;
    } else if ((unsigned int)(v->last - P) < M) {
        v->Ucopy(P, v->last, P + M);
        v->Ufill(v->last, M - (v->last - P), &X);
        for (int* p = P; p != v->last; ++p)
            *p = X;
        v->last += M;
    } else {
        v->Ucopy(v->last - M, v->last, v->last);
        int* d = v->last;
        for (int* f = v->last - M; P != f; )
            *--d = *--f;
        for (int* p = P; p != P + M; ++p)
            *p = X;
        v->last += M;
    }
}

// FUNCTION: RALLYTROPHY 0x4b0a30
// m_v38.push_back is an inlined vector::insert(end(), 1, x) with out-of-line Size() (BlkInsert, see StageInsert).
void ModelFile::ReadBlock(File* file, unsigned int* size)
{
    SceneEntry* entry = ((ModelLoader*)this)->ReadMesh(file, (unsigned int)size);
    size = (unsigned int*)new BmfObject;
    ((BmfObject*)size)->name = "block";
    ((BmfObject*)size)->meshes.Insert(entry, 0.0f);
    unsigned int* dest = (unsigned int*)&((BmfObject*)size)->boundsA;
    unsigned int* src = (unsigned int*)entry;
    unsigned int u = src[1];
    dest[0] = src[0];
    unsigned int v = src[2]; dest[1] = u;
    u = src[3]; dest[2] = v;
    v = src[4]; dest[3] = u;
    u = src[5]; dest[4] = v;
    v = src[6]; dest[5] = u;
    u = src[7]; dest[6] = v;
    v = src[8]; dest[7] = u;
    u = src[9]; dest[8] = v;
    v = src[10]; dest[9] = u;
    unsigned char flags = *(unsigned char*)&src[11];
    dest[10] = v;
    *(unsigned char*)&dest[11] = flags;
    delete entry;
    BlkVec* vec = (BlkVec*)&m_v38;
    BlkInsert(vec, vec->last, 1, (int&)size);
}

// NONMATCHING: RALLYTROPHY 0x4b0e90
// Tried 2026-10-30 night: 220->7 insns (size equal): thiscall-style fixes, Bal(1) x5 at end, throw-first `!= "MODEL"` with BStr (+Eos) message, `double ls = sqrt(..); scale.x = (float)ls;` per row, row 0 via `float* m` and rows 1,2 via object->local.m, record->value passed directly to Attach, int pair stores lo-first; still differs: the first position copy lacks its `mov [ebp+0x18],edx` before rep movs (VC6 defers it), shifting 5 later stores; volatile on every copy store and int/float position forms changed the inline budget or got worse.
// Tried 2026-11-01 re-pass: 25 more variants of the first position copy (int/float field-wise, block temps, per-store volatile on x/y/z, reordered stores, moved after the matrix copy, volatile record, hoisted flags temp) all worse (71-175 insns) or unchanged at 7; permute -r 40 stayed at 7; still differs: y store [ebp+0x18] of the first position copy deferred past rep movs.
void ModelFile::ReadMesh(File* file)
{
    ModelPartRecord* record = FUN_004af730(file);
    FUN_004af230(file);
    BmfObject* object = new BmfObject;
    ((ModelRecordString*)&object->name)->Assign(*(std::string*)record, 0, std::string::npos);
    object->flags |= (unsigned char)FUN_004af890(*(std::string*)record);
    unsigned int* w = (unsigned int*)object;
    object->position = *(ScenePoint*)((char*)record + 0x54);
    *(Matrix*)&object->local = record->transform;
    w[0x14] = w[0x15] = w[0x16] = 0;
    object->position = *(ScenePoint*)((char*)record + 0x54);
    w[0x28] = WORD(record,0x20); w[0x29] = WORD(record,0x21);
    w[0x2a] = WORD(record,0x22); w[0x2b] = WORD(record,0x23);
    w[0x2c] = WORD(record,0x24); w[0x2d] = WORD(record,0x25);
    w[0x2e] = WORD(record,0x1c); w[0x2f] = WORD(record,0x1d);
    w[0x30] = WORD(record,0x1e); w[0x31] = WORD(record,0x1f);
    if (record->flags & 1) object->boundsA.flags |= 1;
    if (record->flags & 2) object->boundsA.flags |= 2;
    EntryVec3 scale;
    float* m = object->local.m;
    double ls = sqrt(m[2]*m[2] + m[1]*m[1] + m[0]*m[0]); scale.x = (float)ls;
    double ls1 = sqrt(object->local.m[6]*object->local.m[6] + object->local.m[5]*object->local.m[5] + object->local.m[4]*object->local.m[4]); scale.y = (float)ls1;
    double ls2 = sqrt(object->local.m[10]*object->local.m[10] + object->local.m[9]*object->local.m[9] + object->local.m[8]*object->local.m[8]); scale.z = (float)ls2;
    float k = 1.0f / (float)sqrt(m[2]*m[2] + m[1]*m[1] + m[0]*m[0]);
    m[0] = k*m[0]; m[1] = k*m[1]; m[2] = k*m[2];
    k = 1.0f / (float)sqrt(object->local.m[6]*object->local.m[6] + object->local.m[5]*object->local.m[5] + object->local.m[4]*object->local.m[4]);
    object->local.m[4] = k*object->local.m[4]; object->local.m[5] = k*object->local.m[5]; object->local.m[6] = k*object->local.m[6];
    k = 1.0f / (float)sqrt(object->local.m[10]*object->local.m[10] + object->local.m[9]*object->local.m[9] + object->local.m[8]*object->local.m[8]);
    object->local.m[8] = k*object->local.m[8]; object->local.m[9] = k*object->local.m[9]; object->local.m[10] = k*object->local.m[10];
    ((ModelLoader*)this)->Attach(object, record->value);
    int start = FUN_004a5fc0(file);
    ModelNamedValue* chunk = FUN_004af560(file);
    BmfTag tag(*chunk);
    if ((*(std::string*)&tag) != "MODEL") {
        BStr message("BMF mesh chunk read failed");
        throw file_error(Name(file), *(std::string*)&message);
    }
    tag.value += start - FUN_004a5fc0(file);
    SceneEntry* entry = ((ModelLoader*)this)->ReadMesh(file, tag.value);
    ((MeshModel*)entry)->Translate((float*)((char*)record + 0x60));
    ((MeshModel*)entry)->Scale(&scale.x);
    if (!(object->boundsA.flags & 3)) ((MeshModel*)entry)->ComputeBoundsAndSphere();
    object->meshes.Insert(entry, 0.0f);
    delete entry;
    Bal(1); Bal(1); Bal(1); Bal(1); Bal(1);
}
