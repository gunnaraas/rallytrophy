/* Shared declarations for the Rally Trophy decomp.
 * Unnamed functions and globals keep their Ghidra names (FUN_/DAT_<address>)
 * until we know what they are, so symbols line up with the original layout. */
#ifndef RALLYTROPHY_H
#define RALLYTROPHY_H

#define _WIN32_DCOM // CoInitializeEx
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4786) // VC6: identifier truncated in debug info (STL)
#include <vector>
#include <stdexcept>
#include <string>

/* Globals */
extern char DAT_0056b300[0x400]; /* game install directory */
extern HKEY DAT_0056b0a8;        /* HKLM\Software\Bugbear\Rally Trophy */
extern char DAT_00501634[];      /* "HDPath" */

// Display modes offered by a renderer adapter (setup dialog).
struct DisplayMode {
    int GetRefreshCount() const { return refreshCount; }
    int GetRefresh(int index) const { return refresh[index]; }
    DWORD GetAntialias() const { return antialias; }

    int unk0;
    long width;       // 0x4
    long height;      // 0x8
    long bpp;         // 0xc
    DWORD antialias;  // 0x10: bit n set = n-sample anti-aliasing supported
    int refreshCount; // 0x14
    int refresh[1];   // 0x18: refresh rates in Hz (0 = default)
};

// Renderer device implemented by the plugin DLL (Device/bbdx8.dll). Only the
// vtable slots and fields the game uses so far are named; slot N is at +4*N.
class RenderDevice {
public:
    virtual ~RenderDevice();
    virtual const char* GetName(); // 0x4
    virtual GUID* GetGuid();                       // 0x8
    virtual std::vector<DisplayMode*>* GetModes(); // 0xc
    virtual void vf04();
    virtual DWORD* GetCaps(); // 0x14: [0] bits 1/2 = detail features, [1] bit 3
    virtual void vf06();
    virtual void vf07();
    virtual char ReadChar(); // 0x20: next typed character
    virtual void vf09();
    virtual void vf10();
    virtual HWND OpenWindow(HINSTANCE instance, const char* title, HICON icon, HCURSOR cursor,
                            DisplayMode* mode, int refresh, int antialias, int flags); // 0x2c
    virtual void Release(); // 0x30: also undoes a failed CreateWindow
    virtual void Present(); // 0x34
    virtual void vf14();
    virtual void vf15();
    virtual void vf16();
    virtual void vf17();
    virtual void vf18();
    virtual void vf19();
    virtual void vf20();
    virtual void vf21();
    virtual void vf22();
    virtual void vf23();
    virtual void vf24();
    virtual void vf25();
    virtual void vf26();
    virtual int BeginScene(); // 0x6c
    virtual void EndScene(); // 0x70
    virtual void vf29();
    virtual void vf30();
    virtual void vf31();
    virtual void vf32();
    virtual void SetRenderState(int state, int value); // 0x84
    virtual void vf34();
    virtual void vf35(class Sprite* material); // 0x8c: select the material
    virtual void vf36();
    virtual void vf37();
    virtual void vf38(int value); // 0x98
    virtual void vf39();
    virtual void vf40();
    virtual void vf41();
    virtual void vf42(int value); // 0xa8
    virtual void vf43();
    virtual void vf44(int value); // 0xb0
    virtual void vf45();
    virtual void vf46(const float* world, int flag); // 0xb8: set the world matrix
    virtual void vf47();
    virtual void vf48(const float* matrix, int flag); // 0xc0
    virtual void vf49();
    virtual void vf50();
    virtual void vf51(float a, float b, float c, float d, int flag); // 0xcc
    virtual void vf52();
    virtual void vf53();
    virtual void vf54();
    virtual void vf55();
    virtual void vf56();
    virtual const float* GetViewMatrix(); // 0xe4: current view matrix (4x4, row vectors)
    virtual const float* GetMatrixB(); // 0xe8: second matrix (4x4, row vectors; the view matrix's counterpart)
    virtual void vf59();
    virtual void vf60();
    virtual void vf61();
    virtual void vf62();
    virtual void vf63();
    virtual void vf64(class Mesh* mesh); // 0x100: draw a mesh
    virtual void DrawMesh(class DynamicMesh* mesh, int flags); // 0x104: draw a dynamic mesh
    virtual void vf66(class HudPoint* positions, class HudPoint* uvs, DWORD color, float sx, float sy); // 0x108: textured quad
    virtual void vf67();
    virtual void vf68();
    virtual void DrawQuad(class EffectVertex* quad, void* texture, float depth); // 0x114: sprite from 4 vertices
    virtual void vf70();
    virtual void vf71();
    virtual void vf72();
    virtual void vf73();
    virtual void vf74();
    virtual void LoadFileData(const char* name, void* data, int size, int slot); // 0x12c
    virtual char vf76(int feature);                                    // 0x130

    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }

    int m_width;   // 0x4
    int m_height;  // 0x8
    DWORD m_memory; // 0xc: video memory (MB)
    char unk10[0x18];
    char m_active;  // 0x28: rendering possible (window active)
};

