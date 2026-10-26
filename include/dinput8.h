/* Minimal DirectInput 8 interface declarations by vtable slot (no SDK
 * headers). Only the slots the game calls matter; parameter types are loose.
 * Include after rallytrophy.h. */
#ifndef DINPUT8_H
#define DINPUT8_H

struct IDirectInputDevice8A {
    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** object) = 0; // 0x00
    virtual ULONG __stdcall AddRef() = 0;                                      // 0x04
    virtual ULONG __stdcall Release() = 0;                                     // 0x08
    virtual HRESULT __stdcall GetCapabilities(void* caps) = 0;                 // 0x0c
    virtual HRESULT __stdcall EnumObjects(void* callback, void* context, DWORD flags) = 0; // 0x10
    virtual HRESULT __stdcall GetProperty(const GUID& property, void* header) = 0; // 0x14
    virtual HRESULT __stdcall SetProperty(const GUID& property, const void* header) = 0; // 0x18
    virtual HRESULT __stdcall Acquire() = 0;                                   // 0x1c
    virtual HRESULT __stdcall Unacquire() = 0;                                 // 0x20
    virtual HRESULT __stdcall GetDeviceState(DWORD size, void* data) = 0;      // 0x24
    virtual HRESULT __stdcall GetDeviceData(DWORD size, void* data, DWORD* count, DWORD flags) = 0; // 0x28
    virtual HRESULT __stdcall SetDataFormat(const void* format) = 0;           // 0x2c
    virtual HRESULT __stdcall SetEventNotification(HANDLE event) = 0;          // 0x30
    virtual HRESULT __stdcall SetCooperativeLevel(HWND window, DWORD flags) = 0; // 0x34
    virtual HRESULT __stdcall GetObjectInfo(void* info, DWORD object, DWORD how) = 0; // 0x38
    virtual HRESULT __stdcall GetDeviceInfo(void* instance) = 0;               // 0x3c
    virtual HRESULT __stdcall RunControlPanel(HWND owner, DWORD flags) = 0;    // 0x40
    virtual HRESULT __stdcall Initialize(HINSTANCE instance, DWORD version, const GUID& guid) = 0; // 0x44
    virtual HRESULT __stdcall CreateEffect(const GUID& guid, const void* effect, IDirectInputEffect** out, void* outer) = 0; // 0x48
    virtual HRESULT __stdcall EnumEffects(void* callback, void* context, DWORD type) = 0; // 0x4c
    virtual HRESULT __stdcall GetEffectInfo(void* info, const GUID& guid) = 0; // 0x50
    virtual HRESULT __stdcall GetForceFeedbackState(DWORD* state) = 0;         // 0x54
    virtual HRESULT __stdcall SendForceFeedbackCommand(DWORD flags) = 0;       // 0x58
    virtual HRESULT __stdcall EnumCreatedEffectObjects(void* callback, void* context, DWORD flags) = 0; // 0x5c
    virtual HRESULT __stdcall Escape(void* escape) = 0;                        // 0x60
    virtual HRESULT __stdcall Poll() = 0;                                      // 0x64
    virtual HRESULT __stdcall SendDeviceData(DWORD size, const void* data, DWORD* count, DWORD flags) = 0; // 0x68
    virtual HRESULT __stdcall EnumEffectsInFile(const char* file, void* callback, void* context, DWORD flags) = 0; // 0x6c
    virtual HRESULT __stdcall WriteEffectToFile(const char* file, DWORD count, void* effects, DWORD flags) = 0; // 0x70
    virtual HRESULT __stdcall BuildActionMap(void* format, const char* user, DWORD flags) = 0; // 0x74
    virtual HRESULT __stdcall SetActionMap(void* format, const char* user, DWORD flags) = 0; // 0x78
    virtual HRESULT __stdcall GetImageInfo(void* header) = 0;                  // 0x7c
};

struct IDirectInputEffect {
    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** object) = 0; // 0x00
    virtual ULONG __stdcall AddRef() = 0;                                      // 0x04
    virtual ULONG __stdcall Release() = 0;                                     // 0x08
    virtual HRESULT __stdcall Initialize(HINSTANCE instance, DWORD version, const GUID& guid) = 0; // 0x0c
    virtual HRESULT __stdcall GetEffectGuid(GUID* guid) = 0;                   // 0x10
    virtual HRESULT __stdcall GetParameters(void* effect, DWORD flags) = 0;    // 0x14
    virtual HRESULT __stdcall SetParameters(const void* effect, DWORD flags) = 0; // 0x18
    virtual HRESULT __stdcall Start(DWORD iterations, DWORD flags) = 0;        // 0x1c
    virtual HRESULT __stdcall Stop() = 0;                                      // 0x20
};

// DIPROPDWORD; DIPROP_* GUIDs are small integers cast to a GUID reference.
struct PropertyDword {
    DWORD size;       // 0x0 = 0x14
    DWORD headerSize; // 0x4 = 0x10
    DWORD object;     // 0x8
    DWORD how;        // 0xc
    DWORD data;       // 0x10
};
#define MAKEDIPROP(n) (*(const GUID*)(n))

// DIDEVICEOBJECTDATA (0x14 bytes).
struct ObjectData {
    DWORD offset;    // 0x00
    DWORD data;      // 0x04
    DWORD timeStamp; // 0x08
    DWORD sequence;  // 0x0c
    DWORD appData;   // 0x10: InputAction::appData (game action id)
};

