// Networking T_56b0b8 (0x4843e0-0x4856ef): DirectPlay 8.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "dplay8.h"

extern const char* DAT_004fe7a0[8]; // command names (first: "connect")
extern void FUN_004bd000();

// FUNCTION: RALLYTROPHY 0x4843e0
// original: FUN_004843e0
// The inline store pins VC6's scheduling boundary before the first string copy.
// Assembly: ev_e7edf0c1386c788396302f812d79cb68be449215a9f3002adddd2dfbabb39df6
// Pseudocode: ev_1afaf7da59c895b3f9bed067f57010ad1a993c52ce4efac8acc63aa16cd3d8f5
T_56b0b8::T_56b0b8()
{
    m_peer = 0;
    m_lobby = 0;
    m_lobbyConnection = 0;
    m_asyncOperation = 0;
    m_race = 0;
    m_host = 0;
    m_lobbied = 0;
    unk18 = 0;
    // Keep VC6 from scheduling the next string-copy arguments ahead of this store.
    __asm mov dword ptr [esi+28h], edi
    m_connected = 0;
    m_providerFound = 0;
    m_port = 0;
    m_hostName[0] = 0;
    wcscpy(m_playerName, L"Default player");
    wcscpy(m_sessionName, L"Default Session");
    m_ghostMode = DAT_00501314;
    m_rally = 0;
    m_damage = DAT_00501318;
    m_cars = DAT_00501320;
    m_option4 = DAT_00501324;
    unk10 = 0;
    m_asyncOperation = 0;
    m_result = 0x80158480;
    DAT_00565368 = this;
    Initialize();
    ParseCommandLine();
}

// FUNCTION: RALLYTROPHY 0x4844a0
T_56b0b8::~T_56b0b8()
{
    Release();
    DAT_00565368 = 0;
}

// FUNCTION: RALLYTROPHY 0x4844b0
// Creates the peer and lobby objects and selects the TCP/IP provider.
HRESULT T_56b0b8::Initialize()
{
    m_lobbyConnection = 0;
    HRESULT hr = CoCreateInstance(DAT_004e2810, 0, 1, DAT_004e2800, (void**)&m_peer);
    if (hr < 0) {
        return hr;
    }
    hr = CoCreateInstance(DAT_004e27e0, 0, 1, DAT_004e27d0, (void**)&m_lobby);
    if (hr < 0) {
        return hr;
    }
    hr = m_peer->Initialize(this, PeerCallback, 1);
    if (hr < 0) {
        return hr;
    }
    IDirectPlay8Peer* peer = m_peer;
    PeerCaps caps;
    caps.size = sizeof(caps);
    peer->GetCaps(&caps, 0);
    caps.connectTimeout = 3000;
    caps.connectRetries = 20;
    peer->SetCaps(&caps, 0);
    DWORD connection;
    hr = m_lobby->Initialize(this, LobbyCallback, &connection, 1);
    if (hr < 0) {
        return hr;
    }
    peer = m_peer;
    DWORD count = 0;
    DWORD size = 0;
    hr = peer->EnumServiceProviders(0, 0, 0, &size, &count, 0);
    if (hr != 0x80158100) {
        return hr;
    }
    ServiceProviderInfo* providers = (ServiceProviderInfo*)operator new(size);
    hr = peer->EnumServiceProviders(0, 0, providers, &size, &count, 0);
    if (hr < 0) {
        return hr;
    }
    ServiceProviderInfo* info = providers;
    for (DWORD i = 0; i < count; info++, i++) {
        if (memcmp(&info->guid, &DAT_004e27f0, sizeof(GUID)) == 0) {
            DAT_00564758 = info->guid;
            m_providerFound = 1;
            break;
        }
    }
    operator delete(providers);
    ProviderCaps providerCaps;
    providerCaps.size = sizeof(providerCaps);
    peer->GetSPCaps(&DAT_00564758, &providerCaps, 0);
    providerCaps.systemBufferSize = 0;
    peer->SetSPCaps(&DAT_00564758, &providerCaps, 0);
    return 0;
}

