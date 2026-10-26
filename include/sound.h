/* Sound system T_56b0b0 (0x4982c0-0x49b630): a wrapper around the Miles
 * Sound System (mss32.dll) for 2D samples, 3D samples, a music stream and a
 * queue of speech samples. Included by rallytrophy.h. */
#ifndef SOUND_H
#define SOUND_H

#include <deque>

/* Miles Sound System API (mss32.dll). No SDK headers are used; these are
 * declared from the imports the game calls. Handle types are opaque. */
typedef unsigned long HPROVIDER;
typedef unsigned long HPROENUM;
typedef struct _DIG_DRIVER* HDIGDRIVER;
typedef struct _SAMPLE* HSAMPLE;
typedef void* H3DSAMPLE;
typedef struct _STREAM* HSTREAM;

struct AILSOUNDINFO {
    int format;
    const void* data_ptr;
    unsigned long data_len;
    unsigned long rate;
    int bits;
    int channels;
    unsigned long samples;
    unsigned long block_size;
    const void* initial_ptr;
};

typedef void(__stdcall* AILSAMPLECB)(HSAMPLE sample);
typedef void(__stdcall* AIL3DSAMPLECB)(H3DSAMPLE sample);

#define MSSAPI extern "C" __declspec(dllimport)
MSSAPI char* __stdcall AIL_set_redist_directory(const char* dir);
MSSAPI int __stdcall AIL_startup(void);
MSSAPI void __stdcall AIL_shutdown(void);
MSSAPI HWND __stdcall AIL_HWND(void);
MSSAPI int __stdcall AIL_set_DirectSound_HWND(HDIGDRIVER driver, HWND window);
MSSAPI HDIGDRIVER __stdcall AIL_open_digital_driver(unsigned long rate, int bits, int channels, unsigned long flags);
MSSAPI void __stdcall AIL_close_digital_driver(HDIGDRIVER driver);
MSSAPI void __stdcall AIL_set_digital_master_volume(HDIGDRIVER driver, int volume);
MSSAPI int __stdcall AIL_enumerate_3D_providers(HPROENUM* next, HPROVIDER* provider, char** name);
MSSAPI int __stdcall AIL_open_3D_provider(HPROVIDER provider);
MSSAPI void __stdcall AIL_close_3D_provider(HPROVIDER provider);
MSSAPI void __stdcall AIL_set_3D_speaker_type(HPROVIDER provider, int type);
MSSAPI HSAMPLE __stdcall AIL_allocate_sample_handle(HDIGDRIVER driver);
MSSAPI void __stdcall AIL_release_sample_handle(HSAMPLE sample);
MSSAPI void __stdcall AIL_init_sample(HSAMPLE sample);
MSSAPI void __stdcall AIL_stop_sample(HSAMPLE sample);
MSSAPI void __stdcall AIL_start_sample(HSAMPLE sample);
MSSAPI int __stdcall AIL_sample_status(HSAMPLE sample);
MSSAPI int __stdcall AIL_set_sample_file(HSAMPLE sample, const void* data, int block);
MSSAPI void __stdcall AIL_set_sample_playback_rate(HSAMPLE sample, int rate);
MSSAPI void __stdcall AIL_set_sample_volume(HSAMPLE sample, int volume);
MSSAPI void __stdcall AIL_set_sample_pan(HSAMPLE sample, int pan);
MSSAPI void __stdcall AIL_set_sample_loop_count(HSAMPLE sample, int count);
MSSAPI void __stdcall AIL_set_sample_loop_block(HSAMPLE sample, int start, int end);
MSSAPI H3DSAMPLE __stdcall AIL_allocate_3D_sample_handle(HPROVIDER provider);
MSSAPI void __stdcall AIL_release_3D_sample_handle(H3DSAMPLE sample);
MSSAPI void __stdcall AIL_stop_3D_sample(H3DSAMPLE sample);
MSSAPI void __stdcall AIL_start_3D_sample(H3DSAMPLE sample);
MSSAPI int __stdcall AIL_set_3D_sample_file(H3DSAMPLE sample, const void* data);
MSSAPI void __stdcall AIL_set_3D_sample_playback_rate(H3DSAMPLE sample, int rate);
MSSAPI void __stdcall AIL_set_3D_sample_volume(H3DSAMPLE sample, int volume);
MSSAPI void __stdcall AIL_set_3D_sample_loop_count(H3DSAMPLE sample, int count);
MSSAPI void __stdcall AIL_set_3D_sample_loop_block(H3DSAMPLE sample, int start, int end);
MSSAPI void __stdcall AIL_set_3D_sample_distances(H3DSAMPLE sample, float maxDistance, float minDistance);
MSSAPI void __stdcall AIL_set_3D_position(H3DSAMPLE object, float x, float y, float z);
MSSAPI void __stdcall AIL_set_3D_velocity_vector(H3DSAMPLE object, float dx, float dy, float dz);
MSSAPI void __stdcall AIL_update_3D_position(H3DSAMPLE object, float dt);
MSSAPI AILSAMPLECB __stdcall AIL_register_EOS_callback(HSAMPLE sample, AILSAMPLECB callback);
MSSAPI AIL3DSAMPLECB __stdcall AIL_register_3D_EOS_callback(H3DSAMPLE sample, AIL3DSAMPLECB callback);
MSSAPI void __stdcall AIL_mem_free_lock(void* ptr);
MSSAPI int __stdcall AIL_file_type(const void* data, unsigned long size);
MSSAPI int __stdcall AIL_WAV_info(const void* data, AILSOUNDINFO* info);
MSSAPI int __stdcall AIL_decompress_ADPCM(const AILSOUNDINFO* info, void** out, unsigned long* outSize);
MSSAPI int __stdcall AIL_decompress_ASI(const void* data, unsigned long size, const char* name, void** out,
                                        unsigned long* outSize, void* callback);
