// Tyre marks SkidMarks (0x4741a0-0x474c4f, vtable 0x4e2014): one object per
// wheel, embedded in the Car at +0x2748. Map: analysis/range-4741a0.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_4741a0.h"
#include "unk_479170.h"

extern int DAT_00501428; // render quality, 2 = high
extern float DAT_004e1b9c;

// std::string layout with _Grow(n, true) expanded inline, as the original constructor has it.
struct SkStr {
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
    void Tidy(bool built);        // 0x402700
    void Copy(unsigned int n);    // 0x402ab0
    static void Xlen();           // 0x4bd00b
    static unsigned char& Rc(const char* u) { return ((unsigned char*)u)[-1]; }
    SkStr& Assign(const char* s)
    {
        unsigned int n = strlen(s);
        if (n > 0xfffffffd)
            Xlen();
        if (ptr != 0 && Rc(ptr) != 0 && Rc(ptr) != 255) {
            if (n == 0) {
                --Rc(ptr);
                ptr = 0;
                len = 0;
                res = 0;
                return *this;
            }
            Copy(n);
        } else if (n == 0) {
            if (ptr != 0) {
                if (Rc(ptr) != 0 && Rc(ptr) != 255)
                    --Rc(ptr);
                else
                    alloc.deallocate(ptr - 1, res + 2);
            }
            ptr = 0;
            len = 0;
            res = 0;
            return *this;
        } else if (res > 0x1f || res < n) {
            Tidy(true);
            Copy(n);
        }
        std::char_traits<char>::copy(ptr, s, n);
        len = n;
        ptr[n] = 0;
        return *this;
    }
};