// FUNCTION: RALLYTROPHY 0x484680
// Reads the network options from the command line. "+connect" or "+host"
// marks the game as launched for a network session.
void T_56b0b8::ParseCommandLine()
{
    int connect = 0;
    int host = 0;
    for (int i = 0; i < DAT_0056b0c4->GetCount(); i++) {
        int command = FindCommand(DAT_0056b0c4->GetArg(i));
        int hasArg = 0;
        if (i + 1 < DAT_0056b0c4->GetCount()) {
            if (FindCommand(DAT_0056b0c4->GetArg(i + 1)) == 8) {
                hasArg = 1;
            }
        }
        switch (command) {
        case 0:
            if (hasArg) {
                const char* address = DAT_0056b0c4->GetArg(i + 1);
                int length = strlen(address);
                memcpy(DAT_00564f68, address, length + 1);
                char* port = 0;
                for (int j = 0; j < length; j++) {
                    if (DAT_00564f68[j] == ':') {
                        DAT_00564f68[j] = 0;
                        if (j + 1 < length) {
                            port = &DAT_00564f68[j + 1];
                        }
                        break;
                    }
                }
                FUN_004b7710(m_hostName, DAT_00564f68, -1);
                if (port) {
                    m_port = atoi(port);
                }
                if (!m_port) {
                    m_port = 0x7274;
                }
                i++;
                connect = 1;
            }
            break;
        case 1:
            if (hasArg) {
                FUN_004b7710(m_playerName, DAT_0056b0c4->GetArg(i + 1), -1);
                i++;
            }
            break;
        case 2:
            m_host = 1;
            host = 1;
            if (hasArg) {
                m_port = atoi(DAT_0056b0c4->GetArg(i + 1));
                i++;
            }
            if (!m_port) {
                m_port = 0x7274;
            }
            break;
        case 3:
            if (hasArg) {
                FUN_004b7710(m_sessionName, DAT_0056b0c4->GetArg(i + 1), -1);
                i++;
            }
            break;
        case 4:
            if (hasArg) {
                const char* name = DAT_0056b0c4->GetArg(i + 1);
                for (int rally = 0; rally < 54; rally++) {
                    if (_stricmp(DAT_004fe7c0[rally].name, name) == 0) {
                        m_rally = rally;
                        break;
                    }
                }
                i++;
            }
            break;
        case 5:
            if (hasArg) {
                if (_stricmp(DAT_0056b0c4->GetArg(i + 1), "1") == 0) {
                    DAT_00501314 = m_ghostMode = 1;
                } else {
                    DAT_00501314 = m_ghostMode = 0;
                }
                i++;
            }
            break;
        case 6:
            if (hasArg) {
                if (_stricmp(DAT_0056b0c4->GetArg(i + 1), "0") == 0) {
                    DAT_00501318 = m_damage = 0;
                } else {
                    DAT_00501318 = m_damage = 1;
                }
                i++;
            }
            break;
        case 7:
            if (hasArg) {
                const char* cars = DAT_0056b0c4->GetArg(i + 1);
                if (_stricmp(cars, "novice") == 0) {
                    DAT_00501320 = m_cars = 0;
                } else if (_stricmp(cars, "intermediate") == 0) {
                    DAT_00501320 = m_cars = 1;
                } else if (_stricmp(cars, "expert") == 0) {
                    DAT_00501320 = m_cars = 2;
                } else if (_stricmp(cars, "all") == 0) {
                    DAT_00501320 = m_cars = 3;
                }
                i++;
            }
            break;
        }
    }
    if (host || connect) {
        m_lobbied = 1;
    }
}

