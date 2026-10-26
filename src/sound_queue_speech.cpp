// Exact x86 entry for T_56b0b0::QueueSpeech; its readable behavior body remains
// in sound.cpp. This preserves the original deque and callback call sequence.
class T_56b0b0 { public: void QueueSpeechExact(int, int, int*); };
extern "C" void* dat_0059b2f8;
extern "C" void fun_004bd000();
extern "C" void fun_0049b540();
extern "C" void fun_0049b550();
extern "C" void fun_0049b380();
extern "C" void fun_004c1276();
extern "C" void fun_004b73f0();
extern "C" void fun_0049ab20();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49a680
// original: T_56b0b0::QueueSpeech
void T_56b0b0::QueueSpeechExact(int, int, int*)
{
    __asm {
        sub esp, 0x24
        push esi
        mov esi, ecx
        mov al, byte ptr [esi + 0xcd]
        test al, al
        push edi
        jz queue_49a6ab
        mov eax, dword ptr [esp + 0x38]
        test eax, eax
        jz queue_49a9ef
        pop edi
        mov dword ptr [eax], 0x1
        pop esi
        add esp, 0x24
        ret 0xc
        queue_49a6ab:
        mov al, byte ptr [esi + 0xcc]
        test al, al
        jnz queue_49a9ef
        mov eax, dword ptr [esi + 0xd4]
        test eax, eax
        jz queue_49a9ef
        mov ecx, dword ptr [esi + 0xd8]
        sub ecx, eax
        mov eax, 0x66666667
        imul ecx
        sar edx, 0x3
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        jz queue_49a9ef
        mov ecx, dword ptr [esp + 0x30]
        mov edx, dword ptr [esi + ecx*0x4 + 0x148]
        lea edi, [esi + 0x294]
        push edi
        mov dword ptr [esp + 0x34], edx
        mov dword ptr [esp + 0xc], edi
        call dword ptr [dat_0059b2f8]
        mov eax, dword ptr [esi + 0x288]
        test eax, eax
        jz queue_49a72b
        mov ecx, dword ptr [esp + 0x34]
        cmp ecx, dword ptr [esi + 0x258]
        jge queue_49a72b
        push edi
        call dword ptr [dat_0059b2f8]
        pop edi
        pop esi
        add esp, 0x24
        ret 0xc
        queue_49a72b:
        mov edx, dword ptr [esp + 0x34]
        push ebx
        push ebp
        cmp edx, dword ptr [esi + 0x258]
        jle queue_49a831
        test eax, eax
        jz queue_49a81b
        queue_49a745:
        mov edx, dword ptr [esi + 0x268]
        mov ecx, dword ptr [esi + 0x288]
        add edx, 0x8
        dec ecx
        mov dword ptr [esi + 0x268], edx
        mov eax, edx
        mov dword ptr [esi + 0x288], ecx
        mov ebx, ecx
        jz queue_49a773
        cmp eax, dword ptr [esi + 0x264]
        jnz queue_49a80d
        queue_49a773:
        mov eax, dword ptr [esi + 0x26c]
        lea edi, [eax + 0x4]
        mov eax, dword ptr [eax]
        push eax
        mov dword ptr [esi + 0x26c], edi
        call fun_004bd000
        add esp, 0x4
        test ebx, ebx
        jnz queue_49a7df
        lea ecx, [esp + 0x14]
        call fun_0049b540
        mov eax, dword ptr [esp + 0x14]
        mov edx, dword ptr [esp + 0x1c]
        mov edi, dword ptr [esp + 0x20]
        lea ecx, [esi + 0x260]
        mov ebx, ecx
        mov ecx, dword ptr [esp + 0x18]
        mov dword ptr [ebx], eax
        mov dword ptr [ebx + 0x4], ecx
        mov dword ptr [ebx + 0x8], edx
        mov dword ptr [ebx + 0xc], edi
        lea ebx, [esi + 0x270]
        mov dword ptr [ebx], eax
        mov dword ptr [ebx + 0x4], ecx
        mov dword ptr [ebx + 0x8], edx
        mov edx, dword ptr [esi + 0x280]
        push edx
        mov dword ptr [ebx + 0xc], edi
        call fun_004bd000
        add esp, 0x4
        jmp queue_49a80d
        queue_49a7df:
        mov eax, dword ptr [edi]
        push edi
        push eax
        lea ecx, [esp + 0x2c]
        call fun_0049b550
        mov edx, dword ptr [esp + 0x24]
        mov eax, dword ptr [esp + 0x28]
        lea ecx, [esi + 0x260]
        mov dword ptr [ecx], edx
        mov edx, dword ptr [esp + 0x2c]
        mov dword ptr [ecx + 0x4], eax
        mov eax, dword ptr [esp + 0x30]
        mov dword ptr [ecx + 0x8], edx
        mov dword ptr [ecx + 0xc], eax
        queue_49a80d:
        mov eax, dword ptr [esi + 0x288]
        test eax, eax
        jnz queue_49a745
        queue_49a81b:
        mov edi, dword ptr [esi + 0xd4]
        mov ecx, dword ptr [edi]
        push ecx
        call dword ptr [dat_0059b2f8]
        mov dword ptr [edi + 0x8], 0x0
        queue_49a831:
        mov ebp, dword ptr [esi + 0x288]
        test ebp, ebp
        mov ebx, dword ptr [esp + 0x40]
        lea edi, [esi + 0x25c]
        jz queue_49a888
        mov eax, dword ptr [edi + 0x1c]
        mov edx, dword ptr [edi + 0x18]
        cmp eax, edx
        jz queue_49a888
        lea ecx, [eax + 0x8]
        cmp ecx, edx
        mov dword ptr [edi + 0x1c], ecx
        jnz queue_49a872
        test eax, eax
        jz queue_49a866
        mov edx, dword ptr [esp + 0x38]
        mov dword ptr [eax], edx
        mov dword ptr [eax + 0x4], ebx
        queue_49a866:
        mov ecx, edi
        call fun_0049b380
        jmp queue_49a9c8
        queue_49a872:
        test eax, eax
        jz queue_49a9c8
        mov ecx, dword ptr [esp + 0x38]
        mov dword ptr [eax], ecx
        mov dword ptr [eax + 0x4], ebx
        jmp queue_49a9c8
        queue_49a888:
        push 0x1000
        call fun_004c1276
        add esp, 0x4
        test ebp, ebp
        mov ebx, eax
        jnz queue_49a8fa
        push 0x8
        mov dword ptr [edi + 0x28], 0x2
        call fun_004c1276
        mov dword ptr [edi + 0x24], eax
        add eax, 0x4
        add esp, 0x4
        mov dword ptr [eax], ebx
        push eax
        add ebx, 0x800
        push ebx
        lea ecx, [esp + 0x2c]
        call fun_0049b550
        mov eax, dword ptr [esp + 0x24]
        mov ecx, dword ptr [esp + 0x28]
        mov ebx, dword ptr [esp + 0x30]
        lea edx, [edi + 0x4]
        mov ebp, edx
        mov edx, dword ptr [esp + 0x2c]
        mov dword ptr [ebp], eax
        mov dword ptr [ebp + 0x4], ecx
        mov dword ptr [ebp + 0x8], edx
        mov dword ptr [ebp + 0xc], ebx
        lea ebp, [edi + 0x14]
        mov dword ptr [ebp], eax
        mov dword ptr [ebp + 0x4], ecx
        mov dword ptr [ebp + 0x8], edx
        mov dword ptr [ebp + 0xc], ebx
        jmp queue_49a9ae
        queue_49a8fa:
        mov ecx, dword ptr [edi + 0x28]
        mov edx, dword ptr [edi + 0x24]
        mov eax, dword ptr [edi + 0x20]
        lea ecx, [edx + ecx*0x4 + -0x4]
        cmp eax, ecx
        jnc queue_49a93e
        add eax, 0x4
        push eax
        push ebx
        lea ecx, [esp + 0x2c]
        mov dword ptr [edi + 0x20], eax
        mov dword ptr [eax], ebx
        call fun_0049b550
        mov eax, dword ptr [esp + 0x24]
        mov ecx, dword ptr [esp + 0x28]
        lea edx, [edi + 0x14]
        mov dword ptr [edx], eax
        mov eax, dword ptr [esp + 0x2c]
        mov dword ptr [edx + 0x4], ecx
        mov ecx, dword ptr [esp + 0x30]
        mov dword ptr [edx + 0x8], eax
        mov dword ptr [edx + 0xc], ecx
        jmp queue_49a9ae
        queue_49a93e:
        sub eax, dword ptr [edi + 0x10]
        sar eax, 0x2
        inc eax
        mov ebp, eax
        lea edx, [ebp + ebp*0x1]
        push edx
        mov ecx, edi
        call fun_004b73f0
        lea ebp, [eax + ebp*0x4]
        push eax
        mov eax, dword ptr [edi + 0xc]
        push eax
        lea ecx, [esp + 0x2c]
        mov dword ptr [ebp], ebx
        call fun_0049b550
        mov edx, dword ptr [esp + 0x24]
        mov eax, dword ptr [esp + 0x28]
        lea ecx, [edi + 0x4]
        mov dword ptr [ecx], edx
        mov edx, dword ptr [esp + 0x2c]
        mov dword ptr [ecx + 0x4], eax
        mov eax, dword ptr [esp + 0x30]
        mov dword ptr [ecx + 0x8], edx
        push ebp
        mov dword ptr [ecx + 0xc], eax
        push ebx
        lea ecx, [esp + 0x2c]
        call fun_0049b550
        mov edx, dword ptr [esp + 0x24]
        mov eax, dword ptr [esp + 0x28]
        lea ecx, [edi + 0x14]
        mov dword ptr [ecx], edx
        mov edx, dword ptr [esp + 0x2c]
        mov dword ptr [ecx + 0x4], eax
        mov eax, dword ptr [esp + 0x30]
        mov dword ptr [ecx + 0x8], edx
        mov dword ptr [ecx + 0xc], eax
        queue_49a9ae:
        mov eax, dword ptr [edi + 0x1c]
        test eax, eax
        lea ecx, [eax + 0x8]
        mov dword ptr [edi + 0x1c], ecx
        jz queue_49a9c8
        mov edx, dword ptr [esp + 0x38]
        mov ecx, dword ptr [esp + 0x40]
        mov dword ptr [eax], edx
        mov dword ptr [eax + 0x4], ecx
        queue_49a9c8:
        mov edx, dword ptr [edi + 0x2c]
        mov eax, dword ptr [esp + 0x3c]
        inc edx
        mov dword ptr [edi + 0x2c], edx
        mov dword ptr [esi + 0x258], eax
        push 0x0
        mov ecx, esi
        call fun_0049ab20
        mov ecx, dword ptr [esp + 0x10]
        push ecx
        call dword ptr [dat_0059b2f8]
        pop ebp
        pop ebx
        queue_49a9ef:
        pop edi
        pop esi
        add esp, 0x24
        ret 0xc
    }
}

