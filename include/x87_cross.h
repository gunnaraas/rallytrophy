#ifndef X87_CROSS_H
#define X87_CROSS_H
// o = a x b. Inline assembler in the original: the operands are reloaded from stack locals, which forces an ebp frame.
inline void X87Cross(float* o, const float* a, const float* b)
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
#endif
