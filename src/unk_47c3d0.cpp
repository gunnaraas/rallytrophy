// Two file-scope float constants derived from a fade fraction (0x47c3d0-0x47c45f);
// nothing reads them in the shipped binary. Map: analysis/range-478870.md.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"

static const float kFadeFraction = 0.3f; // 0x4e2298

float DAT_00563f5c = 1.0f / kFadeFraction;
float DAT_00563f58 = 1.0f / (1.0f - kFadeFraction);

// Compiler-generated: the dynamic initialiser wrapper and body of each float.

// FUNCTION: RALLYTROPHY 0x47c3d0
// SYMBOL: _$E19
typedef void Generated_47c3d0();

// FUNCTION: RALLYTROPHY 0x47c3e0
// SYMBOL: _$E18
typedef void Generated_47c3e0();

// FUNCTION: RALLYTROPHY 0x47c400
// SYMBOL: _$E22
typedef void Generated_47c400();

// FUNCTION: RALLYTROPHY 0x47c410
// SYMBOL: _$E21
typedef void Generated_47c410();

// Module end: the STL header statics guard and its atexit registration.

// FUNCTION: RALLYTROPHY 0x47c430
// SYMBOL: _$E29
typedef void Generated_47c430();

// FUNCTION: RALLYTROPHY 0x47c450
// SYMBOL: _$E28
typedef void Generated_47c450();
