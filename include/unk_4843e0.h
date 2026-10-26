/* Networking T_56b0b8 (0x4843e0-0x4856ef): DirectPlay 8 peer sessions and
 * lobby launches. Only src/unk_4843e0.cpp includes this, before
 * rallytrophy.h; other files use the opaque declaration there (see the
 * controller module for why). */
#ifndef UNK_4843E0_H
#define UNK_4843E0_H

#include <windows.h>

struct IDirectPlay8Peer;
struct IDirectPlay8LobbiedApplication;
struct IDirectPlay8Address;

class NetRace; // the network race mode (include/unk_481390.h)

class T_56b0b8 { // 0x4a0 bytes
public:
    T_56b0b8();  // 0x4843e0
    ~T_56b0b8(); // 0x4844a0

    static HRESULT __stdcall PeerCallback(void* context, DWORD type, void* message);  // 0x484b70
    static HRESULT __stdcall LobbyCallback(void* context, DWORD type, void* message); // 0x484b90
    HRESULT OnPeerMessage(DWORD type, void* message);  // 0x484bc0
    HRESULT OnLobbyMessage(DWORD type, void* message); // 0x484e70

    HRESULT Initialize();          // 0x4844b0: create DirectPlay objects, pick the TCP/IP provider
    void ParseCommandLine();       // 0x484680: +connect +name +host +hostname +game +ghostmode +damage +cars
    static int FindCommand(const char* name); // 0x484a90: index in the 8-entry command table, 8 if none
    void Close();                  // 0x484ac0
    HRESULT Release();             // 0x484b20
    int FUN_004853d0();            // 0x4853d0: launched from a lobby (skip the front end)
    int GetStatus();               // 0x4853e0: 1 ok, 0 0x15800e, -1 otherwise
    HRESULT Start();               // 0x485020: host or join the session given on the command line or by the lobby
    void FUN_00485400();           // 0x485400: host: publish the session description, send the settings
    void SetSessionDesc(DWORD maxPlayers); // 0x485540
    void FUN_00485610();           // 0x485610: host: resend the settings when flagged
    void FUN_0048e660();           // 0x48e660 (another module)

    IDirectPlay8Peer* m_peer;                  // 0x000
    IDirectPlay8LobbiedApplication* m_lobby;   // 0x004
    DWORD m_lobbyConnection;                   // 0x008
    DWORD m_asyncOperation;                    // 0x00c
    LONG unk10;                                // 0x010: set when the session settings must be resent
    int m_lobbied;                             // 0x014
    int unk18;                                 // 0x018
    int m_providerFound;                       // 0x01c
    int m_connected;                           // 0x020
    NetRace* m_race;                           // 0x024: the race receiving player events
    int m_joined;                              // 0x028: connect completed, cleared on terminate
    HRESULT m_result;                          // 0x02c
    WCHAR m_hostName[0x100];                   // 0x030: "connect host[:port]" / lobby hostname
    int m_port;                                // 0x230: 0x7274 by default
    int m_host;                                // 0x234: hosting rather than joining
    WCHAR m_playerName[30];                    // 0x238
    WCHAR m_sessionName[0x104];                // 0x274
    int m_rally;                               // 0x47c: index into the rally table at 0x4fe7c0
    int m_ghostMode;                           // 0x480: mirrors DAT_00501314 (+ghostmode 1)
    int m_damage;                              // 0x484: mirrors DAT_00501318 (+damage 0 turns it off)
    int m_cars;                                // 0x488: mirrors DAT_00501320 (novice, intermediate, expert, all)
    int m_option4;                             // 0x48c: mirrors DAT_00501324
    DWORD m_rallyInfo[4];                      // 0x490: copied from the rally table entry
};

extern T_56b0b8* DAT_00565368;  // the networking object
extern int DAT_00501314;        // game options shared with the session (see m_ghostMode..)
extern int DAT_00501318;
extern int DAT_00501320;
extern int DAT_00501324;
void FUN_0047f1a0();            // called after closing a hosted session
void FUN_0047f090(int);         // called with 0x1f7 before hosting
extern const DWORD DAT_004e24c4; // 0x10002: connect data (checked by the host's INDICATE_CONNECT)

// DPN_PLAYER_INFO.
struct PlayerInfo {
    DWORD size;
    DWORD infoFlags;
    WCHAR* name;
    void* data;
    DWORD dataSize;
    DWORD playerFlags;
};

// One entry of the rally table at 0x4fe7c0 (0x14 bytes, 54 entries).
// Session settings, the application data of the session description (0x14 bytes).
struct SessionSettings {
    int rally;
    int ghostMode;
    int damage;
    int cars;
    int option4;
};

// Message type 2: the host's session settings (0x16 bytes, packed).
#pragma pack(push, 1)
struct SettingsMessage {
    BYTE type;
    BYTE unk1;
    SessionSettings settings;
};
#pragma pack(pop)

// DPN_APPLICATION_DESC (0x48 bytes).
struct SessionDesc {
    DWORD size;
    DWORD flags;
    GUID instance;
    GUID application;
    DWORD maxPlayers;
    DWORD currentPlayers;
    WCHAR* sessionName;
    WCHAR* password;
    void* reserved;
    DWORD reservedSize;
    void* applicationData;
    DWORD applicationDataSize;
};

// DPL_CONNECTION_SETTINGS.
struct LobbyConnectSettings {
    DWORD size;
    DWORD flags; // 1: host
    SessionDesc session;
    IDirectPlay8Address* hostAddress;
    IDirectPlay8Address** deviceAddresses;
    DWORD deviceAddressCount;
    WCHAR* playerName;
};

// DPN_SERVICE_PROVIDER_INFO (0x20 bytes).
struct ServiceProviderInfo {
    DWORD flags;
    GUID guid;
    WCHAR* name;
    void* reserved;
    DWORD reserved2;
};

// DPN_CAPS.
struct PeerCaps {
    DWORD size;
    DWORD flags;
    DWORD connectTimeout;
    DWORD connectRetries;
    DWORD timeoutUntilKeepAlive;
};

// DPN_SP_CAPS.
struct ProviderCaps {
    DWORD size;
    DWORD flags;
    DWORD threads;
    DWORD defaultEnumCount;
    DWORD defaultEnumRetryInterval;
    DWORD defaultEnumTimeout;
    DWORD maxEnumPayloadSize;
    DWORD buffersPerThread;
    DWORD systemBufferSize;
};

extern const GUID DAT_004e27d0; // IID_IDirectPlay8LobbiedApplication
extern const GUID DAT_004e27e0; // CLSID_DirectPlay8LobbiedApplication
extern const GUID DAT_004e27f0; // CLSID_DP8SP_TCPIP
extern const GUID DAT_004e2820; // IID_IDirectPlay8Address
extern const GUID DAT_004e2830; // CLSID_DirectPlay8Address
extern const GUID DAT_004e2800; // IID_IDirectPlay8Peer
extern const GUID DAT_004e2810; // CLSID_DirectPlay8Peer
extern GUID DAT_00564758;       // the chosen service provider

// DPN_BUFFER_DESC.
struct BufferDesc {
    DWORD size;
    void* data;
};

extern const GUID DAT_005012d0; // the game's application GUID

struct RallyEntry {
    const char* name;
    DWORD info[4]; // copied to T_56b0b8::m_rallyInfo
};
extern RallyEntry DAT_004fe7c0[54];
extern char DAT_00564f68[]; // "+connect" argument scratch buffer

#endif
