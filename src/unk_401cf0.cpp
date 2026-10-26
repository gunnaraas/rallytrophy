// Mesh-set loader (0x401cf0).
#include <string>
#include <stdexcept>
#include <string.h>
#include "unk_401cf0.h"

// NONMATCHING: RALLYTROPHY 0x401cf0 (register allocation and throw-path layout differ: 1184 vs 1213 bytes)
void MeshSetLoader::Load(MSModelFile* file)
{
    frames = file->parts.size();
    if (frames != 0) {
        MSPart* first = new MSPart(file->parts[0]);
        int* pv = (int*)first->pivot;
        int px = pv[0];
        int py = pv[1];
        ((int*)tint.f)[0] = px;
        ((int*)tint.f)[2] = pv[2];
        owner = (Owner*)first;
        ((int*)tint.f)[1] = py;
        MSSubList* sl = first->meshes.At(0);
        unsigned i;
        for (i = 0; i < sl->subs.size(); i++)
            count += sl->subs[i]->n;
        MeshElem* el = new MeshElem[count];
        elems = el;
        if (count > 0) {
            MeshElem* q = el;
            int n = count;
            int fr = frames;
            do {
                q->a = (MeshVec*)new MV[fr];
                q->b = (MeshVec*)new MV[fr];
                q++;
                n--;
            } while (n != 0);
        }
        MeshElem* e = el;
        for (i = 0; i < sl->subs.size(); i++) {
            MSVert* v = sl->subs[i]->verts;
            int m = sl->subs[i]->n;
            for (; m > 0; m--) {
                e->dst = (float*)v;
                e++;
                v++;
            }
        }
        int nf = frames;
        void** pp = file->parts.begin();
        for (int k = 0; k < nf; k++) {
            MSPart* p = (MSPart*)pp[k];
            const char* name = p->name;
            if (name == 0)
                name = "";
            if (strnicmp(name, "frame", 5) == 0 && strlen(name) >= 7) {
                int idx = name[5] * 10 + name[6] - 0x210;
                if (idx < 0 || idx >= nf) {
                    std::string msg("Invalid frame '");
                    msg += name;
                    msg += "'";
                    throw std::logic_error(msg);
                }
                MSSubList* s = p->meshes.At(0);
                MeshElem* d = el;
                for (i = 0; i < s->subs.size(); i++) {
                    float* v = (float*)s->subs[i]->verts;
                    int m = s->subs[i]->n;
                    if (m > 0)
                        do {
                            MeshVec* a = &d->a[idx];
                            a->f[0] = v[0];
                            a->f[1] = v[1];
                            a->f[2] = v[2];
                            MeshVec* b = &d->b[idx];
                            b->f[0] = v[3];
                            b->f[1] = v[4];
                            b->f[2] = v[5];
                            d++;
                            v += 11;
                        } while (--m != 0);
                }
            }
        }
    }
}
