// Exact x86 entry for T_56b0b0::Update. The readable behavior implementation
// remains in sound.cpp; this isolates the original VC6 frame/register layout.
class T_56b0b0 { public: void UpdateExact(); };
extern "C" void* dat_0059b2f8;
extern "C" void* dat_00569e70;
extern "C" float dat_004e1398;
extern "C" float dat_004e134c;
extern "C" void fun_0049ab20();
extern "C" void fun_0049b570();
extern "C" void fun_00414490();
extern "C" void fun_004c1276();
extern "C" void fun_004999c0();
extern "C" void fun_00499a10();
extern "C" void fun_00499770();
extern "C" void fun_004c2221();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x499f80
// original: T_56b0b0::Update
void T_56b0b0::UpdateExact()
{
    __asm {
        push ebp
        mov ebp, esp
        sub esp, 0x28
        push ebx
        push esi
        mov esi, ecx
        mov al, byte ptr [esi + 0xcd]
        test al, al
        push edi
        mov dword ptr [ebp + -0x4], esi
        jnz update_49a48b
        mov eax, dword ptr [esi + 0x290]
        test eax, eax
        jz update_499fba
        mov eax, dword ptr [esi + 0x28c]
        mov ecx, dword ptr [esi + 0xc8]
        push eax
        push ecx
        call dword ptr [dat_0059b2f8]
        update_499fba:
        push 0x1
        mov ecx, esi
        call fun_0049ab20
        lea ebx, [esi + 0x5c]
        add esi, 0x1c
        mov ecx, 0x10
        mov edi, ebx
        rep movsd
        mov edi, dword ptr [ebp + -0x4]
        mov ecx, dword ptr [edi + 0xa4]
        mov eax, dword ptr [edi + 0xa0]
        mov edx, dword ptr [edi + 0x9c]
        mov dword ptr [edi + 0x94], ecx
        mov dword ptr [edi + 0x90], eax
        fld float ptr [ebx + 0x18]
        mov dword ptr [edi + 0x8c], edx
        fmul float ptr [ebx + 0x38]
        mov ecx, ebx
        fld float ptr [ebx + 0x14]
        fmul float ptr [ebx + 0x34]
        faddp st(1), st(0)
        fld float ptr [ebx + 0x10]
        fmul float ptr [ebx + 0x30]
        faddp st(1), st(0)
        fchs
        fld float ptr [ebx + 0x28]
        fmul float ptr [ebx + 0x38]
        fld float ptr [ebx + 0x24]
        fmul float ptr [ebx + 0x34]
        faddp st(1), st(0)
        fld float ptr [ebx + 0x20]
        fmul float ptr [ebx + 0x30]
        faddp st(1), st(0)
        fchs
        fstp float ptr [ebp + -0x10]
        mov edx, dword ptr [ebp + -0x10]
        fld float ptr [ebx + 0x38]
        mov dword ptr [ebx + 0x38], edx
        fmul float ptr [ebx + 0x8]
        fld float ptr [ebx + 0x34]
        fmul float ptr [ebx + 0x4]
        faddp st(1), st(0)
        fld float ptr [ebx + 0x30]
        fmul float ptr [ebx]
        faddp st(1), st(0)
        fchs
        fstp float ptr [ebx + 0x30]
        fstp float ptr [ebx + 0x34]
        call fun_0049b570
        mov eax, dword ptr [edi + 0x12c]
        test eax, eax
        lea esi, [edi + 0x12c]
        jz update_49a074
        update_49a066:
        push eax
        mov ecx, esi
        call fun_00414490
        mov eax, dword ptr [esi]
        test eax, eax
        jnz update_49a066
        update_49a074:
        mov eax, dword ptr [edi + 0x104]
        test eax, eax
        mov dword ptr [ebp + -0x8], eax
        jz update_49a114
        update_49a085:
        mov eax, dword ptr [ebp + -0x8]
        mov edi, dword ptr [eax + 0x8]
        cmp dword ptr [edi + 0x68], 0x1
        jz update_49a09f
        mov eax, dword ptr [edi + 0x60]
        test eax, eax
        jnz update_49a09f
        mov eax, dword ptr [edi + 0x64]
        test eax, eax
        jz update_49a0eb
        update_49a09f:
        mov eax, dword ptr [esi + 0x8]
        test eax, eax
        jnz update_49a0c4
        push 0xc
        call fun_004c1276
        xor ecx, ecx
        add esp, 0x4
        cmp eax, ecx
        jz update_49a0c0
        mov dword ptr [eax + 0x4], ecx
        mov dword ptr [eax], ecx
        mov dword ptr [eax + 0x8], ecx
        jmp update_49a0cc
        update_49a0c0:
        xor eax, eax
        jmp update_49a0cc
        update_49a0c4:
        mov ecx, dword ptr [eax]
        mov dword ptr [esi + 0x8], ecx
        dec dword ptr [esi + 0x10]
        update_49a0cc:
        mov ecx, dword ptr [esi + 0x4]
        test ecx, ecx
        mov dword ptr [eax + 0x8], edi
        mov dword ptr [eax + 0x4], ecx
        mov dword ptr [eax], 0x0
        jz update_49a0e3
        mov dword ptr [ecx], eax
        jmp update_49a0e5
        update_49a0e3:
        mov dword ptr [esi], eax
        update_49a0e5:
        mov dword ptr [esi + 0x4], eax
        inc dword ptr [esi + 0xc]
        update_49a0eb:
        cmp dword ptr [edi + 0x68], 0x2
        jnz update_49a101
        mov ecx, dword ptr [ebp + -0x4]
        push edi
        call fun_004999c0
        mov dword ptr [edi + 0x68], 0x0
        update_49a101:
        mov edx, dword ptr [ebp + -0x8]
        mov eax, dword ptr [edx]
        test eax, eax
        mov dword ptr [ebp + -0x8], eax
        jnz update_49a085
        mov edi, dword ptr [ebp + -0x4]
        update_49a114:
        mov al, byte ptr [edi + 0xcc]
        test al, al
        jnz update_49a48b
        mov eax, dword ptr [edi + 0x138]
        cmp eax, dword ptr [edi + 0x18]
        jg update_49a297
        mov esi, dword ptr [esi]
        test esi, esi
        mov dword ptr [ebp + -0x8], esi
        jz update_49a481
        lea eax, [ebp + -0x28]
        mov dword ptr [ebp + -0xc], ebx
        mov dword ptr [ebp + -0x10], eax
        jmp update_49a14c
        update_49a149:
        mov esi, dword ptr [ebp + -0x8]
        update_49a14c:
        mov esi, dword ptr [esi + 0x8]
        lea ecx, [esi + 0x3c]
        mov dword ptr [ebp + -0x4], ecx
        mov ecx, dword ptr [ebp + -0x4]
        mov edx, dword ptr [ebp + -0xc]
        mov eax, dword ptr [ebp + -0x10]
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
        fld float ptr [ebp + -0x24]
        fmul float ptr [ebp + -0x24]
        fld float ptr [ebp + -0x20]
        fmul float ptr [ebp + -0x20]
        faddp st(1), st(0)
        fld float ptr [ebp + -0x28]
        fmul float ptr [ebp + -0x28]
        faddp st(1), st(0)
        fstp float ptr [ebp + -0x4]
        fld float ptr [esi + 0x38]
        fld st(0)
        fmul st(0), st(1)
        fld float ptr [ebp + -0x4]
        fcompp
        fnstsw ax
        fstp st(0)
        test ah, 0x41
        jnz update_49a206
        mov eax, dword ptr [esi + 0x64]
        test eax, eax
        jnz update_49a1f6
        push esi
        mov ecx, edi
        call fun_004999c0
        update_49a1f6:
        mov al, byte ptr [esi + 0xa]
        test al, al
        jz update_49a273
        mov dword ptr [esi + 0x64], 0x1
        jmp update_49a273
        update_49a206:
        fld float ptr [esi + 0x24]
        mov eax, dword ptr [esi + 0x30]
        fld float ptr [esi + 0x34]
        mov edx, dword ptr [esi + 0x2c]
        fld st(0)
        mov dword ptr [esi + 0x28], eax
        fmul st(0), st(1)
        mov dword ptr [esi + 0x24], edx
        fld float ptr [ebp + -0x4]
        fcompp
        fnstsw ax
        fstp st(0)
        test ah, 0x1
        jz update_49a241
        cmp dword ptr [esi + 0x14], 0x7f
        jz update_49a241
        fld float ptr [ebp + -0x4]
        fsqrt
        fmul float ptr [esi + 0x20]
        fadd float ptr [dat_004e1398]
        fstp float ptr [esi + 0x24]
        update_49a241:
        fcomp float ptr [esi + 0x24]
        fnstsw ax
        test ah, 0x40
        jnz update_49a254
        mov ecx, dword ptr [esi + 0x5c]
        or ecx, 0x20
        mov dword ptr [esi + 0x5c], ecx
        update_49a254:
        cmp dword ptr [esi + 0x68], 0x1
        jz update_49a26b
        mov eax, dword ptr [esi + 0x64]
        test eax, eax
        jnz update_49a26b
        push esi
        mov ecx, edi
        call fun_00499a10
        jmp update_49a27a
        update_49a26b:
        push esi
        mov ecx, edi
        call fun_00499770
        update_49a273:
        mov dword ptr [esi + 0x68], 0x0
        update_49a27a:
        mov edx, dword ptr [ebp + -0x8]
        mov eax, dword ptr [edx]
        test eax, eax
        mov dword ptr [ebp + -0x8], eax
        jnz update_49a149
        mov dword ptr [edi + 0x290], eax
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
        update_49a297:
        mov edi, dword ptr [esi]
        test edi, edi
        mov [dat_00569e70], eax
        jz update_49a3de
        lea eax, [ebp + -0x28]
        mov dword ptr [ebp + -0x18], ebx
        mov dword ptr [ebp + -0x14], eax
        mov ebx, 0x569e74
        update_49a2b4:
        mov esi, dword ptr [edi + 0x8]
        lea ecx, [esi + 0x3c]
        mov dword ptr [ebp + -0x10], 0x3f800000
        mov dword ptr [ebp + -0x8], ecx
        mov ecx, dword ptr [ebp + -0x8]
        mov edx, dword ptr [ebp + -0x18]
        mov eax, dword ptr [ebp + -0x14]
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
        mov edx, dword ptr [esi + 0x24]
        mov eax, dword ptr [esi + 0x2c]
        mov ecx, dword ptr [esi + 0x30]
        mov dword ptr [esi + 0x24], eax
        mov dword ptr [esi + 0x28], ecx
        fld float ptr [ebp + -0x24]
        fld float ptr [ebp + -0x20]
        mov dword ptr [ebp + -0x1c], edx
        fld float ptr [ebp + -0x28]
        mov edx, dword ptr [esi + 0x34]
        fmul float ptr [ebp + -0x28]
        mov dword ptr [ebp + -0xc], edx
        fld st(1)
        fmul st(0), st(2)
        faddp st(1), st(0)
        fld st(2)
        fmul st(0), st(3)
        faddp st(1), st(0)
        fsqrt
        fstp st(2)
        fstp st(0)
        fadd float ptr [dat_004e1398]
        fstp float ptr [ebp + -0x8]
        fld float ptr [ebp + -0x8]
        fcomp float ptr [ebp + -0xc]
        fnstsw ax
        test ah, 0x41
        jnz update_49a391
        fld float ptr [esi + 0x38]
        fcomp float ptr [ebp + -0x8]
        fnstsw ax
        test ah, 0x1
        jz update_49a389
        fld float ptr [dat_004e134c]
        jmp update_49a3a9
        update_49a389:
        fld float ptr [ebp + -0xc]
        fdiv float ptr [ebp + -0x8]
        jmp update_49a3a9
        update_49a391:
        cmp dword ptr [esi + 0x14], 0x7f
        jz update_49a3a6
        fld float ptr [ebp + -0x8]
        fmul float ptr [esi + 0x20]
        fadd float ptr [dat_004e1398]
        fstp float ptr [esi + 0x24]
        update_49a3a6:
        fld float ptr [ebp + -0x10]
        update_49a3a9:
        fld float ptr [esi + 0x24]
        fcomp float ptr [ebp + -0x1c]
        fnstsw ax
        test ah, 0x40
        jnz update_49a3bf
        mov eax, dword ptr [esi + 0x5c]
        or eax, 0x20
        mov dword ptr [esi + 0x5c], eax
        update_49a3bf:
        mov ecx, dword ptr [esi + 0x58]
        mov edi, dword ptr [edi]
        and ecx, 0xf
        fadd float ptr [ecx*0x4 + 0x500438]
        mov dword ptr [ebx], esi
        add ebx, 0x4
        test edi, edi
        fstp float ptr [esi + 0x6c]
        jnz update_49a2b4
        update_49a3de:
        mov edx, dword ptr [dat_00569e70]
        push 0x498360
        push 0x4
        push edx
        push 0x569e74
        call fun_004c2221
        mov eax, dword ptr [ebp + -0x4]
        mov edi, dword ptr [eax + 0x18]
        mov eax, [dat_00569e70]
        add esp, 0x10
        cmp edi, eax
        jge update_49a435
        lea ebx, [edi*0x4 + 0x569e74]
        update_49a40f:
        mov esi, dword ptr [ebx]
        mov ecx, dword ptr [ebp + -0x4]
        push esi
        call fun_004999c0
        mov al, byte ptr [esi + 0xa]
        test al, al
        jz update_49a428
        mov dword ptr [esi + 0x64], 0x1
        update_49a428:
        mov eax, [dat_00569e70]
        inc edi
        add ebx, 0x4
        cmp edi, eax
        jl update_49a40f
        update_49a435:
        mov ecx, dword ptr [ebp + -0x4]
        mov eax, dword ptr [ecx + 0x18]
        xor ebx, ebx
        test eax, eax
        jle update_49a47e
        mov edi, 0x569e74
        update_49a446:
        mov esi, dword ptr [edi]
        cmp dword ptr [esi + 0x68], 0x1
        jz update_49a460
        mov eax, dword ptr [esi + 0x64]
        test eax, eax
        jnz update_49a460
        mov ecx, dword ptr [ebp + -0x4]
        push esi
        call fun_00499a10
        jmp update_49a470
        update_49a460:
        mov ecx, dword ptr [ebp + -0x4]
        push esi
        call fun_00499770
        mov dword ptr [esi + 0x68], 0x0
        update_49a470:
        mov edx, dword ptr [ebp + -0x4]
        mov eax, dword ptr [edx + 0x18]
        inc ebx
        add edi, 0x4
        cmp ebx, eax
        jl update_49a446
        update_49a47e:
        mov edi, dword ptr [ebp + -0x4]
        update_49a481:
        mov dword ptr [edi + 0x290], 0x0
        update_49a48b:
        pop edi
        pop esi
        pop ebx
        mov esp, ebp
        pop ebp
        ret
    }
}

