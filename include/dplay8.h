/* Minimal DirectPlay 8 interface declarations by vtable slot (no SDK
 * headers). Only the slots the game calls are named; parameter types are
 * loose. Only the networking module includes this. */
#ifndef DPLAY8_H
#define DPLAY8_H

#define DP8_SLOT(n) virtual void slot##n();

struct IDirectPlay8LobbiedApplication;

struct IDirectPlay8Address {
    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** object) = 0; // 0x00
    virtual ULONG __stdcall AddRef() = 0;                                      // 0x04
    virtual ULONG __stdcall Release() = 0;                                     // 0x08
    DP8_SLOT(3) DP8_SLOT(4) DP8_SLOT(5) DP8_SLOT(6) DP8_SLOT(7) DP8_SLOT(8) DP8_SLOT(9)
    DP8_SLOT(10) DP8_SLOT(11) DP8_SLOT(12)
    virtual HRESULT __stdcall SetSP(const GUID* provider) = 0;                // 0x34
    DP8_SLOT(14) DP8_SLOT(15)
    virtual HRESULT __stdcall GetComponentByName(const WCHAR* name, void* buffer, DWORD* size, DWORD* type) = 0; // 0x40
    DP8_SLOT(17)
    virtual HRESULT __stdcall AddComponent(const WCHAR* name, const void* data, DWORD size, DWORD type) = 0; // 0x48
};

struct IDirectPlay8Peer {
    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** object) = 0; // 0x00
    virtual ULONG __stdcall AddRef() = 0;                                      // 0x04
    virtual ULONG __stdcall Release() = 0;                                     // 0x08
    virtual HRESULT __stdcall Initialize(void* context, void* handler, DWORD flags) = 0; // 0x0c
    virtual HRESULT __stdcall EnumServiceProviders(const GUID* provider, const GUID* application, void* info,
                                                   DWORD* size, DWORD* count, DWORD flags) = 0; // 0x10
    virtual HRESULT __stdcall CancelAsyncOperation(DWORD handle, DWORD flags) = 0; // 0x14
    virtual HRESULT __stdcall Connect(const void* desc, IDirectPlay8Address* host, IDirectPlay8Address* device,
                                      const void* security, const void* credentials, const void* data, DWORD size,
                                      void* playerContext, void* asyncContext, DWORD* handle, DWORD flags) = 0; // 0x18
    virtual HRESULT __stdcall SendTo(DWORD player, const void* buffers, DWORD count, DWORD timeout,
                                     void* context, DWORD* handle, DWORD flags) = 0; // 0x1c
    DP8_SLOT(8)
    virtual HRESULT __stdcall Host(const void* desc, IDirectPlay8Address** devices, DWORD count, const void* security,
                                   const void* credentials, void* playerContext, DWORD flags) = 0; // 0x24
    DP8_SLOT(10)
    virtual HRESULT __stdcall SetApplicationDesc(const void* desc, DWORD flags) = 0; // 0x2c
    DP8_SLOT(12)
    DP8_SLOT(13) DP8_SLOT(14) DP8_SLOT(15) DP8_SLOT(16) DP8_SLOT(17) DP8_SLOT(18) DP8_SLOT(19)
    virtual HRESULT __stdcall SetPeerInfo(const void* info, void* asyncContext, DWORD* handle, DWORD flags) = 0; // 0x50
    virtual HRESULT __stdcall GetPeerInfo(DWORD player, void* info, DWORD* size, DWORD flags) = 0; // 0x54
    virtual HRESULT __stdcall GetPeerAddress(DWORD player, IDirectPlay8Address** address, DWORD flags) = 0; // 0x58
    virtual HRESULT __stdcall GetLocalHostAddresses(IDirectPlay8Address** addresses, DWORD* count, DWORD flags) = 0; // 0x5c
    virtual HRESULT __stdcall Close(DWORD flags) = 0;                          // 0x60
    DP8_SLOT(25)
    virtual HRESULT __stdcall DestroyPeer(DWORD player, const void* data, DWORD size, DWORD flags) = 0; // 0x68
    DP8_SLOT(27) DP8_SLOT(28) DP8_SLOT(29)
    virtual HRESULT __stdcall GetCaps(void* caps, DWORD flags) = 0;            // 0x78
    virtual HRESULT __stdcall SetCaps(const void* caps, DWORD flags) = 0;      // 0x7c
    virtual HRESULT __stdcall SetSPCaps(const GUID* provider, const void* caps, DWORD flags) = 0; // 0x80
    virtual HRESULT __stdcall GetSPCaps(const GUID* provider, void* caps, DWORD flags) = 0; // 0x84
    virtual HRESULT __stdcall GetConnectionInfo(DWORD player, void* info, DWORD flags) = 0; // 0x88
    virtual HRESULT __stdcall RegisterLobby(DWORD handle, IDirectPlay8LobbiedApplication* lobby, DWORD flags) = 0; // 0x8c
    virtual HRESULT __stdcall TerminateSession(void* data, DWORD size, DWORD flags) = 0; // 0x90
};

struct IDirectPlay8LobbiedApplication {
    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** object) = 0; // 0x00
    virtual ULONG __stdcall AddRef() = 0;                                      // 0x04
    virtual ULONG __stdcall Release() = 0;                                     // 0x08
    virtual HRESULT __stdcall Initialize(void* context, void* handler, DWORD* connection, DWORD flags) = 0; // 0x0c
    DP8_SLOT(4) DP8_SLOT(5) DP8_SLOT(6) DP8_SLOT(7) DP8_SLOT(8)
    virtual HRESULT __stdcall Close(DWORD flags) = 0;                          // 0x24
};

#undef DP8_SLOT
#endif
