// Compiler-generated helper entries identified in the Sound module.
// Their readable behavior is represented by ObjectList cleanup and the
// Transpose3x3 helper in sound.cpp.
extern "C" void fun_004bd000();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49ad30
// original: FUN_0049ad30
void __fastcall SoundObjectListCleanup(void*, void*)
{
    __asm {
        push ebx
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi]
        xor ebx, ebx
        cmp eax, ebx
        push edi
        jz cleanup_49ad73
cleanup_49ad3d:
        cmp eax, dword ptr [esi + 0x4]
        mov ecx, dword ptr [eax]
        mov dword ptr [esi], ecx
        jnz cleanup_49ad4e
        mov ecx, dword ptr [eax + 0x4]
        mov dword ptr [esi + 0x4], ecx
        jmp cleanup_49ad54
cleanup_49ad4e:
        mov edx, dword ptr [eax + 0x4]
        mov dword ptr [ecx + 0x4], edx
cleanup_49ad54:
        mov ecx, dword ptr [esi + 0x8]
        mov edi, dword ptr [esi + 0x10]
        mov edx, dword ptr [esi + 0xc]
        inc edi
        mov dword ptr [eax], ecx
        mov dword ptr [eax + 0x4], ebx
        mov dword ptr [esi + 0x8], eax
        mov eax, dword ptr [esi]
        dec edx
        cmp eax, ebx
        mov dword ptr [esi + 0x10], edi
        mov dword ptr [esi + 0xc], edx
        jnz cleanup_49ad3d
cleanup_49ad73:
        mov eax, dword ptr [esi + 0x8]
        cmp eax, ebx
        jz cleanup_49ad8f
cleanup_49ad7a:
        cmp eax, ebx
        mov edi, dword ptr [eax]
        jz cleanup_49ad89
        push eax
        call fun_004bd000
        add esp, 0x4
cleanup_49ad89:
        cmp edi, ebx
        mov eax, edi
        jnz cleanup_49ad7a
cleanup_49ad8f:
        pop edi
        mov dword ptr [esi + 0x8], ebx
        pop esi
        pop ebx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49b570
// original: FUN_0049b570
void __fastcall SoundTranspose3x3(void*, void*)
{
    __asm {
        fld float ptr [ecx + 0x4]
        mov eax, dword ptr [ecx + 0x10]
        mov edx, dword ptr [ecx + 0x20]
        fstp float ptr [ecx + 0x10]
        fld float ptr [ecx + 0x8]
        mov dword ptr [ecx + 0x4], eax
        mov eax, dword ptr [ecx + 0x24]
        fstp float ptr [ecx + 0x20]
        fld float ptr [ecx + 0x18]
        mov dword ptr [ecx + 0x8], edx
        fstp float ptr [ecx + 0x24]
        mov dword ptr [ecx + 0x18], eax
        ret
    }
}

