// Scene entry (0x4b83a0-0x4b8b10).
#include "unk_4b83a0.h"
#include <string.h>

// FUNCTION: RALLYTROPHY 0x4b83a0
void IntVec::Resize(unsigned int n, const int& x)
{
    if (size() < n)
        insert(end(), n - size(), x);
    else if (n < size())
        erase(begin() + n, end());
}

// NONMATCHING: RALLYTROPHY 0x4b8640
// reason: original loads the uninitialised allocator byte into dl twice; ours uses cl then dl
// Tried 2026-10-30 night: zeroing-statement orders, ml()/sets()/SetVec() init forms, explicit allocator args, plain std::vector sets; all 2-5 insns differ, ours hoists the sets allocator load into dl and uses cl for the meshes one.
// Tried 2026-10-31 re-pass: 16 more variants (all 5! store orders sampled, chained zero, explicit ml(), shared int z, volatile on verts/count40/pad50, dead volatile char local, placement-new sets) 2-6 insns differ; permute -r 40 best 2 insns, no MATCH; still the uninitialised allocator-byte load (dl before xor vs ours hoisted dl + cl).
SceneEntry::SceneEntry(): sets()
{
    count44 = 0;
    count40 = 0;
    pad50 = 0;
    verts = 0;
    data4c = 0;
}

// NONMATCHING: RALLYTROPHY 0x4b8680
// SYMBOL: ??4SceneEntry@@QAEAAV0@ABV0@@Z
// reason: draft of the deep-copy assignment
// Tried 2026-10-30 night: 366->53 insns differ (delete data4c, inline sets.size(), EntryMesh copy ctor + 0x38 size, unrolled head copy, memcpy, no sets push (the original leaks the copied sets, then calls LinkDeformVertices 0x4b9170 and returns its eax)); still differs: original keeps `this` in [esp+0x18] and reuses ebp for the new set, has an unused zero-init local at esp+0x14 (a dummy volatile local gave 46 but is unfaithful), and Erase arg load order.
// Tried 2026-11-01 re-pass: 53->7 insns differ (same size 871): Erase via local SetVec*/MeshVec* pointers (push order, no this-spill needed), sets guard as size()>0, volatile dummy int zz=0 stored before both Erase blocks (original has an unused zero store at esp+0x14); 45 variants + permute -r 40 (best 7). Remaining: m (EntryMesh* temp) takes slot esp+0x14 and the new[] temps esp+0x10 where the original has them swapped (zz/new temps at 0x14, m at 0x10), plus the two zz stores placed one slot off.
SceneEntry& SceneEntry::operator=(const SceneEntry& o)
{
    int i;
    delete[] verts;
    delete data4c;
    for (i = 0; (unsigned)i < ml.meshes.size(); i++)
        delete ml.meshes[i];
    for (i = 0; (unsigned)i < sets.size(); i++) {
        delete[] sets[i]->mats;
        delete sets[i];
    }
    int zz;
    *(volatile int*)&zz = 0;
    SetVec* sp = &sets;
        if (sp->size() > 0)
        sp->Erase(sp->begin(), sp->end());
    *(volatile int*)&zz = 0;
    MeshVec* mp = &ml.meshes;
    if (mp->Size() > 0)
        mp->Erase(mp->begin(), mp->end());
    for (i = 0; (unsigned)i < o.ml.meshes.size(); i++)
        {
        EntryMesh* m = new EntryMesh(*o.ml.meshes[i]);
        MeshVec* r = &ml.meshes;
        r->Ins(r->end(), 1, m);
    }
    head[0] = o.head[0]; head[1] = o.head[1]; head[2] = o.head[2]; head[3] = o.head[3];
    head[4] = o.head[4]; head[5] = o.head[5]; head[6] = o.head[6]; head[7] = o.head[7];
    head[8] = o.head[8]; head[9] = o.head[9]; head[10] = o.head[10];
    ml.flag2c = o.ml.flag2c;
    count40 = o.count40;
    count44 = o.count44;
    if (count40 || count44) {
        verts = new EntryVec3[count40];
        data4c = new float[count44 * 4];
        memcpy(verts, o.verts, count40 * sizeof(EntryVec3));
        memcpy(data4c, o.data4c, count44 * 16);
    }
    for (i = 0; (unsigned)i < o.sets.size(); i++) {
        EntryMatSet* os = o.sets[i];
        EntryMatSet* s = new EntryMatSet;
        s->count = os->count;
        s->extra = os->extra;
        s->mats = new EntryMat[s->count];
        memcpy(s->mats, os->mats, s->count * sizeof(EntryMat));
    }
    return LinkDeformVertices();
}

// FUNCTION: RALLYTROPHY 0x4b89f0
SceneEntry::~SceneEntry()
{
    unsigned i;
    for (i = 0; (unsigned)i < ml.meshes.size(); i++)
        delete ml.meshes[i];
    for (i = 0; (unsigned)i < sets.size(); i++) {
        delete[] sets[i]->mats;
        delete sets[i];
    }
    delete[] verts;
    delete data4c;
}
