// Scene entry (0x4b83a0-0x4b8b10).
#include "unk_4b83a0.h"

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
SceneEntry& SceneEntry::operator=(const SceneEntry& o)
{
    int i, j;
    delete[] verts;
    for (i = 0; (unsigned)i < ml.meshes.size(); i++)
        delete ml.meshes[i];
    for (i = 0; (unsigned)i < sets.size(); i++) {
        delete[] sets[i]->mats;
        delete sets[i];
    }
    if (sets.Size())
        sets.Erase(sets.begin(), sets.end());
    if (ml.meshes.Size())
        ml.meshes.Erase(ml.meshes.begin(), ml.meshes.end());
    for (i = 0; (unsigned)i < o.ml.meshes.size(); i++)
        {
        EntryMesh* m = new EntryMesh(*o.ml.meshes[i]);
        ml.meshes.Ins(ml.meshes.end(), 1, m);
    }
    for (i = 0; i < 11; i++)
        head[i] = o.head[i];
    ml.flag2c = o.ml.flag2c;
    count40 = o.count40;
    count44 = o.count44;
    if (count40 || count44) {
        verts = new EntryVec3[count40];
        data4c = new float[count44 * 4];
        for (i = 0; i < count40; i++)
            verts[i] = o.verts[i];
        for (i = 0; i < count44 * 4; i++)
            data4c[i] = o.data4c[i];
    }
    for (i = 0; (unsigned)i < o.sets.size(); i++) {
        EntryMatSet* s = new EntryMatSet;
        s->count = o.sets[i]->count;
        s->extra = o.sets[i]->extra;
        s->mats = new EntryMat[s->count];
        for (j = 0; j < s->count; j++)
            s->mats[j] = o.sets[i]->mats[j];
        sets.push_back(s);
    }
    return *this;
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
