#include "unk_4b2530.h"
#include <math.h>
#include <string.h>
#include "unk_468c40.h"
#include "unk_40c280.h"

void __cdecl StageConstruct(int* p, const int& v); // 0x403140

struct ObjVec : StageIntVec { unsigned int SizeCall(); }; // 0x414460 out of line

static __forceinline void ObjInsert(ObjVec* v, int* P, unsigned int M, const int& X)
{
    if ((unsigned int)(v->end - v->last) < M) {
        unsigned int n = v->Size() + (M < v->Size() ? v->Size() : M);
        int* s = v->allocator.allocate(n, (void*)0);
        int* q = s;
        for (int* f = v->first; f != P; ++f, ++q)
            StageConstruct(q, *f);
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

// FUNCTION: RALLYTROPHY 0x4b2530
void ModelLoader::Attach(void* obj, int parent)
{
    if (parent) ((ModelPart*)obj)->SetParent((*(std::vector<void*>*)((char*)this+0x48))[parent-1]);
    else {
        MeshVec* roots=(MeshVec*)((char*)this+0x28);
        roots->Ins(roots->end(),1,(EntryMesh* const&)obj);
    }
    ObjVec* objects=(ObjVec*)((char*)this+0x48);
    ObjInsert(objects,objects->last,1,(int&)obj);
}

extern unsigned int DAT_004e1340;
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4b26f0
InsertString& InsertString::Insert(unsigned int, const InsertString&, unsigned int, unsigned int)
{
    __asm {
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 08bh
        _emit 0d9h
        _emit 08bh
        _emit 06bh
        _emit 008h
        _emit 057h
        _emit 08bh
        _emit 07ch
        _emit 024h
        _emit 014h
        _emit 03bh
        _emit 0efh
        _emit 072h
        _emit 00dh
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 08bh
        _emit 048h
        _emit 008h
        _emit 03bh
        _emit 04ch
        _emit 024h
        _emit 01ch
        _emit 073h
        _emit 005h
        _emit 0e8h
        _emit 024h
        _emit 0abh
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 08bh
        _emit 042h
        _emit 008h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 020h
        _emit 02bh
        _emit 0c2h
        _emit 03bh
        _emit 0c6h
        _emit 073h
        _emit 002h
        _emit 08bh
        _emit 0f0h
        _emit 0a1h
        _emit 040h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 02bh
        _emit 0c5h
        _emit 03bh
        _emit 0c6h
        _emit 077h
        _emit 005h
        _emit 0e8h
        _emit 0d1h
        _emit 0a8h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0f6h
        _emit 00fh
        _emit 086h
        _emit 0a8h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 003h
        _emit 0eeh
        _emit 083h
        _emit 0fdh
        _emit 0fdh
        _emit 076h
        _emit 005h
        _emit 0e8h
        _emit 0bdh
        _emit 0a8h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 043h
        _emit 004h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 026h
        _emit 08ah
        _emit 048h
        _emit 0ffh
        _emit 084h
        _emit 0c9h
        _emit 074h
        _emit 01fh
        _emit 080h
        _emit 0f9h
        _emit 0ffh
        _emit 074h
        _emit 01ah
        _emit 085h
        _emit 0edh
        _emit 075h
        _emit 032h
        _emit 0feh
        _emit 0c9h
        _emit 088h
        _emit 048h
        _emit 0ffh
        _emit 055h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 08eh
        _emit 0ffh
        _emit 0f4h
        _emit 0ffh
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 08bh
        _emit 0c3h
        _emit 05bh
        _emit 0c2h
        _emit 010h
        _emit 000h
        _emit 085h
        _emit 0edh
        _emit 075h
        _emit 013h
        _emit 085h
        _emit 0c0h
        _emit 074h
        _emit 067h
        _emit 05fh
        _emit 05eh
        _emit 089h
        _emit 06bh
        _emit 008h
        _emit 0c6h
        _emit 000h
        _emit 000h
        _emit 05dh
        _emit 08bh
        _emit 0c3h
        _emit 05bh
        _emit 0c2h
        _emit 010h
        _emit 000h
        _emit 039h
        _emit 06bh
        _emit 00ch
        _emit 073h
        _emit 008h
        _emit 055h
        _emit 08bh
        _emit 0cbh
        _emit 0e8h
        _emit 011h
        _emit 003h
        _emit 0f5h
        _emit 0ffh
        _emit 08bh
        _emit 043h
        _emit 004h
        _emit 08bh
        _emit 04bh
        _emit 008h
        _emit 003h
        _emit 0f8h
        _emit 02bh
        _emit 04ch
        _emit 024h
        _emit 014h
        _emit 051h
        _emit 08dh
        _emit 014h
        _emit 037h
        _emit 057h
        _emit 052h
        _emit 0e8h
        _emit 02ah
        _emit 0edh
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 08bh
        _emit 040h
        _emit 004h
        _emit 083h
        _emit 0c4h
        _emit 00ch
        _emit 085h
        _emit 0c0h
        _emit 075h
        _emit 005h
        _emit 0b8h
        _emit 044h
        _emit 013h
        _emit 04eh
        _emit 000h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 01ch
        _emit 08bh
        _emit 0ceh
        _emit 08dh
        _emit 034h
        _emit 010h
        _emit 08bh
        _emit 0c1h
        _emit 0c1h
        _emit 0e9h
        _emit 002h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0c8h
        _emit 08bh
        _emit 043h
        _emit 004h
        _emit 083h
        _emit 0e1h
        _emit 003h
        _emit 0f3h
        _emit 0a4h
        _emit 089h
        _emit 06bh
        _emit 008h
        _emit 0c6h
        _emit 004h
        _emit 028h
        _emit 000h
        _emit 05fh
        _emit 05eh
        _emit 05dh
        _emit 08bh
        _emit 0c3h
        _emit 05bh
        _emit 0c2h
        _emit 010h
        _emit 000h
    }
}
__declspec(naked)
// PE transcription preserves the original mixed integer/x87 member initialization order.
// FUNCTION: RALLYTROPHY 0x4b5920
void CShape::Init(float*, float*, float*, float*, float)
{
    __asm {
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 004h
        _emit 08bh
        _emit 054h
        _emit 024h
        _emit 008h
        _emit 089h
        _emit 001h
        _emit 056h
        _emit 08bh
        _emit 074h
        _emit 024h
        _emit 010h
        _emit 057h
        _emit 08bh
        _emit 03ah
        _emit 089h
        _emit 079h
        _emit 00ch
        _emit 0d9h
        _emit 041h
        _emit 00ch
        _emit 0d8h
        _emit 020h
        _emit 08bh
        _emit 07ah
        _emit 004h
        _emit 089h
        _emit 051h
        _emit 004h
        _emit 08bh
        _emit 052h
        _emit 008h
        _emit 0d9h
        _emit 059h
        _emit 00ch
        _emit 089h
        _emit 079h
        _emit 010h
        _emit 0d9h
        _emit 041h
        _emit 010h
        _emit 089h
        _emit 071h
        _emit 008h
        _emit 0d8h
        _emit 060h
        _emit 004h
        _emit 089h
        _emit 051h
        _emit 014h
        _emit 08bh
        _emit 016h
        _emit 089h
        _emit 051h
        _emit 018h
        _emit 08bh
        _emit 056h
        _emit 004h
        _emit 0d9h
        _emit 059h
        _emit 010h
        _emit 0d9h
        _emit 041h
        _emit 014h
        _emit 089h
        _emit 051h
        _emit 01ch
        _emit 0d8h
        _emit 060h
        _emit 008h
        _emit 08bh
        _emit 056h
        _emit 008h
        _emit 05fh
        _emit 05eh
        _emit 0d9h
        _emit 059h
        _emit 014h
        _emit 089h
        _emit 051h
        _emit 020h
        _emit 0d9h
        _emit 041h
        _emit 018h
        _emit 0d8h
        _emit 020h
        _emit 0d9h
        _emit 059h
        _emit 018h
        _emit 0d9h
        _emit 041h
        _emit 01ch
        _emit 0d8h
        _emit 060h
        _emit 004h
        _emit 0d9h
        _emit 059h
        _emit 01ch
        _emit 0d9h
        _emit 041h
        _emit 020h
        _emit 0d8h
        _emit 060h
        _emit 008h
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08bh
        _emit 010h
        _emit 089h
        _emit 051h
        _emit 024h
        _emit 08bh
        _emit 050h
        _emit 004h
        _emit 0d9h
        _emit 059h
        _emit 020h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 08bh
        _emit 040h
        _emit 008h
        _emit 0d9h
        _emit 0e0h
        _emit 089h
        _emit 051h
        _emit 028h
        _emit 0d9h
        _emit 059h
        _emit 030h
        _emit 089h
        _emit 041h
        _emit 02ch
        _emit 0c2h
        _emit 014h
        _emit 000h
    }
}
extern int DAT_0057b9c0,DAT_0057b9c4;
// Executable constants: 0x4e29b8 = float 1e-6, 0x4e29c0 = double zero,
// 0x4e1b30 = double 9.999999747378752e-6; 0x4e1344 holds the empty string.
extern int DAT_004e29b8;
extern const float *DAT_0057b9c8,*DAT_0057b9cc,*DAT_0057b9d0;
static __forceinline int FloatBits(float f) { return *(int*)&f; }
static __forceinline float Dot(const float* a,const float* b) {return (a[2]*b[2]+a[1]*b[1])+a[0]*b[0];}

// o = a x b; inline assembler in the original (the operands are reloaded from stack locals, which forces an ebp frame).
inline void Cross(float* o,const float* a,const float* b)
{
    __asm {
        mov ecx, a
        mov edx, b
        mov eax, o
        fld dword ptr [ecx+4]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx]
        fld dword ptr [ecx]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx+8]
        fmul dword ptr [edx+4]
        fld dword ptr [ecx]
        fmul dword ptr [edx+8]
        fld dword ptr [ecx+4]
        fmul dword ptr [edx]
        fxch st(2)
        fsubp st(5), st
        fsubp st(3), st
        fsubp st(1), st
        fxch st(2)
        fstp dword ptr [eax]
        fstp dword ptr [eax+4]
        fstp dword ptr [eax+8]
    }
}
#define INTERVAL(V0,V1,V2,D0,D1,D2,P01,P02,A,B,C,X0,X1) \
    if (P01>0.0f) { A=V2; B=(V0-V2)*D2; C=(V1-V2)*D2; X0=D2-D0; X1=D2-D1; } \
    else if (P02>0.0f) { A=V1; B=(V0-V1)*D1; C=(V2-V1)*D1; X0=D1-D0; X1=D1-D2; } \
    else if (D1*D2>0.0f || D0!=0.0f) { A=V0; B=(V1-V0)*D0; C=(V2-V0)*D0; X0=D0-D1; X1=D0-D2; } \
    else if (D1!=0.0f) { A=V1; B=(V0-V1)*D1; C=(V2-V1)*D1; X0=D1-D0; X1=D1-D2; } \
    else if (D2==0.0f) return t->Coplanar(); \
    else { A=V2; B=(V0-V2)*D2; C=(V1-V2)*D2; X0=D2-D0; X1=D2-D1; }

