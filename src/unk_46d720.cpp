// SpringFollower (0x46d720-0x46d7f8, stub 0x46d800). Map: analysis/range-46a8a0.md section 5.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_46d720.h"

// FUNCTION: RALLYTROPHY 0x46d720
SpringFollower::SpringFollower()
{
    lower = upper = velocity = position = rest = damping = stiffness = 0;
}

// FUNCTION: RALLYTROPHY 0x46d740
void SpringFollower::Update(float dt, float a, float b)
{
    float old = position;
    float acc;
    if (position - rest > 0.0f)
        acc = 0.0f;
    else
        acc = -((position - rest) * stiffness) - velocity * damping;
    acc = acc - a * 0.375f - b * 9.81f;
    position = velocity * dt + position;
    if (position < lower) {
        position = lower;
        if (old > lower)
            velocity *= -0.5f;
        else
            velocity = 0;
    }
    if (position > upper) {
        position = upper;
        velocity = 0;
    }
    velocity = acc * dt + velocity;
}

// FUNCTION: RALLYTROPHY 0x46d7f0
void SpringFollower::Reset()
{
    velocity = position = 0;
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x46d800
// SYMBOL: _$E23
typedef void Generated_46d800();

// FUNCTION: RALLYTROPHY 0x46d820
// SYMBOL: _$E22
typedef void Generated_46d820();
