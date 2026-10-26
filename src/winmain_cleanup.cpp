// Exact WinMain shutdown entry. The readable behavior reference is in
// winmain.cpp; this preserves VC6's observed register and destructor-call
// sequence, including the renderer-loader pointer in ECX.
extern "C" void* dat_0056b0fc;
extern "C" void* dat_0056b0bc;
extern "C" void* dat_0056b0c0;
extern "C" void* dat_0056b0b4;
extern "C" void* dat_0056b0b0;
extern "C" void* dat_0056b0ac;
extern "C" void* dat_0056b0b8;
extern "C" void* dat_0056b980;
extern "C" void fun_004bd000();
extern "C" void fun_004b6f60();
extern "C" void fun_004b6bf0();
extern "C" void fun_004959a0();
extern "C" void fun_00498650();
extern "C" void fun_004bbe20();
extern "C" void fun_004844a0();
extern "C" void fun_004052f0();
extern "C" void fun_004a4cb0();
extern "C" void fun_004a7da0();

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4a3a00
// original: FUN_004a3a00
void WinMainCleanupExact()
{
    __asm {
        push esi
        push edi
        mov esi, offset dat_0056b0fc
        xor edi, edi
    cleanup_files:
        mov eax, dword ptr [esi]
        cmp eax, edi
        jz cleanup_next_file
        push eax
        call fun_004bd000
        add esp, 4
    cleanup_next_file:
        add esi, 4
        cmp esi, 0x56b1fc
        jl cleanup_files
        mov esi, dword ptr [dat_0056b0bc]
        cmp esi, edi
        jz cleanup_keyboard_done
        mov ecx, esi
        call fun_004b6f60
        push esi
        call fun_004bd000
        add esp, 4
    cleanup_keyboard_done:
        mov esi, dword ptr [dat_0056b0c0]
        cmp esi, edi
        mov dword ptr [dat_0056b0bc], edi
        jz cleanup_mouse_done
        mov ecx, esi
        call fun_004b6bf0
        push esi
        call fun_004bd000
        add esp, 4
    cleanup_mouse_done:
        mov esi, dword ptr [dat_0056b0b4]
        cmp esi, edi
        mov dword ptr [dat_0056b0c0], edi
        jz cleanup_controllers_done
        mov ecx, esi
        call fun_004959a0
        push esi
        call fun_004bd000
        add esp, 4
    cleanup_controllers_done:
        mov esi, dword ptr [dat_0056b0b0]
        cmp esi, edi
        mov dword ptr [dat_0056b0b4], edi
        jz cleanup_sound_done
        mov ecx, esi
        call fun_00498650
        push esi
        call fun_004bd000
        add esp, 4
    cleanup_sound_done:
        mov ecx, dword ptr [dat_0056b0ac]
        cmp ecx, edi
        mov dword ptr [dat_0056b0b0], edi
        jz cleanup_device_done
        mov eax, dword ptr [ecx]
        call dword ptr [eax + 0x30]
        mov dword ptr [dat_0056b0ac], edi
    cleanup_device_done:
        mov ecx, dword ptr [dat_0056b980]
        cmp ecx, edi
        jz cleanup_loader_done
        mov esi, ecx
        call fun_004bbe20
        push esi
        call fun_004bd000
        add esp, 4
        mov dword ptr [dat_0056b980], edi
    cleanup_loader_done:
        mov esi, dword ptr [dat_0056b0b8]
        cmp esi, edi
        jz cleanup_network_done
        mov ecx, esi
        call fun_004844a0
        push esi
        call fun_004bd000
        add esp, 4
    cleanup_network_done:
        mov dword ptr [dat_0056b0b8], edi
        call fun_004052f0
        call fun_004a4cb0
        call fun_004a7da0
        pop edi
        pop esi
        ret
    }
}