// Buffered file (0x4078 bytes, mostly a 16 KB buffer), implemented around 0x4a4d40.
class File {
public:
    File();                             // 0x4a4d40
    File(const char* name, int mode);   // 0x4a4e00
    ~File();                            // 0x4a4d90
    int Open(const char* name, int mode); // 0x4a4f50
    static bool Exists(const char* name); // 0x4a6050
    void Close();                       // 0x4a5d00
    int Read(void* data, int size);     // 0x4a5e00
    int Write(const void* data, int size); // 0x4a5e90
    int GetSize();                      // 0x4a5ff0
    int ReadLine(char* buffer, int size); // 0x4a5ee0

private:
    int OpenArchiveEntry(const char* name, int mode, int probe);
#ifdef RT_FILE_TYPED_LAYOUT
    void* m_stream;       // 0x0
    std::string m_name;   // 0x4, 16 bytes in the VC6 ABI
    char m_state[0x4064]; // 0x14..0x4077
#else
    // Other translation units see the same-sized opaque storage. Giving VC6
    // the string member declaration there changes unrelated caller codegen.
    char m_data[0x4078];
#endif
};

// Archive-backed file lookup; nonzero means a matching asset exists.
int FUN_004a5920(const char* name);

typedef RenderDevice DisplayAdapter; // the adapter list holds the device objects themselves

struct AdapterEntry { // 0x14 bytes; array at DAT_0056b968
    DisplayAdapter* adapter; // 0x0
    DisplayMode* mode;       // 0x4: selected mode
    int modeIndex;           // 0x8
    int refresh;             // 0xc: Settings::refresh
    int antialias;           // 0x10: Settings::antialias
};

// Doubly linked list (not STL) with a free list of recycled nodes (0x14 bytes).
template <class T>
class List {
public:
    struct Node {
        ~Node() {}
        Node* next; // 0x0
        Node* prev; // 0x4
        T data;     // 0x8
    };

    List()
    {
        m_head = m_tail = m_free = 0;
        m_count = 0;
        m_freeCount = 0;
    }

    ~List()
    {
        RemoveAll();
        Node* node = m_free;
        while (node) {
            Node* next = node->next;
            delete node;
            node = next;
        }
        m_free = 0;
    }

    void AddTail(T item);         // out of line (0x4aecf0 for pointer types)
    void RemoveNode(Node* node);  // out of line (0x414490 for pointer types)

    // Unlinks the head node and recycles it on the free list.
    void RemoveHead()
    {
        Node* node = m_head;
        m_head = node->next;
        if (node == m_tail) {
            m_tail = node->prev;
        } else {
            m_head->prev = node->prev;
        }
        node->next = m_free;
        node->prev = 0;
        m_free = node;
        m_freeCount++;
        m_count--;
    }