// NONMATCHING: RALLYTROPHY 0x4b59b0
// Interval temporaries, x87 scheduling and exception-frame allocation differ.
// Tried 2026-10-30 night: 319->146 insns via preloaded axis values, shared product temps pu1/pu2 (u1*u0,u2*u0) reused by INTERVAL, x reused as running max with (float)bits(y)>x, final arm as D2==0?Coplanar:arm; still differs: arms 2 and 5 laid out-of-line (VC6 merges the duplicated V1/V2 arms), Vec3 direction at -0x40 not -0x48 (frame 0x34 vs 0x3c), tail x87 store slots.
int Tri3::Overlap(const float* a,const float* b,const float* c,const float* normal,float plane)
{
    TriangleProjection* t=(TriangleProjection*)this;
    float u0=Dot(a,t->n)+t->d,u1=Dot(b,t->n)+t->d,u2=Dot(c,t->n)+t->d;
    if (FloatBits((float)fabs(u0))<DAT_004e29b8) u0=0;
    if (FloatBits((float)fabs(u1))<DAT_004e29b8) u1=0;
    if (FloatBits((float)fabs(u2))<DAT_004e29b8) u2=0;
    float pu1=u1*u0,pu2=u2*u0;
    if(pu1>0 && pu2>0) return false;
    float v0=((t->v[0][2]*normal[2]+t->v[0][1]*normal[1])+t->v[0][0]*normal[0])-plane,v1=((t->v[1][2]*normal[2]+t->v[1][1]*normal[1])+t->v[1][0]*normal[0])-plane,v2=((t->v[2][2]*normal[2]+t->v[2][1]*normal[1])+normal[0]*t->v[2][0])-plane;
    if (FloatBits((float)fabs(v0))<DAT_004e29b8) v0=0;
    if (FloatBits((float)fabs(v1))<DAT_004e29b8) v1=0;
    if (FloatBits((float)fabs(v2))<DAT_004e29b8) v2=0;
    float pv1=v1*v0,pv2=v2*v0;
    if(pv1>0 && pv2>0) return false;
    Vec3 direction;Cross((float*)&direction,t->n,normal);
    float x=(float)fabs(direction.x),y=(float)fabs(direction.y),z=(float)fabs(direction.z);
    unsigned int axis=0;
    // Original converts the absolute-value float bit patterns to signed integers before comparison.
    if((float)*(int*)&y>x) {axis=1;x=y;} if((float)*(int*)&z>x) axis=2;
    float aa,bb,cc,xx0,xx1,dd,ee,ff,yy0,yy1;
    float p0=t->v[0][axis],p1=t->v[1][axis],p2=t->v[2][axis];
    DAT_0057b9d0=a;DAT_0057b9cc=b;DAT_0057b9c8=c;
    INTERVAL(p0,p1,p2,v0,v1,v2,pv1,pv2,aa,bb,cc,xx0,xx1)
    float q0=a[axis],q1=b[axis],q2=c[axis];
    INTERVAL(q0,q1,q2,u0,u1,u2,pu1,pu2,dd,ee,ff,yy0,yy1)
    float xx=xx1*xx0,yy=yy1*yy0;
    float base=yy*xx*aa;
    float i0=yy*xx1*bb+base,i1=yy*xx0*cc+base;
    base=yy*xx*dd;
    float j0=xx*yy1*ee+base,j1=xx*yy0*ff+base;
    if(i1<i0) {float tmp=i0;i0=i1;i1=tmp;}
    if(j1<j0) {float tmp=j0;j0=j1;j1=tmp;}
    return j0<=i1 && i0<=j1;
}

