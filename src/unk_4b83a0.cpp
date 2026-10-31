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
SceneEntry::SceneEntry() : sets()
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
    unsigned n = sets.size();
    if (n)
        sets.Erase(sets.begin(), sets.end());
    if (ml.meshes.Size() > 0)
        ml.meshes.Erase(ml.meshes.begin(), ml.meshes.end());
    for (i = 0; (unsigned)i < o.ml.meshes.size(); i++)
        {
        EntryMesh* m = new EntryMesh(*o.ml.meshes[i]);
        ml.meshes.Ins(ml.meshes.end(), 1, m);
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
