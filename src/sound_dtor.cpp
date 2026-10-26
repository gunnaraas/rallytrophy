// Exact x86 entry for T_56b0b0::~T_56b0b0, isolated from compiler-generated
// member destruction so its observed EH scopes and cleanup order remain intact.
class T_56b0b0 { public: void Shutdown(); };
extern "C" void* dat_0059b2f8;
extern "C" void fun_0049b000();
extern "C" void fun_00414490();
extern "C" void fun_004bd000();
extern "C" void fun_0049ada0();
extern "C" void fun_00403130();
static void __fastcall callsoundshutdown(T_56b0b0* self, void*) { self->Shutdown(); }

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x498650
// original: T_56b0b0::~T_56b0b0
void __fastcall SoundDtorExact(T_56b0b0*, void*)
{
    __asm {
        push -0x1
        push 0x4ddcdc
        mov eax, fs:[0x0]
        push eax
        mov dword ptr fs:[0x0], esp
        push ecx
        push ebx
        push ebp
        push esi
        push edi
        mov edi, ecx
        mov dword ptr [esp + 0x10], edi
        mov dword ptr [esp + 0x1c], 0xb
        call callsoundshutdown
        lea eax, [edi + 0x294]
        push eax
        call dword ptr [dat_0059b2f8]
        mov eax, dword ptr [edi + 0x288]
        lea esi, [edi + 0x25c]
        xor ebx, ebx
        cmp eax, ebx
        mov byte ptr [esp + 0x1c], 0xa
        jz sounddtor_4986ad
        sounddtor_4986a1:
        mov ecx, esi
        call fun_0049b000
        cmp dword ptr [esi + 0x2c], ebx
        jnz sounddtor_4986a1
        sounddtor_4986ad:
        mov eax, dword ptr [edi + 0x12c]
        cmp eax, ebx
        lea esi, [edi + 0x12c]
        mov byte ptr [esp + 0x1c], 0x9
        jz sounddtor_4986d0
        sounddtor_4986c2:
        push eax
        mov ecx, esi
        call fun_00414490
        mov eax, dword ptr [esi]
        cmp eax, ebx
        jnz sounddtor_4986c2
        sounddtor_4986d0:
        mov eax, dword ptr [esi + 0x8]
        cmp eax, ebx
        jz sounddtor_4986ec
        sounddtor_4986d7:
        cmp eax, ebx
        mov ebp, dword ptr [eax]
        jz sounddtor_4986e6
        push eax
        call fun_004bd000
        add esp, 0x4
        sounddtor_4986e6:
        cmp ebp, ebx
        mov eax, ebp
        jnz sounddtor_4986d7
        sounddtor_4986ec:
        mov eax, dword ptr [edi + 0x118]
        cmp eax, ebx
        mov dword ptr [esi + 0x8], ebx
        lea esi, [edi + 0x118]
        mov byte ptr [esp + 0x1c], 0x8
        jz sounddtor_498712
        sounddtor_498704:
        push eax
        mov ecx, esi
        call fun_00414490
        mov eax, dword ptr [esi]
        cmp eax, ebx
        jnz sounddtor_498704
        sounddtor_498712:
        mov eax, dword ptr [esi + 0x8]
        cmp eax, ebx
        jz sounddtor_49872e
        sounddtor_498719:
        cmp eax, ebx
        mov ebp, dword ptr [eax]
        jz sounddtor_498728
        push eax
        call fun_004bd000
        add esp, 0x4
        sounddtor_498728:
        cmp ebp, ebx
        mov eax, ebp
        jnz sounddtor_498719
        sounddtor_49872e:
        mov eax, dword ptr [edi + 0x104]
        cmp eax, ebx
        mov dword ptr [esi + 0x8], ebx
        lea esi, [edi + 0x104]
        mov byte ptr [esp + 0x1c], 0x7
        jz sounddtor_498754
        sounddtor_498746:
        push eax
        mov ecx, esi
        call fun_00414490
        mov eax, dword ptr [esi]
        cmp eax, ebx
        jnz sounddtor_498746
        sounddtor_498754:
        mov eax, dword ptr [esi + 0x8]
        cmp eax, ebx
        jz sounddtor_498770
        sounddtor_49875b:
        cmp eax, ebx
        mov ebp, dword ptr [eax]
        jz sounddtor_49876a
        push eax
        call fun_004bd000
        add esp, 0x4
        sounddtor_49876a:
        cmp ebp, ebx
        mov eax, ebp
        jnz sounddtor_49875b
        sounddtor_498770:
        mov eax, dword ptr [edi + 0xf0]
        cmp eax, ebx
        mov dword ptr [esi + 0x8], ebx
        lea esi, [edi + 0xf0]
        mov byte ptr [esp + 0x1c], 0x6
        jz sounddtor_498796
        sounddtor_498788:
        push eax
        mov ecx, esi
        call fun_00414490
        mov eax, dword ptr [esi]
        cmp eax, ebx
        jnz sounddtor_498788
        sounddtor_498796:
        mov ecx, dword ptr [esi + 0x8]
        cmp ecx, ebx
        jz sounddtor_4987b0
        sounddtor_49879d:
        cmp ecx, ebx
        mov ebp, dword ptr [ecx]
        jz sounddtor_4987aa
        push 0x1
        call fun_0049ada0
        sounddtor_4987aa:
        cmp ebp, ebx
        mov ecx, ebp
        jnz sounddtor_49879d
        sounddtor_4987b0:
        mov edx, dword ptr [edi + 0xe4]
        mov dword ptr [esi + 0x8], ebx
        mov ecx, dword ptr [edi + 0xe8]
        lea esi, [edi + 0xe0]
        push ecx
        push edx
        mov ecx, esi
        mov byte ptr [esp + 0x24], 0x5
        call fun_00403130
        mov eax, dword ptr [esi + 0x4]
        push eax
        call fun_004bd000
        mov dword ptr [esi + 0x4], ebx
        mov ecx, dword ptr [edi + 0xd4]
        mov dword ptr [esi + 0x8], ebx
        mov eax, dword ptr [edi + 0xd8]
        mov dword ptr [esi + 0xc], ebx
        lea esi, [edi + 0xd0]
        add esp, 0x4
        push eax
        push ecx
        mov ecx, esi
        mov byte ptr [esp + 0x24], 0x4
        call fun_00403130
        mov eax, dword ptr [esi + 0x4]
        push eax
        call fun_004bd000
        add esp, 0x4
        mov dword ptr [esi + 0x8], ebx
        mov edx, dword ptr [edi + 0x8]
        mov dword ptr [esi + 0x4], ebx
        mov eax, dword ptr [edi + 0x4]
        push edx
        push eax
        mov ecx, edi
        mov dword ptr [esi + 0xc], ebx
        mov dword ptr [esp + 0x24], 0xffffffff
        call fun_00403130
        mov ecx, dword ptr [edi + 0x4]
        push ecx
        call fun_004bd000
        mov ecx, dword ptr [esp + 0x18]
        add esp, 0x4
        mov dword ptr [edi + 0x4], ebx
        mov dword ptr [edi + 0x8], ebx
        mov dword ptr [edi + 0xc], ebx
        pop edi
        pop esi
        pop ebp
        pop ebx
        mov dword ptr fs:[0x0], ecx
        add esp, 0x10
        ret
    }
}

