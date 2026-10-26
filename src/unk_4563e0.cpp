// ViewManager camera file (0x4563e0-0x4581ef, module end 0x4581c0): the camera preset table, the ViewManager
// constructor/destructor and its per-frame helpers, the camera step, the camera offsets, the preset loader. Map:
// analysis/range-4563e0.md section 1; layout: ViewManager and MirrorView in include/unk_481390.h, CameraPreset in
// include/unk_4563e0.h.
// Work groups: G1/G1b (statics, 0x456490-0x4569e0), G2 (0x456b10; helpers G2_), G3 (0x4578c0, 0x457b10, 0x458150, module
// end; helpers G3).
#include <string>
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46a8a0.h"
#include "unk_465e90.h"
#include "unk_4563e0.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// ---- G1: statics ----

// The nine camera presets (static array with an empty constructor and destructor).
CameraPreset CameraTable::presets[9];

// An empty static object: the constructor and the destructor are empty functions (0x456460, 0x456480).
struct CameraEmpty {
    CameraEmpty() {}
    ~CameraEmpty() {}
};
CameraEmpty g_cameraEmpty;

float DAT_00508c98, DAT_00508c9c, DAT_00508ca0;
extern "C" float dat_00508d68;
extern "C" float dat_00508d6c;
extern "C" float dat_00508d70;
extern "C" float dat_00508d74;
extern "C" float dat_00508d78;
extern "C" float dat_00508d7c;

// ---- G1 helpers ----

// The Car as the camera code sees it: the body at +0x24 with the position at +0x14.
struct G1_Body {
    BYTE unk0[0x14];
    Vec3 position;                     // 0x14
};
struct G1_Car {
    BYTE unk0[0x24];
    G1_Body* body;                     // 0x24
    void SetCockpit(int cockpit);      // 0x4691e0
};
// The Stage's camera-zone lookup (the zone object has the value copied into the view at +0x130).
struct G1_Zone {
    BYTE unk0[0x130];
    int value;                         // 0x130
};
struct G1_Stage {
    G1_Zone* FindZone(const Vec3* position); // 0x414870
};

// The std::vector<Car*> of the ViewManager as the constructor expands vector::reserve inline: _Construct (0x403140)
// stays a call.
struct G1_PtrVec {
    BYTE alloc;
    Car** first;                       // 0x04
    Car** last;                        // 0x08
    Car** end;                         // 0x0c
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    unsigned int Capacity() const { return first == 0 ? 0 : end - first; }
};
void G1_ConstructPtr(Car** p, Car* const& v); // 0x403140 (std::_Construct, cdecl)

// Compiler-generated pieces of the two file-scope statics (four per static: init-table wrapper, constructor,
// atexit registration, destructor thunk). Symbols: objdump -t.

// FUNCTION: RALLYTROPHY 0x4563e0
// SYMBOL: _$E23
typedef void Generated_4563e0();

// FUNCTION: RALLYTROPHY 0x4563f0
// SYMBOL: _$E19
typedef void Generated_4563f0();

// FUNCTION: RALLYTROPHY 0x456410
// SYMBOL: _$E22
typedef void Generated_456410();

// FUNCTION: RALLYTROPHY 0x456420
// SYMBOL: _$E20
typedef void Generated_456420();

// FUNCTION: RALLYTROPHY 0x456450
// SYMBOL: _$E28
typedef void Generated_456450();

// FUNCTION: RALLYTROPHY 0x456460
// SYMBOL: _$E25
typedef void Generated_456460();

// FUNCTION: RALLYTROPHY 0x456470
// SYMBOL: _$E27
typedef void Generated_456470();

// FUNCTION: RALLYTROPHY 0x456480
// SYMBOL: _$E26
typedef void Generated_456480();

// The std::vector<int> of the ViewManager as the constructor expands vector::reserve inline (the STL source of
// VC6): _Construct (0x403140) stays a call, _Destroy of the ints expands to nothing.
void G1_ConstructInt(int* p, const int& v); // 0x403140 (std::_Construct, cdecl; out of line in the original)

struct G1_IntVec {
    std::allocator<int> allocator;
    int* first;                        // 0x04
    int* last;                         // 0x08
    int* end;                          // 0x0c
    unsigned int Size() const { return first == 0 ? 0 : last - first; }
    unsigned int Capacity() const { return first == 0 ? 0 : end - first; }
    int* Ucopy(const int* f, const int* l, int* p)
    {
        for (; f != l; ++p, ++f)
            G1_ConstructInt(p, *f);
        return p;
    }
    void Destroy(int* f, int* l)
    {
        for (; f != l; ++f)
            ;
    }
    void Reserve(unsigned int n)
    {
        if (Capacity() < n) {
            int* s = allocator.allocate(n, (void*)0);
            Ucopy(first, last, s);
            Destroy(first, last);
            allocator.deallocate(first, end - first);
            end = s + n;
            last = s + Size();
            first = s;
        }
    }
};

// FUNCTION: RALLYTROPHY 0x456490
// SYMBOL: ??0ViewManager@@QAE@XZ
// Memberwise copy of a Vec3 temporary (field by field from the returned object).
inline void G1_Set(Vec3& d, const Vec3& s)
{
    d.x = s.x;
    d.y = s.y;
    d.z = s.z;
}