MSSAPI HSTREAM __stdcall AIL_open_stream(HDIGDRIVER driver, const char* name, int memory);
MSSAPI void __stdcall AIL_close_stream(HSTREAM stream);
MSSAPI void __stdcall AIL_start_stream(HSTREAM stream);
MSSAPI void __stdcall AIL_set_stream_volume(HSTREAM stream, int volume);
MSSAPI void __stdcall AIL_set_stream_loop_count(HSTREAM stream, int count);
#undef MSSAPI

/* RAD Bink (binkw32.dll): routes Bink audio through the Miles driver. */
extern "C" __declspec(dllimport) void* __stdcall BinkOpenMiles(unsigned long param);
extern "C" __declspec(dllimport) int __stdcall BinkSetSoundSystem(void* open, unsigned long param);

void FUN_004a4ae0();
extern int DAT_00500478[]; // Miles speaker type per Settings::speakers
struct SoundSource;
extern int DAT_00501440;   // speech volume, percent
extern float DAT_00500438[]; // priority bias per SoundSource::priorityClass
extern int DAT_00569e70;     // number of sources in DAT_00569e74
extern SoundSource* DAT_00569e74[]; // audible 3D sources sorted by priority
extern char* DAT_00500328[]; // speech file names, "data/speech/left1_eng.wav", ...

// A 3D sound provider offered by Miles (8 bytes; m_devices[0] is "No Sound").
struct DeviceInfo {
    char* name;         // 0x0
    HPROVIDER provider; // 0x4
};

struct Vector3 { // 0xc bytes
    ~Vector3() {}
    float x, y, z;
};

struct Matrix { // 4x4, 0x40 bytes
    ~Matrix() {}

    float m[4][4];
};

// A loaded sample, shared by reference count (0x2c bytes).
// original: FUN_00498ec0 (allocation, fields), FUN_00499340 (release)
struct Sample {
    ~Sample() {}
    int rate;         // 0x0
    int length;       // 0x4
    int bits;         // 0x8
    int channels;     // 0xc
    void* data;       // 0x10
    int refs;         // 0x14
    bool milesMemory; // 0x18: data was allocated by Miles (decompressed)
    std::string name; // 0x1c
};

struct SoundSource;

