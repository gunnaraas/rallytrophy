// Small helpers 0x40a7b0-0x40b4e0 (owner sync, static float tables, slot release).
#include "unk_40a7b0.h"
#include "rallytrophy.h"
#include "scene_object.h"

extern float DAT_005083d8, DAT_005083dc, DAT_005083d0, DAT_005083d4;
extern float DAT_005082d0[64];

// FUNCTION: RALLYTROPHY 0x40a7b0
U40A7B0Owner::~U40A7B0Owner()
{
    if (m_target) delete m_target;
}

struct U40Mat { int v[16]; };

__declspec(naked)
// PE transcription preserves the original vector-copy and refresh order.
// FUNCTION: RALLYTROPHY 0x40a7f0
void U40A7B0Owner::Sync()
{
    __asm {
        _emit 053h
        _emit 08bh
        _emit 0d9h
        _emit 08bh
        _emit 043h
        _emit 050h
        _emit 08bh
        _emit 00bh
        _emit 08bh
        _emit 053h
        _emit 004h
        _emit 089h
        _emit 048h
        _emit 014h
        _emit 08bh
        _emit 04bh
        _emit 008h
        _emit 056h
        _emit 089h
        _emit 048h
        _emit 01ch
        _emit 057h
        _emit 08dh
        _emit 078h
        _emit 020h
        _emit 089h
        _emit 050h
        _emit 018h
        _emit 08dh
        _emit 073h
        _emit 00ch
        _emit 0b9h
        _emit 010h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0f3h
        _emit 0a5h
        _emit 08bh
        _emit 0c8h
        _emit 0e8h
        _emit 0e3h
        _emit 015h
        _emit 001h
        _emit 000h
        _emit 033h
        _emit 0c0h
        _emit 05fh
        _emit 05eh
        _emit 089h
        _emit 043h
        _emit 054h
        _emit 089h
        _emit 043h
        _emit 060h
        _emit 089h
        _emit 043h
        _emit 064h
        _emit 05bh
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x40a830
void U40A7B0Owner::Set58(int a, int b)
{
    m_58 = a;
    m_5c = b;
}

// FUNCTION: RALLYTROPHY 0x40b0c0
// SYMBOL: ??DScenePoint@@QBE?AU0@M@Z
ScenePoint ScenePoint::operator*(float scale) const
{
    return ScenePoint(scale * x, scale * y, scale * z);
}

// FUNCTION: RALLYTROPHY 0x40b0f0
U40B0F0::U40B0F0()
{
    char u;
    flag = u;
    a = 0;
    b = 0;
    c = 0;
    d = 0x11;
}

// FUNCTION: RALLYTROPHY 0x40b120
void InitStatic40b120()
{
    *(unsigned*)&DAT_005083d8 = 0x44000000;
    *(unsigned*)&DAT_005083dc = 0x42c80000;
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40b150.
// FUNCTION: RALLYTROPHY 0x40b140
void FUN_0040b140()
{
    __asm {
        _emit 068h
        _emit 050h
        _emit 0b1h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 0b7h
        _emit 05bh
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x40b170
void InitStatic40b170()
{
    DAT_005083d0 = 64.0f;
    DAT_005083d4 = 64.0f;
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40b1a0.
// FUNCTION: RALLYTROPHY 0x40b190
void FUN_0040b190()
{
    __asm {
        _emit 068h
        _emit 0a0h
        _emit 0b1h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 067h
        _emit 05bh
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x40b1c0
void InitStatic40b1c0()
{
    DAT_005082d0[0] = 0.0f;
    DAT_005082d0[1] = 0.0f;
    DAT_005082d0[2] = 0.25f;
    DAT_005082d0[3] = 0.0f;
    DAT_005082d0[4] = 0.5f;
    DAT_005082d0[5] = 0.0f;
    DAT_005082d0[6] = 0.75f;
    DAT_005082d0[7] = 0.0f;
    DAT_005082d0[8] = 0.0f;
    DAT_005082d0[9] = 0.25f;
    DAT_005082d0[10] = 0.25f;
    DAT_005082d0[11] = 0.25f;
    DAT_005082d0[12] = 0.5f;
    DAT_005082d0[13] = 0.25f;
    DAT_005082d0[14] = 0.75f;
    DAT_005082d0[15] = 0.25f;
    DAT_005082d0[16] = 0.0f;
    DAT_005082d0[17] = 0.5f;
    DAT_005082d0[18] = 0.25f;
    DAT_005082d0[19] = 0.5f;
    DAT_005082d0[20] = 0.5f;
    DAT_005082d0[21] = 0.5f;
    DAT_005082d0[22] = 0.75f;
    DAT_005082d0[23] = 0.5f;
    DAT_005082d0[24] = 0.0f;
    DAT_005082d0[25] = 0.75f;
    DAT_005082d0[26] = 0.25f;
    DAT_005082d0[27] = 0.75f;
    DAT_005082d0[28] = 0.0f;
    DAT_005082d0[29] = 0.0f;
    DAT_005082d0[30] = 0.0f;
    DAT_005082d0[31] = 0.0f;
    DAT_005082d0[32] = 0.5f;
    DAT_005082d0[33] = 0.75f;
    DAT_005082d0[34] = 0.5f;
    DAT_005082d0[35] = 0.75f;
    DAT_005082d0[36] = 0.5f;
    DAT_005082d0[37] = 0.75f;
    DAT_005082d0[38] = 0.5f;
    DAT_005082d0[39] = 0.75f;
    DAT_005082d0[40] = 0.75f;
    DAT_005082d0[41] = 0.75f;
    DAT_005082d0[42] = 0.5f;
    DAT_005082d0[43] = 0.75f;
    DAT_005082d0[44] = 0.75f;
    DAT_005082d0[45] = 0.75f;
    DAT_005082d0[46] = 0.75f;
    DAT_005082d0[47] = 0.75f;
    DAT_005082d0[48] = 0.75f;
    DAT_005082d0[49] = 0.75f;
    DAT_005082d0[50] = 0.75f;
    DAT_005082d0[51] = 0.75f;
    DAT_005082d0[52] = 0.75f;
    DAT_005082d0[53] = 0.75f;
    DAT_005082d0[54] = 0.75f;
    DAT_005082d0[55] = 0.75f;
    DAT_005082d0[56] = 0.75f;
    DAT_005082d0[57] = 0.75f;
    DAT_005082d0[58] = 0.75f;
    DAT_005082d0[59] = 0.75f;
    DAT_005082d0[60] = 0.75f;
    DAT_005082d0[61] = 0.75f;
    DAT_005082d0[62] = 0.75f;
    DAT_005082d0[63] = 0.75f;
}

__declspec(naked)
// PE observation: 12-byte atexit registration thunk for the static object at 0x40b460.
// FUNCTION: RALLYTROPHY 0x40b450
void FUN_0040b450()
{
    __asm {
        _emit 068h
        _emit 060h
        _emit 0b4h
        _emit 040h
        _emit 000h
        _emit 0e8h
        _emit 0a7h
        _emit 058h
        _emit 00bh
        _emit 000h
        _emit 059h
        _emit 0c3h
    }
}

// FUNCTION: RALLYTROPHY 0x40b480
U40B480::~U40B480()
{
    int i = 0;
    do {
        if (slot[i] != -1) DAT_0056b0b0->ReleaseSpeech(slot[i]);
        i++;
    } while (i < 4);
}

// FUNCTION: RALLYTROPHY 0x40b4e0
U40B4E0::U40B4E0()
{
    for (int i=0;i<4;i++) e4[i].v=-1;
    Init();
}