// original: FUN_00456490
ViewManager::ViewManager()
{
    local = 1;
    target = 0;
    stage = 0;
    view = 0;
    reset = 1;
    autoZone = 0;
    zone = 0;
    prevZone = 0;
    zoneCount = 0;
    ((G1_IntVec*)&buffer.v)->Reserve(0x400);
    DAT_00508c98 = 0.0f;
    DAT_00508c9c = -0.1f;
    DAT_00508ca0 = 10.0f;
}
// FUNCTION: RALLYTROPHY 0x456600
// SYMBOL: ??_GViewManager@@UAEPAXI@Z
// Scalar deleting destructor (vtable slot 0).
typedef void Generated_456600();

// original: FUN_00456620
// FUNCTION: RALLYTROPHY 0x456620
ViewManager::~ViewManager()
{
}

// original: FUN_00456660
// FUNCTION: RALLYTROPHY 0x456660
// Sets the followed Car and Stage; the previous Car leaves the cockpit view.
void ViewManager::FUN_00456660(Car* car, Stage* s)
{
    if (target && car != target)
        ((G1_Car*)target)->SetCockpit(0);
    if (!local && view == 8)
        view = 7;
    target = car;
    stage = s;
}

// original: FUN_004566e0
// FUNCTION: RALLYTROPHY 0x4566e0
// Places the camera at the preset of the current view and restarts the smoothing.
void ViewManager::FUN_004566e0()
{
    G1_Body* body = ((G1_Car*)target)->body;
    FUN_004578c0((float*)&eye, (float*)&look);
    Vec3* pos = &body->position;
    G1_Set(smoothEye, eye - *pos);
    G1_Set(smoothLook, look - *pos);
    reset = 1;
    FUN_00456b10(0);
}

// original: FUN_00456780
// FUNCTION: RALLYTROPHY 0x456780
// Selects the camera view 0..8, or -1 for the automatic zone-driven selection.
void ViewManager::FUN_00456780(int v)
{
    if (v == -1) {
        autoZone = 1;
        zone = 0;
        prevZone = 0;
        if (!local && view == 8)
            view = 7;
    } else {
        autoZone = 0;
        zone = 0;
        prevZone = 0;
        view = v;
        FUN_004566e0();
        if (v == 8 && target) {
            ((G1_Car*)target)->SetCockpit(1);
            goto done;
        }
    }
    if (target)
        ((G1_Car*)target)->SetCockpit(0);
done:
    resetSmoothing = 0;
    reset = 1;
}

