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

// NONMATCHING: RALLYTROPHY 0x474a80
// 401 of 401 bytes, 6 differ: the three depth terms are summed y, z, x here (VC6 canonicalises the order whatever the source
// order) but y, x, z in the original.
// Draws the live quads that pass the visibility test, depth-sorted by the view z.
void SkidMarks::Render(RenderDevice* device, Camera* camera)
{
    if (quality == 0) {
        return;
    }
    int n = capacity;
    if (count < n) {
        n = count;
    }
    const float* m = device->GetViewMatrix();
    SkidFrustum frustum;
    SkidSphere sphere;
    sphere.flag = 1;
    sphere.radius = 0.75f;
    frustum.Init(device);
    SkidSeg* s = seg;
    for (int i = n; i > 0; i--, s++) {
        sphere.center[0] = s->v[0].position.x;
        sphere.center[1] = s->v[0].position.y;
        sphere.center[2] = s->v[0].position.z;
        if (frustum.Test(&sphere)) {
            float depth = m[6] * s->v[0].position.y + m[2] * s->v[0].position.x + m[10] * s->v[0].position.z + m[14];
            device->DrawQuad(s->v, texture, depth);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x474c20
// SYMBOL: _$E23
typedef void Generated_474c20();

// FUNCTION: RALLYTROPHY 0x474c40
// SYMBOL: _$E22
typedef void Generated_474c40();