    // Unlinks any node and recycles it on the free list.
    void Remove(Node* node)
    {
        if (node == m_head) {
            m_head = node->next;
        } else {
            node->prev->next = node->next;
        }
        if (node == m_tail) {
            m_tail = node->prev;
        } else {
            node->next->prev = node->prev;
        }
        node->next = m_free;
        node->prev = 0;
        m_free = node;
        m_freeCount++;
        m_count--;
    }

    void RemoveAll()
    {
        while (m_head) {
            RemoveHead();
        }
    }

    Node* m_head;    // 0x0
    Node* m_tail;    // 0x4
    Node* m_free;    // 0x8
    int m_count;     // 0xc
    int m_freeCount; // 0x10
};

typedef List<RenderDevice*>::Node AdapterNode;

// Growable array (not STL): contiguous storage reallocated in steps of m_grow.
template <class T>
class Array {
public:
    Array(int capacity, int grow)
    {
        m_count = 0;
        m_capacity = capacity;
        m_grow = grow;
        m_data = (T*)operator new(capacity * sizeof(T));
        m_end = m_data;
    }

    void Add(const T& item)
    {
        if (m_count == m_capacity) {
            T* data = (T*)operator new((m_capacity + m_grow) * sizeof(T));
            memcpy(data, m_data, m_capacity * sizeof(T));
            m_capacity += m_grow;
            operator delete(m_data);
            m_data = data;
            m_end = m_data + m_count;
        }
        *m_end = item;
        m_count++;
        m_end++;
    }

    T& operator[](int index) { return m_data[index]; }

    T* m_data;      // 0x0
    T* m_end;       // 0x4
    int m_count;    // 0x8
    int m_capacity; // 0xc
    int m_grow;     // 0x10
};

// Subsystem objects owned by the WinMain module, named after their global
// until their purpose is known. All have non-virtual destructors.
#include "sound.h" // DeviceInfo, T_56b0b0 (sound system)

// Game controllers. src/unk_495640.cpp includes include/unk_495640.h first,
// which defines the full class (same 0x188-byte layout); other files see this
// opaque view, because the full declaration changes the setup dialog's code
// generation in winmain.cpp.
#ifndef UNK_495640_H
class T_56b0b4 { // 0x188 bytes
public:
    T_56b0b4(HWND window, int unk); // 0x495650
    ~T_56b0b4();                    // 0x4959a0

private:
    char m_data[0x188];
};
#endif
// Networking (DirectPlay 8): src/unk_4843e0.cpp sees the full class from
// include/unk_4843e0.h (same layout); other files see this opaque view.
#ifndef UNK_4843E0_H
class T_56b0b8 { // 0x4a0 bytes
public:
    T_56b0b8();           // 0x4843e0
    ~T_56b0b8();          // 0x4844a0
    void FUN_0048e660();
    int FUN_004853d0();   // non-zero: skip the front end and go straight to FUN_00434300

private:
    char m_data[0x4a0];
};
#endif
class T_56b0bc { // input
public:
    T_56b0bc();       // 0x4b6e10
    ~T_56b0bc();      // 0x4b6f60
    void ClearKeys(); // 0x4b7390
};
class T_56b0c0 {
public:
    T_56b0c0();  // 0x4b6ac0
    ~T_56b0c0(); // 0x4b6bf0
};
// A loaded renderer plugin DLL.
struct Plugin { // 8 bytes
    List<RenderDevice*>* devices; // 0x0: returned by the DLL's BeGetDevices()
    HMODULE module;               // 0x4
};

typedef List<RenderDevice*>* (*BeGetDevicesFunc)();

class T_56b980 { // renderer plugin loader (0x28 bytes), loads device/bbdx8.dll
public:
    T_56b980();                                // 0x4bbdf0
    ~T_56b980();                               // 0x4bbe20
    int FUN_004bbf60(const char* dll);         // loads a plugin; returns the device count
    int FUN_004bbff0(DisplayAdapter* adapter); // selects the adapter
    List<RenderDevice*>* FUN_00403c90();       // all devices
    void FUN_004bc120();                       // unloads the plugins