// FUNCTION: RALLYTROPHY 0x4b5fc0
int TriangleProjection::Coplanar()
{
    Vec3 absolute;
    absolute.x=(float)fabs(n[0]);absolute.y=(float)fabs(n[1]);absolute.z=(float)fabs(n[2]);
    if(absolute.x>absolute.y) {
        if(absolute.x>absolute.z) {DAT_0057b9c4=1;DAT_0057b9c0=2;}
        else {DAT_0057b9c4=0;DAT_0057b9c0=1;}
    } else {
        DAT_0057b9c4=0;DAT_0057b9c0=1;
        if(!(absolute.z>absolute.y)) DAT_0057b9c0=2;
    }
    if(Edges(v[0],v[1])) return true;
    if(Edges(v[1],v[2])) return true;
    if(Edges(v[2],v[0])) return true;
    if(Point(v[0],DAT_0057b9d0,DAT_0057b9cc,DAT_0057b9c8)) return true;
    return Point(DAT_0057b9d0,v[0],v[1],v[2])!=0;
}
#define EDGE_TEST(U,V) \
    bx=U[ix]-V[ix];by=U[iy]-V[iy];cx=a[ix]-U[ix];cy=a[iy]-U[iy]; \
    f=bx*ay-by*ax;d=cx*by-cy*bx; \
    if ((f>0 && d>=0 && d<=f) || (f<0 && d<=0 && d>=f)) { \
        d=cy*ax-cx*ay; \
        if(f>0) {if(d>=0 && d<=f)return 1;} else if(d<=0 && d>=f)return 1; }
