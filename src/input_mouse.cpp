// Mouse position helpers. Layout-local view, as used by the race-mode callers.
#include "rallytrophy.h"

extern float DAT_0057ba00;
extern float DAT_0057ba04;

struct MousePosition {
    float x, y;
};

class MouseInput {
public:
    void SetPosition(MousePosition* position);
};

// FUNCTION: RALLYTROPHY 0x4b6d00
// original: FUN_004b6d00
void MouseInput::SetPosition(MousePosition* position)
{
    SetCursorPos((int)position->x, (int)position->y);
    DAT_0057ba00 = position->x;
    DAT_0057ba04 = position->y;
}

// FUNCTION: RALLYTROPHY 0x4b6d40
// original: FUN_004b6d40
// Original module's position-global initializer.
void FUN_004b6d40()
{
    DAT_0057ba00 = 0;
    DAT_0057ba04 = 0;
}