    List<RenderDevice*> m_devices; // 0x0
    List<Plugin*> m_plugins;       // 0x14
};

// Bink video player (0xac bytes).
class BinkVideo {
public:
    BinkVideo(const char* name, RenderDevice* device, int flags); // 0x4097a0
    ~BinkVideo();                                                 // 0x40a060
    void Render(RenderDevice* device);                            // 0x40a0f0
    int Update();                                                 // 0x40a4a0: -1 done, 1 new frame

private:
    char m_data[0xac];
};

// The game itself (0x139894 bytes). The full layout is include/unk_4303e0.h (its TU defines GAME_FULL_LAYOUT).
#ifndef GAME_FULL_LAYOUT
class Game {
public:
    Game();                // 0x4303e0
    ~Game();               // 0x430760
    void FUN_004309d0();   // runs the game (menus and races)
    void FUN_00434300();   // alternative entry used when T_56b0b8::FUN_004853d0() is set

private:
    char m_data[0x139894];
};
#endif

// Full-screen image such as the loading screen (0x48 bytes).
class LoadingScreen {
public:
    LoadingScreen(RenderDevice* device, const char* image, int unk1, int unk2); // 0x4a8ae0
    ~LoadingScreen();                                    // 0x4a8bb0
    void Draw(RenderDevice* device, int unk);            // 0x4a92c0

private:
    char m_data[0x48];
};

extern char DAT_0057b9f4; /* quit requested */

int FUN_004b69f0(int wait); /* pumps window messages; 0 once the game should quit */

// Exceptions the game throws (RTTI exists only for these).
void FUN_00402c90(const char* format, ...); // empty in release; identical bodies fold

// original: FUN_00405aa0 (implicit copy constructor establishes the layout).
class file_error : public std::runtime_error {
public:
    file_error(const std::string& filename, const std::string& message)
        : std::runtime_error("File error: " + message + " " + filename),
          m_message(message), m_filename(filename)
    {
        FUN_00402c90("file error: %s %s", filename.c_str(), message.c_str());
    }

    std::string m_message;  // 0x1c
    std::string m_filename; // 0x2c; total size 0x3c
};

class assertion_error : public exception {
public:
    assertion_error(const std::string& what);
    std::string m_message;
};

// Parsed command line (0x1008 bytes).
class CommandLine {
public:
    CommandLine(const char* commandLine); // 0x4bc210
    ~CommandLine();                       // 0x4bc240
    void Parse(const char* commandLine);  // 0x4bc250
    int GetCount() const { return m_count; }
    const char* GetArg(int index) const { return m_args[index]; }

    int m_count;           // 0x0
    char* m_args[0x400];   // 0x4
    char* m_buffer;        // 0x1004: copy of the command line, split in place
};

