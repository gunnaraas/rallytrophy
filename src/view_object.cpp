#include "rallytrophy.h"
#include "view_object.h"

typedef char ViewBaseSizeCheck[sizeof(ViewObjectBase) == 0x124 ? 1 : -1];
typedef char ViewObjectSizeCheck[sizeof(ViewObject) == 0x1cc ? 1 : -1];

// FUNCTION: RALLYTROPHY 0x4b77b0
// original: FUN_004b77b0
ViewObject::ViewObject()
{
    *(int*)data4 = 3;
    basis.ClearOffDiagonal();
    zone = resetSmoothing = 0;
    basis.SetIdentityDiagonal();
    previous.ClearOffDiagonal();
    nearPlane = 1;
    farPlane = 1000;
    fov = 1.5707964f;
    previous.SetIdentityDiagonal();
    flag = 1;
}

// FUNCTION: RALLYTROPHY 0x4b78e0
// original: FUN_004b78e0
ViewObject::~ViewObject()
{
}

// FUNCTION: RALLYTROPHY 0x4b78c0
// SYMBOL: ??_GViewObject@@UAEPAXI@Z
typedef void Generated_4b78c0();

struct ViewVector {
    void Set(float x_, float y_, float z_) { x = x_; y = y_; z = z_; }
    float SquaredLength() const { return x * x + y * y + z * z; }
    float x, y, z;
};

// FUNCTION: RALLYTROPHY 0x4b78f0
// original: FUN_004b78f0
void ViewObject::UpdateBasis()
{
    const float* source = (const float*)this;
    ViewVector& forward = *(ViewVector*)&basis.m[8];
    ViewVector& up = *(ViewVector*)&basis.m[4];
    basis.m[8] = -source[0x80 / 4];
    basis.m[10] = -source[0x88 / 4];
    up.Set(0, 1, 0);
    basis.m[9] = 0;
    if (forward.SquaredLength() < 0.001f) {
        basis.m[8] = -source[0x70 / 4];
        basis.m[9] = 0;
        basis.m[10] = -source[0x78 / 4];
    }
    const float* cb = &forward.x;
    const float* ca = &up.x;
    float* cresult = &basis.m[0];
    // Preserve the original inline vector cross-product evaluation order.
    // cresult = ca x cb; no normalization is performed here.
    __asm {
        mov ecx, ca
        mov edx, cb
        mov eax, cresult
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
