// Exact x86 entry for T_56b0b0::LoadSample, isolated so the original stack
// frame and VC6 register allocation are preserved. The readable behavior body
// remains in sound.cpp.
class T_56b0b0 { public: void* LoadSampleExact(const char*); };
extern "C" void* dat_0059b2f8;
extern "C" void* dat_004e1340;
extern "C" void* dat_004e130c;
extern "C" void fun_004c1290();
extern "C" void fun_004ce290();
extern "C" void fun_004c1276();
extern "C" void fun_00402700();
extern "C" void fun_00402980();
extern "C" void fun_004a6050();
extern "C" void fun_004bd00b();
extern "C" void fun_00402960();
extern "C" void fun_004c12db();
extern "C" void fun_004024f0();
extern "C" void fun_004c13ca();
extern "C" void fun_004a4e00();
extern "C" void fun_004a5ff0();
extern "C" void fun_004a5e00();
extern "C" void fun_004a5d00();
extern "C" void fun_004bd000();
extern "C" void fun_004a4d90();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x498ec0
// original: T_56b0b0::LoadSample
void* T_56b0b0::LoadSampleExact(const char*)
{
    __asm {
        push -0x1
        mov eax, fs:[0x0]
        push 0x4ddd35
        push eax
        mov eax, 0x40f4
        mov dword ptr fs:[0x0], esp
        call fun_004c1290
        push ebx
        push ebp
        push esi
        mov esi, dword ptr [ecx + 0xf0]
        test esi, esi
        push edi
        mov dword ptr [esp + 0x14], ecx
        jz loadsample_498f1b
        loadsample_498ef1:
        mov edi, dword ptr [esi + 0x8]
        mov eax, dword ptr [edi + 0x20]
        test eax, eax
        jnz loadsample_498f00
        mov eax, 0x4e1344
        loadsample_498f00:
        mov ecx, dword ptr [esp + 0x4114]
        push ecx
        push eax
        call fun_004ce290
        add esp, 0x8
        test eax, eax
        jz loadsample_498f4c
        mov esi, dword ptr [esi]
        test esi, esi
        jnz loadsample_498ef1
        loadsample_498f1b:
        push 0x2c
        call fun_004c1276
        mov ebp, eax
        add esp, 0x4
        mov dword ptr [esp + 0x48], ebp
        test ebp, ebp
        mov dword ptr [esp + 0x410c], 0x0
        jz loadsample_498f56
        mov dl, byte ptr [esp + 0x13]
        lea ecx, [ebp + 0x1c]
        push 0x0
        mov byte ptr [ecx], dl
        call fun_00402700
        jmp loadsample_498f58
        loadsample_498f4c:
        inc dword ptr [edi + 0x14]
        mov eax, edi
        jmp loadsample_499325
        loadsample_498f56:
        xor ebp, ebp
        loadsample_498f58:
        mov esi, dword ptr [esp + 0x4114]
        or ecx, 0xffffffff
        mov dword ptr [esp + 0x410c], ecx
        xor eax, eax
        mov edi, esi
        repne scasb
        not ecx
        dec ecx
        mov ebx, ecx
        lea edx, [ebp + 0x1c]
        push 0x1
        push ebx
        mov ecx, edx
        mov byte ptr [ebp + 0x18], 0x0
        call fun_00402980
        test al, al
        jz loadsample_498fac
        mov eax, dword ptr [ebp + 0x20]
        mov ecx, ebx
        mov edx, ecx
        shr ecx, 0x2
        mov edi, eax
        rep movsd
        mov ecx, edx
        and ecx, 0x3
        rep movsb
        mov esi, dword ptr [esp + 0x4114]
        mov dword ptr [ebp + 0x24], ebx
        mov byte ptr [eax + ebx*0x1], 0x0
        loadsample_498fac:
        push esi
        mov dword ptr [ebp + 0x14], 0x1
        call fun_004a6050
        add esp, 0x4
        test al, al
        jnz loadsample_4990f9
        mov al, byte ptr [esp + 0x13]
        push 0x0
        lea ecx, [esp + 0x20]
        mov byte ptr [esp + 0x20], al
        call fun_00402700
        or ecx, 0xffffffff
        xor eax, eax
        mov edi, 0x500cd0
        repne scasb
        not ecx
        dec ecx
        mov ebx, ecx
        push 0x1
        push ebx
        lea ecx, [esp + 0x24]
        call fun_00402980
        test al, al
        jz loadsample_499024
        mov edi, dword ptr [esp + 0x20]
        mov ecx, ebx
        mov edx, ecx
        shr ecx, 0x2
        mov esi, 0x500cd0
        rep movsd
        mov ecx, edx
        and ecx, 0x3
        rep movsb
        mov eax, dword ptr [esp + 0x20]
        mov esi, dword ptr [esp + 0x4114]
        mov dword ptr [esp + 0x24], ebx
        mov byte ptr [eax + ebx*0x1], 0x0
        loadsample_499024:
        mov edx, dword ptr [esp + 0x24]
        or ecx, 0xffffffff
        xor eax, eax
        mov edi, esi
        repne scasb
        not ecx
        dec ecx
        mov ebx, ecx
        mov ecx, dword ptr [dat_004e1340]
        sub ecx, edx
        cmp ecx, ebx
        mov dword ptr [esp + 0x410c], 0x1
        ja loadsample_499052
        call fun_004bd00b
        loadsample_499052:
        test ebx, ebx
        jbe loadsample_499092
        mov edx, dword ptr [esp + 0x24]
        lea ebp, [edx + ebx*0x1]
        push 0x0
        push ebp
        lea ecx, [esp + 0x24]
        call fun_00402980
        test al, al
        jz loadsample_499092
        mov ecx, dword ptr [esp + 0x24]
        mov eax, dword ptr [esp + 0x20]
        lea edi, [ecx + eax*0x1]
        mov ecx, ebx
        mov edx, ecx
        shr ecx, 0x2
        rep movsd
        mov ecx, edx
        and ecx, 0x3
        rep movsb
        push ebp
        lea ecx, [esp + 0x20]
        call fun_00402960
        loadsample_499092:
        lea eax, [esp + 0x14]
        push eax
        lea ecx, [esp + 0x30]
        mov dword ptr [esp + 0x18], 0x5070b0
        call fun_004c12db
        mov cl, byte ptr [esp + 0x1c]
        mov byte ptr [esp + 0x38], cl
        push 0x0
        lea ecx, [esp + 0x3c]
        mov byte ptr [esp + 0x4110], 0x2
        call fun_00402700
        mov edx, dword ptr [dat_004e1340]
        push edx
        push 0x0
        lea eax, [esp + 0x24]
        push eax
        lea ecx, [esp + 0x44]
        call fun_004024f0
        push 0x4e4328
        lea ecx, [esp + 0x30]
        push ecx
        mov dword ptr [esp + 0x34], 0x4e2c78
        mov byte ptr [esp + 0x4114], 0x1
        call fun_004c13ca
        loadsample_4990f9:
        push 0x81
        push esi
        lea ecx, [esp + 0x94]
        call fun_004a4e00
        lea ecx, [esp + 0x8c]
        mov dword ptr [esp + 0x410c], 0x3
        call fun_004a5ff0
        mov edi, eax
        push edi
        call fun_004c1276
        add esp, 0x4
        mov esi, eax
        push edi
        push esi
        lea ecx, [esp + 0x94]
        call fun_004a5e00
        lea ecx, [esp + 0x8c]
        call fun_004a5d00
        push edi
        push esi
        call dword ptr [dat_0059b2f8]
        dec eax
        mov ebx, dword ptr [dat_004e130c]
        jz loadsample_49927c
        dec eax
        jz loadsample_499251
        sub eax, 0xb
        jz loadsample_499236
        mov dl, byte ptr [esp + 0x13]
        push 0x0
        lea ecx, [esp + 0x30]
        mov byte ptr [esp + 0x30], dl
        call fun_00402700
        or ecx, 0xffffffff
        xor eax, eax
        mov edi, 0x500cb8
        repne scasb
        not ecx
        dec ecx
        mov ebx, ecx
        push 0x1
        push ebx
        lea ecx, [esp + 0x34]
        call fun_00402980
        test al, al
        jz loadsample_4991c7
        mov edi, dword ptr [esp + 0x30]
        mov ecx, ebx
        mov eax, ecx
        shr ecx, 0x2
        mov esi, 0x500cb8
        rep movsd
        mov ecx, eax
        and ecx, 0x3
        rep movsb
        mov ecx, dword ptr [esp + 0x30]
        mov dword ptr [esp + 0x34], ebx
        mov byte ptr [ecx + ebx*0x1], 0x0
        loadsample_4991c7:
        lea edx, [esp + 0x14]
        mov bl, 0x4
        push edx
        lea ecx, [esp + 0x50]
        mov byte ptr [esp + 0x4110], bl
        mov dword ptr [esp + 0x18], 0x5070b0
        call fun_004c12db
        mov al, byte ptr [esp + 0x2c]
        push 0x0
        lea ecx, [esp + 0x5c]
        mov byte ptr [esp + 0x4110], 0x5
        mov byte ptr [esp + 0x5c], al
        call fun_00402700
        mov ecx, dword ptr [dat_004e1340]
        push ecx
        push 0x0
        lea edx, [esp + 0x34]
        push edx
        lea ecx, [esp + 0x64]
        call fun_004024f0
        push 0x4e4328
        lea eax, [esp + 0x50]
        push eax
        mov dword ptr [esp + 0x54], 0x4e2c78
        mov byte ptr [esp + 0x4114], bl
        call fun_004c13ca
        loadsample_499236:
        mov edx, dword ptr [esp + 0x4114]
        push 0x0
        push 0x0
        lea ecx, [esp + 0x20]
        push ecx
        push edx
        push edi
        push esi
        call dword ptr [dat_0059b2f8]
        jmp loadsample_49926b
        loadsample_499251:
        lea eax, [esp + 0x68]
        push eax
        push esi
        call ebx
        push 0x0
        lea ecx, [esp + 0x1c]
        push ecx
        lea edx, [esp + 0x70]
        push edx
        call dword ptr [dat_0059b2f8]
        loadsample_49926b:
        push esi
        call fun_004bd000
        mov esi, dword ptr [esp + 0x1c]
        add esp, 0x4
        mov byte ptr [ebp + 0x18], 0x1
        loadsample_49927c:
        lea eax, [esp + 0x68]
        push eax
        push esi
        call ebx
        mov ecx, dword ptr [esp + 0x74]
        mov dword ptr [ebp], ecx
        mov edx, dword ptr [esp + 0x70]
        mov dword ptr [ebp + 0x4], edx
        mov eax, dword ptr [esp + 0x78]
        mov dword ptr [ebp + 0x8], eax
        mov ecx, dword ptr [esp + 0x7c]
        mov dword ptr [ebp + 0x10], esi
        mov esi, dword ptr [esp + 0x14]
        mov eax, dword ptr [esi + 0xf8]
        xor edi, edi
        cmp eax, edi
        mov dword ptr [ebp + 0xc], ecx
        jnz loadsample_4992cf
        push 0xc
        call fun_004c1276
        add esp, 0x4
        cmp eax, edi
        jz loadsample_4992cb
        mov dword ptr [eax + 0x4], edi
        mov dword ptr [eax], edi
        mov dword ptr [eax + 0x8], edi
        jmp loadsample_4992dd
        loadsample_4992cb:
        xor eax, eax
        jmp loadsample_4992dd
        loadsample_4992cf:
        mov ecx, dword ptr [eax]
        mov dword ptr [esi + 0xf8], ecx
        dec dword ptr [esi + 0x100]
        loadsample_4992dd:
        mov ecx, dword ptr [esi + 0xf4]
        cmp ecx, edi
        mov dword ptr [eax + 0x8], ebp
        mov dword ptr [eax + 0x4], ecx
        mov dword ptr [eax], edi
        jz loadsample_4992f3
        mov dword ptr [ecx], eax
        jmp loadsample_4992f9
        loadsample_4992f3:
        mov dword ptr [esi + 0xf0], eax
        loadsample_4992f9:
        mov dword ptr [esi + 0xf4], eax
        mov eax, dword ptr [esi + 0xfc]
        inc eax
        lea ecx, [esp + 0x8c]
        mov dword ptr [esi + 0xfc], eax
        mov dword ptr [esp + 0x410c], 0xffffffff
        call fun_004a4d90
        mov eax, ebp
        loadsample_499325:
        mov ecx, dword ptr [esp + 0x4104]
        pop edi
        pop esi
        pop ebp
        pop ebx
        mov dword ptr fs:[0x0], ecx
        add esp, 0x4100
        ret 0x4
    }
}

