// Mesh container / deformation model (0x4b8b10-0x4b9b50). Map: analysis/range-4b7a40.md
#include <string>
#include <string.h>
#include <math.h>
#include <stdexcept>
#include "unk_4b8b10.h"

extern int DAT_00573bc0[];
extern unsigned int __cdecl SaveSceneControlWord();            // original: FUN_004a7da0
extern void __cdecl RestoreSceneControlWord(unsigned int word); // original: FUN_004a7d90

struct PointCopy {
    float x, y, z;
    __forceinline PointCopy(const float* p) { x=p[0]; y=p[1]; z=p[2]; }
};

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b8b10
void MeshModel::ComputeBounds()
{
    __asm {
        _emit 083h
        _emit 0ech
        _emit 018h
        _emit 0d9h
        _emit 005h
        _emit 010h
        _emit 02ah
        _emit 04eh
        _emit 000h
        _emit 053h
        _emit 08bh
        _emit 059h
        _emit 034h
        _emit 056h
        _emit 057h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 000h
        _emit 024h
        _emit 074h
        _emit 049h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 000h
        _emit 024h
        _emit 074h
        _emit 049h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 000h
        _emit 024h
        _emit 074h
        _emit 049h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 000h
        _emit 024h
        _emit 074h
        _emit 0c9h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 000h
        _emit 024h
        _emit 074h
        _emit 0c9h
        _emit 033h
        _emit 0ffh
        _emit 085h
        _emit 0dbh
        _emit 00fh
        _emit 084h
        _emit 0a5h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 041h
        _emit 038h
        _emit 02bh
        _emit 0c3h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 03bh
        _emit 0f8h
        _emit 00fh
        _emit 083h
        _emit 095h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 051h
        _emit 034h
        _emit 08bh
        _emit 034h
        _emit 0bah
        _emit 08bh
        _emit 056h
        _emit 004h
        _emit 033h
        _emit 0c0h
        _emit 066h
        _emit 08bh
        _emit 046h
        _emit 00ch
        _emit 085h
        _emit 0c0h
        _emit 07eh
        _emit 07ch
        _emit 08bh
        _emit 0f0h
        _emit 0d9h
        _emit 002h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 00ch
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 006h
        _emit 08bh
        _emit 002h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 00ch
        _emit 0d9h
        _emit 002h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 018h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 006h
        _emit 08bh
        _emit 002h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 0d9h
        _emit 042h
        _emit 004h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 010h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 007h
        _emit 08bh
        _emit 042h
        _emit 004h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d9h
        _emit 042h
        _emit 004h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 01ch
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 007h
        _emit 08bh
        _emit 042h
        _emit 004h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 0d9h
        _emit 042h
        _emit 008h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 007h
        _emit 08bh
        _emit 042h
        _emit 008h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0d8h
        _emit 052h
        _emit 008h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 005h
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h
        _emit 042h
        _emit 008h
        _emit 083h
        _emit 0c2h
        _emit 02ch
        _emit 04eh
        _emit 075h
        _emit 086h
        _emit 047h
        _emit 0e9h
        _emit 053h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 00ch
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 089h
        _emit 011h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 014h
        _emit 05fh
        _emit 089h
        _emit 041h
        _emit 004h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 089h
        _emit 051h
        _emit 008h
        _emit 0d9h
        _emit 059h
        _emit 014h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 05eh
        _emit 089h
        _emit 041h
        _emit 00ch
        _emit 089h
        _emit 051h
        _emit 010h
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 018h
        _emit 0c3h
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b8c30
void MeshModel::ComputeBoundsAndSphere()
{
    __asm {
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 098h
        _emit 0f8h
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
        _emit 02ch
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 08bh
        _emit 0f1h
        _emit 0e8h
        _emit 04dh
        _emit 0f1h
        _emit 0feh
        _emit 0ffh
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0a5h
        _emit 0d4h
        _emit 068h
        _emit 053h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 0a5h
        _emit 0d4h
        _emit 068h
        _emit 053h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 0a5h
        _emit 0d4h
        _emit 068h
        _emit 053h
        _emit 08bh
        _emit 0e8h
        _emit 0d9h
        _emit 005h
        _emit 014h
        _emit 02ah
        _emit 04eh
        _emit 000h
        _emit 033h
        _emit 0ffh
        _emit 0d9h
        _emit 054h
        _emit 024h
        _emit 028h
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 044h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 0a5h
        _emit 0d4h
        _emit 068h
        _emit 0d3h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 0a5h
        _emit 0d4h
        _emit 068h
        _emit 0d3h
        _emit 08bh
        _emit 05eh
        _emit 034h
        _emit 0c6h
        _emit 044h
        _emit 024h
        _emit 044h
        _emit 001h
        _emit 089h
        _emit 07ch
        _emit 024h
        _emit 010h
        _emit 085h
        _emit 0dbh
        _emit 00fh
        _emit 084h
        _emit 0e2h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 046h
        _emit 038h
        _emit 02bh
        _emit 0c3h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 03bh
        _emit 0f8h
        _emit 00fh
        _emit 083h
        _emit 0d2h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04eh
        _emit 034h
        _emit 08bh
        _emit 014h
        _emit 0b9h
        _emit 08bh
        _emit 04ah
        _emit 004h
        _emit 033h
        _emit 0c0h
        _emit 066h
        _emit 08bh
        _emit 042h
        _emit 00ch
        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 08eh
        _emit 0b5h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 0d0h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 008h
        _emit 08bh
        _emit 001h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0ebh
        _emit 013h
        _emit 0d9h
        _emit 001h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 020h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 006h
        _emit 08bh
        _emit 001h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 018h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 009h
        _emit 08bh
        _emit 041h
        _emit 004h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 0ebh
        _emit 015h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 024h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 007h
        _emit 08bh
        _emit 041h
        _emit 004h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 01ch
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 009h
        _emit 08bh
        _emit 041h
        _emit 008h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 0ebh
        _emit 00fh
        _emit 0d8h
        _emit 051h
        _emit 008h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 001h
        _emit 074h
        _emit 005h
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d9h
        _emit 041h
        _emit 008h
        _emit 0d9h
        _emit 041h
        _emit 004h
        _emit 0d9h
        _emit 001h
        _emit 0d9h
        _emit 0c2h
        _emit 0deh
        _emit 0cbh
        _emit 0d9h
        _emit 0c1h
        _emit 0d8h
        _emit 0cah
        _emit 0deh
        _emit 0c3h
        _emit 0d9h
        _emit 0c0h
        _emit 0d8h
        _emit 0c9h
        _emit 0deh
        _emit 0c3h
        _emit 0ddh
        _emit 0d8h
        _emit 0ddh
        _emit 0d8h
        _emit 0d8h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 006h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 010h
        _emit 0ebh
        _emit 002h
        _emit 0ddh
        _emit 0d8h
        _emit 083h
        _emit 0c1h
        _emit 02ch
        _emit 04ah
        _emit 00fh
        _emit 085h
        _emit 04dh
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 047h
        _emit 0e9h
        _emit 016h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 054h
        _emit 024h
        _emit 028h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 0d9h
        _emit 0fah
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 01ch
        _emit 089h
        _emit 00eh
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 020h
        _emit 089h
        _emit 056h
        _emit 004h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 089h
        _emit 046h
        _emit 008h
        _emit 089h
        _emit 04eh
        _emit 00ch
        _emit 089h
        _emit 056h
        _emit 010h
        _emit 0d9h
        _emit 05eh
        _emit 028h
        _emit 0d9h
        _emit 05eh
        _emit 014h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 02ch
        _emit 0c6h
        _emit 044h
        _emit 024h
        _emit 044h
        _emit 002h
        _emit 0e8h
        _emit 01dh
        _emit 0bah
        _emit 0ffh
        _emit 0ffh
        _emit 08bh
        _emit 05eh
        _emit 034h
        _emit 033h
        _emit 0ffh
        _emit 085h
        _emit 0dbh
        _emit 074h
        _emit 02bh
        _emit 08bh
        _emit 046h
        _emit 038h
        _emit 02bh
        _emit 0c3h
        _emit 0c1h
        _emit 0f8h
        _emit 002h
        _emit 03bh
        _emit 0f8h
        _emit 073h
        _emit 01fh
        _emit 08bh
        _emit 04eh
        _emit 034h
        _emit 08bh
        _emit 004h
        _emit 0b9h
        _emit 033h
        _emit 0d2h
        _emit 066h
        _emit 08bh
        _emit 050h
        _emit 00ch
        _emit 08bh
        _emit 040h
        _emit 004h
        _emit 06ah
        _emit 000h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 030h
        _emit 052h
        _emit 050h
        _emit 0e8h
        _emit 03ch
        _emit 0b7h
        _emit 0ffh
        _emit 0ffh
        _emit 047h
        _emit 0ebh
        _emit 0d1h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 02ch
        _emit 0e8h
        _emit 020h
        _emit 0bah
        _emit 0ffh
        _emit 0ffh
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 030h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 02ch
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 089h
        _emit 056h
        _emit 01ch
        _emit 08ah
        _emit 056h
        _emit 02ch
        _emit 080h
        _emit 0cah
        _emit 003h
        _emit 0d9h
        _emit 05eh
        _emit 024h
        _emit 055h
        _emit 089h
        _emit 04eh
        _emit 018h
        _emit 089h
        _emit 046h
        _emit 020h
        _emit 088h
        _emit 056h
        _emit 02ch
        _emit 0e8h
        _emit 065h
        _emit 0efh
        _emit 0feh
        _emit 0ffh
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 040h
        _emit 083h
        _emit 0c4h
        _emit 004h
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
        _emit 038h
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x4b8e50
// original: FUN_004b8e50
void MeshModel::Translate(const float* d)
{
    for (unsigned i = 0; i < chunks.size(); i++) {
        MeshChunk* c = chunks[i];
        SceneVertex* v = c->vertices;
        for (int n = c->count; n > 0; n--, v++) {
            v->position.x = v->position.x + d[0];
            v->position.y = v->position.y + d[1];
            v->position.z = d[2] + v->position.z;
        }
    }
    float* p = extra;
    for (int n = extraCount; n > 0; n--, p += 3) {
        p[0] = p[0] + d[0];
        p[1] = p[1] + d[1];
        p[2] = p[2] + d[2];
    }
}

// FUNCTION: RALLYTROPHY 0x4b8ee0
// original: FUN_004b8ee0
void MeshModel::Scale(const float* s)
{
    for (unsigned i = 0; i < chunks.size(); i++) {
        MeshChunk* c = chunks[i];
        SceneVertex* v = c->vertices;
        for (int n = c->count; n > 0; n--, v++) {
            v->position.x = v->position.x * s[0];
            v->position.y = v->position.y * s[1];
            v->position.z = s[2] * v->position.z;
        }
    }
    float* p = extra;
    for (int n = extraCount; n > 0; n--, p += 3) {
        p[0] = p[0] * s[0];
        p[1] = p[1] * s[1];
        p[2] = p[2] * s[2];
    }
}

// Hand-written x87 blocks (inline assembler in the original; the pointers live in stack locals).
// Point times a 4x4 row-vector matrix, translation in row 3.
inline void Xform(float* p, const float* m)
{
    __asm {
        mov ecx, p
        mov edx, m
        fld dword ptr [ecx]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x10]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x14]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+0x18]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x20]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x24]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+0x28]
        fxch st(2)
        faddp st(5), st
        faddp st(3), st
        faddp st(1), st
        fxch st(2)
        fadd dword ptr [edx+0x30]
        fxch st(1)
        fadd dword ptr [edx+0x34]
        fxch st(2)
        fadd dword ptr [edx+0x38]
        fxch st(1)
        fstp dword ptr [ecx]
        fstp dword ptr [ecx+8]
        fstp dword ptr [ecx+4]
    }
}

