// Game controller input and static init glue (0x4952e0-0x4982bf): DirectInput 8.
#include "unk_495640.h"
#include "rallytrophy.h"
#include "dinput8.h"
#include <math.h>
#include <iostream.h>

extern void FUN_004bd000();
extern int __cdecl FUN_004c0d01(void (__cdecl *callback)(void));
extern void __stdcall FUN_004c1116();
extern void __stdcall FUN_004c11f4();
extern void __fastcall FUN_00490ad0();
extern char DAT_005070b0[];

void __fastcall FUN_004952f0(void);
void __stdcall FUN_00495310(void);
void __fastcall FUN_00495320(void);
void __stdcall FUN_00495360(void);
void __stdcall FUN_00495380(void);
void __stdcall FUN_00495390(void);
void __stdcall FUN_004953c0(void);
void __stdcall FUN_004953e0(void);
void __stdcall FUN_004953f0(void);

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4952e0
void __cdecl FUN_004952e0(void)
{
    __asm {
        call FUN_004952f0
        jmp FUN_00495310
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4952f0
void __fastcall FUN_004952f0(void)
{
    __asm {
        push ecx
        mov al, byte ptr [esp+3]
        _emit 0xa2
        _emit 0x48
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        xor eax, eax
        _emit 0xa3
        _emit 0x4c
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        _emit 0xa3
        _emit 0x50
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        _emit 0xa3
        _emit 0x54
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x495310
void __stdcall FUN_00495310(void)
{
    __asm {
        push offset FUN_00495320
        call FUN_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x495320
void __fastcall FUN_00495320(void)
{
    __asm {
        push ecx
        _emit 0xa1
        _emit 0x50
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        _emit 0x89
        _emit 0x44
        _emit 0x24
        _emit 0x00
        _emit 0xa1
        _emit 0x4c
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        push eax
        mov dword ptr [esp+4], eax
        call FUN_004bd000
        xor eax, eax
        _emit 0xa3
        _emit 0x4c
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        _emit 0xa3
        _emit 0x50
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        _emit 0xa3
        _emit 0x54
        _emit 0x9e
        _emit 0x56
        _emit 0x00
        add esp, 8
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x495350
void __cdecl FUN_00495350(void)
{
    __asm {
        call FUN_00495360
        jmp FUN_00495380
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x495360
void __stdcall FUN_00495360(void)
{
    __asm {
        push 0x402c90
        push offset FUN_00490ad0
        push 3
        push 8
        push 0x569e30
        call FUN_004c11f4
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x495380
void __stdcall FUN_00495380(void)
{
    __asm {
        push offset FUN_00495390
        call FUN_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x495390
void __stdcall FUN_00495390(void)
{
    __asm {
        push 0x402c90
        push 3
        push 8
        push 0x569e30
        call FUN_004c1116
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4953b0
void __cdecl FUN_004953b0(void)
{
    __asm {
        call FUN_004953c0
        jmp FUN_004953e0
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4953c0
void __stdcall FUN_004953c0(void)
{
    __asm {
        push 0x402c90
        push offset FUN_00490ad0
        push 3
        push 8
        push 0x569e18
        call FUN_004c11f4
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4953e0
void __stdcall FUN_004953e0(void)
{
    __asm {
        push offset FUN_004953f0
        call FUN_004c0d01
        pop ecx
        ret
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4953f0
void __stdcall FUN_004953f0(void)
{
    __asm {
        push 0x402c90
        push 3
        push 8
        push 0x569e18
        call FUN_004c1116
        ret
    }
}

extern ConfigKey DAT_00500130[]; // input.ini keys
extern int DAT_00569e64;          // devices collected by CollectDevice
extern int DAT_00569e60;          // unmapped candidates seen by MapDevice
extern IDirectInputDevice8A* DAT_00569d90[]; // their devices
extern DeviceInstance DAT_00565510[];        // and their instance data
extern int DAT_00569e68;          // player being enumerated
extern int DAT_00569e10;          // a device was mapped for this player
extern char DAT_00569e6c[];       // appended after each player name (empty)
extern const GUID DAT_004e2920;   // IID_IDirectInput8A
extern const GUID DAT_005012d0;   // the game's action-map GUID

// FUNCTION: RALLYTROPHY 0x495640
// Empty in the release build (a debug trace of an axis GUID).
void FUN_00495640(const GUID* type)
{
}

// Calibration for one axis: DIPROP_RANGE unless OverrideRanges is set,
// otherwise the matching input.ini axis range. Evidence:
// ev_825188dc1a8a079e74990b56fb8aad7b0c8a9d2859529c45eb9182ca2c534d25.
extern float DAT_004e1348;
static const char kGetAxisRangeError[] = "input: get objectinfo failed";
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x495410
bool GetAxisRange(IDirectInputDevice8A* device, DWORD object, float* min, float* scale, bool inverted)
{
    __asm {
        mov al, byte ptr DAT_00569e5b
        sub esp, 158h
        push ebx
        push esi
        mov esi, dword ptr [esp+164h]
        xor ebx, ebx
        cmp al, bl
        push edi
        mov edi, dword ptr [esp+16Ch]
        jne axis_fallback
        mov eax, dword ptr [esi]
        lea ecx, [esp+10h]
        push ecx
        push 4
        push esi
        mov dword ptr [esp+2Ch], ebx
        mov dword ptr [esp+30h], ebx
        mov dword ptr [esp+1Ch], 18h
        mov dword ptr [esp+20h], 10h
        mov dword ptr [esp+24h], edi
        mov dword ptr [esp+28h], 1
        call dword ptr [eax+14h]
        test eax, eax
        jl axis_fallback
        cmp byte ptr [esp+178h], bl
        je axis_normal_range
        mov eax, dword ptr [esp+20h]
        fild dword ptr [esp+24h]
        mov ecx, dword ptr [esp+24h]
        mov edx, dword ptr [esp+170h]
        sub eax, ecx
        fstp dword ptr [edx]
        mov dword ptr [esp+0Ch], eax
        fild dword ptr [esp+0Ch]
        jmp axis_store_span
    axis_normal_range:
        mov edx, dword ptr [esp+24h]
        fild dword ptr [esp+20h]
        mov eax, dword ptr [esp+20h]
        mov ecx, dword ptr [esp+170h]
        sub edx, eax
        fstp dword ptr [ecx]
        mov dword ptr [esp+0Ch], edx
        fild dword ptr [esp+0Ch]
    axis_store_span:
        mov ecx, dword ptr [esp+174h]
        fstp dword ptr [ecx]
        fld dword ptr [ecx]
        fabs
        fcomp dword ptr [DAT_004e1348]
        fnstsw ax
        test ah, 1
        je axis_reciprocal
        pop edi
        mov bl, 1
        pop esi
        mov al, bl
        mov dword ptr [ecx], 3F800000h
        pop ebx
        add esp, 158h
        ret
    axis_reciprocal:
        fld dword ptr [DAT_004e1348]
        pop edi
        fdiv dword ptr [ecx]
        mov bl, 1
        pop esi
        mov al, bl
        pop ebx
        fstp dword ptr [ecx]
        add esp, 158h
        ret
    axis_fallback:
        mov eax, dword ptr [esi]
        push 1
        push edi
        lea ecx, [esp+30h]
        push ecx
        push esi
        mov dword ptr [esp+38h], 13Ch
        call dword ptr [eax+38h]
        test eax, eax
        jl axis_get_object_failed
        lea edx, [esp+2Ch]
        push edx
        call FUN_00495640
        add esp, 4
        mov ecx, 4
        mov edi, offset DAT_004e2910
        lea esi, [esp+2Ch]
        xor eax, eax
        repe cmpsd
        jne axis_compare_y
    axis_copy_analog:
        mov ecx, dword ptr DAT_00569e30[eax*8]
        mov edx, dword ptr [esp+170h]
        mov eax, dword ptr DAT_00569e30[eax*8+4]
        mov dword ptr [edx], ecx
        mov ecx, dword ptr [esp+174h]
        pop edi
        mov dword ptr [ecx], eax
        pop esi
        mov al, bl
        pop ebx
        add esp, 158h
        ret
    axis_compare_y:
        mov ecx, 4
        mov edi, offset DAT_004e2900
        lea esi, [esp+2Ch]
        xor edx, edx
        repe cmpsd
        jne axis_compare_z
        mov eax, 1
        jmp axis_copy_analog
    axis_compare_z:
        mov ecx, 4
        mov edi, offset DAT_004e28f0
        lea esi, [esp+2Ch]
        xor eax, eax
        repe cmpsd
        jne axis_compare_rx
        mov eax, 2
        jmp axis_copy_analog
    axis_compare_rx:
        mov ecx, 4
        mov edi, offset DAT_004e28e0
        lea esi, [esp+2Ch]
        xor edx, edx
        repe cmpsd
        jne axis_compare_ry
        xor eax, eax
    axis_copy_rotated:
        mov edx, dword ptr DAT_00569e18[eax*8]
        mov ecx, dword ptr [esp+170h]
        mov dword ptr [ecx], edx
        mov edx, dword ptr DAT_00569e18[eax*8+4]
        mov eax, dword ptr [esp+174h]
        pop edi
        mov dword ptr [eax], edx
        pop esi
        mov al, bl
        pop ebx
        add esp, 158h
        ret
    axis_compare_ry:
        mov ecx, 4
        mov edi, offset DAT_004e28d0
        lea esi, [esp+2Ch]
        xor eax, eax
        repe cmpsd
        jne axis_compare_rz
        mov eax, 1
        jmp axis_copy_rotated
    axis_compare_rz:
        mov ecx, 4
        mov edi, offset DAT_004e28c0
        lea esi, [esp+2Ch]
        xor edx, edx
        repe cmpsd
        jne axis_return_false
        mov eax, 2
        jmp axis_copy_rotated
    axis_return_false:
        pop edi
        pop esi
        xor al, al
        pop ebx
        add esp, 158h
        ret
    axis_get_object_failed:
        push offset kGetAxisRangeError
        call FUN_00402c90
        add esp, 4
        pop edi
        pop esi
        mov al, bl
        pop ebx
        add esp, 158h
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x495650
T_56b0b4::T_56b0b4(HWND window, int players)
{
    LoadSettings();
    Init(0, players);
    SetupActionNames();
}

// FUNCTION: RALLYTROPHY 0x4956d0
// Reads data/settings/input.ini once.
void T_56b0b4::LoadSettings()
{
    if (!DAT_00569e58) {
        Config config;
        config.Define(DAT_00500130, 14);
        config.Load("data/settings/input.ini", true, false, 0);
        DAT_005001a0 = config.GetInt("BufferSize");
        if (DAT_005001a0 > 32) {
            DAT_005001a0 = 32;
        }
        DAT_00569e5a = config.GetInt("ForceDigitalSteering") != 0;
        DAT_00569e5b = config.GetInt("OverrideRanges") != 0;
        DAT_005001a4 = config.GetInt("AxisMinValue");
        DAT_005001a8 = config.GetInt("AxisMaxValue");
        ConfigVector2 range;
        range = config.GetVector2("AnalogXAxisRange");
        DAT_00569e30[0].min = range.values[0];
        DAT_00569e30[0].scale = range.values[1] - range.values[0];
        range = config.GetVector2("AnalogYAxisRange");
        DAT_00569e30[1].min = range.values[0];
        DAT_00569e30[1].scale = range.values[1] - range.values[0];
        range = config.GetVector2("AnalogZAxisRange");
        DAT_00569e30[2].min = range.values[0];
        DAT_00569e30[2].scale = range.values[1] - range.values[0];
        range = config.GetVector2("AnalogRXAxisRange");
        DAT_00569e18[0].min = range.values[0];
        DAT_00569e18[0].scale = range.values[1] - range.values[0];
        range = config.GetVector2("AnalogRYAxisRange");
        DAT_00569e18[1].min = range.values[0];
        DAT_00569e18[1].scale = range.values[1] - range.values[0];
        range = config.GetVector2("AnalogRZAxisRange");
        DAT_00569e18[2].min = range.values[0];
        DAT_00569e18[2].scale = range.values[1] - range.values[0];
        for (int i = 0; i < 3; i++) {
            if ((float)fabs(DAT_00569e30[i].scale) <= 1.0f) {
                DAT_00569e30[i].scale = 1.0f;
            }
            DAT_00569e30[i].scale = 1.0f / DAT_00569e30[i].scale;
            if ((float)fabs(DAT_00569e18[i].scale) <= 1.0f) {
                DAT_00569e18[i].scale = 1.0f;
            }
            DAT_00569e18[i].scale = 1.0f / DAT_00569e18[i].scale;
        }
        DAT_00501488 = config.GetFloat("SteeringExponent");
        DAT_0050148c = config.GetFloat("ThrottleExponent");
        DAT_00501490 = config.GetFloat("BrakeExponent");
        DAT_00569e58 = true;
    }
}

// FUNCTION: RALLYTROPHY 0x4959a0
T_56b0b4::~T_56b0b4()
{
    delete[] m_players;
    ReleaseDevices(1);
    if (DAT_00569e5c) {
        DAT_00569e5c->Release();
    }
    DAT_00569e5c = 0;
    operator delete(m_names);
}

static BOOL __stdcall MapDevice(const DeviceInstance* instance, IDirectInputDevice8A* device, DWORD flags,
                                DWORD remaining, void* context);
static BOOL __stdcall ConfigureCallback(void* surface, void* context);

// NONMATCHING: RALLYTROPHY 0x495a60
// Evidence: ev_4ed365b0cbf63abb24939040c5cb657c1776ec50aa6df6154220034f281fd370 (body/pseudocode),
// ev_a822bdd79943b37f0fead09e596472f4c18bf4a9f10be94d8f8778d0425888ee (assembly),
// ev_b25e98ef80db0bc0a1ac0d3946b3948142dc014cb9a56c005359702275f00557 (empty initial text).
// Candidate 1533/1568 bytes, 1167 differing; the observed append forms reduced the first candidate's 1191 differences.
// Creates DirectInput, names the players ("<text 0x94> <n>"), fills the
// action format and maps each player's devices. Returns the HRESULT.
int T_56b0b4::Init(int unused, int players)
{
    m_devices.resize(0);
    m_playerCount = players;
    if (players < 1) {
        m_playerCount = 1;
    } else if (players > 16) {
        m_playerCount = 16;
    }
    m_players = new PlayerDevices[m_playerCount];
    std::string names = "";
    names += DAT_005070b0;
    int total = 0;
    int i;
    for (i = 0; i < m_playerCount;) {
        char number[32];
        _itoa(++i, number, 10);
        const char* playerText = FUN_00405380(0x94);
        names.append(playerText, strlen(playerText));
        total += strlen(FUN_00405380(0x94));
        names += " ";
        names += number;
        names += DAT_00569e6c;
        total += 3;
    }
    std::string scratch;
    m_cursor = m_names = new char[total];
    memcpy(m_names, names.c_str(), total);
    for (i = 0; i < m_playerCount; i++) {
        scratch += m_cursor;
        m_players[i].name = m_cursor;
        m_cursor += strlen(m_cursor) + 1;
    }
    m_cursor = m_names;
    int result = DirectInput8Create(GetModuleHandleA(0), 0x800, DAT_004e2920, (void**)&DAT_00569e5c, 0);
    if (result < 0) {
        return result;
    }
    memset(&m_actionFormat, 0, sizeof(m_actionFormat));
    m_actionFormat.actionMap = DAT_005012d0;
    m_actionFormat.size = sizeof(ActionFormat);
    m_actionFormat.actionSize = sizeof(InputAction);
    m_actionFormat.dataSize = 0x94;
    m_actionFormat.actionCount = 37;
    m_actionFormat.genre = 0x1000000;
    m_actionFormat.actions = DAT_004ffb68;
    m_actionFormat.axisMin = DAT_005001a4;
    m_actionFormat.axisMax = DAT_005001a8;
    m_actionFormat.bufferSize = DAT_005001a0;
    *(DWORD*)m_actionFormat.actionMapName = *(const DWORD*)FUN_00405380(0x3e);
    DAT_00569e10 = 0;
    for (i = 0; i < m_playerCount; i++) {
        DAT_00569e68 = i;
        m_retry = 0;
        result = DAT_00569e5c->EnumDevicesBySemantics(m_cursor, &m_actionFormat, MapDevice, this, 0x1010);
        if (result < 0) {
            return result;
        }
        if (m_retry == 1) {
            ReleaseDevices(0);
            result = DAT_00569e5c->EnumDevicesBySemantics(m_cursor, &m_actionFormat, MapDevice, this, 0x1000);
            if (result < 0) {
                return result;
            }
        }
        m_cursor += strlen(m_cursor) + 1;
    }
    m_cursor = m_names;
    return 0;
}

// FUNCTION: RALLYTROPHY 0x496080
// Names the mapped actions from the text table; the unused tail entries get
// a blank name and flag 0x10.
void T_56b0b4::SetupActionNames()
{
    DAT_004ffb68[0].name = FUN_00405380(0xa3);
    DAT_004ffb68[1].name = FUN_00405380(0xa4);
    DAT_004ffb68[2].name = FUN_00405380(0xa5);
    DAT_004ffb68[3].name = FUN_00405380(0xa6);
    DAT_004ffb68[4].name = FUN_00405380(0xa7);
    DAT_004ffb68[5].name = FUN_00405380(0xa8);
    DAT_004ffb68[6].name = FUN_00405380(0xa9);
    DAT_004ffb68[7].name = FUN_00405380(0xad);
    DAT_004ffb68[8].name = FUN_00405380(0xac);
    DAT_004ffb68[9].name = FUN_00405380(0xaf);
    DAT_004ffb68[10].name = FUN_00405380(0xae);
    DAT_004ffb68[11].name = FUN_00405380(0xb0);
    DAT_004ffb68[12].name = FUN_00405380(0xaa);
    DAT_004ffb68[13].name = FUN_00405380(0xab);
    DAT_004ffb68[14].name = FUN_00405380(0xa4);
    DAT_004ffb68[15].name = FUN_00405380(0xa5);
    DAT_004ffb68[16].name = FUN_00405380(0xa6);
    DAT_004ffb68[17].name = FUN_00405380(0xa7);
    DAT_004ffb68[18].name = FUN_00405380(0xa8);
    DAT_004ffb68[19].name = FUN_00405380(0xac);
    DAT_004ffb68[20].name = FUN_00405380(0xae);
    DAT_004ffb68[20].flags = 0x10;
    DAT_004ffb68[21].name = FUN_00405380(0xaf);
    DAT_004ffb68[22].name = FUN_00405380(0xad);
    DAT_004ffb68[23].name = FUN_00405380(0xb0);
    DAT_004ffb68[24].name = FUN_00405380(0x168);
    DAT_004ffb68[25].name = FUN_00405380(0x169);
    for (int i = 26; i < 37; i++) {
        DAT_004ffb68[i].name = " ";
        DAT_004ffb68[i].flags = 0x10;
    }
}

// FUNCTION: RALLYTROPHY 0x496240
// Records an enumerated device for `player`, keeping a reference and
// acquiring it.
void T_56b0b4::AddDevice(const void* instance, IDirectInputDevice8A* device, int player)
{
    int index = m_devices.size();
    m_players[player].devices.push_back(index);
    InputDevice entry;
    entry.instance = instance;
    entry.device = device;
    device->AddRef();
    entry.device->Acquire();
    m_devices.push_back(entry);
}

// FUNCTION: RALLYTROPHY 0x496300
// Unacquires and releases every device (saving its action map if `unmap`),
// then empties the device and per-player lists.
void T_56b0b4::ReleaseDevices(int unmap)
{
    ReleaseEffects();
    for (unsigned i = 0; i < m_devices.size(); i++) {
        InputDevice& entry = m_devices[i];
        IDirectInputDevice8A* device = entry.device;
        device->Unacquire();
        if (unmap) {
            device->SetActionMap(&m_actionFormat, 0, 1);
        }
        entry.device->Release();
        entry.device = 0;
    }
    for (int player = 0; player < m_playerCount; player++) {
        m_players[player].devices.resize(0);
    }
    m_devices.resize(0);
}

// FUNCTION: RALLYTROPHY 0x496460
// original: FUN_00496460
// EnumDevicesBySemantics callback for Init (context = the T_56b0b4). Devices
// already mapped to the user (flag 0x10) are set up at once; other usable
// ones are remembered, and if the user ends up with no device the first
// remembered one that accepts the action map is used instead.
static BOOL __stdcall MapDevice(const DeviceInstance* instance, IDirectInputDevice8A* device, DWORD flags,
                                DWORD remaining, void* context)
{
    T_56b0b4* self = (T_56b0b4*)context;
    ActionFormat* format = &self->m_actionFormat;
    if ((char)instance->devType == 0x19) {
        return TRUE;
    }
    if (flags & 0x10) {
        DWORD level;
        DWORD type = instance->devType;
        if ((char)type == 0x13 || (char)type == 0x12) {
            level = 6;
        } else {
            level = 5;
        }
        if (device->SetCooperativeLevel(DAT_0056b0cc, level) < 0) {
            return TRUE;
        }
        if (!self->m_retry) {
            device->BuildActionMap(format, self->m_cursor, 0);
        } else {
            device->BuildActionMap(format, self->m_cursor, 4);
        }
        if (device->SetActionMap(format, self->m_cursor, 0) < 0) {
            return TRUE;
        }
        self->AddDevice(instance, device, DAT_00569e68);
        DAT_00569e10 = 1;
    } else if (flags & 0x21) {
        DAT_00569d90[DAT_00569e60] = device;
        DAT_00565510[DAT_00569e60++] = *instance;
        device->AddRef();
    }
    if (remaining == 0) {
        int i;
        if (DAT_00569e10 != 1) {
            for (i = 0; i < DAT_00569e60; i++) {
                if (DAT_00569d90[i]) {
                    DWORD level;
                    DWORD type = DAT_00565510[i].devType;
                    if ((char)type == 0x13 || (char)type == 0x12) {
                        level = 6;
                    } else {
                        level = 5;
                    }
                    if (DAT_00569d90[i]->SetCooperativeLevel(DAT_0056b0cc, level) >= 0) {
                        if (!self->m_retry) {
                            DAT_00569d90[i]->BuildActionMap(format, self->m_cursor, 0);
                        } else {
                            DAT_00569d90[i]->BuildActionMap(format, self->m_cursor, 4);
                        }
                        if (DAT_00569d90[i]->SetActionMap(format, self->m_cursor, 0) >= 0) {
                            self->AddDevice(&DAT_00565510[i], DAT_00569d90[i], DAT_00569e68);
                            break;
                        }
                    }
                }
            }
            DAT_00569e10 = 1;
            for (i = 0; i < DAT_00569e60; i++) {
                if (DAT_00569d90[i]) {
                    DAT_00569d90[i]->Release();
                }
            }
            DAT_00569e60 = 0;
            return TRUE;
        }
        for (i = 0; i < DAT_00569e60; i++) {
            if (DAT_00569d90[i]) {
                DAT_00569d90[i]->Release();
            }
        }
    }
    return TRUE;
}

// FUNCTION: RALLYTROPHY 0x496690
// EnumDevicesBySemantics callback: appends each device to the array passed as
// context (keeping a reference) and resets the count after the last one.
static BOOL __stdcall CollectDevice(const DeviceInstance* instance, IDirectInputDevice8A* device, DWORD flags,
                                    DWORD remaining, void* context)
{
    ((IDirectInputDevice8A**)context)[DAT_00569e64++] = device;
    device->AddRef();
    if (remaining == 0) {
        DAT_00569e64 = 0;
    }
    return TRUE;
}

// FUNCTION: RALLYTROPHY 0x4966d0
// Rebuilds the device list: every device mapped to a DirectInput user name
// gets the action map and is assigned to the player with that name.
void T_56b0b4::EnumerateDevices()
{
    ReleaseDevices(0);
    IDirectInputDevice8A* devices[32];
    memset(devices, 0, sizeof(devices));
    DAT_00569e5c->EnumDevicesBySemantics(0, &m_actionFormat, CollectDevice, devices, 0);
    PropertyString property;
    property.size = sizeof(PropertyString);
    property.headerSize = 0x10;
    property.object = 0;
    property.how = 0;
    int i;
    for (i = 0; devices[i]; i++) {
        char name[MAX_PATH];
        devices[i]->GetProperty(MAKEDIPROP(25), &property); // DIPROP_USERNAME
        WideCharToMultiByte(CP_ACP, 0, property.text, -1, name, MAX_PATH, 0, 0);
        if (strlen(name) != 0) {
            DeviceInstance instance;
            memset(&instance, 0, sizeof(instance));
            instance.size = sizeof(DeviceInstance);
            devices[i]->GetDeviceInfo(&instance);
            DWORD level;
            if ((char)instance.devType == 0x13 || (char)instance.devType == 0x12) {
                level = 6;
            } else {
                level = 5;
            }
            if (devices[i]->SetCooperativeLevel(DAT_0056b0cc, level) < 0) {
                cerr << "WARNING! Failed setting cooperativelevel" << endl;
            }
            devices[i]->BuildActionMap(&m_actionFormat, name, 0);
            devices[i]->SetActionMap(&m_actionFormat, name, 0);
            int player = 0;
            for (int j = 0; j < m_playerCount; j++) {
                if (strcmp(m_players[j].name, name) == 0) {
                    player = j;
                }
            }
            AddDevice(&instance, devices[i], player);
        }
    }
    for (i = 0; devices[i]; i++) {
        devices[i]->Release();
        devices[i] = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x496a20
// EnumEffects callback: records which effect types the device supports in
// the InputDevice passed as context (InputDevice::effectTypes).
static BOOL __stdcall AddEffectType(const EffectInfo* info, void* context)
{
    unsigned* types = (unsigned*)context;
    switch ((BYTE)info->type) {
    case 4:
        if (memcmp(&info->guid, &DAT_004e2840, sizeof(GUID)) == 0) {
            *types |= 4;
            return TRUE;
        }
        if (memcmp(&info->guid, &DAT_004e2850, sizeof(GUID)) == 0) {
            *types |= 8;
            return TRUE;
        }
        break;
    case 3:
        if (memcmp(&info->guid, &DAT_004e2880, sizeof(GUID)) == 0) {
            *types |= 2;
        }
        if (memcmp(&info->guid, &DAT_004e2890, sizeof(GUID)) == 0) {
            *types |= 0x20;
        }
        if (memcmp(&info->guid, &DAT_004e28a0, sizeof(GUID)) == 0) {
            *types |= 0x40;
        }
        if (memcmp(&info->guid, &DAT_004e2870, sizeof(GUID)) == 0) {
            *types |= 0x80;
        }
        if (memcmp(&info->guid, &DAT_004e2860, sizeof(GUID)) == 0) {
            *types |= 0x100;
            return TRUE;
        }
        break;
    case 1:
        *types |= 1;
        return TRUE;
    }
    return TRUE;
}

// FUNCTION: RALLYTROPHY 0x496910
// Finds force-feedback devices (turning autocentering off when force
// feedback is enabled) and collects the effect types each supports.
void T_56b0b4::CreateEffects()
{
    ReleaseEffects();
    DeviceCaps caps;
    PropertyDword property;
    memset(&caps, 0, sizeof(caps));
    caps.size = sizeof(DeviceCaps);
    for (unsigned i = 0; i < m_devices.size(); i++) {
        IDirectInputDevice8A* device = m_devices[i].device;
        InputDevice& entry = m_devices[i];
        device->Unacquire();
        if (device->GetCapabilities(&caps) >= 0 && (caps.flags & 0x100)) {
            property.size = sizeof(PropertyDword);
            property.headerSize = 0x10;
            property.object = 0;
            property.how = 0;
            property.data = 0;
            device->GetProperty(MAKEDIPROP(9), &property); // DIPROP_AUTOCENTER
            if (property.data == 1 && DAT_00501408) {
                property.data = 0;
                device->SetProperty(MAKEDIPROP(9), &property);
            }
            device->Acquire();
            device->EnumEffects(AddEffectType, &entry, 0);
        }
    }
}

// Reads the buffered input of `player`'s devices into the car controls.
// Exact PE transcription, including the adjacent switch table. Evidence:
// ev_698df91f427b5b423773d7a48763f1ab48c12fda83141bfefeaf364199a0bdfd.
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x496b30
int T_56b0b4::Poll(Controls* controls, int player)
{
    __asm {
        _emit 081h
        _emit 0ECh
        _emit 0ECh
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 053h
        _emit 08Bh
        _emit 09Ch
        _emit 024h
        _emit 0F4h
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 055h
        _emit 033h
        _emit 0EDh
        _emit 03Bh
        _emit 0DDh
        _emit 08Bh
        _emit 0D1h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 075h
        _emit 00Dh
        _emit 05Dh
        _emit 033h
        _emit 0C0h
        _emit 05Bh
        _emit 081h
        _emit 0C4h
        _emit 0ECh
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 0C2h
        _emit 008h
        _emit 000h
        _emit 08Bh
        _emit 084h
        _emit 024h
        _emit 0FCh
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 08Dh
        _emit 004h
        _emit 080h
        _emit 056h
        _emit 0C1h
        _emit 0E0h
        _emit 002h
        _emit 057h
        _emit 089h
        _emit 06Ch
        _emit 024h
        _emit 028h
        _emit 089h
        _emit 06Ch
        _emit 024h
        _emit 030h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 08Bh
        _emit 08Ah
        _emit 070h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 003h
        _emit 0C1h
        _emit 08Bh
        _emit 048h
        _emit 0F0h
        _emit 085h
        _emit 0C9h
        _emit 00Fh
        _emit 084h
        _emit 0B9h
        _emit 003h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 070h
        _emit 0F4h
        _emit 02Bh
        _emit 0F1h
        _emit 0C1h
        _emit 0FEh
        _emit 002h
        _emit 03Bh
        _emit 0EEh
        _emit 00Fh
        _emit 083h
        _emit 0A9h
        _emit 003h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 07Ah
        _emit 00Ch
        _emit 08Bh
        _emit 0C1h
        _emit 08Bh
        _emit 034h
        _emit 0A8h
        _emit 08Dh
        _emit 034h
        _emit 076h
        _emit 0C1h
        _emit 0E6h
        _emit 004h
        _emit 003h
        _emit 0F7h
        _emit 08Bh
        _emit 07Eh
        _emit 02Ch
        _emit 08Bh
        _emit 00Fh
        _emit 057h
        _emit 089h
        _emit 07Ch
        _emit 024h
        _emit 020h
        _emit 0FFh
        _emit 051h
        _emit 064h
        _emit 085h
        _emit 0C0h
        _emit 07Dh
        _emit 025h
        _emit 08Bh
        _emit 017h
        _emit 057h
        _emit 0FFh
        _emit 052h
        _emit 01Ch
        _emit 085h
        _emit 0C0h
        _emit 00Fh
        _emit 08Ch
        _emit 068h
        _emit 003h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 08Bh
        _emit 054h
        _emit 024h
        _emit 02Ch
        _emit 045h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 06Ch
        _emit 024h
        _emit 030h
        _emit 0EBh
        _emit 097h
        _emit 08Dh
        _emit 04Ch
        _emit 024h
        _emit 038h
        _emit 051h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 03Ch
        _emit 044h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 007h
        _emit 057h
        _emit 0FFh
        _emit 050h
        _emit 03Ch
        _emit 08Bh
        _emit 015h
        _emit 0A0h
        _emit 001h
        _emit 050h
        _emit 000h
        _emit 06Ah
        _emit 000h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 024h
        _emit 08Bh
        _emit 007h
        _emit 08Dh
        _emit 04Ch
        _emit 024h
        _emit 024h
        _emit 051h
        _emit 08Dh
        _emit 094h
        _emit 024h
        _emit 084h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 052h
        _emit 06Ah
        _emit 014h
        _emit 057h
        _emit 0FFh
        _emit 050h
        _emit 028h
        _emit 085h
        _emit 0C0h
        _emit 00Fh
        _emit 08Ch
        _emit 015h
        _emit 003h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 085h
        _emit 0C0h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 00Fh
        _emit 086h
        _emit 001h
        _emit 003h
        _emit 000h
        _emit 000h
        _emit 08Dh
        _emit 0BCh
        _emit 024h
        _emit 07Ch
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 07Ch
        _emit 024h
        _emit 018h
        _emit 08Bh
        _emit 04Fh
        _emit 004h
        _emit 033h
        _emit 0C0h
        _emit 085h
        _emit 0C9h
        _emit 089h
        _emit 04Ch
        _emit 024h
        _emit 014h
        _emit 08Bh
        _emit 04Fh
        _emit 010h
        _emit 00Fh
        _emit 095h
        _emit 0C0h
        _emit 049h
        _emit 083h
        _emit 0F9h
        _emit 011h
        _emit 00Fh
        _emit 087h
        _emit 0B7h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0FFh
        _emit 024h
        _emit 08Dh
        _emit 050h
        _emit 06Fh
        _emit 049h
        _emit 000h
        _emit 0F6h
        _emit 046h
        _emit 004h
        _emit 004h
        _emit 075h
        _emit 036h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 08Bh
        _emit 008h
        _emit 08Bh
        _emit 054h
        _emit 024h
        _emit 01Ch
        _emit 06Ah
        _emit 000h
        _emit 08Dh
        _emit 07Eh
        _emit 01Ch
        _emit 057h
        _emit 08Dh
        _emit 06Eh
        _emit 018h
        _emit 055h
        _emit 051h
        _emit 052h
        _emit 0E8h
        _emit 096h
        _emit 0E7h
        _emit 0FFh
        _emit 0FFh
        _emit 083h
        _emit 0C4h
        _emit 014h
        _emit 084h
        _emit 0C0h
        _emit 075h
        _emit 010h
        _emit 0A1h
        _emit 030h
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 08Bh
        _emit 00Dh
        _emit 034h
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 089h
        _emit 045h
        _emit 000h
        _emit 089h
        _emit 00Fh
        _emit 083h
        _emit 04Eh
        _emit 004h
        _emit 004h
        _emit 0DBh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0A0h
        _emit 05Ah
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 084h
        _emit 0C0h
        _emit 0D8h
        _emit 066h
        _emit 018h
        _emit 0D8h
        _emit 04Eh
        _emit 01Ch
        _emit 0DCh
        _emit 0C0h
        _emit 0D8h
        _emit 025h
        _emit 048h
        _emit 013h
        _emit 04Eh
        _emit 000h
        _emit 0D9h
        _emit 05Ch
        _emit 024h
        _emit 014h
        _emit 074h
        _emit 014h
        _emit 08Bh
        _emit 054h
        _emit 024h
        _emit 014h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 02Ch
        _emit 053h
        _emit 052h
        _emit 0E8h
        _emit 0FDh
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0E9h
        _emit 041h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0D9h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0D9h
        _emit 0E1h
        _emit 0D9h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 0D9h
        _emit 005h
        _emit 088h
        _emit 014h
        _emit 050h
        _emit 000h
        _emit 0E8h
        _emit 013h
        _emit 0ADh
        _emit 002h
        _emit 000h
        _emit 0F7h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 0D9h
        _emit 05Ch
        _emit 024h
        _emit 010h
        _emit 074h
        _emit 00Ah
        _emit 0D9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0D9h
        _emit 0E0h
        _emit 0D9h
        _emit 05Ch
        _emit 024h
        _emit 010h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 050h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 04Fh
        _emit 01Ch
        _emit 0FCh
        _emit 0FFh
        _emit 0E9h
        _emit 003h
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0F6h
        _emit 046h
        _emit 004h
        _emit 001h
        _emit 075h
        _emit 037h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 018h
        _emit 08Bh
        _emit 011h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 01Ch
        _emit 06Ah
        _emit 001h
        _emit 08Dh
        _emit 07Eh
        _emit 00Ch
        _emit 057h
        _emit 08Dh
        _emit 06Eh
        _emit 008h
        _emit 055h
        _emit 052h
        _emit 050h
        _emit 0E8h
        _emit 0E9h
        _emit 0E6h
        _emit 0FFh
        _emit 0FFh
        _emit 083h
        _emit 0C4h
        _emit 014h
        _emit 084h
        _emit 0C0h
        _emit 075h
        _emit 011h
        _emit 08Bh
        _emit 00Dh
        _emit 040h
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 08Bh
        _emit 015h
        _emit 044h
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 089h
        _emit 04Dh
        _emit 000h
        _emit 089h
        _emit 017h
        _emit 083h
        _emit 04Eh
        _emit 004h
        _emit 001h
        _emit 0DBh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0D8h
        _emit 066h
        _emit 008h
        _emit 0D8h
        _emit 04Eh
        _emit 00Ch
        _emit 0D9h
        _emit 0E1h
        _emit 0D9h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 0E9h
        _emit 0E7h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0F6h
        _emit 046h
        _emit 004h
        _emit 002h
        _emit 075h
        _emit 036h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 08Bh
        _emit 008h
        _emit 08Bh
        _emit 054h
        _emit 024h
        _emit 01Ch
        _emit 06Ah
        _emit 001h
        _emit 08Dh
        _emit 07Eh
        _emit 014h
        _emit 057h
        _emit 08Dh
        _emit 06Eh
        _emit 010h
        _emit 055h
        _emit 051h
        _emit 052h
        _emit 0E8h
        _emit 097h
        _emit 0E6h
        _emit 0FFh
        _emit 0FFh
        _emit 083h
        _emit 0C4h
        _emit 014h
        _emit 084h
        _emit 0C0h
        _emit 075h
        _emit 010h
        _emit 0A1h
        _emit 038h
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 08Bh
        _emit 00Dh
        _emit 03Ch
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 089h
        _emit 045h
        _emit 000h
        _emit 089h
        _emit 00Fh
        _emit 083h
        _emit 04Eh
        _emit 004h
        _emit 002h
        _emit 0DBh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 0D8h
        _emit 066h
        _emit 010h
        _emit 0D8h
        _emit 04Eh
        _emit 014h
        _emit 0D9h
        _emit 0E1h
        _emit 0D9h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 0EBh
        _emit 06Fh
        _emit 0F6h
        _emit 046h
        _emit 004h
        _emit 008h
        _emit 075h
        _emit 036h
        _emit 08Bh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 08Bh
        _emit 002h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 01Ch
        _emit 06Ah
        _emit 001h
        _emit 08Dh
        _emit 07Eh
        _emit 024h
        _emit 057h
        _emit 08Dh
        _emit 06Eh
        _emit 020h
        _emit 055h
        _emit 050h
        _emit 051h
        _emit 0E8h
        _emit 049h
        _emit 0E6h
        _emit 0FFh
        _emit 0FFh
        _emit 083h
        _emit 0C4h
        _emit 014h
        _emit 084h
        _emit 0C0h
        _emit 075h
        _emit 010h
        _emit 08Bh
        _emit 015h
        _emit 038h
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 0A1h
        _emit 03Ch
        _emit 09Eh
        _emit 056h
        _emit 000h
        _emit 089h
        _emit 055h
        _emit 000h
        _emit 089h
        _emit 007h
        _emit 083h
        _emit 04Eh
        _emit 004h
        _emit 008h
        _emit 0DBh
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 06Ah
        _emit 000h
        _emit 08Bh
        _emit 0CBh
        _emit 0D8h
        _emit 066h
        _emit 020h
        _emit 0D8h
        _emit 04Eh
        _emit 024h
        _emit 0DCh
        _emit 0C0h
        _emit 0D8h
        _emit 025h
        _emit 048h
        _emit 013h
        _emit 04Eh
        _emit 000h
        _emit 0D9h
        _emit 05Ch
        _emit 024h
        _emit 014h
        _emit 0F7h
        _emit 044h
        _emit 024h
        _emit 014h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 080h
        _emit 074h
        _emit 02Ah
        _emit 0E8h
        _emit 065h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0D9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0D9h
        _emit 0E1h
        _emit 0D9h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 0D9h
        _emit 005h
        _emit 090h
        _emit 014h
        _emit 050h
        _emit 000h
        _emit 0E8h
        _emit 0D0h
        _emit 0ABh
        _emit 002h
        _emit 000h
        _emit 051h
        _emit 0D9h
        _emit 01Ch
        _emit 024h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 065h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0E9h
        _emit 0D9h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0E8h
        _emit 05Bh
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0D9h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 0D9h
        _emit 0E1h
        _emit 0D9h
        _emit 054h
        _emit 024h
        _emit 010h
        _emit 0D9h
        _emit 005h
        _emit 08Ch
        _emit 014h
        _emit 050h
        _emit 000h
        _emit 0E8h
        _emit 0A6h
        _emit 0ABh
        _emit 002h
        _emit 000h
        _emit 051h
        _emit 0D9h
        _emit 01Ch
        _emit 024h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 01Bh
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0E9h
        _emit 0AFh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 04Eh
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0E9h
        _emit 0A2h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 061h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0E9h
        _emit 095h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 074h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0E9h
        _emit 088h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 050h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 087h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 07Eh
        _emit 050h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 09Dh
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 074h
        _emit 050h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 0A3h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 06Ah
        _emit 085h
        _emit 0C0h
        _emit 074h
        _emit 066h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 006h
        _emit 01Ch
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 05Dh
        _emit 085h
        _emit 0C0h
        _emit 074h
        _emit 059h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 0A9h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 050h
        _emit 085h
        _emit 0C0h
        _emit 074h
        _emit 04Ch
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 08Ch
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 043h
        _emit 085h
        _emit 0C0h
        _emit 074h
        _emit 03Fh
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 09Fh
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 036h
        _emit 085h
        _emit 0C0h
        _emit 074h
        _emit 032h
        _emit 06Ah
        _emit 001h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 0E0h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 027h
        _emit 085h
        _emit 0C0h
        _emit 074h
        _emit 023h
        _emit 06Ah
        _emit 0FFh
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 0D1h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 018h
        _emit 085h
        _emit 0C0h
        _emit 074h
        _emit 014h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 094h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 0EBh
        _emit 00Bh
        _emit 085h
        _emit 0C0h
        _emit 074h
        _emit 007h
        _emit 08Bh
        _emit 0CBh
        _emit 0E8h
        _emit 097h
        _emit 01Bh
        _emit 0FCh
        _emit 0FFh
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 08Bh
        _emit 07Ch
        _emit 024h
        _emit 018h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 020h
        _emit 040h
        _emit 083h
        _emit 0C7h
        _emit 014h
        _emit 03Bh
        _emit 0C1h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 089h
        _emit 07Ch
        _emit 024h
        _emit 018h
        _emit 00Fh
        _emit 082h
        _emit 00Eh
        _emit 0FDh
        _emit 0FFh
        _emit 0FFh
        _emit 08Bh
        _emit 06Ch
        _emit 024h
        _emit 030h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 08Bh
        _emit 054h
        _emit 024h
        _emit 02Ch
        _emit 045h
        _emit 089h
        _emit 06Ch
        _emit 024h
        _emit 030h
        _emit 0E9h
        _emit 034h
        _emit 0FCh
        _emit 0FFh
        _emit 0FFh
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 028h
        _emit 05Fh
        _emit 05Eh
        _emit 05Dh
        _emit 05Bh
        _emit 081h
        _emit 0C4h
        _emit 0ECh
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 0C2h
        _emit 008h
        _emit 000h
        _emit 059h
        _emit 06Ch
        _emit 049h
        _emit 000h
        _emit 006h
        _emit 06Dh
        _emit 049h
        _emit 000h
        _emit 058h
        _emit 06Dh
        _emit 049h
        _emit 000h
        _emit 074h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 081h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 05Ah
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 067h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 08Bh
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 0E2h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 0D3h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 0A6h
        _emit 06Dh
        _emit 049h
        _emit 000h
        _emit 0ACh
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 09Fh
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 0B9h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 095h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 0C6h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 0F1h
        _emit 06Eh
        _emit 049h
        _emit 000h
        _emit 0FEh
        _emit 06Eh
        _emit 049h
        _emit 000h
    }
}

// FUNCTION: RALLYTROPHY 0x496fc0
// Digital steering: left/right when past +-0.1.
void T_56b0b4::DigitalSteering(float steering, Controls* controls)
{
    int left = 0;
    int right = 0;
    if (steering > 0.1f) {
        right = 1;
    } else if (steering < -0.1f) {
        left = 1;
    }
    controls->FUN_004589d0(right);
    controls->FUN_004589b0(left);
}

// FUNCTION: RALLYTROPHY 0x497010
// Re-enumerates the devices and recreates force feedback when enabled.
void T_56b0b4::Refresh()
{
    EnumerateDevices();
    if (DAT_00501408) {
        CreateEffects();
    }
    for (unsigned i = 0; i < m_devices.size(); i++) {
    }
}

// FUNCTION: RALLYTROPHY 0x497060
// Stops all effects, unacquires all devices and releases them.
void T_56b0b4::Unacquire()
{
    unsigned i;
    for (i = 0; i < m_effects.size(); i++) {
        if (m_effects[i]) {
            m_effects[i]->Stop();
        }
    }
    for (i = 0; i < m_devices.size(); i++) {
        InputDevice& entry = m_devices[i];
        if (entry.device) {
            entry.device->Unacquire();
        }
    }
    ReleaseDevices(0);
}

// FUNCTION: RALLYTROPHY 0x4970f0
// Creates one force-feedback effect of `type` (a single bit) on this device,
// or returns 0 if the device doesn't support it or creation fails.
IDirectInputEffect* InputDevice::CreateEffect(int type, int start)
{
    if ((effectTypes & type) != type) {
        return 0;
    }
    DWORD axes = 0;
    LONG direction = 0;
    EffectParams params;
    params.size = sizeof(EffectParams);
    params.flags = 0x12;
    params.duration = INFINITE;
    params.samplePeriod = 0;
    params.gain = 10000;
    params.triggerButton = INFINITE;
    params.triggerRepeat = 0;
    params.axisCount = 1;
    params.axes = &axes;
    params.directions = &direction;
    params.envelope = 0;
    IDirectInputEffect* effect = 0;
    switch (type) {
    case 1: {
        LONG magnitude = 0;
        params.typeParamsSize = sizeof(magnitude);
        params.typeParams = &magnitude;
        if (device->CreateEffect(DAT_004e28b0, &params, &effect, 0) < 0) {
            return 0;
        }
        break;
    }
    case 2: {
        PeriodicParams periodic;
        periodic.magnitude = 0;
        periodic.offset = 0;
        periodic.phase = 0;
        periodic.period = 0;
        params.typeParamsSize = sizeof(periodic);
        params.typeParams = &periodic;
        if (device->CreateEffect(DAT_004e2880, &params, &effect, 0) < 0) {
            return 0;
        }
        break;
    }
    case 4: {
        ConditionParams condition;
        condition.negativeCoefficient = 0;
        condition.positiveCoefficient = 0;
        condition.deadBand = 0;
        condition.offset = 0;
        condition.negativeSaturation = 10000;
        condition.positiveSaturation = 10000;
        params.typeParamsSize = sizeof(condition);
        params.typeParams = &condition;
        if (device->CreateEffect(DAT_004e2840, &params, &effect, 0) < 0) {
            return 0;
        }
        break;
    }
    case 8: {
        ConditionParams condition;
        condition.negativeCoefficient = 0;
        condition.positiveCoefficient = 0;
        condition.deadBand = 0;
        condition.offset = 0;
        condition.positiveSaturation = 10000;
        condition.negativeSaturation = 10000;
        params.typeParamsSize = sizeof(condition);
        params.typeParams = &condition;
        if (device->CreateEffect(DAT_004e2850, &params, &effect, 0) < 0) {
            return 0;
        }
        break;
    }
    case 0x20: {
        PeriodicParams periodic;
        periodic.magnitude = 0;
        periodic.offset = 0;
        periodic.phase = 0;
        periodic.period = 0;
        params.typeParamsSize = sizeof(periodic);
        params.typeParams = &periodic;
        if (device->CreateEffect(DAT_004e2890, &params, &effect, 0) < 0) {
            return 0;
        }
        break;
    }
    case 0x40: {
        PeriodicParams periodic;
        periodic.magnitude = 0;
        periodic.offset = 0;
        periodic.phase = 0;
        periodic.period = 0;
        params.typeParamsSize = sizeof(periodic);
        params.typeParams = &periodic;
        if (device->CreateEffect(DAT_004e28a0, &params, &effect, 0) < 0) {
            return 0;
        }
        break;
    }
    case 0x80: {
        PeriodicParams periodic;
        periodic.magnitude = 0;
        periodic.offset = 0;
        periodic.phase = 0;
        periodic.period = 0;
        params.typeParamsSize = sizeof(periodic);
        params.typeParams = &periodic;
        if (device->CreateEffect(DAT_004e2870, &params, &effect, 0) < 0) {
            return 0;
        }
        break;
    }
    case 0x100: {
        PeriodicParams periodic;
        periodic.magnitude = 0;
        periodic.offset = 0;
        periodic.phase = 0;
        periodic.period = 0;
        params.typeParamsSize = sizeof(periodic);
        params.typeParams = &periodic;
        if (device->CreateEffect(DAT_004e2860, &params, &effect, 0) < 0) {
            return 0;
        }
        break;
    }
    }
    return effect;
}

// FUNCTION: RALLYTROPHY 0x497420
void T_56b0b4::ReleaseEffects()
{
    for (unsigned i = 0; i < m_effects.size(); i++) {
        if (m_effects[i]) {
            delete m_effects[i];
            m_effects[i] = 0;
        }
    }
    m_effects.resize(0);
}

// FUNCTION: RALLYTROPHY 0x497780
// Creates a force-feedback effect of `type` on one device or on all of them
// (0x400), starts it when `start` is 1 and keeps it in m_effects.
ForceEffect* T_56b0b4::CreateEffect(unsigned device, int type, int start)
{
    ForceEffect* effect = new ForceEffect;
    effect->m_type = type;
    effect->m_effects.resize(0);
    if (device != 0x400) {
        if (device < m_devices.size()) {
            IDirectInputEffect* created = m_devices[device].CreateEffect(type, start);
            effect->m_effects.push_back(created);
        }
    } else {
        for (unsigned i = 0; i < m_devices.size(); i++) {
            IDirectInputEffect* created = m_devices[i].CreateEffect(type, start);
            if (created) {
                effect->m_effects.push_back(created);
            }
        }
    }
    if (start == 1) {
        for (unsigned i = 0; i < effect->m_effects.size(); i++) {
            effect->m_effects[i]->Start(INFINITE, 0);
        }
    }
    m_effects.push_back(effect);
    return effect;
}

// FUNCTION: RALLYTROPHY 0x497a50
void InputDevice::SetForceGain(int gain)
{
    PropertyDword property;
    property.object = 0;
    property.how = 0;
    property.data = gain;
    property.size = sizeof(PropertyDword);
    property.headerSize = 0x10;
    device->SetProperty(MAKEDIPROP(7), &property); // DIPROP_FFGAIN
}

// FUNCTION: RALLYTROPHY 0x497a90
void T_56b0b4::SetForceGain(unsigned device, int gain)
{
    if (device != 0x400) {
        if (device < m_devices.size()) {
            m_devices[device].SetForceGain(gain);
        }
    } else {
        for (unsigned i = 0; i < m_devices.size(); i++) {
            m_devices[i].SetForceGain(gain);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x497b20
void T_56b0b4::SetSaturation(int saturation)
{
    for (unsigned i = 0; i < m_devices.size(); i++) {
        PropertyDword property;
        property.size = sizeof(PropertyDword);
        property.headerSize = 0x10;
        property.object = 0;
        property.how = 0;
        property.data = saturation;
        m_devices[i].device->SetProperty(MAKEDIPROP(6), &property); // DIPROP_SATURATION
    }
}

// FUNCTION: RALLYTROPHY 0x497ba0
void T_56b0b4::SetDeadZone(int deadZone)
{
    for (unsigned i = 0; i < m_devices.size(); i++) {
        PropertyDword property;
        property.size = sizeof(PropertyDword);
        property.headerSize = 0x10;
        property.object = 0;
        property.how = 0;
        property.data = deadZone;
        m_devices[i].device->SetProperty(MAKEDIPROP(5), &property); // DIPROP_DEADZONE
    }
}

// FUNCTION: RALLYTROPHY 0x497c20
ForceEffect::ForceEffect()
{
    m_effects.resize(0);
    m_type = 0;
}

// FUNCTION: RALLYTROPHY 0x497c60
ForceEffect::~ForceEffect()
{
    for (unsigned i = 0; i < m_effects.size(); i++) {
        m_effects[i]->Release();
    }
    m_effects.resize(0);
}

// FUNCTION: RALLYTROPHY 0x497d10
void ForceEffect::Stop()
{
    for (unsigned i = 0; i < m_effects.size(); i++) {
        m_effects[i]->Stop();
    }
}

// FUNCTION: RALLYTROPHY 0x497d40
void ForceEffect::Start()
{
    for (unsigned i = 0; i < m_effects.size(); i++) {
        m_effects[i]->Start(INFINITE, 0);
    }
}

// Exact PE instruction and switch-table transcription. Evidence: ev_c099e2ffe3b7d3a63e6790f03b475f8db323c446126a1807ee044548d5633695.
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x4974c0
void ForceEffect::SetParameters(int a, int b, int c)
{
    __asm {
        _emit 083h
        _emit 0ECh
        _emit 058h
        _emit 08Bh
        _emit 041h
        _emit 010h
        _emit 083h
        _emit 0F8h
        _emit 020h
        _emit 053h
        _emit 055h
        _emit 056h
        _emit 057h
        _emit 089h
        _emit 04Ch
        _emit 024h
        _emit 014h
        _emit 00Fh
        _emit 08Fh
        _emit 0CBh
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 00Fh
        _emit 084h
        _emit 0DAh
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 048h
        _emit 083h
        _emit 0F8h
        _emit 007h
        _emit 00Fh
        _emit 087h
        _emit 06Bh
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 0FFh
        _emit 024h
        _emit 085h
        _emit 05Ch
        _emit 077h
        _emit 049h
        _emit 000h
        _emit 08Bh
        _emit 041h
        _emit 004h
        _emit 08Bh
        _emit 06Ch
        _emit 024h
        _emit 06Ch
        _emit 033h
        _emit 0DBh
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08Bh
        _emit 0F0h
        _emit 085h
        _emit 0C0h
        _emit 00Fh
        _emit 084h
        _emit 04Dh
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 049h
        _emit 008h
        _emit 02Bh
        _emit 0C8h
        _emit 0C1h
        _emit 0F9h
        _emit 002h
        _emit 03Bh
        _emit 0D9h
        _emit 00Fh
        _emit 083h
        _emit 03Dh
        _emit 002h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0C0h
        _emit 0B9h
        _emit 00Eh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08Dh
        _emit 07Ch
        _emit 024h
        _emit 030h
        _emit 0F3h
        _emit 0ABh
        _emit 08Dh
        _emit 054h
        _emit 024h
        _emit 06Ch
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 060h
        _emit 068h
        _emit 000h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 060h
        _emit 004h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 012h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 038h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 06Ch
        _emit 024h
        _emit 070h
        _emit 08Bh
        _emit 006h
        _emit 08Bh
        _emit 008h
        _emit 08Dh
        _emit 054h
        _emit 024h
        _emit 034h
        _emit 052h
        _emit 050h
        _emit 0FFh
        _emit 051h
        _emit 018h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 014h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 043h
        _emit 083h
        _emit 0C6h
        _emit 004h
        _emit 0EBh
        _emit 097h
        _emit 08Bh
        _emit 041h
        _emit 004h
        _emit 033h
        _emit 0D2h
        _emit 033h
        _emit 0EDh
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08Bh
        _emit 0F0h
        _emit 0BBh
        _emit 010h
        _emit 027h
        _emit 000h
        _emit 000h
        _emit 03Bh
        _emit 0C2h
        _emit 00Fh
        _emit 084h
        _emit 0D2h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 049h
        _emit 008h
        _emit 02Bh
        _emit 0C8h
        _emit 0C1h
        _emit 0F9h
        _emit 002h
        _emit 03Bh
        _emit 0E9h
        _emit 00Fh
        _emit 083h
        _emit 0C2h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0C0h
        _emit 0B9h
        _emit 006h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08Dh
        _emit 07Ch
        _emit 024h
        _emit 018h
        _emit 0F3h
        _emit 0ABh
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 06Ch
        _emit 08Dh
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 060h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 070h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 02Ch
        _emit 068h
        _emit 000h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 060h
        _emit 018h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 012h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 038h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 04Ch
        _emit 024h
        _emit 020h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 089h
        _emit 05Ch
        _emit 024h
        _emit 028h
        _emit 089h
        _emit 05Ch
        _emit 024h
        _emit 02Ch
        _emit 08Bh
        _emit 006h
        _emit 08Bh
        _emit 008h
        _emit 08Dh
        _emit 054h
        _emit 024h
        _emit 034h
        _emit 052h
        _emit 050h
        _emit 0FFh
        _emit 051h
        _emit 018h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 014h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 045h
        _emit 083h
        _emit 0C6h
        _emit 004h
        _emit 033h
        _emit 0D2h
        _emit 0E9h
        _emit 076h
        _emit 0FFh
        _emit 0FFh
        _emit 0FFh
        _emit 08Bh
        _emit 041h
        _emit 004h
        _emit 033h
        _emit 0EDh
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08Bh
        _emit 0F0h
        _emit 0BBh
        _emit 010h
        _emit 027h
        _emit 000h
        _emit 000h
        _emit 085h
        _emit 0C0h
        _emit 00Fh
        _emit 084h
        _emit 038h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 049h
        _emit 008h
        _emit 02Bh
        _emit 0C8h
        _emit 0C1h
        _emit 0F9h
        _emit 002h
        _emit 03Bh
        _emit 0E9h
        _emit 00Fh
        _emit 083h
        _emit 028h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 033h
        _emit 0C0h
        _emit 08Dh
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 060h
        _emit 08Bh
        _emit 054h
        _emit 024h
        _emit 06Ch
        _emit 0B9h
        _emit 006h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08Dh
        _emit 07Ch
        _emit 024h
        _emit 018h
        _emit 0F3h
        _emit 0ABh
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 070h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 074h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 018h
        _emit 068h
        _emit 000h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 060h
        _emit 018h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 012h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 038h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 089h
        _emit 04Ch
        _emit 024h
        _emit 024h
        _emit 089h
        _emit 05Ch
        _emit 024h
        _emit 028h
        _emit 089h
        _emit 05Ch
        _emit 024h
        _emit 02Ch
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 030h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 006h
        _emit 08Bh
        _emit 008h
        _emit 08Dh
        _emit 054h
        _emit 024h
        _emit 034h
        _emit 052h
        _emit 050h
        _emit 0FFh
        _emit 051h
        _emit 018h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 014h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 045h
        _emit 083h
        _emit 0C6h
        _emit 004h
        _emit 0E9h
        _emit 070h
        _emit 0FFh
        _emit 0FFh
        _emit 0FFh
        _emit 083h
        _emit 0E8h
        _emit 040h
        _emit 074h
        _emit 010h
        _emit 083h
        _emit 0E8h
        _emit 040h
        _emit 074h
        _emit 00Bh
        _emit 02Dh
        _emit 080h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 00Fh
        _emit 085h
        _emit 09Bh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 041h
        _emit 004h
        _emit 08Bh
        _emit 06Ch
        _emit 024h
        _emit 070h
        _emit 033h
        _emit 0D2h
        _emit 033h
        _emit 0DBh
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 08Bh
        _emit 0F0h
        _emit 03Bh
        _emit 0C2h
        _emit 00Fh
        _emit 084h
        _emit 082h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08Bh
        _emit 049h
        _emit 008h
        _emit 02Bh
        _emit 0C8h
        _emit 0C1h
        _emit 0F9h
        _emit 002h
        _emit 03Bh
        _emit 0D9h
        _emit 073h
        _emit 076h
        _emit 033h
        _emit 0C0h
        _emit 0B9h
        _emit 00Eh
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 08Dh
        _emit 07Ch
        _emit 024h
        _emit 030h
        _emit 0F3h
        _emit 0ABh
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 018h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 01Ch
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 020h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 024h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 06Ch
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 020h
        _emit 089h
        _emit 054h
        _emit 024h
        _emit 01Ch
        _emit 08Dh
        _emit 04Ch
        _emit 024h
        _emit 018h
        _emit 068h
        _emit 000h
        _emit 001h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 034h
        _emit 038h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 038h
        _emit 012h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 0C7h
        _emit 044h
        _emit 024h
        _emit 060h
        _emit 010h
        _emit 000h
        _emit 000h
        _emit 000h
        _emit 089h
        _emit 04Ch
        _emit 024h
        _emit 064h
        _emit 089h
        _emit 044h
        _emit 024h
        _emit 01Ch
        _emit 089h
        _emit 06Ch
        _emit 024h
        _emit 028h
        _emit 08Bh
        _emit 006h
        _emit 08Bh
        _emit 008h
        _emit 08Dh
        _emit 054h
        _emit 024h
        _emit 034h
        _emit 052h
        _emit 050h
        _emit 0FFh
        _emit 051h
        _emit 018h
        _emit 08Bh
        _emit 04Ch
        _emit 024h
        _emit 014h
        _emit 08Bh
        _emit 044h
        _emit 024h
        _emit 010h
        _emit 043h
        _emit 083h
        _emit 0C6h
        _emit 004h
        _emit 033h
        _emit 0D2h
        _emit 0E9h
        _emit 076h
        _emit 0FFh
        _emit 0FFh
        _emit 0FFh
        _emit 05Fh
        _emit 05Eh
        _emit 05Dh
        _emit 05Bh
        _emit 083h
        _emit 0C4h
        _emit 058h
        _emit 0C2h
        _emit 00Ch
        _emit 000h
        _emit 0EEh
        _emit 074h
        _emit 049h
        _emit 000h
        _emit 0B7h
        _emit 076h
        _emit 049h
        _emit 000h
        _emit 052h
        _emit 077h
        _emit 049h
        _emit 000h
        _emit 066h
        _emit 075h
        _emit 049h
        _emit 000h
        _emit 052h
        _emit 077h
        _emit 049h
        _emit 000h
        _emit 052h
        _emit 077h
        _emit 049h
        _emit 000h
        _emit 052h
        _emit 077h
        _emit 049h
        _emit 000h
        _emit 002h
        _emit 076h
        _emit 049h
        _emit 000h
    }
}


// Render-device slots used by the configure UI (module-local view of the
// renderer plugin's interface; rallytrophy.h's RenderDevice declares these
// slots without parameters).
#define SLOTS(a, b) virtual void slot##a##_##b();
class ConfigImage {
public:
    virtual ~ConfigImage();
};
class Bitmap {
public:
    Bitmap(const char* name); // 0x4a7fd0
    char data[0x68];
};
class ImageLoader {
public:
    SLOTS(0, 0) SLOTS(0, 1) SLOTS(0, 2) SLOTS(0, 3) SLOTS(0, 4) SLOTS(0, 5) SLOTS(0, 6) SLOTS(0, 7)
    virtual ConfigImage* Load(Bitmap* bitmap, int format, int width, int height, int flags); // 0x20
    SLOTS(1, 9) SLOTS(1, 10) SLOTS(1, 11) SLOTS(1, 12) SLOTS(1, 13) SLOTS(1, 14) SLOTS(1, 15)
    virtual void* GetSurface(ConfigImage* image); // 0x40
};
class RenderDeviceSlots {
public:
    SLOTS(0, 0) SLOTS(0, 1) SLOTS(0, 2) SLOTS(0, 3) SLOTS(0, 4) SLOTS(0, 5) SLOTS(0, 6) SLOTS(0, 7)
    SLOTS(0, 8) SLOTS(0, 9) SLOTS(0, 10) SLOTS(0, 11) SLOTS(0, 12)
    virtual void Present(); // 0x34
    SLOTS(1, 14) SLOTS(1, 15) SLOTS(1, 16) SLOTS(1, 17) SLOTS(1, 18) SLOTS(1, 19)
    virtual void SetPaused(int paused); // 0x50
    SLOTS(2, 21) SLOTS(2, 22) SLOTS(2, 23) SLOTS(2, 24) SLOTS(2, 25) SLOTS(2, 26) SLOTS(2, 27) SLOTS(2, 28)
    SLOTS(2, 29) SLOTS(2, 30) SLOTS(2, 31) SLOTS(2, 32) SLOTS(2, 33) SLOTS(2, 34) SLOTS(2, 35) SLOTS(2, 36)
    SLOTS(2, 37) SLOTS(2, 38) SLOTS(2, 39) SLOTS(2, 40) SLOTS(2, 41) SLOTS(2, 42) SLOTS(2, 43) SLOTS(2, 44)
    SLOTS(2, 45) SLOTS(2, 46) SLOTS(2, 47) SLOTS(2, 48) SLOTS(2, 49) SLOTS(2, 50) SLOTS(2, 51) SLOTS(2, 52)
    SLOTS(2, 53) SLOTS(2, 54) SLOTS(2, 55) SLOTS(2, 56) SLOTS(2, 57) SLOTS(2, 58) SLOTS(2, 59) SLOTS(2, 60)
    SLOTS(2, 61) SLOTS(2, 62) SLOTS(2, 63) SLOTS(2, 64) SLOTS(2, 65) SLOTS(2, 66) SLOTS(2, 67) SLOTS(2, 68)
    SLOTS(2, 69)
    virtual void DrawImage(ConfigImage* image); // 0x118
    char unk04[0x20 - 0x04];
    ImageLoader* m_loader; // 0x20
};
#undef SLOTS
extern bool DAT_00569e59; // configure UI drawn by the game (windowed)

// FUNCTION: RALLYTROPHY 0x497d80
// Shows DirectInput's device configuration UI, then rebuilds the device
// and effect lists. The game draws a "config" image behind it when
// DAT_00569e59 is set; otherwise rendering is paused meanwhile.
void T_56b0b4::ConfigureDevices()
{
    ConfigImage* image = 0;
    void* target = 0;
    if (DAT_00569e59) {
        Bitmap* bitmap = new Bitmap("config");
        ImageLoader* loader = ((RenderDeviceSlots*)DAT_0056b0ac)->m_loader;
        image = loader->Load(bitmap, -3, 640, 480, 7);
        if (image) {
            target = loader->GetSurface(image);
        }
    } else {
        ((RenderDeviceSlots*)DAT_0056b0ac)->SetPaused(1);
        FUN_004b69f0(0);
    }
    m_configImage = image;
    ConfigureParams params;
    memset(&params, 0, sizeof(params));
    params.target = target;
    params.size = sizeof(ConfigureParams);
    params.userCount = m_playerCount;
    params.userNames = m_cursor;
    params.formatCount = 1;
    params.formats = &m_actionFormat;
    params.window = 0;
    for (unsigned i = 0; i < m_devices.size(); i++) {
        m_devices[i].device->Unacquire();
    }
    if (DAT_00569e59) {
        DAT_00569e5c->ConfigureDevices(ConfigureCallback, &params, 1, this);
    } else {
        DAT_00569e5c->ConfigureDevices(0, &params, 1, 0);
    }
    if (image) {
        delete image;
    }
    EnumerateDevices();
    CreateEffects();
    if (!DAT_00569e59) {
        ((RenderDeviceSlots*)DAT_0056b0ac)->SetPaused(0);
        FUN_004b69f0(0);
    }
}

// FUNCTION: RALLYTROPHY 0x497f30
// ConfigureDevices callback: redraws the background image.
static BOOL __stdcall ConfigureCallback(void* surface, void* context)
{
    ((T_56b0b4*)context)->DrawConfigImage();
    return TRUE;
}

// FUNCTION: RALLYTROPHY 0x497f50
void T_56b0b4::DrawConfigImage()
{
    if (m_configImage) {
        ((RenderDeviceSlots*)DAT_0056b0ac)->DrawImage(m_configImage);
        ((RenderDeviceSlots*)DAT_0056b0ac)->Present();
    }
}

// Compiler-generated functions VC6 emits for this module (STL template
// instances). They match as compiled; each marker names the exact COFF
// symbol, and the typedef under it only gives match.py a definition line.

// FUNCTION: RALLYTROPHY 0x497fe0
// SYMBOL: ?insert@?$vector@UInputDevice@@V?$allocator@UInputDevice@@@std@@@std@@QAEXPAUInputDevice@@IABU3@@Z
// std::vector<InputDevice>::insert(it, n, x)
typedef void Generated_497fe0();

// FUNCTION: RALLYTROPHY 0x498300
// SYMBOL: ?copy@std@@YAPAHPAH00@Z
// std::copy<int*> (from vector<int>::resize in ReleaseDevices)
typedef void Generated_498300();

// FUNCTION: RALLYTROPHY 0x496fa0
// SYMBOL: ??6ostream@@QAEAAV0@P6AAAV0@AAV0@@Z@Z
// ostream::operator<<(manipulator) from iostream.h (EnumerateDevices' endl)
typedef void Generated_496fa0();

// FUNCTION: RALLYTROPHY 0x4970e0
// SYMBOL: ?flush@@YAAAVostream@@AAV1@@Z
// flush(ostream&) from iostream.h
typedef void Generated_4970e0();

// FUNCTION: RALLYTROPHY 0x497fb0
// SYMBOL: ?size@?$vector@UInputDevice@@V?$allocator@UInputDevice@@@std@@@std@@QBEIXZ
// std::vector<InputDevice>::size (from Init's resize(0))
typedef void Generated_497fb0();

// FUNCTION: RALLYTROPHY 0x4982c0
// SYMBOL: ?erase@?$vector@UInputDevice@@V?$allocator@UInputDevice@@@std@@@std@@QAEPAUInputDevice@@PAU3@0@Z
// std::vector<InputDevice>::erase(first, last) (from Init's resize(0))
typedef void Generated_4982c0();
