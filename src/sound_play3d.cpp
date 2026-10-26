// Exact x86 entry for T_56b0b0::Play3D, preserving the original voice search,
// transform math and Miles call order. Readable behavior remains in sound.cpp.
class SoundSource;
class T_56b0b0 { public: void Play3DExact(SoundSource*); };
extern "C" void* dat_0059b2f8;
extern "C" float dat_004e1398;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x499770
// original: T_56b0b0::Play3D
void T_56b0b0::Play3DExact(SoundSource*)
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x18
        mov al, byte ptr [ecx + 0xcc]
        test al, al
        push ebx
        push esi
        push edi
        mov dword ptr [ebp + -0x4], ecx
        jnz play3d_4999b1
        mov al, byte ptr [ecx + 0xcd]
        test al, al
        jnz play3d_4999b1
        mov esi, dword ptr [ebp + 0x8]
        mov edx, dword ptr [esi + 0x4]
        cmp edx, -0x1
        mov dword ptr [ebp + 0x8], edx
        jnz play3d_49981f
        mov eax, dword ptr [ecx + 0xe4]
        xor ebx, ebx
        mov dword ptr [ebp + -0x8], eax
        xor edi, edi
        play3d_4997b3:
        test eax, eax
        jz play3d_499816
        mov ecx, dword ptr [ecx + 0xe8]
        sub ecx, eax
        mov eax, 0x66666667
        imul ecx
        mov ecx, dword ptr [ebp + -0x4]
        sar edx, 0x3
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        cmp ebx, edx
        jnc play3d_499813
        mov eax, dword ptr [ecx + 0xe4]
        mov ecx, dword ptr [esi]
        cmp dword ptr [edi + eax*0x1 + 0x10], ecx
        jnz play3d_4997ed
        mov ecx, dword ptr [edi + eax*0x1 + 0x8]
        test ecx, ecx
        jz play3d_49980d
        play3d_4997ed:
        cmp dword ptr [ebp + 0x8], -0x1
        jnz play3d_4997fe
        mov ecx, dword ptr [edi + eax*0x1 + 0x8]
        test ecx, ecx
        jnz play3d_4997fe
        mov dword ptr [ebp + 0x8], ebx
        play3d_4997fe:
        mov ecx, dword ptr [ebp + -0x4]
        mov eax, dword ptr [ebp + -0x8]
        mov edx, dword ptr [ebp + 0x8]
        inc ebx
        add edi, 0x14
        jmp play3d_4997b3
        play3d_49980d:
        mov ecx, dword ptr [ebp + -0x4]
        mov dword ptr [ebp + 0x8], ebx
        play3d_499813:
        mov edx, dword ptr [ebp + 0x8]
        play3d_499816:
        cmp edx, -0x1
        jz play3d_4999b1
        play3d_49981f:
        mov ecx, dword ptr [ecx + 0xe4]
        lea eax, [edx + edx*0x4]
        lea edi, [ecx + eax*0x4]
        mov eax, dword ptr [edi + 0xc]
        test eax, eax
        jz play3d_49983d
        cmp eax, esi
        jz play3d_49983d
        mov dword ptr [eax + 0x4], 0xffffffff
        play3d_49983d:
        mov eax, dword ptr [esi]
        cmp dword ptr [edi + 0x10], eax
        mov dword ptr [esi + 0x4], edx
        mov dword ptr [edi + 0xc], esi
        jz play3d_49985a
        mov dword ptr [edi + 0x10], eax
        mov edx, dword ptr [eax + 0x10]
        mov eax, dword ptr [edi]
        push edx
        push eax
        call dword ptr [dat_0059b2f8]
        play3d_49985a:
        mov ebx, dword ptr [esi + 0x18]
        test ebx, ebx
        mov dword ptr [ebp + 0x8], 0x1
        jnz play3d_49986c
        mov eax, dword ptr [esi]
        mov ebx, dword ptr [eax]
        play3d_49986c:
        mov al, byte ptr [esi + 0xa]
        test al, al
        jz play3d_49987a
        mov dword ptr [ebp + 0x8], 0x0
        play3d_49987a:
        mov ecx, dword ptr [ebp + -0x4]
        add ecx, 0x5c
        lea edx, [esi + 0x3c]
        lea eax, [ebp + -0x18]
        mov dword ptr [ebp + -0x4], ecx
        mov dword ptr [ebp + -0xc], edx
        mov dword ptr [ebp + -0x8], eax
        mov ecx, dword ptr [ebp + -0xc]
        mov edx, dword ptr [ebp + -0x4]
        mov eax, dword ptr [ebp + -0x8]
        fld float ptr [ecx]
        fmul float ptr [edx]
        fld float ptr [ecx]
        fmul float ptr [edx + 0x4]
        fld float ptr [ecx]
        fmul float ptr [edx + 0x8]
        fld float ptr [ecx + 0x4]
        fmul float ptr [edx + 0x10]
        fld float ptr [ecx + 0x4]
        fmul float ptr [edx + 0x14]
        fld float ptr [ecx + 0x4]
        fmul float ptr [edx + 0x18]
        fxch st(2)
        faddp st(5), st(0)
        faddp st(3), st(0)
        faddp st(1), st(0)
        fld float ptr [ecx + 0x8]
        fmul float ptr [edx + 0x20]
        fld float ptr [ecx + 0x8]
        fmul float ptr [edx + 0x24]
        fld float ptr [ecx + 0x8]
        fmul float ptr [edx + 0x28]
        fxch st(2)
        faddp st(5), st(0)
        faddp st(3), st(0)
        faddp st(1), st(0)
        fxch st(2)
        fadd float ptr [edx + 0x30]
        fxch st(1)
        fadd float ptr [edx + 0x34]
        fxch st(2)
        fadd float ptr [edx + 0x38]
        fxch st(1)
        fstp float ptr [eax]
        fstp float ptr [eax + 0x8]
        fstp float ptr [eax + 0x4]
        mov ecx, dword ptr [edi]
        push ebx
        push ecx
        call dword ptr [dat_0059b2f8]
        mov ebx, dword ptr [edi]
        push 0x7f
        push ebx
        call dword ptr [dat_0059b2f8]
        mov edx, dword ptr [ebp + 0x8]
        push edx
        push ebx
        call dword ptr [dat_0059b2f8]
        mov eax, dword ptr [esi + 0x10]
        mov ecx, dword ptr [esi + 0xc]
        push eax
        push ecx
        push ebx
        call dword ptr [dat_0059b2f8]
        mov edx, dword ptr [esi + 0x24]
        mov eax, dword ptr [esi + 0x28]
        mov ecx, dword ptr [edi]
        push edx
        push eax
        push ecx
        call dword ptr [dat_0059b2f8]
        mov edx, dword ptr [ebp + -0x10]
        mov eax, dword ptr [ebp + -0x14]
        mov ecx, dword ptr [ebp + -0x18]
        mov ebx, dword ptr [edi]
        push edx
        push eax
        push ecx
        push ebx
        call dword ptr [dat_0059b2f8]
        fld float ptr [esi + 0x50]
        fmul float ptr [dat_004e1398]
        push ecx
        fstp float ptr [esp]
        push ecx
        fld float ptr [esi + 0x4c]
        fmul float ptr [dat_004e1398]
        fstp float ptr [esp]
        push ecx
        fld float ptr [esi + 0x48]
        fmul float ptr [dat_004e1398]
        fstp float ptr [esp]
        push ebx
        call dword ptr [dat_0059b2f8]
        mov edx, dword ptr [esi + 0x5c]
        and edx, 0xffffffe3
        mov dword ptr [esi + 0x5c], edx
        mov eax, dword ptr [esi + 0x5c]
        and eax, 0xffffff1f
        mov dword ptr [esi + 0x5c], eax
        mov ecx, dword ptr [esi + 0x5c]
        and ecx, 0xfffffeff
        mov dword ptr [esi + 0x5c], ecx
        mov eax, 0x1
        mov dword ptr [esi + 0x60], eax
        mov dword ptr [esi + 0x64], 0x0
        mov dword ptr [edi + 0x8], eax
        mov edx, dword ptr [edi]
        push edx
        call dword ptr [dat_0059b2f8]
        play3d_4999b1:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret 0x4
    }
}