__declspec(naked)
// PE transcription preserves x87 stack scheduling in the final smooth-look update.
// FUNCTION: RALLYTROPHY 0x456820
void ViewManager::FUN_00456820(float)
{
    __asm {
        _emit 064h
        _emit 0a1h
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 06ah
        _emit 0ffh
        _emit 068h
        _emit 030h

        _emit 0a7h
        _emit 04dh
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
        _emit 034h
        _emit 056h

        _emit 08bh
        _emit 0f1h
        _emit 08bh
        _emit 086h
        _emit 0cch

        _emit 001h
        _emit 000h
        _emit 000h
        _emit 057h
        _emit 08bh

        _emit 078h
        _emit 024h
        _emit 08bh
        _emit 086h
        _emit 0e4h

        _emit 001h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0c0h

        _emit 074h
        _emit 00eh
        _emit 08bh
        _emit 086h
        _emit 0e8h

        _emit 001h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0c0h

        _emit 00fh
        _emit 085h
        _emit 06ah
        _emit 001h
        _emit 000h

        _emit 000h
        _emit 053h
        _emit 055h
        _emit 08dh
        _emit 04ch

        _emit 024h
        _emit 020h
        _emit 051h
        _emit 08dh
        _emit 054h

        _emit 024h
        _emit 018h
        _emit 052h
        _emit 08bh
        _emit 0ceh

        _emit 0c7h
        _emit 044h
        _emit 024h
        _emit 054h
        _emit 001h

        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0e8h
        _emit 048h

        _emit 010h
        _emit 000h
        _emit 000h
        _emit 08bh
        _emit 086h

        _emit 0d4h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0c1h

        _emit 0e0h
        _emit 005h
        _emit 08bh
        _emit 088h
        _emit 0c0h

        _emit 08ch
        _emit 050h
        _emit 000h
        _emit 089h
        _emit 04ch

        _emit 024h
        _emit 010h
        _emit 0d9h
        _emit 044h
        _emit 024h

        _emit 010h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 054h

        _emit 0e8h
        _emit 058h
        _emit 0b1h
        _emit 006h
        _emit 000h

        _emit 0d8h
        _emit 02dh
        _emit 048h
        _emit 013h
        _emit 04eh

        _emit 000h
        _emit 083h
        _emit 0c7h
        _emit 014h
        _emit 08dh

        _emit 0aeh
        _emit 008h
        _emit 002h
        _emit 000h
        _emit 000h

        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 054h
        _emit 055h

        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 08dh

        _emit 054h
        _emit 024h
        _emit 030h
        _emit 0d8h
        _emit 027h

        _emit 052h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 01ch

        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 01ch
        _emit 0d9h

        _emit 044h
        _emit 024h
        _emit 020h
        _emit 0d8h
        _emit 067h

        _emit 004h
        _emit 0d9h
        _emit 05ch
        _emit 024h
        _emit 020h

        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 0d8h

        _emit 067h
        _emit 008h
        _emit 0d9h
        _emit 05ch
        _emit 024h

        _emit 024h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 028h

        _emit 0d8h
        _emit 027h
        _emit 0d9h
        _emit 05ch
        _emit 024h

        _emit 028h
        _emit 0d9h
        _emit 044h
        _emit 024h
        _emit 02ch

        _emit 0d8h
        _emit 067h
        _emit 004h
        _emit 0d9h
        _emit 05ch

        _emit 024h
        _emit 02ch
        _emit 0d9h
        _emit 044h
        _emit 024h

        _emit 030h
        _emit 0d8h
        _emit 067h
        _emit 008h
        _emit 0d9h

        _emit 05ch
        _emit 024h
        _emit 030h
        _emit 0e8h
        _emit 036h

        _emit 0dah
        _emit 0fbh
        _emit 0ffh
        _emit 08bh
        _emit 04ch

        _emit 024h
        _emit 054h
        _emit 051h
        _emit 08dh
        _emit 054h

        _emit 024h
        _emit 03ch
        _emit 052h
        _emit 08bh
        _emit 0c8h

        _emit 0c6h
        _emit 044h
        _emit 024h
        _emit 054h
        _emit 002h

        _emit 0e8h
        _emit 0b0h
        _emit 047h
        _emit 0fbh
        _emit 0ffh

        _emit 0d9h
        _emit 045h
        _emit 000h
        _emit 0d8h
        _emit 000h

        _emit 08dh
        _emit 09eh
        _emit 014h
        _emit 002h
        _emit 000h

        _emit 000h
        _emit 053h
        _emit 08dh
        _emit 04ch
        _emit 024h

        _emit 024h
        _emit 0d9h
        _emit 05dh
        _emit 000h
        _emit 0c6h

        _emit 044h
        _emit 024h
        _emit 050h
        _emit 001h
        _emit 0d9h

        _emit 040h
        _emit 004h
        _emit 0d8h
        _emit 045h
        _emit 004h

        _emit 0d9h
        _emit 05dh
        _emit 004h
        _emit 0d9h
        _emit 040h

        _emit 008h
        _emit 08dh
        _emit 044h
        _emit 024h
        _emit 03ch

        _emit 0d8h
        _emit 045h
        _emit 008h
        _emit 050h
        _emit 0d9h

        _emit 05dh
        _emit 008h
        _emit 0e8h
        _emit 0ech
        _emit 0d9h

        _emit 0fbh
        _emit 0ffh
        _emit 08bh
        _emit 04ch
        _emit 024h

        _emit 054h
        _emit 051h
        _emit 08dh
        _emit 054h
        _emit 024h

        _emit 030h
        _emit 052h
        _emit 08bh
        _emit 0c8h
        _emit 0c6h

        _emit 044h
        _emit 024h
        _emit 054h
        _emit 003h
        _emit 0e8h

        _emit 066h
        _emit 047h
        _emit 0fbh
        _emit 0ffh
        _emit 0d9h

        _emit 000h
        _emit 0d8h
        _emit 003h
        _emit 0d9h
        _emit 01bh

        _emit 0d9h
        _emit 040h
        _emit 004h
        _emit 055h
        _emit 0d8h

        _emit 043h
        _emit 004h
        _emit 08bh
        _emit 0cfh
        _emit 0c6h

        _emit 044h
        _emit 024h
        _emit 050h
        _emit 001h
        _emit 0d9h

        _emit 05bh
        _emit 004h
        _emit 0d9h
        _emit 040h
        _emit 008h

        _emit 08dh
        _emit 044h
        _emit 024h
        _emit 03ch
        _emit 0d8h

        _emit 043h
        _emit 008h
        _emit 050h
        _emit 0d9h
        _emit 05bh

        _emit 008h
        _emit 0e8h
        _emit 07ch
        _emit 035h
        _emit 0fch

        _emit 0ffh
        _emit 08bh
        _emit 008h
        _emit 08bh
        _emit 050h

        _emit 004h
        _emit 08bh
        _emit 040h
        _emit 008h
        _emit 089h

        _emit 08eh
        _emit 0f0h
        _emit 001h
        _emit 000h
        _emit 000h

        _emit 053h
        _emit 08dh
        _emit 04ch
        _emit 024h
        _emit 03ch

        _emit 051h
        _emit 08bh
        _emit 0cfh
        _emit 089h
        _emit 096h

        _emit 0f4h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 089h

        _emit 086h
        _emit 0f8h
        _emit 001h
        _emit 000h
        _emit 000h

        _emit 0e8h
        _emit 055h
        _emit 035h
        _emit 0fch
        _emit 0ffh

        _emit 08bh
        _emit 010h
        _emit 08bh
        _emit 048h
        _emit 004h

        _emit 089h
        _emit 096h
        _emit 0fch
        _emit 001h
        _emit 000h

        _emit 000h
        _emit 08bh
        _emit 050h
        _emit 008h
        _emit 05dh

        _emit 089h
        _emit 08eh
        _emit 000h
        _emit 002h
        _emit 000h

        _emit 000h
        _emit 089h
        _emit 096h
        _emit 004h
        _emit 002h

        _emit 000h
        _emit 000h
        _emit 05bh
        _emit 08bh
        _emit 04ch

        _emit 024h
        _emit 03ch
        _emit 05fh
        _emit 05eh
        _emit 064h

        _emit 089h
        _emit 00dh
        _emit 000h
        _emit 000h
        _emit 000h

        _emit 000h
        _emit 083h
        _emit 0c4h
        _emit 040h
        _emit 0c2h

        _emit 004h
        _emit 000h
    }
}