// Changed-parameter bits of a sound source (4 bytes).
// original: FUN_00499a10 copies the word and tests 0x100/0x8/0x20/0x80.
struct SoundFlags {
    unsigned unk0 : 2;
    unsigned changed : 3;     // bits 2-4: 2D parameters (0x8 = rate)
    unsigned changed3D : 3;   // bits 5-7: 3D parameters (0x20 = distances, 0x80 = velocity)
    unsigned changedFile : 1; // bit 8: sample changed
};

// One Miles sample handle (0x14 bytes).
struct Voice {
    HSAMPLE handle;      // 0x0 (H3DSAMPLE for 3D voices)
    int unk4;            // 0x4
    int active;          // 0x8
    SoundSource* owner;  // 0xc
    Sample* sample;      // 0x10: sample last bound to the handle
};

// A playable sound instance (0x70 bytes or more).
struct SoundSource {
    SoundSource();  // 0x49b630 (SEH wrapper around Reset)
    ~SoundSource(); // 0x49b6e0
    void Reset();   // 0x49b670
    void Play();    // 0x49b730
    void Stop();    // 0x49b760
    void SetSample(Sample* s);               // 0x49b790
    void SetVolume(int v);                   // 0x49b7b0
    void SetRate(int r);                     // 0x49b820
    void SetRange(float nearDist, float farDist); // 0x49b840
    void SetPosition(const Vector3* p);      // 0x49b880
    float GetPriority() const { return priority; }
    Sample* sample;   // 0x0
    int voice;        // 0x4: index into the voice list, or -1
    bool registered;  // 0x8: in the sound system's 2D or 3D source list
    bool is3D;        // 0x9
    bool loop;        // 0xa
    int loopStart;    // 0xc
    int loopEnd;      // 0x10
    int volume;       // 0x14
    int rate;         // 0x18: 0 = the sample's rate
    int pan;          // 0x1c
    float rolloff;    // 0x20: min distance grows with listener distance
    float minDistance; // 0x24: current Miles min distance
    float maxDistance; // 0x28: current Miles max distance
    float baseMinDistance; // 0x2c
    float baseMaxDistance; // 0x30
    float nearDistance; // 0x34
    float farDistance;  // 0x38: inaudible beyond this
    Vector3 position;   // 0x3c
    Vector3 velocity;   // 0x48
    int unk54;          // 0x54
    unsigned priorityClass; // 0x58: low 4 bits index the priority bias table
    SoundFlags flags; // 0x5c
    int playing;      // 0x60
    int culled;       // 0x64
    int state;        // 0x68
    float priority;   // 0x6c
};

struct SpeechItem { // queued speech sample (8 bytes)
    Sample* sample; // 0x0
    int* done;      // 0x4: set to 1 when the sample starts
};

// Linked list with inline AddTail; its destructor removes nodes through the
// out-of-line RemoveNode (0x414490). The plain List<T> template frees nodes
// with inline code instead, so this is kept as a separate template.
// original: FUN_00498ec0 (inline AddTail), FUN_00498650 (destructor)
template <class T>
class ObjectList {
public:
    struct Node {
        Node() { next = prev = 0; data = 0; }
        ~Node() {}
        Node* next; // 0x0
        Node* prev; // 0x4
        T data;     // 0x8
    };

    ObjectList()
    {
        m_head = m_tail = m_free = 0;
        m_count = 0;
        m_freeCount = 0;
    }

    ~ObjectList()
    {
        while (m_head) {
            RemoveNode(m_head);
        }
        Node* node = m_free;
        while (node) {
            Node* next = node->next;
            delete node;
            node = next;
        }
        m_free = 0;
    }

    void AddTail(T item)
    {
        Node* node;
        if (!m_free) {
            node = new Node;
        } else {
            node = m_free;
            m_free = node->next;
            m_freeCount--;
        }
        node->data = item;
        node->prev = m_tail;
        node->next = 0;
        if (m_tail) {
            m_tail->next = node;
        } else {
            m_head = node;
        }
        m_tail = node;
        m_count++;
    }

