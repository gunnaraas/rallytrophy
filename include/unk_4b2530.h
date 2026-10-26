#ifndef UNK_4B2530_H
#define UNK_4B2530_H
#include "unk_4b13c0.h"
#include "unk_41ca70.h"
// These existing headers use CTri for unrelated partial layout views.
#define CTri BodyCollisionTriangle
#include "unk_41d7d0.h"
#undef CTri
#include "unk_414fe0.h"
// Layout view shared by the existing Tri3 and CShape entry-point aliases.
struct TriangleProjection {
    const float* v[3];
    float e[3], f[3], n[3], d;
    int Coplanar();
    int Edges(const float* a,const float* b);
    int Point(const float* p,const float* a,const float* b,const float* c);
    int Contains(const float* p);
};
struct InsertString : PlaceStr {
    InsertString& Insert(unsigned int pos, const InsertString& src, unsigned int off, unsigned int count);
};
#endif