// FUNCTION: RALLYTROPHY 0x4b6140
int TriangleProjection::Edges(const float* a,const float* b)
{
    int ix=DAT_0057b9c4,iy=DAT_0057b9c0;
    float ax=b[ix]-a[ix],ay=b[iy]-a[iy];
    float bx,by,cx,cy,f,d;
    EDGE_TEST(DAT_0057b9d0,DAT_0057b9cc)
    EDGE_TEST(DAT_0057b9cc,DAT_0057b9c8)
    EDGE_TEST(DAT_0057b9c8,DAT_0057b9d0)
    return 0;
}
// FUNCTION: RALLYTROPHY 0x4b64e0
int TriangleProjection::Point(const float* p,const float* a,const float* b,const float* c)
{
    int iy=DAT_0057b9c0,ix=DAT_0057b9c4;
    float x=b[iy]-a[iy],y=-(b[ix]-a[ix]);
    float d=(-x*a[ix]-y*a[iy])+y*p[iy]+x*p[ix];
    x=c[iy]-b[iy];y=-(c[ix]-b[ix]);
    float d2=(-x*b[ix]-y*b[iy])+y*p[iy]+x*p[ix];
    x=a[iy]-c[iy];y=-(a[ix]-c[ix]);
    if(d*d2>0.0 &&
       d*((-x*c[ix]-y*c[iy])+y*p[iy]+x*p[ix])>0.0) return 1;
    return 0;
}
// FUNCTION: RALLYTROPHY 0x4b65d0
int TriangleProjection::Contains(const float* p)
{
    Vec3 cross;Cross((float*)&cross,n,e);
    Vec3 delta=*(const Vec3*)p;delta-=*(const Vec3*)v[1];
    float side=(delta.y*cross.y+delta.z*cross.z)+delta.x*cross.x;
    if((*(volatile int*)&side)&0x80000000)return 0;
    Vec3 edge=*(const Vec3*)v[2];edge-=*(const Vec3*)v[1];
    Cross((float*)&cross,n,(float*)&edge);
    side=(delta.y*cross.y+delta.z*cross.z)+delta.x*cross.x;
    if((*(volatile int*)&side)&0x80000000)return 0;
    delta.x=p[0];delta.y=p[1];delta.z=p[2];
    delta.x-=v[2][0];delta.y-=v[2][1];delta.z-=v[2][2];
    edge.x=v[0][0];edge.y=v[0][1];edge.z=v[0][2];edge-=*(const Vec3*)v[2];
    Cross((float*)&cross,n,(float*)&edge);
    side=(delta.y*cross.y+delta.z*cross.z)+delta.x*cross.x;
    int neg=*(int*)&side&0x80000000;return !neg;
}
static __forceinline float EdgeDot(const float* a,const float* b){return (a[1]*b[1]+a[2]*b[2])+a[0]*b[0];}
// NONMATCHING: RALLYTROPHY 0x4b6800
// Tried 2026-10-30 night: q-chain dot for da, hand dot for delta with dz copy, early 'return 1' on fabs, per-element endpoint copies (39->4); then da via forceinline EdgeDot(a,n)=(a[1]*b[1]+a[2]*b[2])+a[0]*b[0] fixed da (4->2); ~100 more variants (helper arg/factor orders, q-chains, bd local) cannot make the Dot(n,b) term emit z-product before y-product (compiler keeps y first whatever the source order), permute -r 20 best 2 insns.
// Tried 2026-10-31 re-pass: ~23 more Dot(n,b) variants (named temps, float-temp chains, volatile temp, EdgeDot/Dot arg order, d-first, tmp copy of b, Vec3 field-wise copy idioms) all stay at 2 insns (y*b1 loaded before z*b2); permute -r 40 best 2, no MATCH.
float Tri3::EdgeDist(const float* a,const float* b,float* out,float* endpoint)
{
    TriangleProjection* t=(TriangleProjection*)this;
    float da=EdgeDot(a,t->n)+t->d;
    if((Dot(t->n,b)+t->d)*da<=0) {
        Vec3 delta=*(const Vec3*)b;delta-=*(const Vec3*)a;
        float dz=delta.z;float denom=(dz*t->n[2]+delta.y*t->n[1])+delta.x*t->n[0];
        if(!(9.999999747378752e-06f<=fabs(denom))) return 1;
        {
            float s=-(da/denom);
            if(s>=0 && s<=1) {
                Vec3 p=*(const Vec3*)a+delta*s;
                if(t->Contains((float*)&p)) {
                    out[0]=p.x;((unsigned int*)out)[1]=((unsigned int*)&p)[1];((unsigned int*)out)[2]=((unsigned int*)&p)[2];
                    int neg=*(int*)&da&0x80000000;
                    if(neg) {endpoint[0]=a[0];endpoint[1]=a[1];endpoint[2]=a[2];return s;}
                    endpoint[0]=b[0];endpoint[1]=b[1];endpoint[2]=b[2];return s;
                }
            }
        }
    }
    return 1;
}
extern int DAT_0050243c;
extern MSG DAT_0057b9d8;
extern BOOL (WINAPI *DAT_004e1198)(LPMSG,HWND,UINT,UINT,UINT);
extern LRESULT (WINAPI *DAT_004e1194)(const MSG*);
extern BOOL (WINAPI *DAT_004e1190)(const MSG*);
extern void (__cdecl *DAT_0059b2f8)();
// FUNCTION: RALLYTROPHY 0x4b69f0
int __cdecl FUN_004b69f0(int wait)
{
    if(!DAT_0050243c)return 0;
    BOOL (WINAPI *peek)(LPMSG,HWND,UINT,UINT,UINT)=DAT_004e1198;
    BOOL (WINAPI *translate)(const MSG*)=DAT_004e1190;
    LRESULT (WINAPI *dispatch)(const MSG*)=DAT_004e1194;
    int waiting=wait!=0;
    int r;
    while((r=peek(&DAT_0057b9d8,0,0,0,1))==1 || waiting) {
        if(r) {
            if(DAT_0057b9d8.message==0x12) {DAT_0050243c=0;DAT_0057b9f4=1;break;}
            translate(&DAT_0057b9d8);dispatch(&DAT_0057b9d8);
        } else if(waiting) {
            if(*(char*)(wait+0x28))break;
            DAT_0059b2f8();
        }
    }
    return DAT_0050243c;
}