// original: FUN_004569e0
// FUNCTION: RALLYTROPHY 0x4569e0
// Zone scan: with the automatic camera, finds the camera zone at the car position and, after the same zone has been
// seen for more than five frames, switches to it (no zone: a random preset).
void ViewManager::FUN_004569e0()
{
    if (autoZone) {
        G1_Body* body = ((G1_Car*)target)->body;
        Vec3 position(body->position);
        G1_Zone* z = ((G1_Stage*)stage)->FindZone(&position);
        if (z != zone) {
            if (z != prevZone) {
                zoneCount = 0;
            } else if (++zoneCount > 5) {
                reset = 1;
                resetSmoothing = 0;
                zone = z;
                zoneCount = 0;
                if (z == 0) {
                    if ((rand() & 0xff) > 0x80)
                        view = 8;
                    else
                        view = rand() % 9;
                    if (!local && view == 8)
                        view = 7;
                    FUN_004566e0();
                } else {
                    zoneValue = z->value;
                }
            }
            prevZone = z;
        }
    }
}

// ---- G2: camera step ----

// A plain float triple (no ctor/dtor, so it adds no unwind state).
struct G2_P3 {
    float x, y, z;
};

// The car body (Car+0x24) as the camera step reads it.
struct G2_Body {
    BYTE unk0[0x14];
    G2_P3 pos;                         // 0x14
    float matrix[16];                  // 0x20: rows 0x10 apart (up = row at 0x30, forward = row at 0x40)
    BYTE unk60[0xa4 - 0x60];
    float unka4;                       // 0xa4
    float unka8;                       // 0xa8
    BYTE unkac[0xb0 - 0xac];
    float unkb0;                       // 0xb0
    float unkb4;                       // 0xb4
    G2_P3 unkb8;                       // 0xb8
    float size;                        // 0xc4
};

struct G2_Car {
    BYTE unk0[0x24];
    G2_Body* body;                     // 0x24
    BYTE unk28[0x20b8 - 0x28];
    Vec3 vec20b8;                      // 0x20b8
};

// A camera zone of the stage (Stage zone lookup 0x414870).
struct G2_Zone {
    BYTE unk0[0x14];
    G2_P3 pos;                         // 0x14
    float matrix[16];                  // 0x20
    BYTE unk60[0x12c - 0x60];
    float fov;                         // 0x12c
    int flag130;                       // 0x130
    BYTE unk134[0x1c8 - 0x134];
    BYTE flag1c8;                      // 0x1c8
};

// The stage's collision grid (ViewManager+0x1d0): collects the triangles near a sphere.
struct G2_Trace;
struct G2_Sphere {
    G2_Sphere() {}
    ~G2_Sphere() {}
    Vec3 center;
    float radius;
};
class G2_Grid {
public:
    void CollectNear(G2_Trace* out, G2_Sphere* sphere); // 0x411520
};

// The 0x34-byte query buffer at ViewManager+0x220 (a CarTrace: vector + box + sphere pointer + limit 0x11).
struct G2_Trace {
    void Erase(void* first, void* last); // 0x484380 (vector::erase)
    float Cast(float radius, Vec3* from, Vec3* dir, int a, int b); // 0x418b90
    BYTE alloc[4];                     // 0x00
    void* first;                       // 0x04
    void* last;                        // 0x08
    BYTE unkc[0x14 - 0xc];
    Vec3 boxMin;                       // 0x14
    Vec3 boxMax;                       // 0x20
    G2_Sphere* sphere;                 // 0x2c
    int limit;                         // 0x30
};

// A 4x4 float matrix with an (empty) destructor; the view matrix at ViewManager+0x20 composes with it:
// this = this * m, the translation row of m added (0x4b55e0, thiscall, ret 4).
class G2_Matrix {
public:
    G2_Matrix() {}
    ~G2_Matrix() {}
    void Concat(const G2_Matrix& m);   // 0x4b55e0
    float m[16];
};

class G2_View {
public:
    BYTE unk0[0x14];
    G2_P3 pos;                         // 0x14
    float matrix[16];                  // 0x20: rows right/up/forward at 0x20/0x30/0x40
    BYTE unk60[0x124 - 0x60];
    float nearPlane;                   // 0x124
    BYTE unk128[4];
    float fov;                         // 0x12c
    int flag130;                       // 0x130
    int flag134;                       // 0x134
    G2_P3 savedPos;                    // 0x138
    float prevMatrix[16];              // 0x144
    BYTE unk184[0x1c4 - 0x184];
    float dt;                          // 0x1c4
    BYTE unk1c8[4];
    G2_Car* car;                       // 0x1cc
    G2_Grid* stage;                    // 0x1d0
    int view;                          // 0x1d4
    BYTE unk1d8[0x1e0 - 0x1d8];
    int flag1e0;                       // 0x1e0
    BYTE unk1e4[4];
    G2_Zone* zone;                     // 0x1e8
    BYTE unk1ec[4];
    Vec3 target;                       // 0x1f0
    Vec3 camPos;                       // 0x1fc
    BYTE unk208[0x220 - 0x208];
    G2_Trace trace;                    // 0x220
};

