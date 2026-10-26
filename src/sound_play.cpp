// Exact x86 entry for T_56b0b0::Play. The behavior implementation remains
// in sound.cpp; this isolated entry preserves the original VC6 register plan.
class SoundSource;
class T_56b0b0 { public: void PlayExact(SoundSource*); };
extern "C" void* dat_0059b2f8;

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4995d0
// original: T_56b0b0::Play
void T_56b0b0::PlayExact(SoundSource*)
{
    __asm {
        mov al, byte ptr [ecx + 0xcc]
        test al, al
        push ebx
        push ebp
        push esi
        push edi
        jnz play_499714
        mov al, byte ptr [ecx + 0xcd]
        test al, al
        jnz play_499714
        mov edi, dword ptr [esp + 0x14]
        mov ebp, dword ptr [edi + 0x4]
        cmp ebp, -0x1
        jnz play_499663
        mov ebx, 0x1
        mov esi, 0x14
        play_499606:
        mov eax, dword ptr [ecx + 0xd4]
        test eax, eax
        jz play_49965a
        mov edx, dword ptr [ecx + 0xd8]
        sub edx, eax
        mov eax, 0x66666667
        imul edx
        sar edx, 0x3
        mov eax, edx
        shr eax, 0x1f
        add edx, eax
        cmp ebx, edx
        jnc play_49965a
        mov eax, dword ptr [ecx + 0xd4]
        mov edx, dword ptr [edi]
        cmp dword ptr [esi + eax*0x1 + 0x10], edx
        jnz play_499643
        mov edx, dword ptr [esi + eax*0x1 + 0x8]
        test edx, edx
        jz play_499658
        play_499643:
        cmp ebp, -0x1
        jnz play_499652
        mov edx, dword ptr [esi + eax*0x1 + 0x8]
        test edx, edx
        jnz play_499652
        mov ebp, ebx
        play_499652:
        inc ebx
        add esi, 0x14
        jmp play_499606
        play_499658:
        mov ebp, ebx
        play_49965a:
        cmp ebp, -0x1
        jz play_499714
        play_499663:
        mov eax, dword ptr [ecx + 0xd4]
        lea edx, [ebp + ebp*0x4]
        lea esi, [eax + edx*0x4]
        mov eax, dword ptr [esi + 0xc]
        test eax, eax
        jz play_499682
        cmp eax, edi
        jz play_499682
        mov dword ptr [eax + 0x4], 0xffffffff
        play_499682:
        mov eax, dword ptr [edi]
        cmp dword ptr [esi + 0x10], eax
        mov dword ptr [edi + 0x4], ebp
        mov dword ptr [esi + 0xc], edi
        jz play_4996a1
        mov edx, dword ptr [esi]
        mov dword ptr [esi + 0x10], eax
        mov ecx, dword ptr [eax + 0x10]
        push 0x0
        push ecx
        push edx
        call dword ptr [dat_0059b2f8]
        play_4996a1:
        mov eax, dword ptr [edi + 0x18]
        test eax, eax
        mov ebp, 0x1
        jnz play_4996b1
        mov eax, dword ptr [edi]
        mov eax, dword ptr [eax]
        play_4996b1:
        mov cl, byte ptr [edi + 0xa]
        test cl, cl
        jz play_4996ba
        xor ebp, ebp
        play_4996ba:
        push eax
        mov eax, dword ptr [esi]
        push eax
        call dword ptr [dat_0059b2f8]
        mov ecx, dword ptr [edi + 0x14]
        mov edx, dword ptr [esi]
        push ecx
        push edx
        call dword ptr [dat_0059b2f8]
        mov eax, dword ptr [edi + 0x1c]
        mov ecx, dword ptr [esi]
        push eax
        push ecx
        call dword ptr [dat_0059b2f8]
        mov ebx, dword ptr [esi]
        push ebp
        push ebx
        call dword ptr [dat_0059b2f8]
        mov edx, dword ptr [edi + 0x10]
        mov eax, dword ptr [edi + 0xc]
        push edx
        push eax
        push ebx
        call dword ptr [dat_0059b2f8]
        mov ecx, dword ptr [edi + 0x5c]
        and ecx, 0xffffffe3
        mov dword ptr [edi + 0x5c], ecx
        mov eax, 0x1
        mov dword ptr [edi + 0x60], eax
        mov dword ptr [esi + 0x8], eax
        mov edx, dword ptr [esi]
        push edx
        call dword ptr [dat_0059b2f8]
        play_499714:
        pop edi
        pop esi
        pop ebp
        pop ebx
        ret 0x4
    }
}