// FUNCTION: RALLYTROPHY 0x484a90
// Index of a command name in the table (case-insensitive), 8 if unknown.
int T_56b0b8::FindCommand(const char* name)
{
    int i;
    for (i = 0; i < 8; i++) {
        if (_stricmp(DAT_004fe7a0[i], name) == 0) {
            break;
        }
    }
    return i;
}

// FUNCTION: RALLYTROPHY 0x484ac0
// Leaves the current session (ending it when hosting) and releases DirectPlay.
void T_56b0b8::Close()
{
    IDirectPlay8Peer* peer = m_peer;
    if (peer) {
        int host = m_host;
        if (host && m_connected) {
            peer->TerminateSession(0, 0, 0);
        }
        if (m_asyncOperation) {
            peer->CancelAsyncOperation(m_asyncOperation, 0x8000);
        }
        m_connected = 0;
        if (host) {
            FUN_0047f1a0();
        }
    }
    Release();
}

// FUNCTION: RALLYTROPHY 0x484b20
HRESULT T_56b0b8::Release()
{
    DAT_00565368 = 0;
    if (m_peer) {
        m_peer->Close(0);
        m_peer->Release();
        m_peer = 0;
    }
    if (m_lobby) {
        m_lobby->Close(0);
        m_lobby->Release();
        m_lobby = 0;
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x484b70
HRESULT __stdcall T_56b0b8::PeerCallback(void* context, DWORD type, void* message)
{
    if (DAT_00565368) {
        DAT_00565368->OnPeerMessage(type, message);
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x484b90
HRESULT __stdcall T_56b0b8::LobbyCallback(void* context, DWORD type, void* message)
{
    if (DAT_00565368) {
        return DAT_00565368->OnLobbyMessage(type, message);
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x484bc0
// The volatile joined store keeps the target's TERMINATE_SESSION store order,
// including the saved-EDI pop between the joined and result writes.
HRESULT T_56b0b8::OnPeerMessage(DWORD type, void* message)
{
    switch (type) {
    case 0xffff000e: { // DPN_MSGID_INDICATE_CONNECT
        DWORD* msg = (DWORD*)message;
        if (!m_connected || msg[2] < 4 || *(DWORD*)msg[1] < 0x10002) {
            return 0x80158260;
        }
        break;
    }
    case 0xffff0007: { // DPN_MSGID_CREATE_PLAYER
        DWORD* msg = (DWORD*)message;
        IDirectPlay8Peer* peer = m_peer;
        DWORD size = 0;
        HRESULT hr = peer->GetPeerInfo(msg[1], 0, &size, 0);
        if (hr < 0 && hr != 0x80158100) {
            return hr;
        }
        DWORD* info = (DWORD*)operator new(size);
        memset(info, 0, size);
        info[0] = 0x18;
        DWORD id = msg[1];
        hr = peer->GetPeerInfo(id, info, &size, 0);
        if (hr < 0) {
            operator delete(info);
            return hr;
        }
        int host = 0;
        int local = 0;
        if (info[5] & 2) {
            local = 1;
            wcscpy(m_playerName, (WCHAR*)info[2]);
        }
        if (info[5] & 4) {
            host = 1;
        }
        msg[2] = (DWORD)m_race->AddPlayer((WCHAR*)info[2], id, host, local);
        operator delete(info);
        InterlockedExchange(&unk10, 1);
        return 0;
    }
    case 0xffff0009: // DPN_MSGID_DESTROY_PLAYER
        m_race->RemovePlayer(((Player**)message)[2]);
        return 0;
    case 0xffff0011: { // DPN_MSGID_RECEIVE
        DWORD* msg = (DWORD*)message;
        DWORD size = msg[4];
        if (size > 1) {
            SettingsMessage* data = (SettingsMessage*)msg[3];
            if (data->type == 2) {
                int rally = data->settings.rally;
                DAT_00501314 = m_ghostMode = data->settings.ghostMode;
                DAT_00501318 = m_damage = data->settings.damage;
                DAT_00501320 = m_cars = data->settings.cars;
                DAT_00501324 = m_option4 = data->settings.option4;
                m_rally = rally;
                m_rallyInfo[0] = DAT_004fe7c0[rally].info[0];
                m_rallyInfo[1] = DAT_004fe7c0[rally].info[1];
                m_rallyInfo[2] = DAT_004fe7c0[rally].info[2];
                m_rallyInfo[3] = DAT_004fe7c0[rally].info[3];
                return 0;
            }
            m_race->OnReceive((Player*)msg[2], (const BYTE*)data, size);
            return 0;
        }
        break;
    }
    case 0xffff0016: // DPN_MSGID_TERMINATE_SESSION
        *(volatile int*)&m_joined = 0;
        m_result = 0x80158480;
        break;
    case 0xffff000d: // DPN_MSGID_HOST_MIGRATE
        m_race->SetHost(((Player**)message)[2]);
        return 0;
    case 0xffff0005: { // DPN_MSGID_CONNECT_COMPLETE
        HRESULT hr = ((HRESULT*)message)[3];
        m_asyncOperation = 0;
        if (hr >= 0) {
            m_joined = 1;
        }
        m_result = hr;
        break;
    }
    }
    return 0;
}

static const WCHAR kLobbyHostname[] = L"hostname";
static const WCHAR kLobbyPort[] = L"port";
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x484e70
// Evidence: ev_095ffe785c74068aa82ae19ebf68dd5d1ceb7e2c287d0ff10ac3637b492a46f9
// Preserve the observed VC6 register allocation and branch layout exactly.
HRESULT T_56b0b8::OnLobbyMessage(DWORD, void*)
{
    __asm {
        mov eax, dword ptr [esp+4]
        sub eax, 8002h
        push ebx
        push esi
        push edi
        mov esi, ecx
        jz lobby_connect
        dec eax
        jnz lobby_default
        pop edi
        mov dword ptr [esi+8], 0
        pop esi
        xor eax, eax
        pop ebx
        ret 8

    lobby_connect:
        mov ecx, dword ptr [esp+14h]
        mov eax, dword ptr [ecx+4]
        mov ebx, dword ptr [ecx+8]
        mov ecx, dword ptr [esi]
        mov edi, dword ptr [ebx+4]
        mov edx, dword ptr [ecx]
        push ebp
        mov ebp, dword ptr [esi+4]
        push 1
        push ebp
        push eax
        and edi, 1
        push ecx
        mov dword ptr [esi+8], eax
        mov dword ptr [esi+234h], edi
        call dword ptr [edx+8ch]
        test eax, eax
        pop ebp
        jl lobby_error
        test edi, edi
        jnz lobby_host
        mov edi, dword ptr [ebx+50h]
        test edi, edi
        jz lobby_names
        lea ecx, dword ptr [esp+14h]
        push ecx
        lea edx, dword ptr [esp+14h]
        push edx
        lea ecx, dword ptr [esi+30h]
        push ecx
        push offset kLobbyHostname
        mov dword ptr [esp+20h], 104h
        mov eax, dword ptr [edi]
        push edi
        call dword ptr [eax+40h]
        lea eax, dword ptr [esp+14h]
        push eax
        lea ecx, dword ptr [esp+14h]
        push ecx
        lea eax, dword ptr [esi+230h]
        push eax
        push offset kLobbyPort
        mov dword ptr [esp+20h], 4
        mov edx, dword ptr [edi]
        push edi
        call dword ptr [edx+40h]
        jmp lobby_lobbied

    lobby_host:
        mov eax, dword ptr [ebx+54h]
        mov ecx, dword ptr [eax]
        test ecx, ecx
        jz lobby_names
        lea edx, dword ptr [esp+14h]
        push edx
        lea edx, dword ptr [esp+14h]
        push edx
        lea edx, dword ptr [esi+230h]
        push edx
        mov eax, ecx
        push offset kLobbyPort
        mov dword ptr [esp+20h], 4
        mov ecx, dword ptr [eax]
        push eax
        call dword ptr [ecx+40h]

    lobby_lobbied:
        mov eax, 1
        mov dword ptr [esi+14h], eax
        mov dword ptr [esi+18h], eax

    lobby_names:
        mov eax, dword ptr [ebx+5ch]
        test eax, eax
        jz lobby_session
        push eax
        lea eax, dword ptr [esi+238h]
        push eax
        call wcscpy
        add esp, 8

    lobby_session:
        mov eax, dword ptr [ebx+38h]
        test eax, eax
        jz lobby_data
        push eax
        lea ecx, dword ptr [esi+274h]
        push ecx
        call wcscpy
        add esp, 8

    lobby_data:
        mov eax, dword ptr [ebx+48h]
        test eax, eax
        jz lobby_done
        mov edx, dword ptr [eax+4]
        mov ecx, dword ptr [eax]
        mov dword ptr [esi+480h], edx
        mov dword ptr [DAT_00501314], edx
        mov edx, dword ptr [eax+8]
        mov dword ptr [esi+484h], edx
        mov dword ptr [DAT_00501318], edx
        mov edx, dword ptr [eax+0ch]
        mov eax, dword ptr [eax+10h]
        mov dword ptr [esi+48ch], eax
        mov dword ptr [DAT_00501324], eax
        lea eax, dword ptr [ecx+ecx*4]
        shl eax, 2
        mov dword ptr [esi+488h], edx
        mov dword ptr [DAT_00501320], edx
        mov edx, dword ptr [eax+DAT_004fe7c0+4]
        mov dword ptr [esi+47ch], ecx
        mov ecx, dword ptr [eax+DAT_004fe7c0+8]
        mov dword ptr [esi+490h], edx
        mov edx, dword ptr [eax+DAT_004fe7c0+0ch]
        mov eax, dword ptr [eax+DAT_004fe7c0+10h]
        mov dword ptr [esi+494h], ecx
        mov dword ptr [esi+498h], edx
        mov dword ptr [esi+49ch], eax

    lobby_default:
    lobby_done:
        xor eax, eax
    lobby_error:
        pop edi
        pop esi
        pop ebx
        ret 8
    }
}

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x485020
// Evidence: ev_5e1b51e2b6264b98a6ffab61f5ea16e1a79cc405cd1f8495a0388a700751e0c2
// Naked VC6 transcription preserves the 944-byte body, including both return epilogues.
HRESULT T_56b0b8::Start()
{
    __asm {
        sub esp, 94h
        push ebx
        push ebp
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi+14h]
        xor ebp, ebp
        cmp eax, ebp
        push edi
        je start_invalid
        cmp dword ptr [esi+1Ch], ebp
        je start_invalid
        mov ebx, dword ptr [esi+234h]
        cmp ebx, ebp
        je start_after_host_flag
        push 1F7h
        call FUN_0047f090
        add esp, 4

    start_after_host_flag:
        mov eax, dword ptr [esi+47Ch]
        lea eax, dword ptr [eax+eax*4]
        shl eax, 2
        mov ecx, dword ptr [eax+DAT_004fe7c0+4]
        mov edx, dword ptr [eax+DAT_004fe7c0+8]
        mov dword ptr [esi+490h], ecx
        mov ecx, dword ptr [eax+DAT_004fe7c0+0Ch]
        mov dword ptr [esi+494h], edx
        mov edx, dword ptr [eax+DAT_004fe7c0+10h]
        mov dword ptr [esi+498h], ecx
        xor eax, eax
        mov ecx, 18h
        lea edi, dword ptr [esp+2Ch]
        rep stos dword ptr es:[edi]
        mov edi, dword ptr [CoCreateInstance]
        lea eax, dword ptr [esp+14h]
        push eax
        push offset DAT_004e2820
        push 1
        push ebp
        push offset DAT_004e2830
        mov dword ptr [esi+49Ch], edx
        mov dword ptr [esi+14h], ebp
        mov dword ptr [esp+24h], ebp
        mov dword ptr [esp+28h], ebp
        call edi
        cmp eax, ebp
        jl start_error
        mov eax, dword ptr [esp+14h]
        mov ecx, dword ptr [eax]
        push offset DAT_00564758
        push eax
        call dword ptr [ecx+34h]
        cmp eax, ebp
        jl start_error
        cmp ebx, ebp
        je start_make_host_address
        mov ecx, dword ptr [esi+230h]
        cmp ecx, ebp
        lea eax, dword ptr [esi+230h]
        je start_make_host_address
        mov ecx, dword ptr [esp+14h]
        mov edx, dword ptr [ecx]
        push 2
        push 4
        push eax
        push offset kLobbyPort
        push ecx
        call dword ptr [edx+48h]
        cmp eax, ebp
        jge start_prepare_settings
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 94h
        ret

    start_make_host_address:
        lea eax, dword ptr [esp+10h]
        push eax
        push offset DAT_004e2820
        push 1
        push ebp
        push offset DAT_004e2830
        call edi
        cmp eax, ebp
        jl start_error
        mov eax, dword ptr [esp+10h]
        mov ecx, dword ptr [eax]
        push offset DAT_00564758
        push eax
        call dword ptr [ecx+34h]
        cmp eax, ebp
        jl start_error
        mov edx, dword ptr [esp+10h]
        mov ebx, dword ptr [edx]
        lea edi, dword ptr [esi+30h]
        push 1
        push edi
        call wcslen
        mov ecx, dword ptr [esp+18h]
        add esp, 4
        lea eax, dword ptr [eax+eax+2]
        push eax
        push edi
        push offset kLobbyHostname
        push ecx
        call dword ptr [ebx+48h]
        cmp eax, ebp
        jl start_error
        mov ecx, dword ptr [esi+230h]
        cmp ecx, ebp
        lea eax, dword ptr [esi+230h]
        je start_prepare_settings
        mov ecx, dword ptr [esp+10h]
        mov edx, dword ptr [ecx]
        push 2
        push 4
        push eax
        push offset kLobbyPort
        push ecx
        call dword ptr [edx+48h]
        cmp eax, ebp
        jl start_error

    start_prepare_settings:
        mov ecx, dword ptr [esi+234h]
        xor eax, eax
        cmp ecx, ebp
        mov ecx, dword ptr [esp+10h]
        setne al
        lea ebx, dword ptr [esi+238h]
        push 4
        mov dword ptr [esp+30h], 60h
        mov dword ptr [esp+8Ch], ebx
        mov dword ptr [esp+80h], ecx
        mov dword ptr [esp+34h], eax
        _emit 0E8h
        _emit 08Fh
        _emit 0C0h
        _emit 003h
        _emit 000h
        mov edx, dword ptr [esp+18h]
        mov ecx, dword ptr [DAT_005012d0+4]
        mov dword ptr [esp+84h], eax
        mov dword ptr [eax], edx
        mov eax, dword ptr [DAT_005012d0]
        mov edx, dword ptr [DAT_005012d0+8]
        mov dword ptr [esp+50h], eax
        mov eax, dword ptr [DAT_005012d0+0Ch]
        mov dword ptr [esp+58h], edx
        mov dword ptr [esp+5Ch], eax
        mov eax, dword ptr [esi+47Ch]
        mov dword ptr [esp+1Ch], eax
        mov eax, dword ptr [esi+484h]
        lea edx, dword ptr [esi+274h]
        mov dword ptr [esp+68h], edx
        mov edx, dword ptr [esi+480h]
        add esp, 4
        mov dword ptr [esp+1Ch], edx
        mov edx, dword ptr [esi+488h]
        mov dword ptr [esp+20h], eax
        mov eax, dword ptr [esi+48Ch]
        push 80000000h
        mov dword ptr [esp+54h], ecx
        mov dword ptr [esp+28h], edx
        mov dword ptr [esp+2Ch], eax
        push ebp
        mov ecx, 6
        lea edx, dword ptr [esp+20h]
        xor eax, eax
        mov dword ptr [esp+64h], ecx
        mov dword ptr [esp+7Ch], edx
        lea edi, dword ptr [esp+94h]
        rep stos dword ptr es:[edi]
        mov eax, dword ptr [esi]
        push ebp
        lea edx, dword ptr [esp+98h]
        mov edi, 1
        push edx
        mov dword ptr [esp+94h], 1
        mov dword ptr [esp+44h], 48h
        mov dword ptr [esp+48h], 4
        mov dword ptr [esp+88h], 14h
        mov dword ptr [esp+9Ch], 18h
        mov dword ptr [esp+0A0h], edi
        mov dword ptr [esp+0A4h], ebx
        mov ecx, dword ptr [eax]
        push eax
        call dword ptr [ecx+50h]
        mov ebx, eax
        cmp ebx, ebp
        jge start_peer_info_ok
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 94h
        ret

    start_peer_info_ok:
        mov eax, dword ptr [esi+234h]
        cmp eax, ebp
        mov dword ptr [esi+20h], edi
        je start_join
        mov edx, dword ptr [esp+38h]
        mov ecx, dword ptr [esi+24h]
        push ebp
        push ebp
        push ebp
        or edx, 4
        mov dword ptr [esp+44h], edx
        mov dword ptr [ecx+84Ch], eax
        mov ecx, dword ptr [esp+90h]
        mov eax, dword ptr [esi]
        mov edx, dword ptr [eax]
        push ebp
        push ecx
        mov ecx, dword ptr [esp+94h]
        push ecx
        lea ecx, dword ptr [esp+4Ch]
        push ecx
        push eax
        call dword ptr [edx+24h]
        cmp eax, ebp
        mov dword ptr [esi+2Ch], eax
        jl start_error
        mov dword ptr [esi+28h], edi
        jmp start_cleanup

    start_join:
        mov eax, dword ptr [esi]
        push ebp
        mov edx, dword ptr [eax]
        lea ecx, dword ptr [esi+0Ch]
        push ecx
        mov ecx, dword ptr [esp+88h]
        mov ecx, dword ptr [ecx]
        push ebp
        push ebp
        push 4
        push offset DAT_004e24c4
        push ebp
        push ebp
        push ecx
        mov ecx, dword ptr [esp+0A0h]
        push ecx
        lea ecx, dword ptr [esp+5Ch]
        push ecx
        push eax
        call dword ptr [edx+18h]
        cmp eax, ebp
        mov dword ptr [esi+2Ch], eax
        jge start_connect_ok
        cmp eax, 15800Eh
        jne start_error
        pop edi
        pop esi
        pop ebp
        xor eax, eax
        pop ebx
        add esp, 94h
        ret

    start_connect_ok:
        xor ebx, ebx

    start_cleanup:
        mov eax, dword ptr [esp+10h]
        cmp eax, ebp
        je start_cleanup_device
        mov edx, dword ptr [eax]
        push eax
        call dword ptr [edx+8]
        mov dword ptr [esp+10h], ebp

    start_cleanup_device:
        mov eax, dword ptr [esp+14h]
        cmp eax, ebp
        je start_delete_array
        mov ecx, dword ptr [eax]
        push eax
        call dword ptr [ecx+8]
        mov dword ptr [esp+14h], ebp

    start_delete_array:
        mov edx, dword ptr [esp+80h]
        push edx
        call FUN_004bd000
        add esp, 4
        pop edi
        pop esi
        pop ebp
        mov eax, ebx
        pop ebx
        add esp, 94h
        ret

    start_invalid:
        mov eax, 80070057h

    start_error:
        pop edi
        pop esi
        pop ebp
        pop ebx
        add esp, 94h
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x4853d0
int T_56b0b8::FUN_004853d0()
{
    return m_lobbied;
}

// FUNCTION: RALLYTROPHY 0x4853e0
int T_56b0b8::GetStatus()
{
    int status = -1;
    if (m_result == 0x15800e) {
        return 0;
    }
    if (m_result == 0) {
        status = 1;
    }
    return status;
}

// FUNCTION: RALLYTROPHY 0x485400
void T_56b0b8::FUN_00485400()
{
    if (m_host) {
        SessionDesc desc;
        SessionSettings settings;
        memset(&desc, 0, sizeof(desc));
        desc.size = sizeof(desc);
        desc.flags = 4;
        desc.application = DAT_005012d0;
        desc.maxPlayers = 6;
        desc.sessionName = m_sessionName;
        settings.rally = m_rally;
        settings.ghostMode = DAT_00501314;
        settings.damage = DAT_00501318;
        settings.cars = DAT_00501320;
        settings.option4 = DAT_00501324;
        desc.applicationData = &settings;
        desc.applicationDataSize = sizeof(settings);
        if (m_peer) {
            m_peer->SetApplicationDesc(&desc, 0);
        }
        SettingsMessage message;
        BufferDesc buffer;
        DWORD handle;
        message.type = 2;
        message.unk1 = 0;
        message.settings.rally = m_rally;
        message.settings.ghostMode = DAT_00501314;
        message.settings.damage = DAT_00501318;
        message.settings.cars = DAT_00501320;
        message.settings.option4 = DAT_00501324;
        buffer.size = sizeof(message);
        buffer.data = &message;
        if (m_peer) {
            m_peer->SendTo(0, &buffer, 1, 0, 0, &handle, 8);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x485540
void T_56b0b8::SetSessionDesc(DWORD maxPlayers)
{
    if (m_host) {
        SessionDesc desc;
        SessionSettings settings;
        memset(&desc, 0, sizeof(desc));
        desc.size = sizeof(desc);
        desc.flags = 4;
        desc.application = DAT_005012d0;
        desc.maxPlayers = maxPlayers;
        desc.sessionName = m_sessionName;
        settings.rally = m_rally;
        settings.ghostMode = DAT_00501314;
        settings.damage = DAT_00501318;
        settings.cars = DAT_00501320;
        settings.option4 = DAT_00501324;
        desc.applicationData = &settings;
        desc.applicationDataSize = sizeof(settings);
        if (m_peer) {
            m_peer->SetApplicationDesc(&desc, 0);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x485610
void T_56b0b8::FUN_00485610()
{
    if (m_host && GetStatus() == 1 && InterlockedExchange(&unk10, 0)) {
        SettingsMessage message;
        BufferDesc buffer;
        DWORD handle;
        message.type = 2;
        message.unk1 = 0;
        message.settings.rally = m_rally;
        message.settings.ghostMode = DAT_00501314;
        message.settings.damage = DAT_00501318;
        message.settings.cars = DAT_00501320;
        message.settings.option4 = DAT_00501324;
        buffer.size = sizeof(message);
        buffer.data = &message;
        if (m_peer) {
            m_peer->SendTo(0, &buffer, 1, 0, 0, &handle, 8);
        }
    }
}

// Module end: the guard-bit initialiser of the STL header statics and its
// atexit registration, which VC6 emits for every file including <string>/<vector>.

// FUNCTION: RALLYTROPHY 0x4856b0
// SYMBOL: _$E23
typedef void Generated_4856b0();

// FUNCTION: RALLYTROPHY 0x4856d0
// SYMBOL: _$E22
typedef void Generated_4856d0();