// out = point * mtx (rotation rows 0x10 apart) + trans; inline assembler in the original.
#define G2_TRANSFORM_ADD() \
    __asm mov ecx, point \
    __asm mov edx, mtx \
    __asm mov eax, result \
    __asm mov ebx, trans \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx] \
    __asm fxch st(1) \
    __asm fadd dword ptr [ebx+4] \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx+8] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// dst = a x b of three-float vectors (inline assembler in the original).
#define G2_CROSS() \
    __asm mov ecx, a \
    __asm mov edx, b \
    __asm mov eax, dst \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx] \
    __asm fxch st(2) \
    __asm fsubp st(5), st \
    __asm fsubp st(3), st \
    __asm fsubp st(1), st \
    __asm fxch st(2) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+4] \
    __asm fstp dword ptr [eax+8]

// Near planes of the camera modes: file-scope constants (loaded from memory in the original).
static const float G2_kNearRigid = 0.1f;   // 0x4e1b48
static const float G2_kNearCockpit = 0.15f; // 0x4e1b4c
static const float G2_kNearMount = 0.3f;   // 0x4e1b50

inline float G2_PassFloat(float v)
{
    return v;
}

inline void G2_Normalize(float* p)
{
    float inv = 1.0f / (float)sqrt(p[0] * p[0] + p[1] * p[1] + p[2] * p[2]);
    p[0] *= inv;
    p[1] *= inv;
    p[2] *= inv;
}

