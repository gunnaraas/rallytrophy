// BMF model chunk readers: batch (0x2c / 0x24 byte vertices), vertex array and polylist.
#include <string>
#include <stdexcept>
#include "unk_4b1af0.h"
#include "unk_476490.h"

// Layout view of File: stream pointer then name.
struct FileNameView { void* stream; std::string name; };
static const std::string& FName(File* f) { return ((FileNameView*)f)->name; }

#define BMF_BATCH_BODY(REC, TRI)                                                           \
    int mat = FUN_004af230(file);                                                          \
    int nverts = FUN_004af230(file);                                                       \
    int ntris = FUN_004af230(file);                                                        \
    DynamicMesh* mesh = new DynamicMesh;                                                   \
    BmfVertex* v = new BmfVertex[nverts];                                                  \
    unsigned short* idx = (unsigned short*)operator new(ntris * 6);                        \
    unsigned int* d = (unsigned int*)v;                                                    \
    int i;                                                                                 \
    for (i = nverts; i > 0; i--) {                                                     \
        unsigned int r[11];                                                                \
        file->Read(r, REC);                                                                \
        d[0] = r[0]; d[1] = r[1]; d[2] = r[2]; d[3] = r[3]; d[4] = r[4]; d[5] = r[5];      \
        d[6] = r[6] | 0xff000000; d[7] = r[7]; d[8] = r[8]; d[9] = r[7]; d[10] = r[8];     \
        d += 11;                                                                           \
    }                                                                                      \
    unsigned short* p = idx;                                                               \
    for (i = 0; i < ntris; i++) {                                                      \
        unsigned short t[4];                                                               \
        file->Read(t, TRI);                                                                \
        p[0] = t[0]; p[1] = t[1]; p[2] = t[2];                                             \
        if (t[0] >= nverts || t[1] >= nverts || t[2] >= nverts)                            \
            throw file_error(FName(file), std::string("BMF batch chunk failed due to illegal vertex index")); \
        p += 3;                                                                            \
    }                                                                                      \
    *(short*)((char*)mesh + 0xe) = ntris;                                                  \
    *(short*)((char*)mesh + 0xc) = nverts;                                                 \
    *(void**)((char*)mesh + 8) = idx;                                                      \
    *(void**)((char*)mesh + 4) = v;                                                        \
    *(void**)mesh = materials[mat];                                                        \
    out->batches.push_back(mesh);

// NONMATCHING: RALLYTROPHY 0x4b1af0 (inlined std::string/throw construction differs from original)
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

// NONMATCHING: RALLYTROPHY 0x4b2330 (inlined std::string/throw construction differs from original)
void ModelFile::ReadPolylist(File* file, ModelOut* out)
{
    int n = FUN_004af230(file);
    out->polyCount = n;
    int bytes = n << 4;
    unsigned int* p = (unsigned int*)operator new(bytes);
    out->polys = p;
    file->Read(p, bytes);
    int i = 0;
    if (0 < n) {
        void** b = (void**)materials.begin();
        do {
            if (b == 0 || (unsigned int)(((int)materials.end() - (int)b) >> 2) <= *p) {
                std::string msg("BMF polylist chunk failed: illegal material index");
                throw file_error(FName(file), msg);
            }
            *p = (unsigned int)b[*p];
            i++;
            p += 4;
        } while (i < n);
    }
}