// DICONFIGUREDEVICESPARAMSA (0x40 bytes).
struct ConfigureParams {
    DWORD size;             // 0x00 = 0x40
    DWORD userCount;        // 0x04
    char* userNames;        // 0x08
    DWORD formatCount;      // 0x0c
    void* formats;          // 0x10
    HWND window;            // 0x14
    DWORD colors[9];        // 0x18: DICOLORSET
    void* target;           // 0x3c: IUnknown* surface the callback draws on
};

// DIPROPRANGE (0x18 bytes).
struct PropertyRange {
    DWORD size;       // 0x00 = 0x18
    DWORD headerSize; // 0x04 = 0x10
    DWORD object;     // 0x08
    DWORD how;        // 0x0c
    LONG min;         // 0x10
    LONG max;         // 0x14
};

// DIDEVICEOBJECTINSTANCEA (0x13c bytes).
struct ObjectInstance {
    DWORD size;                  // 0x00 = 0x13c
    GUID type;                   // 0x04: GUID_XAxis etc.
    char rest[0x13c - 0x14];
};

// Axis GUIDs in the game's .rdata.
extern const GUID DAT_004e2910; // X axis
extern const GUID DAT_004e2900; // Y axis
extern const GUID DAT_004e28f0; // Z axis
extern const GUID DAT_004e28e0; // RX axis
extern const GUID DAT_004e28d0; // RY axis
extern const GUID DAT_004e28c0; // RZ axis

// DIPROPSTRING (0x218 bytes).
struct PropertyString {
    DWORD size;           // 0x00 = 0x218
    DWORD headerSize;     // 0x04 = 0x10
    DWORD object;         // 0x08
    DWORD how;            // 0x0c
    WCHAR text[MAX_PATH]; // 0x10
};

// DIDEVICEINSTANCEA (0x244 bytes).
struct DeviceInstance {
    DWORD size;                   // 0x000 = 0x244
    GUID instance;                // 0x004
    GUID product;                 // 0x014
    DWORD devType;                // 0x024: low byte 0x12 mouse, 0x13 keyboard
    char instanceName[MAX_PATH];  // 0x028
    char productName[MAX_PATH];   // 0x12c
    GUID ffDriver;                // 0x230
    WORD usagePage;               // 0x240
    WORD usage;                   // 0x242
};

// DIDEVCAPS (0x2c bytes).
struct DeviceCaps {
    DWORD size;   // 0x00 = 0x2c
    DWORD flags;  // 0x04: 0x100 = force feedback
    DWORD rest[9];
};

// DIEFFECTINFOA (start).
struct EffectInfo {
    DWORD size;    // 0x00
    GUID guid;     // 0x04
    DWORD type;    // 0x14: low byte = DIEFT_ type
};

// DIEFFECT (0x38 bytes).
struct EffectParams {
    DWORD size;               // 0x00 = 0x38
    DWORD flags;              // 0x04
    DWORD duration;           // 0x08
    DWORD samplePeriod;       // 0x0c
    DWORD gain;               // 0x10
    DWORD triggerButton;      // 0x14
    DWORD triggerRepeat;      // 0x18
    DWORD axisCount;          // 0x1c
    DWORD* axes;              // 0x20
    LONG* directions;         // 0x24
    void* envelope;           // 0x28
    DWORD typeParamsSize;     // 0x2c
    void* typeParams;         // 0x30
    DWORD startDelay;         // 0x34
};

// DIPERIODIC / DICONSTANTFORCE / DICONDITION type-specific parameters.
struct PeriodicParams {
    DWORD magnitude;
    LONG offset;
    DWORD phase;
    DWORD period;
};
struct ConditionParams {
    LONG offset;
    LONG positiveCoefficient;
    LONG negativeCoefficient;
    DWORD positiveSaturation;
    DWORD negativeSaturation;
    LONG deadBand;
};

// Effect GUIDs in the game's .rdata.
extern const GUID DAT_004e28b0; // constant force (type 1)
extern const GUID DAT_004e2880; // type 2
extern const GUID DAT_004e2840; // condition, type 4
extern const GUID DAT_004e2850; // condition, type 8
extern const GUID DAT_004e2890; // type 0x20
extern const GUID DAT_004e28a0; // type 0x40
extern const GUID DAT_004e2870; // type 0x80
extern const GUID DAT_004e2860; // type 0x100

struct IDirectInput8A {
    virtual HRESULT __stdcall QueryInterface(const IID& iid, void** object) = 0; // 0x00
    virtual ULONG __stdcall AddRef() = 0;                                      // 0x04
    virtual ULONG __stdcall Release() = 0;                                     // 0x08
    virtual HRESULT __stdcall CreateDevice(const GUID& guid, IDirectInputDevice8A** device, void* outer) = 0; // 0x0c
    virtual HRESULT __stdcall EnumDevices(DWORD type, void* callback, void* context, DWORD flags) = 0; // 0x10
    virtual HRESULT __stdcall GetDeviceStatus(const GUID& guid) = 0;           // 0x14
    virtual HRESULT __stdcall RunControlPanel(HWND owner, DWORD flags) = 0;    // 0x18
    virtual HRESULT __stdcall Initialize(HINSTANCE instance, DWORD version) = 0; // 0x1c
    virtual HRESULT __stdcall FindDevice(const GUID& guid, const char* name, GUID* instance) = 0; // 0x20
    virtual HRESULT __stdcall EnumDevicesBySemantics(const char* user, void* format, void* callback,
                                                     void* context, DWORD flags) = 0; // 0x24
    virtual HRESULT __stdcall ConfigureDevices(void* callback, void* params, DWORD flags, void* data) = 0; // 0x28
};

#endif