// NONMATCHING: RALLYTROPHY 0x456b10
// Same size (3489), logic-complete, EH state numbers (0, 5, 7, 8, 0xa) and the inline-assembler blocks identical; the
// frame slots (the original orders them by use count: d, p0, c, max, hit, min, e, sphere) and the register choice
// differ, and the view-8 normalisation uses fdivr/fst where the original has fld 1.0 / fdiv st,st(1) / fstp.
// Camera step called once per frame by Car::Update (and by FUN_004566e0 with dt 0): saves the previous pose, then
// builds the view pose (V->pos, V->matrix rows right/up/forward, V->fov, V->nearPlane) from the stage camera zone
// (+0x1e8), or from the preset of camera view +0x1d4 (0-5 chase with ray-cast clearance, 6 rigid look-at, 7/8 mounted).
#define V ((G2_View*)this)
void ViewManager::FUN_00456b10(float dt)
{
    float* pos = (float*)&V->pos;
    float* matrix = V->matrix;
    V->dt = dt;
    V->savedPos.x = pos[0];
    V->savedPos.y = pos[1];
    V->savedPos.z = pos[2];
    memcpy(V->prevMatrix, matrix, sizeof(V->matrix));
    if (V->flag1e0) {
        V->flag134 = 0;
        V->flag1e0 = 0;
    } else {
        V->flag134 = 1;
    }
    G2_Car* car = V->car;
    if (car) {
        int cockpit = 0;
        if (V->zone == 0 && V->view == 8) {
            cockpit = 1;
        }
        ((Car*)car)->FUN_004691e0(cockpit);
    }
    if (V->zone) {
        // A stage camera zone: follow the car with the zone's own camera.
        G2_Body* body = car->body;
        G2_P3 local;
        local.x = body->unkb8.x;
        local.y = body->unkb8.y;
        local.z = body->unkb8.z;
        local.z = body->unkb4 * 0.75f + body->unka8 * 0.25f;
        local.y = body->unkb0 * 0.8f + body->unka4 * 0.2f;
        {
            const float* trans = (const float*)&body->pos;
            float* result = (float*)&V->target;
            const float* point = (const float*)&local;
            const float* mtx = body->matrix;
            G2_TRANSFORM_ADD();
        }
        V->camPos.x = V->zone->pos.x;
        V->camPos.y = V->zone->pos.y;
        V->camPos.z = V->zone->pos.z;
        float zoneFov = G2_PassFloat(V->zone->fov);
        V->fov = zoneFov;
        if (V->flag130) {
            Vec3 d(V->target);
            d -= V->camPos;
            float angle = d.Length();
            if (angle > 1.0f) {
                angle = V->car->body->size * 3.0f / angle * 1.5707964f;
                if (angle > zoneFov) {
                    V->fov = zoneFov;
                } else {
                    if (angle < 0.28274336f) {
                        angle = 0.28274336f;
                    }
                    V->fov = angle;
                }
            } else {
                V->fov = 0.28274336f;
            }
        }
        if (!V->zone->flag1c8) {
            memcpy(matrix, V->zone->matrix, sizeof(V->matrix));
            pos[0] = V->zone->pos.x;
            pos[1] = V->zone->pos.y;
            pos[2] = V->zone->pos.z;
            goto finish;
        }
    } else {
        V->fov = CameraTable::presets[V->view].fov;
        if (V->view < 6) {
            // Chase cameras: keep the camera position clear of the track geometry (ray cast).
            G2_Body* body = car->body;
            Vec3 p0;
            {
                const float* trans = (const float*)&body->pos;
                const float* point = (const float*)&body->unkb8;
                float* result = (float*)&p0;
                const float* mtx = body->matrix;
                G2_TRANSFORM_ADD();
            }
            Vec3* cam = &V->camPos;
            Vec3 d(*cam);
            d -= p0;
            Vec3 c(d.x * 0.5f, d.y * 0.5f, d.z * 0.5f);
            c += p0;
            G2_Sphere sph;
            sph.center = c;
            sph.radius = d.Length() * 0.5f;
            Vec3 lo(*cam);
            Vec3 hi(*cam);
            for (int i = 0; i < 3; i++) {
                float* l = &lo.x;
                float* h = &hi.x;
                const float* cp = &cam->x;
                const float* pp = &p0.x;
                float a = cp[i] - 0.1f;
                float b = cp[i] + 0.1f;
                if (a < l[i]) {
                    l[i] = a;
                }
                if (b > h[i]) {
                    h[i] = b;
                }
                a = pp[i] - 0.1f;
                b = pp[i] + 0.1f;
                if (a < l[i]) {
                    l[i] = a;
                }
                if (b > h[i]) {
                    h[i] = b;
                }
            }
            G2_Trace* trace = &V->trace;
            trace->Erase(trace->first, trace->last);
            trace->boxMin = lo;
            trace->boxMax = hi;
            trace->limit = 0x11;
            trace->sphere = &sph;
            V->stage->CollectNear(trace, &sph);
            float t = trace->Cast(0.1f, &p0, &d, 0, 0);
            if (t < 0.999f) {
                Vec3 hit(p0);
                Vec3 e;
                hit += d * t;
                hit.y = cam->y;
                e.x = hit.x - p0.x;
                e.y = hit.y - p0.y;
                e.z = hit.z - p0.z;
                if (trace->Cast(0.1f, &p0, &e, 0, 0) < 0.999f) {
                    *cam = p0;
                    *cam += d * t;
                } else {
                    *cam = hit;
                }
            }
        }
        if (V->view == 8) {
            // Cockpit view: the head moves with the car's acceleration.
            G2_Body* body = V->car->body;
            Vec3 p = *(Vec3*)&CameraTable::presets[8].position;
            p.x -= V->car->vec20b8.x * 0.024f;
            p.y -= V->car->vec20b8.y * 0.027f;
            p.z -= V->car->vec20b8.z * 0.024f;
            {
                const float* trans = (const float*)&body->pos;
                const float* point = (const float*)&p;
                float* result = pos;
                const float* mtx = body->matrix;
                G2_TRANSFORM_ADD();
            }
            memcpy(matrix, body->matrix, sizeof(V->matrix));
            Vec3 n = -V->car->vec20b8;
            n.x = n.x * 0.1f;
            n.y = 1.0f;
            n.z = n.z * 0.05f + 0.02f;
            G2_Normalize(&n.x);
            G2_Matrix rows;
            float f[3];
            f[0] = 0.0f;
            f[1] = 0.0f;
            f[2] = 1.0f;
            {
                float* dst = &rows.m[0];
                const float* b = f;
                const float* a = &n.x;
                G2_CROSS();
            }
            rows.m[4] = n.x;
            rows.m[5] = n.y;
            rows.m[6] = n.z;
            {
                float* dst = &rows.m[8];
                const float* b = &rows.m[4];
                const float* a = &rows.m[0];
                G2_CROSS();
            }
            ((G2_Matrix*)matrix)->Concat(rows);
            V->nearPlane = G2_kNearCockpit;
            return;
        }
        if (V->view == 7) {
            // Mounted camera 2: fixed offset from the car.
            G2_Body* body = V->car->body;
            G2_P3 q = *(G2_P3*)CameraTable::presets[7].position;
            const float* trans = (const float*)&body->pos;
            const float* point = (const float*)&q;
            float* result = pos;
            const float* mtx = body->matrix;
            G2_TRANSFORM_ADD();
            memcpy(matrix, body->matrix, sizeof(V->matrix));
            V->nearPlane = G2_kNearMount;
            return;
        }
        if (V->view == 6) {
            // Rigid mounted camera looking at the car's target point.
            G2_Body* body = V->car->body;
            const float* trans = (const float*)&body->pos;
            const float* point = (const float*)&CameraTable::presets[6].position;
            float* result = pos;
            const float* mtx = body->matrix;
            G2_TRANSFORM_ADD();
            float* row0 = &matrix[0];
            float* row1 = &matrix[4];
            float* row2 = &matrix[8];
            *(Vec3*)row2 = V->target;
            V->nearPlane = G2_kNearRigid;
            *(Vec3*)row2 -= *(Vec3*)pos;
            G2_Normalize(row2);
            const float* up = &body->matrix[4];
            float dot = row2[0] * up[0] + row2[1] * up[1] + row2[2] * up[2];
            if ((float)fabs(dot) < 0.9f) {
                float* dst = row0;
                const float* b = row2;
                const float* a = up;
                G2_CROSS();
                G2_Normalize(row0);
            } else if (dot < 0.0f) {
                float* dst = row0;
                const float* b = row2;
                const float* a = &body->matrix[8];
                G2_CROSS();
            } else {
                Vec3 neg = -*(Vec3*)&body->matrix[8];
                float* dst = row0;
                const float* b = row2;
                const float* a = &neg.x;
                G2_CROSS();
            }
            {
                float* dst = row1;
                const float* b = row0;
                const float* a = row2;
                G2_CROSS();
            }
            return;
        }
    }
    // Look-at camera: right = up x forward, up = forward x right.
    {
        pos[0] = V->camPos.x;
        pos[1] = V->camPos.y;
        pos[2] = V->camPos.z;
        float* row0 = &matrix[0];
        float* row1 = &matrix[4];
        float* row2 = &matrix[8];
        *(Vec3*)row2 = V->target;
        float up[3];
        up[0] = 0.0f;
        up[1] = 1.0f;
        up[2] = 0.0f;
        *(Vec3*)row2 -= *(Vec3*)pos;
        G2_Normalize(row2);
        {
            float* dst = row0;
            const float* b = row2;
            const float* a = up;
            G2_CROSS();
        }
        G2_Normalize(row0);
        {
            float* dst = row1;
            const float* b = row0;
            const float* a = row2;
            G2_CROSS();
        }
    }
finish:
    V->nearPlane = G2_kNearMount;
}