// FUNCTION: RALLYTROPHY 0x4741a0
// This constructor has to be the first function emitted in the translation unit: the target clones the full epilogue
// at each `return` of the inlined string assign, and VC6 only does that for the first emitted function (with any
// function compiled before it the three returns merge into one shared epilogue, jmp 0x474406). SetQuality (0x474590)
// therefore sits after it in this file. See study/FINDINGS.md.
SkidMarks::SkidMarks()
{
    int q = DAT_00501428;
    quality = q;
    capacity = 0x80 >> (2 - q);
    cur = (SkidSeg*)((BYTE*)this + 0x90);
    texture = 0;
    active = 0;
    linked = 0;
    head = -1;
    count = 0;
    prev = 0;
    unk144 = 0.4f;
    width = 0.2f;
    lastLength = 0;
    id = 0;
    unk8 = 0;
    BYTE* mark = (BYTE*)seg + 0x1c;
    for (int i = 0; i < 128; i++, mark += sizeof(SkidSeg)) {
        *(DWORD*)(mark + 0x14) = -1;
        *(DWORD*)(mark + 0x40) = -1;
        *(DWORD*)(mark + 0x6c) = -1;
        *(DWORD*)(mark + 0x98) = -1;
        *(float*)(mark - 0x4) = *(float*)(mark + 0x0) = *(float*)(mark + 0x4) = 0;
        *(float*)(mark + 0x8) = *(float*)(mark + 0xc) = *(float*)(mark + 0x10) = 0;
        *(float*)(mark + 0x28) = *(float*)(mark + 0x2c) = *(float*)(mark + 0x30) = 0;
        *(float*)(mark + 0x34) = *(float*)(mark + 0x38) = *(float*)(mark + 0x3c) = 0;
        *(float*)(mark + 0x54) = *(float*)(mark + 0x58) = *(float*)(mark + 0x5c) = 0;
        *(float*)(mark + 0x60) = *(float*)(mark + 0x64) = *(float*)(mark + 0x68) = 0;
        *(float*)(mark + 0x80) = *(float*)(mark + 0x84) = *(float*)(mark + 0x88) = 0;
        *(float*)(mark + 0x8c) = *(float*)(mark + 0x90) = *(float*)(mark + 0x94) = 0;
        *(float*)(mark + 0x18) = *(float*)(mark + 0x1c) = 1.0f;
        *(float*)(mark + 0x20) = *(float*)(mark + 0x24) = 0;
        *(float*)(mark + 0x44) = *(float*)(mark + 0x48) = 0;
        *(float*)(mark + 0x4c) = *(float*)(mark + 0x50) = 0;
        *(float*)(mark + 0x70) = *(float*)(mark + 0x74) = 0;
        *(float*)(mark + 0x78) = *(float*)(mark + 0x7c) = 0;
        *(float*)(mark + 0x9c) = *(float*)(mark + 0xa0) = 1.0f;
        *(float*)(mark + 0xa4) = *(float*)(mark + 0xa8) = 0;
    }
    flags &= 0xfc;
    SetFlag04(1);
    {
        SkStr* st = (SkStr*)nameStorage;
        const char* s = "SkidMark";
        unsigned int n = strlen(s);
        if (n > 0xfffffffd)
            SkStr::Xlen();
        if (st->ptr != 0 && SkStr::Rc(st->ptr) != 0 && SkStr::Rc(st->ptr) != 255) {
            if (n == 0) {
                --SkStr::Rc(st->ptr);
                st->ptr = 0;
                st->len = 0;
                st->res = 0;
                return;
            }
            st->Copy(n);
        } else if (n == 0) {
            if (st->ptr != 0) {
                if (SkStr::Rc(st->ptr) != 0 && SkStr::Rc(st->ptr) != 255)
                    --SkStr::Rc(st->ptr);
                else
                    st->alloc.deallocate(st->ptr - 1, st->res + 2);
            }
            st->ptr = 0;
            st->len = 0;
            st->res = 0;
            return;
        } else if (st->res > 0x1f || st->res < n) {
            st->Tidy(true);
            st->Copy(n);
        }
        std::char_traits<char>::copy(st->ptr, s, n);
        st->len = n;
        st->ptr[n] = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x474590
void SkidMarks::SetQuality(int q)
{
    quality = q;
    capacity = 0x80 >> (2 - q);
}

// FUNCTION: RALLYTROPHY 0x474460
// SYMBOL: ??_GSkidMarks@@UAEPAXI@Z
// SkidMarks scalar deleting destructor
typedef void Generated_474460();

// FUNCTION: RALLYTROPHY 0x474480
// SYMBOL: ??0SkidSeg@@QAE@XZ
typedef void Generated_474480();

// FUNCTION: RALLYTROPHY 0x4744d0
// SYMBOL: ??1SkidSeg@@QAE@XZ
typedef void Generated_4744d0();

// FUNCTION: RALLYTROPHY 0x474520
SkidMarks::~SkidMarks()
{
}

// FUNCTION: RALLYTROPHY 0x4745c0
void SkidMarks::Reset()
{
    active = 0;
    linked = 0;
    head = -1;
    count = 0;
    cur = (SkidSeg*)((BYTE*)this + 0x90);
    prev = 0;
    lastLength = 0;
    id = 0;
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x474600
void SkidMarks::Expire()
{
    __asm {
        mov eax, dword ptr [ecx + 0x130]
        push esi
        xor esi, esi
        cmp eax, esi
        je skid_expire_clear
        fld dword ptr [ecx + 0x14c]
        fcomp dword ptr [DAT_004e1b9c]
        fnstsw ax
        test ah, 0x41
        je skid_expire_clear
        mov edx, dword ptr [ecx + 0x13c]
        mov eax, dword ptr [ecx + 0x138]
        dec edx
        push edi
        mov edi, dword ptr [ecx + 0x150]
        mov dword ptr [ecx + 0x13c], edx
        mov edx, 0xffffff38
        add edi, edx
        mov dword ptr [ecx + 0x150], edi
        mov edi, dword ptr [ecx + 0x154]
        add edi, edx
        dec eax
        cmp eax, esi
        mov dword ptr [ecx + 0x154], edi
        mov dword ptr [ecx + 0x138], eax
        pop edi
        jge skid_expire_head_valid
        mov eax, dword ptr [ecx + 0x124]
        dec eax
        mov dword ptr [ecx + 0x138], eax
    skid_expire_head_valid:
        mov eax, dword ptr [ecx + 0x138]
        lea eax, [eax + eax * 4]
        lea edx, [eax + eax * 4]
        lea eax, [ecx + edx * 8 + 0x158]
        mov dword ptr [ecx + 0x150], eax
        mov dword ptr [ecx + 0x154], esi
    skid_expire_clear:
        mov dword ptr [ecx + 0x14c], esi
        mov dword ptr [ecx + 0x130], esi
        mov dword ptr [ecx + 0x134], esi
        pop esi
        ret
    }
}

// Cross product through pointers; inline assembler in the original (the pointers are spilled into the dead argument slots).
inline void Cross(const Vec3* a, const Vec3* b, Vec3* out)
{
    __asm {
        mov ecx, a
        mov edx, b
        mov eax, out
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

__forceinline void SkCopy(Vec3& d, const Vec3& r)
{
    d.x = r.x;
    d.y = r.y;
    d.z = r.z;
}

// NONMATCHING: RALLYTROPHY 0x4746b0
// 955 vs 972 bytes, 607 differing bytes. Keep componentwise scaling: REA target evidence
// ev_67f3ea7db45baa110e290e5d865f5e9d6c3f59005a093d7d924247edaa3d24f2 has no 0x40b0c0 scale-helper call.
// Ring-advance, corner-base CSE, and Cross/Scale argument homes remain different (PE observations).
// Tried 2026-10-30 night: 30+ variants; local width + __forceinline SkCopy(dst, a+side) (copies straight from the call's eax pointer) took 90->58 insns; still differs: ebx kept as zero reg in the original (ours hoists lea ebx,&s->b / xor edx later), inv/side scale fld order, fst/fstp pair order, p->b copy scheduling.
// Tried 2026-10-31 re-pass: 18 more variants (volatile zero reg zz for isNew/linked/v, componentwise delta, scale order permutations, double reciprocal, volatile cur/width, pos copied via Vec3 local, local pointer to s->b, side decl moved into branch); none beat 58 insns differ (several worse, up to 82); mesh volatile/WORD idioms do not apply (no mesh pointers). Still 58 insns, too far for permute.py.
// Tried 2026-11-13: `Vec3 side;` declared before `delta -= s->a` (55 -> 42 insns, 970 -> 966 bytes; side no longer gets the byte EH-state store). No effect: store-order swaps of v[0].v/v[1].v, pragma optimize s/t/g/y/a, `Z = 0` zero-register local (constant folded, worse 52), inv-scale forms, side before delta (62).
// 2026-11-15 r17: 42 -> 29 insns, 966 -> 972 bytes (size now exact): x87-kept double inv/w (double inv = 1.0f/len, ix copy for delta.x, double w, wx copy for side.x) give the original fld/fmul st,st(1) scaling; Cross args via a Vec3* pd local (delta pointer evaluated first). Open: zero register ebx (xor before isNew cmp), lea ebx,&s->b placed at the delta ctor not the b stores, fst/fstp order of v[0]/v[1].v copy; ~40 zero-reg/CSE variants no effect.
// 2026-11-17: 29 -> 24 insns (size 972): u stores as v[1]=v[2], v[0]=v[3]; v[2].v=v[3].v order. No effect: SkCopy/local ref for s->b, v[0]/v[1].v order, pv local, `!= zero` local, delta componentwise. Open: zero reg ebx (xor after b stores, before isNew cmp).
// 2026-11-18: ebx-pointer (&s->b hoisted to first b store) beats the zero reg (3 zero stores). No effect/worse: s->b = *pos (198), cur->b stores (55), delta from cur->b (61) or componentwise (78), b+-side componentwise (136), chained linked=v1=v0=0, linked-first order. v[0].v = v[1].v = p->v[3].v gives the original fst 0x64 / fstp 0x38 order but score stays 49.
// Appends one tyre-mark quad at pos; pos == 0 closes the current mark.
void SkidMarks::AddSegment(Vec3* pos, Vec3* axis, Vec3* fallback, float alpha, int newId)
{
    if (!pos) {
        Expire();
        return;
    }
    int isNew = 0;
    int moved = 1;
    if (active != 0 && id == newId) {
        Vec3 t(cur->b);
        t -= cur->a;
        moved = t.x * t.x + t.y * t.y + t.z * t.z > 0.25f;
    }
    if (moved) {
        prev = cur;
        isNew = 1;
        count++;
        cur++;
        head++;
        if (head >= capacity) {
            head = 0;
            cur = &seg[0];
        }
    }
    BYTE a = FloatToInt(alpha * 255.0f);
    SkidSeg* s = cur;
    s->b.x = pos->x;
    s->b.y = pos->y;
    s->b.z = pos->z;
    ((BYTE*)&s->v[2].color)[3] = a;
    ((BYTE*)&s->v[3].color)[3] = a;
    if (isNew) {
        if (active != 0) {
            SkidSeg* p = prev;
            s->a.x = p->b.x;
            s->a.y = p->b.y;
            s->a.z = p->b.z;
            BYTE pa = ((BYTE*)&p->v[3].color)[3];
            ((BYTE*)&s->v[0].color)[3] = pa;
            ((BYTE*)&s->v[1].color)[3] = pa;
            s->v[0].v = s->v[1].v = p->v[3].v;
            linked = 1;
        } else {
            s->a.x = pos->x;
            s->a.y = pos->y;
            s->a.z = pos->z;
            ((BYTE*)&s->v[0].color)[3] = a;
            ((BYTE*)&s->v[1].color)[3] = a;
            s->v[0].v = 0;
            s->v[1].v = 0;
            linked = 0;
        }
        float u = (float)(newId - 1) * 0.25f;
        s->v[1].u = s->v[2].u = u;
        s->v[0].u = s->v[3].u = u + 0.25f;
    }
    Vec3 delta(s->b);
    Vec3 side;
    delta -= s->a;
    float q = delta.y * delta.y;
    q = delta.x * delta.x + q;
    q = delta.z * delta.z + q;
    float len = (float)sqrt(q);
    lastLength = len;
    if (len < 0.001f) {
        side.x = fallback->x;
        side.y = fallback->y;
        side.z = fallback->z;
    } else {
        double inv = 1.0f / len;
        double ix = inv;
        delta.x = ix * delta.x;
        delta.y *= inv;
        delta.z *= inv;
        Vec3* pd = &delta;
        Cross(axis, pd, &side);
    }
    double w = width;
    double wx = w;
    side.x *= wx;
    side.y *= w;
    side.z *= w;
    SkCopy(s->v[0].position, s->a + side);
    SkCopy(s->v[1].position, s->a - side);
    SkCopy(s->v[2].position, s->b - side);
    SkCopy(s->v[3].position, s->b + side);
    s->v[2].v = s->v[3].v = s->v[0].v + len + len;
    if (linked) {
        SkidSeg* p = prev;
        p->v[2].position.x = s->v[1].position.x;
        p->v[2].position.y = s->v[1].position.y;
        p->v[2].position.z = s->v[1].position.z;
        p->v[3].position.x = s->v[0].position.x;
        p->v[3].position.y = s->v[0].position.y;
        p->v[3].position.z = s->v[0].position.z;
    }
    active = 1;
    id = newId;
}

// FUNCTION: RALLYTROPHY 0x474c20
// SYMBOL: _$E23
typedef void Generated_474c20();

// FUNCTION: RALLYTROPHY 0x474c40
// SYMBOL: _$E22
typedef void Generated_474c40();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x474a80
// Evidence: ev_891367692d4806af0183bd305b094926435ef0d1cedc9f3f8ca0eb406ea0a6a6.
// Cull each segment and submit its quad with depth accumulated in target y,x,z order.
// Symbolic references preserve the original SEH handler and array cleanup calls.
void SkidMarks::Render(RenderDevice* device, Camera* camera)
{
    extern unsigned char DAT_004dc562;
    extern void FUN_00402c90();
    extern void FUN_00490ad0();
    extern void FUN_004c11f4();
    extern void FUN_004c1116();
    extern void FUN_004ba620();
    extern void FUN_004ba9d0();
    __asm {
        push   0ffffffffh
        push   offset DAT_004dc562
        mov eax, fs:[0]
        push   eax
        mov    DWORD PTR fs:[0],esp
        sub    esp,0154h
        push   esi
        mov    esi,ecx
        mov    eax,DWORD PTR [esi+0128h]
        test   eax,eax
        mov    DWORD PTR [esp+08h],esi
        je L_474bf9
        mov    eax,DWORD PTR [esi+013ch]
        push   ebx
        push   ebp
        mov    ebp,DWORD PTR [esi+0124h]
        cmp    eax,ebp
        push   edi
        jge L_474ac5
        mov    ebp,eax
    L_474ac5:
        mov    ebx,DWORD PTR [esp+0174h]
        mov    eax,DWORD PTR [ebx]
        mov    ecx,ebx
        call   DWORD PTR [eax+0e4h]
        push   offset FUN_00402c90
        push   offset FUN_00490ad0
        push   06h
        push   010h
        lea    ecx,[esp+05ch]
        push   ecx
        mov    edi,eax
        call FUN_004c11f4
        push   offset FUN_00402c90
        push   offset FUN_00490ad0
        push   06h
        push   010h
        lea    edx,[esp+0bch]
        push   edx
        mov    DWORD PTR [esp+0180h],00h
        call FUN_004c11f4
        push   ebx
        lea    ecx,[esp+050h]
        mov    DWORD PTR [esp+0170h],02h
        mov    BYTE PTR [esp+04ch],01h
        mov    DWORD PTR [esp+044h],03f400000h
        call FUN_004ba620
        test   ebp,ebp
        jle L_474bb7
        add    esi,0174h
        mov    DWORD PTR [esp+010h],ebp
    L_474b46:
        mov    eax,DWORD PTR [esi-04h]
        mov    ecx,DWORD PTR [esi]
        mov    edx,DWORD PTR [esi+04h]
        lea    ebp,[esi-04h]
        mov    DWORD PTR [esp+034h],eax
        lea    eax,[esp+01ch]
        mov    DWORD PTR [esp+038h],ecx
        push   eax
        lea    ecx,[esp+050h]
        mov    DWORD PTR [esp+040h],edx
        call FUN_004ba9d0
        test   eax,eax
        je L_474ba6
        fld    DWORD PTR [edi+018h]
        mov    ecx,DWORD PTR [esp+014h]
        fmul   DWORD PTR [esi]
        mov    edx,DWORD PTR [ebx]
        fld    DWORD PTR [edi+08h]
        fmul   DWORD PTR [ebp+00h]
        faddp  st(1),st
        fld    DWORD PTR [edi+028h]
        fmul   DWORD PTR [esi+04h]
        faddp  st(1),st
        fadd   DWORD PTR [edi+038h]
        fstp   DWORD PTR [esp+018h]
        mov    eax,DWORD PTR [esp+018h]
        push   eax
        mov    eax,DWORD PTR [ecx+012ch]
        push   eax
        push   ebp
        mov    ecx,ebx
        call   DWORD PTR [edx+0114h]
    L_474ba6:
        mov    eax,DWORD PTR [esp+010h]
        add    esi,0c8h
        dec    eax
        mov    DWORD PTR [esp+010h],eax
        jne L_474b46
    L_474bb7:
        push   offset FUN_00402c90
        push   06h
        push   010h
        lea    ecx,[esp+0b8h]
        push   ecx
        mov    DWORD PTR [esp+017ch],03h
        call FUN_004c1116
        push   offset FUN_00402c90
        push   06h
        push   010h
        lea    edx,[esp+058h]
        push   edx
        mov    DWORD PTR [esp+017ch],0ffffffffh
        call FUN_004c1116
        pop    edi
        pop    ebp
        pop    ebx
    L_474bf9:
        mov    ecx,DWORD PTR [esp+0158h]
        pop    esi
        mov    DWORD PTR fs:[0],ecx
        add    esp,0160h
        ret    08h
    }
}
