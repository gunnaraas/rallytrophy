/* Game controller input T_56b0b4 (0x495640-0x4982bf): DirectInput 8 action
 * mapping for joysticks/wheels, per-player device lists and force-feedback
 * effects. Settings come from data/settings/input.ini. Only
 * src/unk_495640.cpp includes this, before rallytrophy.h; other files use the
 * opaque declaration in rallytrophy.h. */
#ifndef UNK_495640_H
#define UNK_495640_H

#include <windows.h>
#include <vector>

/* DirectInput 8 interfaces are declared in dinput8.h, which only the input
 * module includes: declaring them here (virtual methods) changes the setup
 * dialog's code generation in winmain.cpp. */
struct IDirectInputEffect;
struct IDirectInputDevice8A;
struct IDirectInput8A;

/* Statically linked import stub at 0x4a4b06. */
extern "C" HRESULT __stdcall DirectInput8Create(HINSTANCE instance, DWORD version, const IID& iid, void** out,
                                                void* outer);

// DIACTIONFORMATA (0x148 bytes).
struct ActionFormat {
    DWORD size;           // 0x00 = 0x148
    DWORD actionSize;     // 0x04 = 0x28
    DWORD dataSize;       // 0x08
    DWORD actionCount;    // 0x0c
    void* actions;        // 0x10
    GUID actionMap;       // 0x14
    DWORD genre;          // 0x24
    DWORD bufferSize;     // 0x28
    LONG axisMin;         // 0x2c
    LONG axisMax;         // 0x30
    HINSTANCE stringInstance; // 0x34
    FILETIME timeStamp;   // 0x38
    DWORD crc;            // 0x40
    char actionMapName[MAX_PATH]; // 0x44
};

// DIACTIONA (0x28 bytes): one game action for DirectInput action mapping.
struct InputAction {
    DWORD appData;       // 0x00: game action id
    DWORD semantic;      // 0x04
    DWORD flags;         // 0x08
    const char* name;    // 0x0c
    GUID instance;       // 0x10
    DWORD objectId;      // 0x20
    DWORD how;           // 0x24
};

// Calibrated analog range: value -> (value - min) * scale.
struct AxisRange {
    float min;   // 0x0
    float scale; // 0x4
};

// One acquired controller (0x30 bytes).
struct InputDevice {
    InputDevice()
    {
        effectTypes = 0;
        calibrated = 0;
    }
    ~InputDevice() {}

    void SetForceGain(int gain); // 0x497a50
    IDirectInputEffect* CreateEffect(int type, int start); // 0x4970f0

    unsigned effectTypes;         // 0x00: force-feedback effect types supported
    unsigned calibrated;          // 0x04: bits for the axis ranges below
    AxisRange ranges[4];          // 0x08
    const void* instance;         // 0x28: DIDEVICEINSTANCE from enumeration
    IDirectInputDevice8A* device; // 0x2c
};

// Devices mapped to one player (0x14 bytes).
struct PlayerDevices {
    std::vector<int> devices; // 0x00: indices into T_56b0b4::m_devices
    const char* name;         // 0x10: DirectInput user name
};

// A force-feedback effect played on several devices.
class ForceEffect {
public:
    ForceEffect();  // 0x497c20
    ~ForceEffect(); // 0x497c60
    void Stop();    // 0x497d10
    void Start();   // 0x497d40
    void SetParameters(int a, int b, int c); // 0x4974c0

    std::vector<IDirectInputEffect*> m_effects; // 0x00
    int m_type;                                 // 0x10
};

// Car controls fed by the input devices (class CarInput, methods in src/unk_458700.cpp).
#include "unk_458700.h"
typedef CarInput Controls;

bool GetAxisRange(IDirectInputDevice8A* device, DWORD object, float* min, float* scale, bool inverted); // 0x495410

class T_56b0b4 { // 0x188 bytes; game controllers (DirectInput 8)
public:
    T_56b0b4(HWND window, int players); // 0x495650
    ~T_56b0b4();                        // 0x4959a0

    static void LoadSettings();         // 0x4956d0
    int Init(int unused, int players);  // 0x495a60
    void SetupActionNames();            // 0x496080
    void AddDevice(const void* instance, IDirectInputDevice8A* device, int player); // 0x496240
    void ReleaseDevices(int unmap);     // 0x496300
    void ReleaseEffects();              // 0x497420
    void DigitalSteering(float steering, Controls* controls); // 0x496fc0
    int Poll(Controls* controls, int player); // 0x496b30: player is 1-based; 1 if a device was reacquired
    void EnumerateDevices();            // 0x4966d0: per-user devices via action mapping
    void CreateEffects();               // 0x496910
    void Refresh();                     // 0x497010
    void Unacquire();                   // 0x497060
    void ConfigureDevices();            // 0x497d80: DirectInput device configuration UI
    void DrawConfigImage();             // 0x497f50
    void SetForceGain(unsigned device, int gain); // 0x497a90 (device 0x400 = all)
    void SetSaturation(int saturation); // 0x497b20
    ForceEffect* CreateEffect(unsigned device, int type, int start); // 0x497780 (device 0x400 = all)
    void SetDeadZone(int deadZone);     // 0x497ba0

    char* m_cursor;                       // 0x000: walks m_names
    char* m_names;                        // 0x004: player names, NUL separated
    std::vector<InputDevice> m_devices;   // 0x008
    std::vector<ForceEffect*> m_effects;  // 0x018
    ActionFormat m_actionFormat;          // 0x028
    PlayerDevices* m_players;             // 0x170
    int m_playerCount;                    // 0x174
    int m_retry;                          // 0x178
    int unk17c;                           // 0x17c
    int unk180;                           // 0x180
    class ConfigImage* m_configImage;     // 0x184: shown behind the DirectInput configure UI
};

extern IDirectInput8A* DAT_00569e5c;  // DirectInput object
extern InputAction DAT_004ffb68[37];  // action map table
extern bool DAT_00569e58;             // input.ini loaded
extern bool DAT_00569e5a;             // ForceDigitalSteering
extern bool DAT_00569e5b;             // OverrideRanges
extern int DAT_005001a0;              // BufferSize (at most 32)
extern int DAT_005001a4;              // AxisMinValue
extern int DAT_005001a8;              // AxisMaxValue
extern AxisRange DAT_00569e18[3];     // AnalogRX/RY/RZAxisRange
extern AxisRange DAT_00569e30[3];     // AnalogX/Y/ZAxisRange
extern float DAT_00501488;            // SteeringExponent
extern float DAT_0050148c;            // ThrottleExponent
extern float DAT_00501490;            // BrakeExponent


#endif
