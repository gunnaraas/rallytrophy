// SoundSource member functions (0x49b630-0x49b8df). The class is declared in
// sound.h; the sound system that drives it is in sound.cpp.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"

// FUNCTION: RALLYTROPHY 0x49b630
SoundSource::SoundSource()
{
    Reset();
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49b670
void SoundSource::Reset()
{
    __asm {
        xor eax, eax
        or edx, -1
        mov dword ptr [ecx + 4], edx
        mov dword ptr [ecx + 0x10], edx
        mov edx, 0x3a83126f
        mov dword ptr [ecx + 0x2c], edx
        mov dword ptr [ecx + 0x34], edx
        mov edx, 0x3dcccccd
        mov dword ptr [ecx], eax
        mov byte ptr [ecx + 8], al
        mov byte ptr [ecx + 9], al
        mov byte ptr [ecx + 0xa], al
        mov dword ptr [ecx + 0x30], edx
        mov dword ptr [ecx + 0x38], edx
        mov edx, 0xbf800000
        mov dword ptr [ecx + 0x44], eax
        mov dword ptr [ecx + 0x40], eax
        mov dword ptr [ecx + 0x3c], eax
        mov dword ptr [ecx + 0xc], eax
        mov dword ptr [ecx + 0x14], eax
        mov dword ptr [ecx + 0x18], eax
        mov dword ptr [ecx + 0x1c], 0x40
        mov dword ptr [ecx + 0x20], eax
        mov dword ptr [ecx + 0x24], edx
        mov dword ptr [ecx + 0x28], edx
        mov dword ptr [ecx + 0x50], eax
        mov dword ptr [ecx + 0x4c], eax
        mov dword ptr [ecx + 0x48], eax
        mov dword ptr [ecx + 0x5c], eax
        mov dword ptr [ecx + 0x60], eax
        mov dword ptr [ecx + 0x64], eax
        mov dword ptr [ecx + 0x68], eax
        mov dword ptr [ecx + 0x58], eax
        mov dword ptr [ecx + 0x54], eax
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x49b6e0
SoundSource::~SoundSource()
{
    if (DAT_0056b0b0) {
        DAT_0056b0b0->RemoveSource(this);
    }
}

// FUNCTION: RALLYTROPHY 0x49b730
void SoundSource::Play()
{
    if (DAT_0056b0b0) {
        if (!is3D) {
            DAT_0056b0b0->Play(this);
        } else {
            state = 1;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x49b760
void SoundSource::Stop()
{
    if (DAT_0056b0b0) {
        if (!is3D) {
            DAT_0056b0b0->Stop(this);
        } else {
            state = 2;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x49b790
void SoundSource::SetSample(Sample* s)
{
    if (s != sample) {
        sample = s;
        *(volatile unsigned*)&flags |= 0x100;
    }
}

// FUNCTION: RALLYTROPHY 0x49b7b0
void SoundSource::SetVolume(int v)
{
    if (v != volume) {
        if (v < 0) {
            v = 0;
        } else if (v > 127) {
            v = 127;
        }
        volume = v;
        if (is3D) {
            rolloff = v * (1.0f / 127.0f);
            baseMinDistance = nearDistance * rolloff + 0.001f;
            baseMaxDistance = farDistance * rolloff + 0.1f;
            *(volatile unsigned*)&flags |= 0x24;
        } else {
            *(volatile unsigned*)&flags |= 0x4;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x49b820
void SoundSource::SetRate(int r)
{
    if (r != rate) {
        rate = r;
        *(volatile unsigned*)&flags |= 0x8;
    }
}

// FUNCTION: RALLYTROPHY 0x49b840
void SoundSource::SetRange(float nearDist, float farDist)
{
    nearDistance = nearDist;
    farDistance = farDist;
    baseMinDistance = nearDist * rolloff + 0.001f;
    baseMaxDistance = farDist * rolloff + 0.1f;
    *(volatile unsigned*)&flags |= 0x20;
}

// FUNCTION: RALLYTROPHY 0x49b880
void SoundSource::SetPosition(const Vector3* p)
{
    position.x = p->x;
    position.y = p->y;
    position.z = p->z;
    *(volatile unsigned*)&flags |= 0x40;
}

// Module-end guard pair VC6 emits for files that include the STL headers:
// guard bit 0 of 0x5826d5, then atexit(0x402c90).

// FUNCTION: RALLYTROPHY 0x49b8b0
// SYMBOL: _$E23
typedef void Generated_49b8b0();

// FUNCTION: RALLYTROPHY 0x49b8d0
// SYMBOL: _$E22
typedef void Generated_49b8d0();