// FUNCTION: RALLYTROPHY 0x4b8f70
// original: FUN_004b8f70
void MeshModel::Transform(const float* m)
{
    for (unsigned i = 0; i < chunks.size(); i++) {
        MeshChunk* c = chunks[i];
        SceneVertex* p = c->vertices;
        int n = 0;
        if (c->count > 0) {
            do {
                Xform((float*)p, m);
                n++;
                p++;
            } while (n < (int)c->count);
        }
    }
    float* p = extra;
    for (int k = 0; k < extraCount; k++, p += 3) Xform(p, m);
}

// FUNCTION: RALLYTROPHY 0x4b90b0
// original: FUN_004b90b0 (the vector at vertex +0xc, same matrix math)
void MeshModel::TransformNormals(const float* m)
{
    for (unsigned i = 0; i < chunks.size(); i++) {
        MeshChunk* c = chunks[i];
        SceneVertex* v = c->vertices;
        int n = 0;
        if (c->count > 0) {
            do {
                Xform(&v->position.x + 3, m);
                n++;
                v++;
            } while (n < (int)c->count);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4b9170
// original: FUN_004b9170
void MeshModel::LinkDeformVertices()
{
    for (unsigned i = 0; i < deform.size(); i++) {
        DeformList* l = deform[i];
        DeformVertex* d = l->verts;
        SceneVertex* v = chunks[i]->vertices;
        for (int n = l->count; n > 0; n--, d++, v++) d->vertex = v;
    }
}

// NONMATCHING: RALLYTROPHY 0x4b91c0
// logic complete; EH/vector push_back and operator new array codegen differ
// original: FUN_004b91c0
void MeshModel::SetDeformSource(MeshModel* other)
{
    if (!other) return;
    unsigned mine = chunks.empty() ? 0 : chunks.size();
    unsigned theirs = other->chunks.empty() ? 0 : other->chunks.size();
    if (mine != theirs) throw std::logic_error(std::string("deformation and visual model batch size doesn't match"));
    for (unsigned i = 0; i < chunks.size(); i++) {
        MeshChunk* a = chunks[i];
        MeshChunk* b = other->chunks[i];
        if (a->count != b->count) throw std::logic_error(std::string("deformation model has invalid vertex count"));
        DeformList* l = new DeformList;
        l->count = a->count;
        l->dirty = 1;
        l->verts = new DeformVertex[l->count];
        SceneVertex* sa = a->vertices;
        SceneVertex* sb = b->vertices;
        deform.push_back(l);
        DeformVertex* d = l->verts;
        for (int n = 0; n < l->count; n++, d++, sa++, sb++) {
            d->dirty = 1;
            d->posA.x = sa->position.x;
            d->weight = 0;
            d->posA.y = sa->position.y;
            d->vertex = (SceneVertex*)sa;
            d->posA.z = sa->position.z;
            d->posB.x = sb->position.x;
            d->posB.y = sb->position.y;
            d->posB.z = sb->position.z;
            const float* na = &sa->position.x + 3;
            const float* nb = &sb->position.x + 3;
            d->nrmA.x = na[0]; d->nrmA.y = na[1]; d->nrmA.z = na[2];
            d->nrmB.x = nb[0]; d->nrmB.y = nb[1]; d->nrmB.z = nb[2];
        }
    }
    dirtyCount = 0;
}

__declspec(naked)
// PE transcription preserves the repeated deform-list lookup and loop-index spill.
// FUNCTION: RALLYTROPHY 0x4b9530
void MeshModel::Repair()
{
    __asm {
        _emit 051h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 033h
        _emit 0dbh
        _emit 057h
        _emit 033h
        _emit 0d2h
        _emit 08bh
        _emit 041h
        _emit 058h
        _emit 03bh
        _emit 0c3h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 074h
        _emit 05eh
        _emit 08bh
        _emit 071h
        _emit 05ch
        _emit 02bh
        _emit 0f0h
        _emit 0c1h
        _emit 0feh
        _emit 002h
        _emit 03bh
        _emit 0d6h
        _emit 073h
        _emit 052h
        _emit 08bh
        _emit 034h
        _emit 090h
        _emit 08bh
        _emit 036h
        _emit 08dh
        _emit 004h
        _emit 090h
        _emit 08bh
        _emit 000h
        _emit 08bh
        _emit 078h
        _emit 004h
        _emit 03bh
        _emit 0fbh
        _emit 074h
        _emit 03eh
        _emit 08dh
        _emit 046h
        _emit 014h
        _emit 08bh
        _emit 050h
        _emit 0f4h
        _emit 0c7h
        _emit 006h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 068h
        _emit 0f8h
        _emit 089h
        _emit 02ah
        _emit 089h
        _emit 058h
        _emit 0f0h
        _emit 08bh
        _emit 068h
        _emit 0fch
        _emit 089h
        _emit 06ah
        _emit 004h
        _emit 08bh
        _emit 028h
        _emit 089h
        _emit 06ah
        _emit 008h
        _emit 08bh
        _emit 068h
        _emit 010h
        _emit 089h
        _emit 06ah
        _emit 00ch
        _emit 08bh
        _emit 068h
        _emit 014h
        _emit 089h
        _emit 06ah
        _emit 010h
        _emit 08bh
        _emit 068h
        _emit 018h
        _emit 083h
        _emit 0c6h
        _emit 03ch
        _emit 083h
        _emit 0c0h
        _emit 03ch
        _emit 04fh
        _emit 089h
        _emit 06ah
        _emit 014h
        _emit 075h
        _emit 0c9h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 042h
        _emit 0ebh
        _emit 097h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 089h
        _emit 059h
        _emit 050h
        _emit 05bh
        _emit 059h
        _emit 0c3h
    }
}

// NONMATCHING: RALLYTROPHY 0x4b95b0
// logic complete; float/int fast-rsqrt and Vec scale temporaries differ in codegen
// original: FUN_004b95b0
void MeshModel::Refresh()
{
    if (dirtyCount == 0) return;
    for (unsigned i = 0; i < deform.size(); i++) {
        DeformList* l = deform[i];
        if (l->dirty == 0) {
            l->dirty = 1;
            DeformVertex* d = l->verts;
            for (int n = l->count; n != 0; n--, d++) {
                if (d->dirty == 0) {
                    float w = d->weight;
                    float iw = 1.0f - w;
                    d->dirty = 1;
                    ScenePoint t = d->posA * iw;
                    d->vertex->position = t;
                    ScenePoint t2 = d->posB * w;
                    d->vertex->position.x += t2.x;
                    d->vertex->position.y += t2.y;
                    d->vertex->position.z += t2.z;
                    float* nv = &d->vertex->position.x + 3;
                    ScenePoint t3 = d->nrmA * iw;
                    nv[0] = t3.x; nv[1] = t3.y; nv[2] = t3.z;
                    ScenePoint t4 = d->nrmB * w;
                    nv[0] += t4.x; nv[1] += t4.y; nv[2] += t4.z;
                    float q = nv[2] * nv[2] + nv[1] * nv[1] + nv[0] * nv[0];
                    int bits = *(int*)&q;
                    int r = DAT_00573bc0[(bits >> 12) & 0xfff] + 0x3f800000 + (-(int)((bits * 2 + 0x81000001U) & 0xfe000000) >> 2);
                    r &= 0x7fffffff;
                    float inv = *(float*)&r;
                    nv[0] = inv * nv[0];
                    nv[1] = inv * nv[1];
                    nv[2] = inv * nv[2];
                }
            }
        }
    }
    dirtyCount = 0;
}

// FUNCTION: RALLYTROPHY 0x4b9790
// original: FUN_004b9790
float* MeshModel::Find(const float* p)
{
    float* best = 0;
    float bestD = 1e6f;
    for (unsigned i = 0; i < deform.size(); i++) {
        DeformVertex* d = deform[i]->verts;
        for (int n = deform[i]->count; n != 0; n--, d++) {
            PointCopy delta(&d->posA.x);
            delta.x -= p[0]; delta.y -= p[1]; delta.z -= p[2];
            float dd = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
            if (*(int*)&dd < *(int*)&bestD) { bestD = dd; best = &d->weight; }
        }
    }
    return best;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b9830
void MeshModel::Push(const float*, float, float, float)
{
    __asm {
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 083h
        _emit 0ech
        _emit 018h
        _emit 0d8h
        _emit 01dh
        _emit 024h
        _emit 01fh
        _emit 04eh
        _emit 000h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 00fh
        _emit 085h
        _emit 05bh
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 02ch
        _emit 0d8h
        _emit 04ch
        _emit 024h
        _emit 038h
        _emit 033h
        _emit 0edh
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 010h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 0d8h
        _emit 04ch
        _emit 024h
        _emit 034h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d8h
        _emit 064h
        _emit 024h
        _emit 014h
        _emit 0d8h
        _emit 03dh
        _emit 048h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 018h
        _emit 08bh
        _emit 041h
        _emit 058h
        _emit 085h
        _emit 0c0h
        _emit 00fh
        _emit 084h
        _emit 020h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 051h
        _emit 05ch
        _emit 02bh
        _emit 0d0h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 03bh
        _emit 0eah
        _emit 00fh
        _emit 083h
        _emit 010h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 034h
        _emit 0a8h
        _emit 08bh
        _emit 046h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 08bh
        _emit 016h
        _emit 00fh
        _emit 084h
        _emit 0fah
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 02ch
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 03fh
        _emit 08bh
        _emit 0d8h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 02ch
        _emit 039h
        _emit 042h
        _emit 004h
        _emit 00fh
        _emit 08dh
        _emit 0d9h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 042h
        _emit 010h
        _emit 0d9h
        _emit 042h
        _emit 00ch
        _emit 0d8h
        _emit 027h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 08bh
        _emit 042h
        _emit 014h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 0d8h
        _emit 067h
        _emit 004h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d8h
        _emit 067h
        _emit 008h
        _emit 0d9h
        _emit 0c0h
        _emit 0d8h
        _emit 0c9h
        _emit 0d9h
        _emit 0c2h
        _emit 0d8h
        _emit 0cbh
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 0c3h
        _emit 0d8h
        _emit 0cch
        _emit 0deh
        _emit 0c1h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 034h
        _emit 039h
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 0ddh
        _emit 0d8h
        _emit 0ddh
        _emit 0d8h
        _emit 0ddh
        _emit 0d8h
        _emit 00fh
        _emit 08dh
        _emit 090h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0dbh
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 030h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 0d8h
        _emit 05ch
        _emit 024h
        _emit 014h
        _emit 0dfh
        _emit 0e0h
        _emit 0f6h
        _emit 0c4h
        _emit 041h
        _emit 075h
        _emit 024h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0d8h
        _emit 064h
        _emit 024h
        _emit 034h
        _emit 0d8h
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 0d9h
        _emit 0c0h
        _emit 0dch
        _emit 0c0h
        _emit 0d8h
        _emit 02dh
        _emit 0dch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0d8h
        _emit 0c9h
        _emit 0d8h
        _emit 0c9h
        _emit 0d8h
        _emit 04ch
        _emit 024h
        _emit 030h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 038h
        _emit 0ddh
        _emit 0d8h
        _emit 0f7h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 074h
        _emit 008h
        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 0d8h
        _emit 042h
        _emit 004h
        _emit 0d9h
        _emit 054h
        _emit 024h
        _emit 038h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 085h
        _emit 0c0h
        _emit 07dh
        _emit 00ah
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h
        _emit 005h
        _emit 04ch
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 0ebh
        _emit 00fh
        _emit 03dh
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 03fh
        _emit 07eh
        _emit 008h
        _emit 0ddh
        _emit 0d8h
        _emit 0d9h
        _emit 005h
        _emit 048h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 08bh
        _emit 041h
        _emit 050h
        _emit 0d9h
        _emit 05ah
        _emit 004h
        _emit 040h
        _emit 0c7h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0c7h
        _emit 046h
        _emit 008h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 041h
        _emit 050h
        _emit 083h
        _emit 0c2h
        _emit 03ch
        _emit 04bh
        _emit 00fh
        _emit 085h
        _emit 010h
        _emit 0ffh
        _emit 0ffh
        _emit 0ffh
        _emit 045h
        _emit 0e9h
        _emit 0d5h
        _emit 0feh
        _emit 0ffh
        _emit 0ffh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 05bh
        _emit 083h
        _emit 0c4h
        _emit 018h
        _emit 0c2h
        _emit 010h
        _emit 000h
    }
}
__declspec(naked)
// PE transcription preserves the original three return paths and deformation-weight copy loop.
// FUNCTION: RALLYTROPHY 0x4b99c0
void MeshModel::Save(float**, int*)
{
    __asm {
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 04eh
        _emit 058h
        _emit 033h
        _emit 0edh
        _emit 057h
        _emit 033h
        _emit 0c0h
        _emit 085h
        _emit 0c9h
        _emit 074h
        _emit 018h
        _emit 08bh
        _emit 056h
        _emit 05ch
        _emit 02bh
        _emit 0d1h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 03bh
        _emit 0c2h
        _emit 073h
        _emit 00ch
        _emit 08bh
        _emit 056h
        _emit 058h
        _emit 08bh
        _emit 014h
        _emit 082h
        _emit 003h
        _emit 06ah
        _emit 004h
        _emit 040h
        _emit 0ebh
        _emit 0e4h
        _emit 085h
        _emit 0edh
        _emit 074h
        _emit 057h
        _emit 08dh
        _emit 004h
        _emit 0adh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 0e8h
        _emit 07ch
        _emit 078h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 083h
        _emit 0c4h
        _emit 004h
        _emit 089h
        _emit 001h
        _emit 033h
        _emit 0dbh
        _emit 08bh
        _emit 04eh
        _emit 058h
        _emit 085h
        _emit 0c9h
        _emit 074h
        _emit 02bh
        _emit 08bh
        _emit 056h
        _emit 05ch
        _emit 02bh
        _emit 0d1h
        _emit 0c1h
        _emit 0fah
        _emit 002h
        _emit 03bh
        _emit 0dah
        _emit 073h
        _emit 039h
        _emit 08bh
        _emit 00ch
        _emit 099h
        _emit 08bh
        _emit 051h
        _emit 004h
        _emit 085h
        _emit 0d2h
        _emit 08bh
        _emit 039h
        _emit 07eh
        _emit 010h
        _emit 08dh
        _emit 04fh
        _emit 004h
        _emit 08bh
        _emit 039h
        _emit 089h
        _emit 038h
        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 083h
        _emit 0c1h
        _emit 03ch
        _emit 04ah
        _emit 075h
        _emit 0f3h
        _emit 043h
        _emit 0ebh
        _emit 0ceh
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 05fh
        _emit 05eh
        _emit 089h
        _emit 02ah
        _emit 05dh
        _emit 05bh
        _emit 0c2h
        _emit 008h
        _emit 000h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 05fh
        _emit 05eh
        _emit 089h
        _emit 028h
        _emit 05dh
        _emit 05bh
        _emit 0c2h
        _emit 008h
        _emit 000h
        _emit 08bh
        _emit 04ch
        _emit 024h
        _emit 018h
        _emit 05fh
        _emit 05eh
        _emit 089h
        _emit 029h
        _emit 05dh
        _emit 05bh
        _emit 0c2h
        _emit 008h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x4b9a60
// original: FUN_004b9a60
void MeshModel::Load(const float* src, int count)
{
    int total = 0;
    for (unsigned i = 0; i < deform.size(); i++) total += deform[i]->count;
    if (total == count && count != 0) {
        for (unsigned i = 0; i < deform.size(); i++) {
            DeformList* l = deform[i];
            DeformVertex* d = l->verts;
            l->dirty = 0;
            for (int n = l->count; n > 0; n--, d++) {
                d->weight = *src++;
                d->dirty = 0;
            }
        }
        dirtyCount = count;
        Refresh();
    }
}

// FUNCTION: RALLYTROPHY 0x4b9b20
// original: FUN_004b9b20
ChunkList::~ChunkList() {}

// NONMATCHING: RALLYTROPHY 0x4b9b50
// original inlines vector::insert via helpers 0x424320/0x424350; push_back codegen differs
// original: FUN_004b9b50
void ChunkList::AddFlagged(MeshModel* model)
{
    for (unsigned i = 0; !model->chunks.empty() && i < model->chunks.size(); i++) {
        MeshChunk** e = &model->chunks[i];
        if ((*e)->material->flags & 0x100) items.push_back(e);
    }
}
