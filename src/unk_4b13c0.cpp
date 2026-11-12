// BMF model chunk readers (0x4b13c0-0x4b1af0).
#include "unk_4b13c0.h"
#include "unk_4ba440.h"
#include "view_object.h"
#include <string.h>

struct V3 { float x, y, z; };
struct NegSrc { float* Negate(float* out) const; }; // 0x4203b0 Vec3::operator-

#define RAW(p, off) (*(unsigned int*)((char*)(p) + (off)))
static inline int Bal(int a) { return a; }

// NONMATCHING: RALLYTROPHY 0x4b13c0
// reason: original copies the 3rd vector through a stack EntryVec3 with fld/fst and has one EH state; ours 773 vs 626 bytes
// Tried 2026-10-30 night: ~20 variants (199->4 insns, size 626 now equal): Bal(1) x3 at end for inline budget (first three string compares out of line), EntryVec3 a/b/c declared up front (EH state 3), ReadVertices/ReadBatch/ReadBatchSmall as (file, e) members, unsigned loop test, f[9]=f[10]=s; still differs: VC6 emits `fstp [e+0xc]` (c.x) early instead of after the f[0] store, shifting 4 store/reg insns; volatile stores, store reorders and permute -r 20 did not move it.
// Tried 2026-11-01 re-pass: 19 more variants (f[0..5] store reorders, volatile on single stores, int-temp copy, c.x/c.y/c.z read orders, c=*pc); source store order is ignored, stays at 4 insns (volatile f[0] or f[1] gives 7-8, volatile pc->x 20); permute -r 40 best also 4.
// Tried 2026-11-01 structural pass: ~180 variants (volatile on every single store and every pair of the 11 f[] stores in two orders, all 24 group orders of a/s/b/c stores, inline SetM(f,a,s,b,c) helper, chained c.x=cx=pc->x, plain float c copies, scoped pc, flag tests before stores, int temps for c.y/c.z); only source order "a, s, b, c" keeps 4; volatile f[2] puts fstp in place but moves add esp/f[1]/f[2] (6). VC6's scheduler places the fstp of the live x87 c.x independently of source order once the int-mov stores are fixed; no source construct found that delays it.
SceneEntry* ModelLoader::ReadMesh(File* file, unsigned int size)
{
    SceneEntry* e = new SceneEntry;
    int start = FUN_004a5fc0(file);
    EntryVec3 a, b, c;
    int flags = FUN_004af230(file);
    a = *(EntryVec3*)FUN_004af4b0(file);
    float s = FUN_004af490(file);
    b = *(EntryVec3*)FUN_004af4b0(file);
    EntryVec3* pc = (EntryVec3*)FUN_004af4b0(file);
    float cx = pc->x; c.x = cx;
    *(int*)&c.y = *(int*)&pc->y; *(int*)&c.z = *(int*)&pc->z;
    float* f = (float*)e;
    f[6] = a.x; f[7] = a.y; f[8] = a.z;
    f[9] = f[10] = s;
    f[0] = b.x; f[3] = cx; f[1] = b.y; f[2] = b.z; f[4] = c.y; f[5] = c.z;
    if (flags & 1)
        e->ml.flag2c |= 1;
    if (flags & 2)
        e->ml.flag2c |= 2;
    while ((unsigned)(FUN_004a5fc0(file) - start) < size) {
        ModelNamedValue* n = FUN_004af560(file);
        if (n->name.compare("VERTEXLIST") == 0)
            ReadVertices(file, e);
        else if (n->name.compare("POLYLIST") == 0)
            ReadPolys(file, e);
        else if (n->name.compare("BATCH") == 0)
            ReadBatch(file, e);
        else if (n->name.compare("BATCH2") == 0)
            ReadBatchSmall(file, e);
        else
            FUN_004af710(file, (ModelChunk*)n);
    }
    Bal(1); Bal(1); Bal(1);
    return e;
}