    Node* Find(T item)
    {
        for (Node* node = m_head; node; node = node->next) {
            if (node->data == item) {
                return node;
            }
        }
        return 0;
    }

    void RemoveNode(Node* node); // out of line (0x414490 for pointer types)

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

    Node* m_head;    // 0x0
    Node* m_tail;    // 0x4
    Node* m_free;    // 0x8
    int m_count;     // 0xc
    int m_freeCount; // 0x10
};

class T_56b0b0 { // 0x2ac bytes; sound system
public:
    T_56b0b0();  // 0x4983a0
    ~T_56b0b0(); // 0x498650

    void Startup();  // 0x498860
    void Shutdown(); // 0x498950
    // Opens sound output; returns the device actually used.
    DeviceInfo* FUN_004989c0(int rate, int bits, int channels, int voices, int voices3D, DeviceInfo* wanted, int speakers);
    void AllocateVoices(); // 0x498b20
    void ReleaseVoices();  // 0x498e10
    Sample* LoadSample(const char* name); // 0x498ec0
    Sample* ReleaseSample(Sample* sample); // 0x499340
    void OnSampleEnd(HSAMPLE handle);      // 0x499520
    void On3DSampleEnd(H3DSAMPLE handle);  // 0x499570
    void SetVolume(int volume);            // 0x4995b0
    void Play(SoundSource* source);        // 0x4995d0
    void Stop(SoundSource* source);        // 0x499720
    void Play3D(SoundSource* source);      // 0x499770
    void Stop3D(SoundSource* source);      // 0x4999c0
    void Update3D(SoundSource* source);    // 0x499a10
    void AddSource(SoundSource* source);   // 0x499ce0
    void RemoveSource(SoundSource* source); // 0x499ec0
    void Update();                         // 0x499f80
    void SetListener(const Matrix& matrix); // 0x49a4a0
    void SetListenerPosition(const Vector3& position); // 0x49a4c0
    void OpenStream(const char* name);     // 0x49a4f0
    void CloseStream();                    // 0x49a550
    void StartStream();                    // 0x49a580
    void SetStreamVolume(int volume);      // 0x49a5a0
    void LoadSpeech(int index);            // 0x49a5e0
    void ReleaseSpeech(int index);         // 0x49a650
    void QueueSpeech(int index, int priority, int* done); // 0x49a680
    void StopSpeech();                     // 0x49aa00
    void PlayNextSpeech(int lock);         // 0x49ab20

    std::vector<DeviceInfo> m_devices;   // 0x0
    int m_default;                       // 0x10: default device index ("Miles Fast 2D")
    HPROVIDER m_provider;                // 0x14: open 3D provider
    int m_voices3DCount;                 // 0x18
    Matrix m_listener;                   // 0x1c
    Matrix m_view;                       // 0x5c: inverse of m_listener
    Vector3 m_position;                  // 0x9c
    Vector3 m_velocity;                  // 0xa8
    int m_speakers;                      // 0xb4
    int m_rate;                          // 0xb8
    int m_bits;                          // 0xbc
    int m_channels;                      // 0xc0
    int m_voicesCount;                   // 0xc4
    HDIGDRIVER m_driver;                 // 0xc8
    bool m_paused;                       // 0xcc
    bool m_disabled;                     // 0xcd: no sound output
    std::vector<Voice> m_voices;         // 0xd0: 2D voices; [0] plays speech
    std::vector<Voice> m_voices3D;       // 0xe0
    ObjectList<Sample*> m_samples;       // 0xf0
    ObjectList<SoundSource*> m_sources3D; // 0x104
    ObjectList<SoundSource*> m_sources;  // 0x118
    ObjectList<SoundSource*> m_audible;  // 0x12c: 3D sources considered this update
    HSTREAM m_stream;                    // 0x140
    int m_streamVolume;                  // 0x144
    Sample* m_speech[0x44];              // 0x148
    int m_speechPriority;                // 0x258
    std::deque<SpeechItem> m_speechQueue; // 0x25c
    int m_volume;                        // 0x28c
    int m_volumeDirty;                   // 0x290
    CRITICAL_SECTION m_lock;             // 0x294
};

#endif