// ---- G3: camera offsets, camera.txt loader, fixed camera, module end ----

struct G3Vec3 {
    float x, y, z;
    G3Vec3 operator+(const G3Vec3& o) const { G3Vec3 r = { x + o.x, y + o.y, z + o.z }; return r; }
};
// Keys of camera.txt (0x4fa8f0, 9 cameras x 4 keys).
ConfigKey G3CameraKeys[36] = {
    { "Camera1LookAt", 4 },
    { "Camera1Position", 4 },
    { "Camera1Stiffness", 2 },
    { "Camera1Fov", 2 },
    { "Camera2LookAt", 4 },
    { "Camera2Position", 4 },
    { "Camera2Stiffness", 2 },
    { "Camera2Fov", 2 },
    { "Camera3LookAt", 4 },
    { "Camera3Position", 4 },
    { "Camera3Stiffness", 2 },
    { "Camera3Fov", 2 },
    { "Camera4LookAt", 4 },
    { "Camera4Position", 4 },
    { "Camera4Stiffness", 2 },
    { "Camera4Fov", 2 },
    { "Camera5LookAt", 4 },
    { "Camera5Position", 4 },
    { "Camera5Stiffness", 2 },
    { "Camera5Fov", 2 },
    { "Camera6LookAt", 4 },
    { "Camera6Position", 4 },
    { "Camera6Stiffness", 2 },
    { "Camera6Fov", 2 },
    { "Camera7LookAt", 4 },
    { "Camera7Position", 4 },
    { "Camera7Stiffness", 2 },
    { "Camera7Fov", 2 },
    { "Camera8LookAt", 4 },
    { "Camera8Position", 4 },
    { "Camera8Stiffness", 2 },
    { "Camera8Fov", 2 },
    { "Camera9LookAt", 4 },
    { "Camera9Position", 4 },
    { "Camera9Stiffness", 2 },
    { "Camera9Fov", 2 },
};

// The Car body as the camera reads it (Car+0x24): position at +0x14, matrix rows at +0x20 (stride 0x10),
// heading at +0x30/+0x38 and velocity at +0x40/+0x44/+0x48.
struct G3CameraBody {
    BYTE unk0[0x14];
    float pos[3];                      // 0x14
    float row0[3];                     // 0x20
    float unk2c;
    float row1[3];                     // 0x30 (x, y, z: heading uses x and z)
    float unk3c;
    float vel[3];                      // 0x40
};

struct G3CameraCar {
    BYTE unk0[0x24];
    G3CameraBody* body;                // 0x24
};

// The ViewManager fields the camera functions read.
struct G3CameraView {
    BYTE unk0[0x1cc];
    G3CameraCar* car;                  // 0x1cc
    BYTE unk1d0[4];
    int view;                          // 0x1d4
};

// Transforms *point by the three rows at *matrix and adds *trans (as the original's inline assembler).
#define G3_TRANSFORM(point, matrix, result, trans) \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
    __asm mov ebx, trans \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx] \
    __asm fxch st(1) \
    __asm fadd dword ptr [ebx+4] \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx+8] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// NONMATCHING: RALLYTROPHY 0x4578c0
