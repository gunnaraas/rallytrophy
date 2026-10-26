// Effect base class and the shared sprite tables (0x478be0-0x47916f).
// Map: analysis/range-478870.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_478be0.h"
#include <math.h>

// Shared lookup tables, filled by Effect::InitTables on first use. The
// elements have an empty ctor/dtor, so each array gets a static initialiser.
Vec2 EffectTables::sinCos[1024];
Vec2 EffectTables::uv8x8[64];
Vec2 EffectTables::uv4x4a[16];
Vec2 EffectTables::uv4x4b[16];
Vec2 EffectTables::uv2x2a[4];
Vec2 EffectTables::uv2x2b[4];

extern const float kTwoPi;   // 0x4e1ec8
extern const float kInv1024; // 0x4e1358

static int s_tablesReady; // 0x563ec0

// FUNCTION: RALLYTROPHY 0x478e80
Effect::Effect()
{
    texture = 0;
    particles = 0;
    unk8 = 0;
    InitTables();
}

// FUNCTION: RALLYTROPHY 0x478ee0
// SYMBOL: ??_GEffect@@UAEPAXI@Z
typedef void Generated_478ee0();

// FUNCTION: RALLYTROPHY 0x478f00
Effect::~Effect()
{
    Clear();
}

// FUNCTION: RALLYTROPHY 0x478f50
void Effect::InitTables()
{
    if (s_tablesReady) {
        return;
    }
    s_tablesReady = 1;
    int k, i, j;
    for (k = 0; k < 1024; k++) {
        Vec2* e = &EffectTables::sinCos[k];
        float a = k * kTwoPi * kInv1024;
        e->x = (float)cos(a);
        e->y = (float)sin(a);
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            EffectTables::uv8x8[i * 8 + j].x = j * 0.125f;
            EffectTables::uv8x8[i * 8 + j].y = i * 0.125f;
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            EffectTables::uv4x4a[i * 4 + j].x = j * 0.25f;
            EffectTables::uv4x4a[i * 4 + j].y = i * 0.25f;
            EffectTables::uv4x4b[i * 4 + j].x = j * 0.03125f;
            EffectTables::uv4x4b[i * 4 + j].y = i * 0.03125f;
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            EffectTables::uv2x2a[i * 2 + j].x = j * 0.0625f;
            EffectTables::uv2x2a[i * 2 + j].y = i * 0.0625f;
            EffectTables::uv2x2b[i * 2 + j].x = j * 0.125f;
            EffectTables::uv2x2b[i * 2 + j].y = i * 0.125f;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4790e0
void Effect::Clear()
{
    Particle* p = particles;
    boxMin[0] = boxMin[1] = boxMin[2] = 0.0f;
    boxMax[0] = boxMax[1] = boxMax[2] = 0.0f;
    particles = 0;
    if (p) {
        Particle* head = DAT_0056ae90.head;
        Particle* next;
        do {
            next = p->next;
            p->next = head;
            head = p;
            p = next;
        } while (next);
        DAT_0056ae90.head = head;
    }
}

// Static initialisers of the six tables: wrapper, EH vector constructor
// iterator, atexit registration and the guarded destructor.

// FUNCTION: RALLYTROPHY 0x478be0
// SYMBOL: _$E28
typedef void Generated_478be0();

// FUNCTION: RALLYTROPHY 0x478bf0
// SYMBOL: _$E24
typedef void Generated_478bf0();

// FUNCTION: RALLYTROPHY 0x478c10
// SYMBOL: _$E27
typedef void Generated_478c10();

// FUNCTION: RALLYTROPHY 0x478c20
// SYMBOL: _$E25
typedef void Generated_478c20();

// FUNCTION: RALLYTROPHY 0x478c50
// SYMBOL: _$E33
typedef void Generated_478c50();

// FUNCTION: RALLYTROPHY 0x478c60
// SYMBOL: _$E30
typedef void Generated_478c60();

// FUNCTION: RALLYTROPHY 0x478c80
// SYMBOL: _$E32
typedef void Generated_478c80();

// FUNCTION: RALLYTROPHY 0x478c90
// SYMBOL: _$E31
typedef void Generated_478c90();

// FUNCTION: RALLYTROPHY 0x478cc0
// SYMBOL: _$E38
typedef void Generated_478cc0();

// FUNCTION: RALLYTROPHY 0x478cd0
// SYMBOL: _$E35
typedef void Generated_478cd0();

// FUNCTION: RALLYTROPHY 0x478cf0
// SYMBOL: _$E37
typedef void Generated_478cf0();

// FUNCTION: RALLYTROPHY 0x478d00
// SYMBOL: _$E36
typedef void Generated_478d00();

// FUNCTION: RALLYTROPHY 0x478d30
// SYMBOL: _$E43
typedef void Generated_478d30();

// FUNCTION: RALLYTROPHY 0x478d40
// SYMBOL: _$E40
typedef void Generated_478d40();

// FUNCTION: RALLYTROPHY 0x478d60
// SYMBOL: _$E42
typedef void Generated_478d60();

// FUNCTION: RALLYTROPHY 0x478d70
// SYMBOL: _$E41
typedef void Generated_478d70();

// FUNCTION: RALLYTROPHY 0x478da0
// SYMBOL: _$E48
typedef void Generated_478da0();

// FUNCTION: RALLYTROPHY 0x478db0
// SYMBOL: _$E45
typedef void Generated_478db0();

// FUNCTION: RALLYTROPHY 0x478dd0
// SYMBOL: _$E47
typedef void Generated_478dd0();

// FUNCTION: RALLYTROPHY 0x478de0
// SYMBOL: _$E46
typedef void Generated_478de0();

// FUNCTION: RALLYTROPHY 0x478e10
// SYMBOL: _$E53
typedef void Generated_478e10();

// FUNCTION: RALLYTROPHY 0x478e20
// SYMBOL: _$E50
typedef void Generated_478e20();

// FUNCTION: RALLYTROPHY 0x478e40
// SYMBOL: _$E52
typedef void Generated_478e40();

// FUNCTION: RALLYTROPHY 0x478e50
// SYMBOL: _$E51
typedef void Generated_478e50();

// Module end: the STL header statics guard and its atexit registration.

// FUNCTION: RALLYTROPHY 0x479140
// SYMBOL: _$E60
typedef void Generated_479140();

// FUNCTION: RALLYTROPHY 0x479160
// SYMBOL: _$E59
typedef void Generated_479160();