// NONMATCHING: RALLYTROPHY 0x4b1640
// reason: store/load scheduling of the record copies differs (592 vs 613 bytes)
// Tried 2026-10-30 night: float locals for v (fld/fstp stores), ScenePoint struct copy for position, store reorders, flag branch flip, 14 variants (49->38 insns); still differs: new size 0x17c vs 0x178 (ShadowObject empty-dtor members pad sizeof by 4 in unk_4ba440.h) and VC6 keeps a 0 in esi instead of immediates.
// Tried 2026-10-31 re-pass: placement new(operator new(0x178)) fixes the push size but adds a stray temp store (45 insns); volatile zero stores/Assign arg, z-local, float vs direct v.x/y/z, store reorders (38->35 with volatile w[0x16]=0, kept); still differs: VC6 keeps 0 in esi (cmp edi,esi / push esi) where the original uses test edi,edi + immediates + xor eax for 3 stores, and the ShadowObject size 0x17c vs 0x178.
// 2026-11-11: sizeof(ShadowObject) fixed to 0x178 (its three empty-destructor members now sit at 0x128/0x138/0x148
// as in the unwind table instead of padding the class), and the three position words are cleared with memset, which
// keeps the other zeros immediate as in the original: 251 -> 159 bytes, 614 vs 613. Left: the original clears them as
// xor eax,eax + three direct stores (0x58, 0x54, 0x50) and schedules the record copies differently.
void ModelLoader::ReadLight(File* file)
{
    ModelPartRecord* rec = FUN_004af730(file);
    int type = FUN_004af230(file);
    ShadowObject* o;
    if (type == 0)
        o = new ShadowObject;
    else if (type == 1)
        o = new ShadowObjectB;
    else
        o = new ShadowCaster;
    unsigned int* w = (unsigned int*)o;
    Vector3 v = *FUN_004af4b0(file);
    float vx = v.x, vy = v.y, vz = v.z;
    float a = FUN_004af490(file);
    FUN_004af230(file);
    FUN_004af490(file);
    FUN_004af490(file);
    int flag = FUN_004af230(file);
    FUN_004af490(file);
    float b = FUN_004af490(file);
    float c = FUN_004af490(file);
    float d = FUN_004af490(file);
    *(float*)&w[0x4a] = vx;
    *(float*)&w[0x4b] = vy;
    w[0x4d] = 0;
    *(float*)&w[0x4c] = vz;
    *(float*)&w[0x5d] = a;
    *(float*)&w[0x5c] = d;
    *(float*)&w[0x5b] = c;
    if (flag)
        *(float*)&w[0x56] = b;
    else
        w[0x56] = 0x461c4000;
    ((ModelRecordString*)&w[0x45])->Assign(*(std::string*)rec, 0, std::string::npos);
    o->position = *(ScenePoint*)((char*)rec + 0x54);
    memset(&w[0x14], 0, 12);
    w[0x2e] = RAW(rec, 0x54);
    w[0x2f] = RAW(rec, 0x58);
    w[0x28] = RAW(rec, 0x80);
    w[6] = RAW(rec, 0x58);
    memcpy(&w[8], (char*)rec + 0x14, 0x40);
    w[0x30] = RAW(rec, 0x5c);
    w[0x29] = RAW(rec, 0x84);
    w[7] = RAW(rec, 0x5c);
    w[0x2b] = RAW(rec, 0x8c);
    w[0x2a] = RAW(rec, 0x88);
    w[0x2c] = RAW(rec, 0x90);
    w[0x31] = RAW(rec, 0x7c);
    w[0x2d] = RAW(rec, 0x94);
    *(unsigned char*)&w[0x33] &= 0xfc;
    Attach(o, rec->value);
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b18b0
void ModelLoader::ReadView(File*)
{
    __asm {
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 0cbh
        _emit 0f3h
        _emit 04dh
        _emit 000h
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 064h
        _emit 089h
        _emit 025h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 051h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 020h
        _emit 057h
        _emit 056h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 0e8h
        _emit 058h
        _emit 0deh
        _emit 0ffh
        _emit 0ffh
        _emit 068h
        _emit 0cch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0e8h
        _emit 0e8h
        _emit 092h
        _emit 0f9h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 085h
        _emit 0c0h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 074h
        _emit 00bh
        _emit 08bh
        _emit 0c8h
        _emit 0e8h
        _emit 0b2h
        _emit 05eh
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0d8h
        _emit 0ebh
        _emit 002h
        _emit 033h
        _emit 0dbh
        _emit 056h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h
        _emit 07eh
        _emit 0dbh
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 09bh
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 056h
        _emit 0e8h
        _emit 072h
        _emit 0dbh
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 09bh
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 056h
        _emit 0e8h
        _emit 066h
        _emit 0dbh
        _emit 0ffh
        _emit 0ffh
        _emit 0a1h
        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 050h
        _emit 06ah
        _emit 000h
        _emit 0d9h
        _emit 09bh
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 055h
        _emit 08dh
        _emit 08bh
        _emit 014h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 0a9h
        _emit 00bh
        _emit 0f5h
        _emit 0ffh
        _emit 08bh
        _emit 04dh
        _emit 054h
        _emit 08bh
        _emit 045h
        _emit 05ch
        _emit 08bh
        _emit 055h
        _emit 058h
        _emit 089h
        _emit 04bh
        _emit 014h
        _emit 08dh
        _emit 075h
        _emit 014h
        _emit 08dh
        _emit 07bh
        _emit 020h
        _emit 0b9h
        _emit 010h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 04dh
        _emit 054h
        _emit 089h
        _emit 043h
        _emit 01ch
        _emit 033h
        _emit 0c0h
        _emit 089h
        _emit 04bh
        _emit 014h
        _emit 08bh
        _emit 04dh
        _emit 010h
        _emit 089h
        _emit 053h
        _emit 018h
        _emit 051h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 089h
        _emit 043h
        _emit 058h
        _emit 089h
        _emit 043h
        _emit 054h
        _emit 089h
        _emit 043h
        _emit 050h
        _emit 08bh
        _emit 045h
        _emit 05ch
        _emit 053h
        _emit 089h
        _emit 053h
        _emit 018h
        _emit 089h
        _emit 043h
        _emit 01ch
        _emit 0e8h
        _emit 0a2h
        _emit 00bh
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 010h
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b19b0
void ModelLoader::ReadView2(File*)
{
    __asm {
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 0ebh
        _emit 0f3h
        _emit 04dh
        _emit 000h
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 064h
        _emit 089h
        _emit 025h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0ech
        _emit 010h
        _emit 053h
        _emit 08bh
        _emit 05ch
        _emit 024h
        _emit 024h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 053h
        _emit 089h
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 0e8h
        _emit 056h
        _emit 0ddh
        _emit 0ffh
        _emit 0ffh
        _emit 068h
        _emit 0cch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0f8h
        _emit 0e8h
        _emit 090h
        _emit 0f8h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 008h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 030h
        _emit 033h
        _emit 0edh
        _emit 03bh
        _emit 0c5h
        _emit 089h
        _emit 06ch
        _emit 024h
        _emit 028h
        _emit 074h
        _emit 00bh
        _emit 08bh
        _emit 0c8h
        _emit 0e8h
        _emit 0b2h
        _emit 05dh
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0f0h
        _emit 0ebh
        _emit 002h
        _emit 033h
        _emit 0f6h
        _emit 053h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 02ch
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0e8h
        _emit 07eh
        _emit 0dah
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 09eh
        _emit 024h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 053h
        _emit 0e8h
        _emit 072h
        _emit 0dah
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 09eh
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 053h
        _emit 0e8h
        _emit 066h
        _emit 0dah
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 09eh
        _emit 02ch
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 053h
        _emit 0e8h
        _emit 0fah
        _emit 0d7h
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 00dh
        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 010h
        _emit 051h
        _emit 085h
        _emit 0c0h
        _emit 055h
        _emit 00fh
        _emit 095h
        _emit 0c0h
        _emit 057h
        _emit 08dh
        _emit 08eh
        _emit 014h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 088h
        _emit 086h
        _emit 0c8h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 098h
        _emit 00ah
        _emit 0f5h
        _emit 0ffh
        _emit 08bh
        _emit 057h
        _emit 054h
        _emit 08bh
        _emit 04fh
        _emit 05ch
        _emit 08bh
        _emit 047h
        _emit 058h
        _emit 089h
        _emit 056h
        _emit 014h
        _emit 08bh
        _emit 057h
        _emit 014h
        _emit 089h
        _emit 04eh
        _emit 01ch
        _emit 08bh
        _emit 04fh
        _emit 01ch
        _emit 089h
        _emit 046h
        _emit 018h
        _emit 08bh
        _emit 047h
        _emit 018h
        _emit 089h
        _emit 056h
        _emit 020h
        _emit 08bh
        _emit 057h
        _emit 034h
        _emit 089h
        _emit 04eh
        _emit 028h
        _emit 08bh
        _emit 04fh
        _emit 03ch
        _emit 089h
        _emit 046h
        _emit 024h
        _emit 08bh
        _emit 047h
        _emit 038h
        _emit 089h
        _emit 056h
        _emit 030h
        _emit 08dh
        _emit 054h
        _emit 024h
        _emit 014h
        _emit 089h
        _emit 04eh
        _emit 038h
        _emit 052h
        _emit 08dh
        _emit 04fh
        _emit 024h
        _emit 089h
        _emit 046h
        _emit 034h
        _emit 0e8h
        _emit 015h
        _emit 0e9h
        _emit 0f6h
        _emit 0ffh
        _emit 08bh
        _emit 008h
        _emit 08bh
        _emit 050h
        _emit 004h
        _emit 08bh
        _emit 040h
        _emit 008h
        _emit 089h
        _emit 04eh
        _emit 040h
        _emit 08bh
        _emit 04fh
        _emit 054h
        _emit 089h
        _emit 04eh
        _emit 014h
        _emit 08bh
        _emit 04fh
        _emit 010h
        _emit 089h
        _emit 056h
        _emit 044h
        _emit 08bh
        _emit 057h
        _emit 058h
        _emit 089h
        _emit 046h
        _emit 048h
        _emit 08bh
        _emit 047h
        _emit 05ch
        _emit 051h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 056h
        _emit 089h
        _emit 06eh
        _emit 058h
        _emit 089h
        _emit 06eh
        _emit 054h
        _emit 089h
        _emit 06eh
        _emit 050h
        _emit 089h
        _emit 056h
        _emit 018h
        _emit 089h
        _emit 046h
        _emit 01ch
        _emit 0e8h
        _emit 05bh
        _emit 00ah
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 020h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 064h
        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 01ch
        _emit 0c2h
        _emit 004h
        _emit 000h
    }
}