/* winmain.cpp */
extern CommandLine* DAT_0056b0c4;
extern int DAT_0056b998;  /* "setup" on the command line: only show the setup dialog */
extern char DAT_00501471; /* "/s" */
extern char DAT_00501472; /* "/r" */
extern HICON DAT_0056b98c;
extern Array<AdapterEntry> DAT_0056b968; /* display adapters (setup dialog) */
extern HANDLE DAT_0056b960, DAT_0056b964; /* setup dialog images */
int FUN_004a2270(void);
BOOL CALLBACK FUN_004a2440(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
extern int DAT_0056b984;           /* selected adapter */
extern int DAT_0056b988;           /* selected sound device */
extern int DAT_0056b0f8;           /* setup dialog: advanced options enabled */
const char* FUN_00405330(int index);  /* language name */
// WinMain-owned subsystem objects (roles observed from their constructors and
// call sites in winmain.cpp and the subsystem headers): sound, DirectInput
// controllers, DirectPlay networking, keyboard hook, mouse hook, renderer DLL.
extern T_56b0b0* DAT_0056b0b0;
extern T_56b0b4* DAT_0056b0b4;
extern T_56b0b8* DAT_0056b0b8;
extern T_56b0bc* DAT_0056b0bc;
extern T_56b0c0* DAT_0056b0c0;
extern T_56b980* DAT_0056b980;
void FUN_004052f0(void);
void FUN_004a3060(HWND dialog);
void FUN_004a3170(HWND dialog);
void FUN_004a3a00(void);
void FUN_004a3b10(void);
void FUN_004a3ca0(const char* commandLine);
void FUN_004a3e40(void);
void FUN_004a3fe0(void);
void FUN_004a4cb0(void);
void FUN_004a7da0(void);
extern RenderDevice* DAT_0056b0ac; /* active renderer device */
extern void* DAT_0056b0fc[0x40];   /* file data loaded by FUN_004a3930 */
// Game settings, saved as a 0x98-byte file (see FUN_004a4440 / FUN_004a45c0).
#define SETTINGS_MAGIC 0x20011015
struct Settings {
    DWORD magic;       // 0x00: SETTINGS_MAGIC
    GUID adapter;      // 0x04: selected adapter
    char device[80];   // 0x14: selected sound device name
    int speakers;      // 0x64: 0 = 2 speakers, 1 = headphones, 2 = surround, 3 = 4 speakers
    int width;         // 0x68
    int height;        // 0x6c
    int bpp;           // 0x70
    int refresh;       // 0x74: refresh rate (Hz, 0 = default)
    int antialias;     // 0x78: anti-aliasing samples (0 = none)
    char unk7c;        // 0x7c: window creation flag (cleared if the mode fails)
    char unk7d;        // 0x7d
    char unk7e;        // 0x7e: set by FUN_004a45c0
    int language;      // 0x80: index into the language list
    int textureQuality; // 0x84: 0 = 16 bits, 1 = 32 bits, 2 = compressed (render state 6)
    int textureFilter;  // 0x88: 0 nearest, 1 linear, 2 anisotropic (render state 7)
    int mipFilter;      // 0x8c: 0 none, 1 nearest, 2 linear (render state 8)
    int sampleRate;     // 0x90: index into DAT_005015b0 (11/22/44 kHz)
    int voices;         // 0x94: index into DAT_00501590 (8..64 voices)
};
extern Settings DAT_0056b8c8;
extern int DAT_0056b990;
bool FUN_004a6050(const char* name); /* file exists */
int FUN_00405350(void);              /* language count */
int FUN_004053a0(int index);         /* language id (PRIMARYLANGID) */
void FUN_004a4440(const char* name);
extern LARGE_INTEGER DAT_00501530; /* performance counter frequency */
extern int DAT_0056b0ec, DAT_0056b0f0, DAT_0056b0f4;
extern int DAT_005015bc[5], DAT_005015d0[5], DAT_005015e4[5], DAT_005015f8[5], DAT_0050160c[5], DAT_00501620[5];
// Option blocks saved in the progress file (initialised data in .data).
extern int DAT_00501300[0x41];  /* player profile ("Player 1" at +0x38) */
extern int DAT_00501408, DAT_0050140c, DAT_00501410, DAT_00501414;
extern int DAT_00501418[8];     /* detail settings ([0]-[5] set by FUN_004a49b0) */
extern int DAT_00501438[6];
extern int DAT_00501478, DAT_0050147c, DAT_00501480;
extern int DAT_00501498[0x16];  /* [18]-[21] look like unlock bitmasks */

// Progress/options file (0x1c8 bytes): a snapshot of the blocks above.
#define PROGRESS_MAGIC 0x20011217
#define PROGRESS_MAGIC_OLD 0x20010918 /* 0x1bc bytes: lacks unk1c0/unk1c4 */
struct Progress {
    DWORD magic;              // 0x000
    int player[0x41];         // 0x004
    int unk108;               // 0x108
    int unk10c;               // 0x10c
    int unk110;               // 0x110
    int unk114;               // 0x114
    int detail[8];            // 0x118
    int unk138[6];            // 0x138
    unsigned char key[20];    // 0x150: product key blob
    int unk164[0x16];         // 0x164
    int unk1bc;               // 0x1bc
    int unk1c0;               // 0x1c0
    int unk1c4;               // 0x1c4
};
extern Progress DAT_0056b700;
extern int DAT_0056b994;
void FUN_004a4660(const char* name);
void FUN_004a4870(const char* name);
void FUN_004a18d0(int microseconds);
void FUN_004a3930(const char* name, int slot);
void FUN_004a45c0(const char* name, int flag);
void FUN_004a49b0(void);
int FUN_004a1fe0(HINSTANCE instance);  /* setup dialog: 1 OK, -1 no 3D device */
bool FUN_004a3390(HINSTANCE instance); /* opens the display mode and creates subsystems */
extern HWND DAT_0056b0cc;          /* game window */
extern float DAT_0056aea8, DAT_0056aeac; /* screen size relative to 1024x768 */
extern int DAT_005012e0, DAT_005012e8, DAT_005012ec;
extern int DAT_005015b0[], DAT_00501590[];
void FUN_00405090(int language);

// Key/value configuration file (e.g. data/language/language.ini), 20 bytes.
struct ConfigKey {
    const char* name;
    int type;
};

// Common 0x1c-byte header; typed storage starts at 0x1c.
// original: FUN_00409670 (constructor), FUN_004076c0 (allocation sizes).
class ConfigEntry {
public:
    ConfigEntry();
    virtual ~ConfigEntry() {}
    void SetType(int type) { m_type = type; }

    std::string m_name; // 0x4
    int m_type;         // 0x14: 1 = integer
    int m_assigned;     // 0x18: set after parsing a value
};

// Vector2/3/4 storage, confirmed by FUN_00405f50/00406390/004067e0.
struct ConfigVector2 { ~ConfigVector2() {} float values[2]; };
struct ConfigVector3 { ~ConfigVector3() {} float values[3]; };
struct ConfigVector4 { ~ConfigVector4() {} float values[4]; };

template<class T, int Type> class ConfigValue : public ConfigEntry {
public:
    ConfigValue() { SetType(Type); }
    T m_value; // 0x1c; scalar/vector storage is not initialized by Define.
};

typedef ConfigValue<std::string, 0> ConfigStringEntry;
typedef ConfigValue<int, 1> ConfigIntEntry;
typedef ConfigValue<float, 2> ConfigFloatEntry;
typedef ConfigValue<ConfigVector2, 3> ConfigVector2Entry;
typedef ConfigValue<ConfigVector3, 4> ConfigVector3Entry;
typedef ConfigValue<ConfigVector4, 5> ConfigVector4Entry;

class Config {
public:
    Config();                                                // 0x405580
    ~Config();                                               // 0x4055a0
    void Define(const ConfigKey* keys, int count);           // 0x4076c0
    int Load(const char* filename, bool requireAll, bool verifyChecksum,
             unsigned int expectedChecksum);                // 0x406ce0
    void ParseString(ConfigStringEntry* entry);            // 0x405670
    void ParseInt(ConfigIntEntry* entry);                  // 0x405b70
    void ParseFloat(ConfigFloatEntry* entry);              // 0x405d60
    void ParseVector2(ConfigVector2Entry* entry);          // 0x405f50
    void ParseVector3(ConfigVector3Entry* entry);          // 0x406390
    void ParseVector4(ConfigVector4Entry* entry);          // 0x4067e0
    int Validate();                                        // 0x4079c0
    float GetFloat(const char* key);                        // 0x407f60
    int GetInt(const char* key);                             // 0x407d50
    const char* GetString(const char* key);                  // 0x408170
    const ConfigVector2& GetVector2(const char* key);        // 0x408390
    const ConfigVector3& GetVector3(const char* key);        // 0x4085e0
    const ConfigVector4& GetVector4(const char* key);        // 0x408820
    void Close();                                            // 0x405600
    ConfigEntry* Find(const char* key);                      // 0x407c90

    std::vector<ConfigEntry*> m_entries; // 0x0
    const char* m_filename;              // 0x10: borrowed by Load for errors
};

// Original config data-integrity policy and CRC table (Load, 0x406ce0).
extern int DAT_00501454;
extern unsigned char DAT_00501473;
extern unsigned short DAT_004f2c10[256];

/* language.cpp: languages and the game's text table */
extern char* DAT_005074e0[0x1a3]; /* texts of the current language */
extern int DAT_00507b6c;          /* current language */
extern int DAT_00507b70[20];      /* language ids (PRIMARYLANGID) */
extern std::string DAT_00507bc0[20]; /* language initials */
extern std::string DAT_00507d00[20]; /* language names */
extern ConfigKey DAT_004f2598[];     /* keys of language.ini */
extern int DAT_00507e40;          /* language count */
extern int DAT_00507e44;          /* texts loaded */
extern char DAT_005070e0[0x400];  /* line buffer for loading texts */
extern int DAT_00507e7c;          /* read position within the current statement */
extern int DAT_00507e80;          /* length of the current statement */
extern char DAT_00507e84[];       /* current statement */
extern int DAT_00508284;          /* parse position */
extern int DAT_00508288;          /* parse buffer size */
extern char* DAT_0050828c;        /* parse buffer */
const char* FUN_00405360(int index);
const char* FUN_00405380(int index); /* text by index */
int FUN_00405390(void);
int FUN_004053e0(char c);
void FUN_00405410(void);
int FUN_00405440(void);
int FUN_004054f0(char* word);
unsigned int __stdcall FUN_004a7d30(void);
void FUN_00404d50(void);
extern char DAT_0056ae78[20];          /* decoded product key, "XXXX-XXXX-XXXX-XXXX" */
extern unsigned char DAT_0056b0d8[20]; /* stored product key blob (4 x {xor byte, 4 data bytes}) */
extern char DAT_0056b1fc[];            /* product key typed into the key dialog */
extern char DAT_00501774[];            /* "ERROR" */
extern char DAT_00501cd4[];            /* invalid key message */
const char* FUN_004a1820(void);
int FUN_004a4170(const char* key);     /* validates a product key */
int FUN_004a4250(HINSTANCE instance);
void FUN_004a42e0(int keyAccepted);
BOOL CALLBACK FUN_004a43b0(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

/* unk_4b7390.cpp */
extern unsigned char DAT_0057ba28[0x100]; /* key states, indexed by virtual-key code */
extern int DAT_0057ba20, DAT_0057ba24;
extern int DAT_0057bc58;
void FUN_004b73b0(int value);

// Frame timer on top of timeGetTime. The first instance raises the multimedia
// timer resolution; the last one restores it.
class Timer {
public:
    Timer();
    ~Timer();
    void Update();
    void Reset();
    static __int64 GetCycles();

    DWORD m_time;     // 0x0: ms since Reset()
    DWORD m_start;    // 0x4: timeGetTime() at Reset()
    DWORD m_delta;    // 0x8: ms between the last two Update() calls
    float m_deltaSec; // 0xc: m_delta in seconds

    static LARGE_INTEGER s_frequency; // 0x57bc70
    static BOOL s_hasPerfCounter;     // 0x57bc78
    static UINT s_period;             // 0x57bc7c
    static int s_refCount;            // 0x57bc80
};

void FUN_004b7670(WCHAR* dst, const char* src, int len);
void FUN_004b76c0(char* dst, const WCHAR* src, int len);
void FUN_004b7710(WCHAR* dst, const char* src, int len);
void FUN_004b7730(char* dst, const WCHAR* src, int len);
HRESULT FUN_004b7750(HKEY key, const char* name, const char* value); /* registry string write */

#endif
