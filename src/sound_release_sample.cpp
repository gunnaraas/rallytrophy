// Exact x86 entry for T_56b0b0::ReleaseSample. The readable behavior
// implementation remains in sound.cpp.
class Sample {};
class T_56b0b0 { public: Sample* ReleaseSampleExact(Sample*); };
extern "C" void* dat_0059b2f8;
extern "C" void fun_004bd000();
extern "C" void fun_0049b5f0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x499340
// original: T_56b0b0::ReleaseSample
Sample* T_56b0b0::ReleaseSampleExact(Sample*)
{
    __asm {
        push ecx
        push ebp
        mov ebp, dword ptr [esp + 0xc]
        test ebp, ebp
        push esi
        mov esi, ecx
        jnz releasesample_499355
        pop esi
        xor eax, eax
        pop ebp
        pop ecx
        ret 0x4
        releasesample_499355:
        push edi
        mov edi, dword ptr [ebp + 0x14]
        dec edi
        mov ecx, edi
        test ecx, ecx
        mov dword ptr [ebp + 0x14], edi
        mov eax, ebp
        jg releasesample_4994cb
        mov al, byte ptr [ebp + 0x18]
        test al, al
        jz releasesample_49937c
        mov eax, dword ptr [ebp + 0x10]
        push eax
        call dword ptr [dat_0059b2f8]
        jmp releasesample_499388
        releasesample_49937c:
        mov ecx, dword ptr [ebp + 0x10]
        push ecx
        call fun_004bd000
        add esp, 0x4
        releasesample_499388:
        xor edi, edi
        push ebx
        mov ebx, dword ptr [esi + 0xd4]
        mov dword ptr [esp + 0x10], edi
        releasesample_499395:
        test ebx, ebx
        jz releasesample_4993dc
        mov ecx, dword ptr [esi + 0xd8]
        sub ecx, ebx
        mov eax, 0x66666667
        imul ecx
        sar edx, 0x3
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        cmp edi, edx
        jnc releasesample_4993dc
        mov ecx, dword ptr [esi + 0xd4]
        mov edx, dword ptr [esp + 0x10]
        lea eax, [ecx + edx*0x1 + 0x10]
        cmp dword ptr [eax], ebp
        jnz releasesample_4993ce
        mov dword ptr [eax], 0x0
        releasesample_4993ce:
        mov eax, dword ptr [esp + 0x10]
        inc edi
        add eax, 0x14
        mov dword ptr [esp + 0x10], eax
        jmp releasesample_499395
        releasesample_4993dc:
        mov ebx, dword ptr [esi + 0xe4]
        xor edi, edi
        xor ebp, ebp
        releasesample_4993e6:
        test ebx, ebx
        jz releasesample_499425
        mov ecx, dword ptr [esi + 0xe8]
        sub ecx, ebx
        mov eax, 0x66666667
        imul ecx
        sar edx, 0x3
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        cmp edi, edx
        jnc releasesample_499425
        mov ecx, dword ptr [esi + 0xe4]
        mov edx, dword ptr [esp + 0x18]
        lea eax, [ecx + ebp*0x1 + 0x10]
        cmp dword ptr [eax], edx
        jnz releasesample_49941f
        mov dword ptr [eax], 0x0
        releasesample_49941f:
        inc edi
        add ebp, 0x14
        jmp releasesample_4993e6
        releasesample_499425:
        mov ebp, dword ptr [esi + 0xf0]
        mov edi, ebp
        test edi, edi
        jz releasesample_4994c8
        releasesample_499435:
        mov eax, dword ptr [esp + 0x18]
        cmp dword ptr [edi + 0x8], eax
        jz releasesample_49944e
        mov edi, dword ptr [edi]
        test edi, edi
        jnz releasesample_499435
        pop ebx
        pop edi
        pop esi
        xor eax, eax
        pop ebp
        pop ecx
        ret 0x4
        releasesample_49944e:
        test edi, edi
        jz releasesample_4994c8
        mov ebx, dword ptr [edi + 0x8]
        test ebx, ebx
        jz releasesample_499469
        mov ecx, ebx
        call fun_0049b5f0
        push ebx
        call fun_004bd000
        add esp, 0x4
        releasesample_499469:
        cmp edi, ebp
        jnz releasesample_499477
        mov ecx, dword ptr [edi]
        mov dword ptr [esi + 0xf0], ecx
        jmp releasesample_49947e
        releasesample_499477:
        mov edx, dword ptr [edi + 0x4]
        mov eax, dword ptr [edi]
        mov dword ptr [edx], eax
        releasesample_49947e:
        cmp edi, dword ptr [esi + 0xf4]
        jnz releasesample_499491
        mov ecx, dword ptr [edi + 0x4]
        mov dword ptr [esi + 0xf4], ecx
        jmp releasesample_499499
        releasesample_499491:
        mov edx, dword ptr [edi]
        mov eax, dword ptr [edi + 0x4]
        mov dword ptr [edx + 0x4], eax
        releasesample_499499:
        mov ecx, dword ptr [esi + 0xf8]
        mov eax, dword ptr [esi + 0xfc]
        mov dword ptr [edi], ecx
        mov ecx, dword ptr [esi + 0x100]
        inc ecx
        dec eax
        mov dword ptr [edi + 0x4], 0x0
        mov dword ptr [esi + 0xf8], edi
        mov dword ptr [esi + 0x100], ecx
        mov dword ptr [esi + 0xfc], eax
        releasesample_4994c8:
        xor eax, eax
        pop ebx
        releasesample_4994cb:
        pop edi
        pop esi
        pop ebp
        pop ecx
        ret 0x4
    }
}

