// Exact x86 entry for T_56b0b0::T_56b0b0, isolated to prevent implicit
// member constructors from surrounding the observed instruction sequence.
class T_56b0b0 { public: void Startup(); };
extern "C" void* DAT_0059b2f8;
extern void __fastcall FUN_0049b540(void*);
static void __fastcall CallSoundStartup(T_56b0b0* self, void*) { self->Startup(); }

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4983a0
// original: T_56b0b0::T_56b0b0
void __fastcall SoundCtorExact(T_56b0b0*, void*)
{
    __asm {
        push -0x1
        push 0x4ddc2c
        mov eax, fs:[0x0]
        push eax
        mov dword ptr fs:[0x0], esp
        sub esp, 0x8
        mov al, byte ptr [esp + 0x3]
        push ebx
        push esi
        mov esi, ecx
        xor ebx, ebx
        push edi
        mov dword ptr [esp + 0x10], esi
        mov byte ptr [esi], al
        mov dword ptr [esi + 0x4], ebx
        mov dword ptr [esi + 0x8], ebx
        mov dword ptr [esi + 0xc], ebx
        mov dword ptr [esp + 0x1c], ebx
        mov cl, byte ptr [esp + 0xf]
        mov byte ptr [esi + 0xd0], cl
        mov dword ptr [esi + 0xd4], ebx
        mov dword ptr [esi + 0xd8], ebx
        mov dword ptr [esi + 0xdc], ebx
        mov dl, byte ptr [esp + 0xf]
        mov byte ptr [esi + 0xe0], dl
        mov dword ptr [esi + 0xe4], ebx
        mov dword ptr [esi + 0xe8], ebx
        mov dword ptr [esi + 0xec], ebx
        mov dword ptr [esi + 0xf8], ebx
        mov dword ptr [esi + 0xf4], ebx
        mov dword ptr [esi + 0xf0], ebx
        mov dword ptr [esi + 0xfc], ebx
        mov dword ptr [esi + 0x100], ebx
        mov dword ptr [esi + 0x10c], ebx
        mov dword ptr [esi + 0x108], ebx
        mov dword ptr [esi + 0x104], ebx
        mov dword ptr [esi + 0x110], ebx
        mov dword ptr [esi + 0x114], ebx
        mov dword ptr [esi + 0x120], ebx
        mov dword ptr [esi + 0x11c], ebx
        mov dword ptr [esi + 0x118], ebx
        mov dword ptr [esi + 0x124], ebx
        mov dword ptr [esi + 0x128], ebx
        mov dword ptr [esi + 0x134], ebx
        mov dword ptr [esi + 0x130], ebx
        mov dword ptr [esi + 0x12c], ebx
        mov dword ptr [esi + 0x138], ebx
        mov dword ptr [esi + 0x13c], ebx
        mov al, byte ptr [esp + 0xf]
        lea ecx, [esi + 0x260]
        mov byte ptr [esp + 0x1c], 0xa
        mov byte ptr [esi + 0x25c], al
        call FUN_0049b540
        mov dword ptr [esi + 0x270], ebx
        mov dword ptr [esi + 0x274], ebx
        mov dword ptr [esi + 0x278], ebx
        mov dword ptr [esi + 0x27c], ebx
        mov dword ptr [esi + 0x280], ebx
        mov dword ptr [esi + 0x284], ebx
        mov dword ptr [esi + 0x288], ebx
        lea edi, [esi + 0x148]
        mov ecx, 0x44
        xor eax, eax
        mov dword ptr [esi + 0x14], ebx
        mov dword ptr [esi + 0xc8], ebx
        mov byte ptr [esi + 0xcc], bl
        mov byte ptr [esi + 0xcd], 0x1
        mov dword ptr [esi + 0x140], ebx
        mov dword ptr [esi + 0x144], 0x64
        mov byte ptr [esp + 0x1c], 0xb
        rep stosd
        mov dword ptr [esi + 0x258], 0xffffffff
        mov dword ptr [esi + 0x28], ebx
        mov dword ptr [esi + 0x24], ebx
        mov dword ptr [esi + 0x20], ebx
        mov eax, 0x3f800000
        mov dword ptr [esi + 0x1c], eax
        mov dword ptr [esi + 0xa4], ebx
        mov dword ptr [esi + 0xa0], ebx
        mov dword ptr [esi + 0x9c], ebx
        mov ecx, esi
        mov dword ptr [esi + 0x38], ebx
        mov dword ptr [esi + 0x34], ebx
        mov dword ptr [esi + 0x2c], ebx
        mov dword ptr [esi + 0x48], ebx
        mov dword ptr [esi + 0x40], ebx
        mov dword ptr [esi + 0x3c], ebx
        mov dword ptr [esi + 0x54], ebx
        mov dword ptr [esi + 0x50], ebx
        mov dword ptr [esi + 0x4c], ebx
        mov dword ptr [esi + 0x58], eax
        mov dword ptr [esi + 0x44], eax
        mov dword ptr [esi + 0x30], eax
        mov dword ptr [esi + 0xb0], ebx
        mov dword ptr [esi + 0xac], ebx
        mov dword ptr [esi + 0xa8], ebx
        call CallSoundStartup
        lea ecx, [esi + 0x294]
        push ecx
        call dword ptr [DAT_0059b2f8]
        mov ecx, dword ptr [esp + 0x14]
        pop edi
        mov dword ptr [esi + 0x28c], 0x7f
        mov dword ptr [esi + 0x290], 0x1
        mov eax, esi
        pop esi
        pop ebx
        mov dword ptr fs:[0x0], ecx
        add esp, 0x14
        ret
    }
}
