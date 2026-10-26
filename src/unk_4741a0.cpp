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

// FUNCTION: RALLYTROPHY 0x474590
void SkidMarks::SetQuality(int q)
{
    quality = q;
    capacity = 0x80 >> (2 - q);
}

// NONMATCHING: RALLYTROPHY 0x4741a0
// 561 of 696 bytes: the fill loop's induction pointer sits at the 2nd-referenced field (v1.color here, v0.position.y in
// the original) so every offset differs, and the inline std::string assign keeps its three duplicated epilogues there.
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
    for (int i = 0; i < 128; i++) {
        EffectVertex* v = seg[i].v;
        v[0].color = -1;
        v[1].color = -1;
        v[2].color = -1;
        v[3].color = -1;
        v[0].position.x = v[0].position.y = v[0].position.z = 0;
        v[0].normal.x = v[0].normal.y = v[0].normal.z = 0;
        v[1].position.x = v[1].position.y = v[1].position.z = 0;
        v[1].normal.x = v[1].normal.y = v[1].normal.z = 0;
        v[2].position.x = v[2].position.y = v[2].position.z = 0;
        v[2].normal.x = v[2].normal.y = v[2].normal.z = 0;
        v[3].position.x = v[3].position.y = v[3].position.z = 0;
        v[3].normal.x = v[3].normal.y = v[3].normal.z = 0;
        v[0].u = v[0].v = 1.0f;
        v[0].u2 = v[0].v2 = 0;
        v[1].u = v[1].v = 0;
        v[1].u2 = v[1].v2 = 0;
        v[2].u = v[2].v = 0;
        v[2].u2 = v[2].v2 = 0;
        v[3].u = v[3].v = 1.0f;
        v[3].u2 = v[3].v2 = 0;
    }
    flags &= 0xfc;
    SetFlag04(1);
    Name() = "SkidMark";
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

// Cross product through pointers (the original spills the pointers into the dead argument slots).
inline void Cross(const Vec3* a, const Vec3* b, Vec3* out)
{
    out->x = a->y * b->z - a->z * b->y;
    out->y = a->z * b->x - a->x * b->z;
    out->z = a->x * b->y - a->y * b->x;
}

// NONMATCHING: RALLYTROPHY 0x4746b0
// 955 of 972 bytes. Matches up to the ring advance. After that VC6 keeps &s->b / &s->v[k].position in lea'd registers (CSE)
// where the original addresses esi+const, and the original spills the Cross/Scale pointer arguments into dead parameter slots.
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
            s->v[1].v = s->v[0].v = p->v[3].v;
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
        s->v[2].u = s->v[1].u = u;
        s->v[3].u = s->v[0].u = u + 0.25f;
    }
    Vec3 delta(s->b);
    delta -= s->a;
    Vec3 side;
    float len = delta.Length();
    lastLength = len;
    if (len < 0.001f) {
        side = *fallback;
    } else {
        float inv = 1.0f / len;
        delta.x *= inv;
        delta.y *= inv;
        delta.z *= inv;
        Cross(axis, &delta, &side);
    }
    side.x *= width;
    side.y *= width;
    side.z *= width;
    s->v[0].position = s->a + side;
    s->v[1].position = s->a - side;
    s->v[2].position = s->b - side;
    s->v[3].position = s->b + side;
    s->v[3].v = s->v[2].v = s->v[0].v + len + len;
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