// 588 vs 542 bytes. The view-6 asm transforms, the null-car and the heading logic are in place, but the frame is
// 0x28 in the original (0x10 here: more locals live across the FPU code) and the original keeps z on the FPU stack
// (`fld st(0)`) in the speed test; the slot order of the four asm operands also differs.
// original: FUN_004578c0 (ViewManager::GetCameraOffsets)
// The eye and look-at points of the current preset around the followed car.
void ViewManager::FUN_004578c0(float* eye, float* look)
{
    G3CameraView* v = (G3CameraView*)this;
    G3CameraCar* car = v->car;
    if (car == 0) {
        eye[0] = 0;
        eye[1] = 0;
        eye[2] = 100.0f;
        look[0] = 0;
        look[1] = 0;
        look[2] = 0;
        return;
    }
    G3CameraBody* body = car->body;
    if (v->view == 6) {
        const float* trans = body->pos;
        const float* matrixB = body->row0;
        const float* matrix = body->row0;
        const float* point = CameraTable::presets[6].lookAt;
        G3_TRANSFORM(point, matrix, eye, trans);
        matrixB = body->row0;
        point = CameraTable::presets[v->view].position;
        eye = (float*)body->pos;
        G3_TRANSFORM(point, matrixB, look, eye);
        return;
    }
    float x = body->vel[0];
    float z = body->vel[2];
    if (x * x + z * z <= 0.001f) {
        if (*(unsigned int*)&body->vel[1] & 0x80000000) {
            x = body->row1[0];
            z = body->row1[2];
        } else {
            x = -body->row1[0];
            z = -body->row1[2];
        }
    }
    float inv = 1.0f / (float)sqrt(x * x + z * z);
    float nx = x * inv;
    float nz = z * inv;
    const CameraPreset* p = &CameraTable::presets[v->view];
    eye[0] = nz * p->lookAt[0] + nx * p->lookAt[2] + body->pos[0];
    eye[1] = p->lookAt[1] + body->pos[1];
    eye[2] = -nx * p->lookAt[0] + nz * p->lookAt[2] + body->pos[2];
    look[0] = nx * p->position[2] + nz * p->position[0] + body->pos[0];
    look[1] = p->position[1] + body->pos[1];
    look[2] = nz * p->position[2] + -nx * p->position[0] + body->pos[2];
}

// The message text of a temporary string (the original tests the pointer after loading the empty literal).
static inline const char* G3TextOf(const TmpString& s)
{
    const char* text = "";
    if (s.ptr) text = s.ptr;
    return text;
}

// NONMATCHING: RALLYTROPHY 0x457b10
// 1599 vs 1730 bytes. The key table, the nine preset reads and the epilogue match (same instructions, esp offsets
// shifted by one push); only the throw path differs: the std::runtime_error copy inlines _Tidy/assign here (the original
// calls 0x402700 / 0x4024f0), which hoists a zero into ebx. Same open item as 0x465e90 and 0x4634e0.
// original: FUN_00457b10 (stdcall; ViewManager camera.txt loader)
void FUN_00457b10()
{
    Config config;
    config.Define(G3CameraKeys, 36);
    if (!config.Load("data/settings/camera.txt", true, false, 0)) {
        throw std::runtime_error(AsString(
            PathString(G3TextOf(MissingText(PathString("Missing variables in file: "), "data/settings/camera.txt")))));
    }
#define G3_LOAD(n) \
    do { \
        CameraPreset* p_ = &CameraTable::presets[n - 1]; \
        const ConfigVector3& a_ = config.GetVector3("Camera" #n "LookAt"); \
        p_->lookAt[0] = a_.values[0]; \
        p_->lookAt[1] = a_.values[1]; \
        p_->lookAt[2] = a_.values[2]; \
        const ConfigVector3& b_ = config.GetVector3("Camera" #n "Position"); \
        p_->position[0] = b_.values[0]; \
        p_->position[1] = b_.values[1]; \
        p_->position[2] = b_.values[2]; \
        p_->stiffness = config.GetFloat("Camera" #n "Stiffness"); \
        p_->fov = config.GetFloat("Camera" #n "Fov") * 0.0174532925f; \
    } while (0)
    G3_LOAD(1);
    G3_LOAD(2);
    G3_LOAD(3);
    G3_LOAD(4);
    G3_LOAD(5);
    G3_LOAD(6);
    G3_LOAD(7);
    G3_LOAD(8);
    G3_LOAD(9);
    DAT_00508c98 = CameraTable::presets[6].lookAt[0];
    DAT_00508c9c = CameraTable::presets[6].lookAt[1];
    DAT_00508ca0 = CameraTable::presets[6].lookAt[2];
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x458150
// original: FUN_00458150 (thiscall that never reads this; the fixed in-car camera, preset 7)
void ViewManager::FUN_00458150(void* viewport)
{
    __asm {
        mov eax, dword ptr [esp + 4]
        fld dword ptr [eax]
        mov edx, dword ptr [eax + 4]
        fld dword ptr [DAT_00508c98]
        mov ecx, dword ptr [eax]
        fadd st, st(1)
        mov dword ptr [dat_00508d6c], edx
        mov dword ptr [dat_00508d74], ecx
        mov ecx, dword ptr [eax + 8]
        fstp dword ptr [dat_00508d68]
        mov eax, ecx
        mov dword ptr [dat_00508d70], eax
        fstp st(0)
        mov dword ptr [dat_00508d78], edx
        fld dword ptr [dat_00508d6c]
        mov dword ptr [dat_00508d7c], ecx
        fadd dword ptr [DAT_00508c9c]
        fstp dword ptr [dat_00508d6c]
        fld dword ptr [dat_00508d70]
        fadd dword ptr [DAT_00508ca0]
        fstp dword ptr [dat_00508d70]
        ret 4
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x4581c0
// SYMBOL: _$E35
typedef void Generated_4581c0();

// FUNCTION: RALLYTROPHY 0x4581e0
// SYMBOL: _$E34
typedef void Generated_4581e0();
