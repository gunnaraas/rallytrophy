// Sound system T_56b0b0 (0x4982c0-0x49b630): Miles Sound System wrapper.
#include "rallytrophy.h"
#include <math.h>

// Transforms *point by *matrix (row vectors, translation in row 3) into *result.
#define TRANSFORM_POINT() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fadd dword ptr [edx+48] \
    __asm fxch st(1) \
    __asm fadd dword ptr [edx+52] \
    __asm fxch st(2) \
    __asm fadd dword ptr [edx+56] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]

// Transposes the 3x3 rotation part. original: FUN_0049b570
// (These helpers live here rather than as Matrix members in sound.h: adding
// members to Matrix changes the setup dialog's codegen in winmain.cpp.)
static inline void Transpose3x3(Matrix& matrix)
{
    float t = matrix.m[0][1];
    matrix.m[0][1] = matrix.m[1][0];
    matrix.m[1][0] = t;
    t = matrix.m[0][2];
    matrix.m[0][2] = matrix.m[2][0];
    matrix.m[2][0] = t;
    t = matrix.m[1][2];
    matrix.m[1][2] = matrix.m[2][1];
    matrix.m[2][1] = t;
}

// Inverts a rotation + translation matrix in place (inlined in Update).
static inline void InvertRigid(Matrix& matrix)
{
    float y = -(matrix.m[1][2] * matrix.m[3][2] + matrix.m[1][1] * matrix.m[3][1] + matrix.m[1][0] * matrix.m[3][0]);
    float z = -(matrix.m[2][2] * matrix.m[3][2] + matrix.m[2][1] * matrix.m[3][1] + matrix.m[2][0] * matrix.m[3][0]);
    float x = -(matrix.m[0][2] * matrix.m[3][2] + matrix.m[0][1] * matrix.m[3][1] + matrix.m[0][0] * matrix.m[3][0]);
    matrix.m[3][0] = x;
    matrix.m[3][1] = y;
    matrix.m[3][2] = z;
    Transpose3x3(matrix);
}

// FUNCTION: RALLYTROPHY 0x498360
// qsort comparator: orders 3D sources by descending priority.
int ComparePriority(const void* a, const void* b)
{
    SoundSource* first = *(SoundSource**)a;
    SoundSource* second = *(SoundSource**)b;
    int result = 0;
    if (first->GetPriority() > second->GetPriority()) {
        result = -1;
    } else if (first->GetPriority() < second->GetPriority()) {
        result = 1;
    }
    return result;
}

// Behavior reference for T_56b0b0::T_56b0b0 (exact compiler output is in sound_ctor.cpp).
T_56b0b0::T_56b0b0()
{
    m_provider = 0;
    m_driver = 0;
    m_paused = false;
    m_disabled = true;
    m_stream = 0;
    m_streamVolume = 100;
    memset(m_speech, 0, sizeof(m_speech));
    m_speechPriority = -1;
    m_listener.m[0][1] = m_listener.m[0][2] = m_listener.m[0][3] = 0.0f;
    m_listener.m[0][0] = 1.0f;
    m_position.x = m_position.y = m_position.z = 0.0f;
    m_listener.m[1][0] = m_listener.m[1][2] = m_listener.m[1][3] = 0.0f;
    m_listener.m[2][0] = m_listener.m[2][1] = m_listener.m[2][3] = 0.0f;
    m_listener.m[3][0] = m_listener.m[3][1] = m_listener.m[3][2] = 0.0f;
    m_listener.m[1][1] = m_listener.m[2][2] = m_listener.m[3][3] = 1.0f;
    m_velocity.x = m_velocity.y = m_velocity.z = 0.0f;
    Startup();
    InitializeCriticalSection(&m_lock);
    m_volume = 0x7f;
    m_volumeDirty = 1;
}

// Behavior reference for T_56b0b0::~T_56b0b0 (exact machine code is in sound_dtor.cpp).
T_56b0b0::~T_56b0b0()
{
    Shutdown();
    DeleteCriticalSection(&m_lock);
}

// FUNCTION: RALLYTROPHY 0x498860
// Starts Miles and lists the 3D providers after a leading "No Sound" entry.
void T_56b0b0::Startup()
{
    AIL_set_redist_directory("SndDrivers");
    FUN_004a4ae0();
    AIL_startup();
    m_devices.resize(0);
    m_default = 0;
    HPROENUM next = 0;
    DeviceInfo info;
    info.provider = 0;
    info.name = "No Sound";
    m_devices.push_back(info);
    int i = 1;
    while (AIL_enumerate_3D_providers(&next, &info.provider, &info.name)) {
        m_devices.push_back(info);
        if (strstr(info.name, "Miles Fast 2D")) {
            m_default = i;
        }
        i++;
    }
}

// FUNCTION: RALLYTROPHY 0x498950
void T_56b0b0::Shutdown()
{
    CloseStream();
    ReleaseVoices();
    while (m_samples.m_head) {
        ReleaseSample(m_samples.m_head->data);
    }
    if (m_provider) {
        AIL_close_3D_provider(m_provider);
        m_provider = 0;
    }
    if (m_driver) {
        AIL_close_digital_driver(m_driver);
        m_driver = 0;
    }
    AIL_shutdown();
}

// FUNCTION: RALLYTROPHY 0x4989c0
// Opens the digital driver and the wanted 3D provider (falling back to the
// default one). Returns the provider used, or 0 if there is no sound output.
DeviceInfo* T_56b0b0::FUN_004989c0(int rate, int bits, int channels, int voices, int voices3D, DeviceInfo* wanted, int speakers)
{
    m_rate = rate;
    m_bits = bits;
    m_channels = channels;
    m_voicesCount = voices;
    m_voices3DCount = voices3D;
    m_speakers = speakers;
    if (wanted && strcmp(wanted->name, "No Sound") == 0) {
        m_disabled = true;
        return wanted;
    }
    m_disabled = false;
    m_driver = AIL_open_digital_driver(rate, bits, channels, 0);
    if (m_driver) {
        BinkSetSoundSystem(BinkOpenMiles, (unsigned long)m_driver);
        if (AIL_HWND() != DAT_0056b0cc) {
            AIL_set_DirectSound_HWND(m_driver, DAT_0056b0cc);
        }
        if (wanted) {
            HPROVIDER provider = wanted->provider;
            if (AIL_open_3D_provider(provider) == 0) {
                m_provider = provider;
            }
        }
        if (!m_provider) {
            if (!m_devices.empty()) {
                wanted = &m_devices[m_default];
            }
            HPROVIDER provider = wanted->provider;
            if (AIL_open_3D_provider(provider) == 0) {
                m_provider = provider;
            } else {
                wanted = 0;
            }
        }
        if (m_provider) {
            AIL_set_3D_speaker_type(m_provider, DAT_00500478[speakers]);
            AllocateVoices();
            return wanted;
        }
    }
    m_disabled = true;
    return 0;
}

static void __stdcall OnSampleEndCallback(HSAMPLE handle);
static void __stdcall On3DSampleEndCallback(H3DSAMPLE handle);

// FUNCTION: RALLYTROPHY 0x498b20
// original: FUN_00498b20
// Volatile field clears preserve their order before each voice-count test.
// Allocates up to the requested number of 2D and 3D sample handles and keeps
// the counts actually obtained.
void T_56b0b0::AllocateVoices()
{
    int i;
    if (m_driver) {
        Voice voice;
        *(volatile int*)&voice.owner = 0;
        *(volatile int*)&voice.sample = 0;
        *(volatile int*)&voice.unk4 = 0;
        *(volatile int*)&voice.active = 0;
        for (i = 0; i < m_voicesCount; i++) {
            HSAMPLE handle = AIL_allocate_sample_handle(m_driver);
            if (!handle) {
                break;
            }
            voice.handle = handle;
            m_voices.push_back(voice);
            AIL_init_sample(handle);
            AIL_register_EOS_callback(handle, OnSampleEndCallback);
        }
        m_voicesCount = i;
    }
    if (m_provider) {
        Voice voice;
        *(volatile int*)&voice.owner = 0;
        *(volatile int*)&voice.sample = 0;
        *(volatile int*)&voice.unk4 = 0;
        *(volatile int*)&voice.active = 0;
        for (i = 0; i < m_voices3DCount; i++) {
            H3DSAMPLE handle = AIL_allocate_3D_sample_handle(m_provider);
            if (!handle) {
                break;
            }
            voice.handle = (HSAMPLE)handle;
            m_voices3D.push_back(voice);
            AIL_register_3D_EOS_callback(handle, On3DSampleEndCallback);
        }
        m_voices3DCount = i;
    }
}

// FUNCTION: RALLYTROPHY 0x498e10
void T_56b0b0::ReleaseVoices()
{
    unsigned i;
    for (i = 0; i < m_voices.size(); i++) {
        AIL_stop_sample(m_voices[i].handle);
        AIL_release_sample_handle(m_voices[i].handle);
    }
    for (i = 0; i < m_voices3D.size(); i++) {
        AIL_stop_3D_sample((H3DSAMPLE)m_voices3D[i].handle);
        AIL_release_3D_sample_handle((H3DSAMPLE)m_voices3D[i].handle);
    }
}

// Behavior reference for T_56b0b0::LoadSample (exact machine code is in sound_load_sample.cpp).
// Returns the loaded sample with this name (adding a reference), or loads it.
// ADPCM and MPEG layer 3 files are decompressed by Miles into Miles memory.
Sample* T_56b0b0::LoadSample(const char* name)
{
    for (ObjectList<Sample*>::Node* node = m_samples.m_head; node; node = node->next) {
        Sample* sample = node->data;
        if (_stricmp(sample->name.c_str(), name) == 0) {
            sample->refs++;
            return sample;
        }
    }
    Sample* sample = new Sample;
    sample->milesMemory = false;
    sample->name.assign(name);
    sample->refs = 1;
    if (!File::Exists(name)) {
        std::string message;
        message.assign("sample file not found ");
        message += name;
        throw std::runtime_error(message);
    }
    File file(name, 0x81);
    int size = file.GetSize();
    void* data = operator new(size);
    file.Read(data, size);
    file.Close();
    AILSOUNDINFO info;
    void* decompressed;
    switch (AIL_file_type(data, size)) {
    case 1:
        break;
    case 13:
        AIL_decompress_ASI(data, size, name, &decompressed, 0, 0);
        operator delete(data);
        data = decompressed;
        sample->milesMemory = true;
        break;
    case 2:
        AIL_WAV_info(data, &info);
        AIL_decompress_ADPCM(&info, &decompressed, 0);
        operator delete(data);
        data = decompressed;
        sample->milesMemory = true;
        break;
    default:
        throw std::runtime_error("unknown audio format");
    }
    AIL_WAV_info(data, &info);
    sample->rate = info.rate;
    sample->length = info.data_len;
    sample->bits = info.bits;
    sample->channels = info.channels;
    sample->data = data;
    m_samples.AddTail(sample);
    return sample;
}

// Behavior reference for T_56b0b0::ReleaseSample (exact machine code is in sound_release_sample.cpp).
// Drops one reference; frees the sample when none remain. Returns the sample
// if it is still referenced, otherwise 0.
Sample* T_56b0b0::ReleaseSample(Sample* sample)
{
    if (!sample) {
        return 0;
    }
    if (--sample->refs > 0) {
        return sample;
    }
    if (sample->milesMemory) {
        AIL_mem_free_lock(sample->data);
    } else {
        operator delete(sample->data);
    }
    unsigned i;
    for (i = 0; i < m_voices.size(); i++) {
        if (m_voices[i].sample == sample) {
            m_voices[i].sample = 0;
        }
    }
    for (i = 0; i < m_voices3D.size(); i++) {
        if (m_voices3D[i].sample == sample) {
            m_voices3D[i].sample = 0;
        }
    }
    ObjectList<Sample*>::Node* node = m_samples.Find(sample);
    if (node) {
        delete node->data;
        m_samples.Remove(node);
    }
    return 0;
}

// FUNCTION: RALLYTROPHY 0x4994e0
// Miles end-of-sample callback for 2D voices.
static void __stdcall OnSampleEndCallback(HSAMPLE handle)
{
    if (DAT_0056b0b0) {
        DAT_0056b0b0->OnSampleEnd(handle);
    }
}

// FUNCTION: RALLYTROPHY 0x499500
// Miles end-of-sample callback for 3D voices.
static void __stdcall On3DSampleEndCallback(H3DSAMPLE handle)
{
    if (DAT_0056b0b0) {
        DAT_0056b0b0->On3DSampleEnd(handle);
    }
}

// FUNCTION: RALLYTROPHY 0x499520
// Voice 0 plays queued speech: when it ends, the next item starts.
void T_56b0b0::OnSampleEnd(HSAMPLE handle)
{
    int i = 0;
    for (std::vector<Voice>::iterator voice = m_voices.begin(); voice != m_voices.end(); voice++, i++) {
        if (voice->handle == handle) {
            voice->active = 0;
            if (voice->owner) {
                voice->owner->playing = 0;
            }
            break;
        }
    }
    if (i == 0) {
        PlayNextSpeech(1);
    }
}

// FUNCTION: RALLYTROPHY 0x499570
void T_56b0b0::On3DSampleEnd(H3DSAMPLE handle)
{
    for (std::vector<Voice>::iterator voice = m_voices3D.begin(); voice != m_voices3D.end(); voice++) {
        if (voice->handle == (HSAMPLE)handle) {
            voice->active = 0;
            if (voice->owner) {
                voice->owner->playing = 0;
            }
            break;
        }
    }
}

// FUNCTION: RALLYTROPHY 0x4995b0
void T_56b0b0::SetVolume(int volume)
{
    m_volume = volume;
    m_volumeDirty = 1;
}

// Behavior reference for T_56b0b0::Play (exact machine code is in sound_play.cpp).
// Plays a 2D source on its voice, or on a free voice other than the speech
// voice 0 (preferring one that already holds the source's sample).
void T_56b0b0::Play(SoundSource* source)
{
    if (m_paused || m_disabled) {
        return;
    }
    int index = source->voice;
    if (index == -1) {
        for (unsigned i = 1; i < m_voices.size(); i++) {
            if (m_voices[i].sample == source->sample && !m_voices[i].active) {
                index = i;
                break;
            }
            if (index == -1 && !m_voices[i].active) {
                index = i;
            }
        }
        if (index == -1) {
            return;
        }
    }
    Voice* voice = &m_voices[index];
    if (voice->owner && voice->owner != source) {
        voice->owner->voice = -1;
    }
    source->voice = index;
    voice->owner = source;
    if (voice->sample != source->sample) {
        voice->sample = source->sample;
        AIL_set_sample_file(voice->handle, source->sample->data, 0);
    }
    int rate = source->rate;
    if (!rate) {
        rate = source->sample->rate;
    }
    int loops = source->loop ? 0 : 1;
    AIL_set_sample_playback_rate(voice->handle, rate);
    AIL_set_sample_volume(voice->handle, source->volume);
    AIL_set_sample_pan(voice->handle, source->pan);
    HSAMPLE handle = voice->handle;
    AIL_set_sample_loop_count(handle, loops);
    AIL_set_sample_loop_block(handle, source->loopStart, source->loopEnd);
    source->flags.changed = 0;
    source->playing = 1;
    voice->active = 1;
    AIL_start_sample(voice->handle);
}

// FUNCTION: RALLYTROPHY 0x499720
void T_56b0b0::Stop(SoundSource* source)
{
    if (!source || m_disabled) {
        return;
    }
    if (source->voice != -1) {
        if (m_voices[source->voice].active) {
            m_voices[source->voice].active = 0;
            AIL_stop_sample(m_voices[source->voice].handle);
        }
    }
    source->playing = 0;
}

// Behavior reference for T_56b0b0::Play3D (exact machine code is in sound_play3d.cpp).
// Plays a 3D source on its voice or on a free 3D voice, positioned in
// listener space (m_view) by an inline x87 transform.
void T_56b0b0::Play3D(SoundSource* source)
{
    if (m_paused || m_disabled) {
        return;
    }
    int index = source->voice;
    if (index == -1) {
        for (unsigned i = 0; i < m_voices3D.size(); i++) {
            if (m_voices3D[i].sample == source->sample && !m_voices3D[i].active) {
                index = i;
                break;
            }
            if (index == -1 && !m_voices3D[i].active) {
                index = i;
            }
        }
        if (index == -1) {
            return;
        }
    }
    Voice* voice = &m_voices3D[index];
    if (voice->owner && voice->owner != source) {
        voice->owner->voice = -1;
    }
    source->voice = index;
    voice->owner = source;
    if (voice->sample != source->sample) {
        voice->sample = source->sample;
        AIL_set_3D_sample_file((H3DSAMPLE)voice->handle, source->sample->data);
    }
    int rate = source->rate;
    int loops = 1;
    if (!rate) {
        rate = source->sample->rate;
    }
    if (source->loop) {
        loops = 0;
    }
    Vector3 position;
    const Matrix* matrix = &m_view;
    const Vector3* point = &source->position;
    Vector3* result = &position;
    TRANSFORM_POINT();
    AIL_set_3D_sample_playback_rate((H3DSAMPLE)voice->handle, rate);
    H3DSAMPLE handle = (H3DSAMPLE)voice->handle;
    AIL_set_3D_sample_volume(handle, 127);
    AIL_set_3D_sample_loop_count(handle, loops);
    AIL_set_3D_sample_loop_block(handle, source->loopStart, source->loopEnd);
    AIL_set_3D_sample_distances((H3DSAMPLE)voice->handle, source->maxDistance, source->minDistance);
    handle = (H3DSAMPLE)voice->handle;
    AIL_set_3D_position(handle, position.x, position.y, position.z);
    AIL_set_3D_velocity_vector(handle, source->velocity.x * 0.001f, source->velocity.y * 0.001f,
                               source->velocity.z * 0.001f);
    source->flags.changed = 0;
    source->flags.changed3D = 0;
    source->flags.changedFile = 0;
    source->playing = 1;
    source->culled = 0;
    voice->active = 1;
    AIL_start_3D_sample((H3DSAMPLE)voice->handle);
}

// FUNCTION: RALLYTROPHY 0x4999c0
void T_56b0b0::Stop3D(SoundSource* source)
{
    if (m_disabled) {
        return;
    }
    if (source->voice != -1) {
        if (m_voices3D[source->voice].active) {
            m_voices3D[source->voice].active = 0;
            AIL_stop_3D_sample((H3DSAMPLE)m_voices3D[source->voice].handle);
        }
    }
    source->playing = 0;
    source->culled = 0;
}

// FUNCTION: RALLYTROPHY 0x499a10
// original: FUN_00499a10
// Pushes changed parameters of a playing 3D source to its voice.
void T_56b0b0::Update3D(SoundSource* source)
{
    if (m_disabled) {
        return;
    }
    if (source->voice != -1) {
        Voice* voice = &m_voices3D[source->voice];
        if (voice->active) {
            SoundFlags flags = source->flags;
            if (flags.changedFile) {
                AIL_set_3D_sample_file((H3DSAMPLE)voice->handle, source->sample->data);
                int rate = source->rate;
                int loops = 1;
                if (!rate) {
                    rate = source->sample->rate;
                }
                if (source->loop) {
                    loops = 0;
                }
                // The transformed position is computed but not used here.
                Vector3 unused;
                const Matrix* matrix = &m_view;
                const Vector3* point = &source->position;
                Vector3* result = &unused;
                TRANSFORM_POINT();
                AIL_set_3D_sample_playback_rate((H3DSAMPLE)voice->handle, rate);
                H3DSAMPLE handle = (H3DSAMPLE)voice->handle;
                AIL_set_3D_sample_volume(handle, m_volume);
                AIL_set_3D_sample_loop_count(handle, loops);
                AIL_set_3D_sample_loop_block(handle, source->loopStart, source->loopEnd);
                AIL_set_3D_sample_distances((H3DSAMPLE)voice->handle, source->maxDistance, source->minDistance);
                AIL_set_3D_velocity_vector((H3DSAMPLE)voice->handle, source->velocity.x * 0.001f,
                                           source->velocity.y * 0.001f, source->velocity.z * 0.001f);
                AIL_start_3D_sample((H3DSAMPLE)voice->handle);
                *(volatile int*)&source->playing = 1;
            }
            if (m_volumeDirty) {
                AIL_set_3D_sample_volume((H3DSAMPLE)voice->handle, m_volume);
            }
            if (flags.changed & 2) {
                int rate = source->rate;
                if (!rate) {
                    rate = voice->sample->rate;
                }
                AIL_set_3D_sample_playback_rate((H3DSAMPLE)voice->handle, rate);
            }
            Vector3 position;
            const Matrix* matrix = &m_view;
            const Vector3* point = &source->position;
            Vector3* result = &position;
            TRANSFORM_POINT();
            H3DSAMPLE handle = (H3DSAMPLE)voice->handle;
            AIL_set_3D_position(handle, position.x, position.y, position.z);
            if (flags.changed3D & 1) {
                AIL_set_3D_sample_distances(handle, source->maxDistance, source->minDistance);
            }
            if (flags.changed3D & 4) {
                AIL_set_3D_velocity_vector((H3DSAMPLE)voice->handle, source->velocity.x * 0.001f,
                                           source->velocity.y * 0.001f, source->velocity.z * 0.001f);
            }
            AIL_update_3D_position((H3DSAMPLE)voice->handle, 0.0f);
        }
    }
    __asm {
        mov ecx, dword ptr [esi + 0x5c]
        and ecx, 0xfffffeff
        mov dword ptr [esi + 0x5c], ecx
        mov edx, dword ptr [esi + 0x5c]
        and edx, 0xffffffe3
        mov dword ptr [esi + 0x5c], edx
        mov eax, dword ptr [esi + 0x5c]
        and eax, 0xffffff1f
        mov dword ptr [esi + 0x5c], eax
    }
}

// FUNCTION: RALLYTROPHY 0x499ce0
// Registers a source for updates; a 3D source may only be added once.
void T_56b0b0::AddSource(SoundSource* source)
{
    if (source->registered) {
        return;
    }
    source->registered = true;
    if (source->is3D) {
        if (m_sources3D.Find(source)) {
            throw std::runtime_error("sound exists");
        }
        m_sources3D.AddTail(source);
    } else {
        m_sources.AddTail(source);
    }
}

// FUNCTION: RALLYTROPHY 0x499ec0
void T_56b0b0::RemoveSource(SoundSource* source)
{
    if (!source) {
        return;
    }
    if (source->registered) {
        source->registered = false;
        if (source->is3D) {
            Stop3D(source);
            ObjectList<SoundSource*>::Node* node = m_sources3D.Find(source);
            if (node) {
                m_sources3D.RemoveNode(node);
            }
        } else {
            Stop(source);
            ObjectList<SoundSource*>::Node* node = m_sources.Find(source);
            if (node) {
                m_sources.RemoveNode(node);
            }
        }
    }
    if (source->voice != -1) {
        if (source->is3D) {
            m_voices3D[source->voice].owner = 0;
            m_voices3D[source->voice].sample = 0;
        } else {
            m_voices[source->voice].owner = 0;
            m_voices[source->voice].sample = 0;
        }
    }
    source->voice = -1;
}

// Behavior reference for T_56b0b0::Update (exact machine code is in sound_update.cpp).
// Per-frame update: master volume, speech queue, listener view matrix, then
// 3D source culling. With more audible sources than 3D voices, sources are
// ranked by distance-based priority and only the best ones get voices.
void T_56b0b0::Update()
{
    if (m_disabled) {
        return;
    }
    if (m_volumeDirty) {
        AIL_set_digital_master_volume(m_driver, m_volume);
    }
    PlayNextSpeech(1);
    m_view = m_listener;
    m_view.m[3][2] = m_position.z;
    m_view.m[3][1] = m_position.y;
    m_view.m[3][0] = m_position.x;
    InvertRigid(m_view);
    while (m_audible.m_head) {
        m_audible.RemoveNode(m_audible.m_head);
    }
    ObjectList<SoundSource*>::Node* node;
    for (node = m_sources3D.m_head; node; node = node->next) {
        SoundSource* source = node->data;
        if (source->state == 1 || source->playing || source->culled) {
            m_audible.AddTail(source);
        }
        if (source->state == 2) {
            Stop3D(source);
            source->state = 0;
        }
    }
    if (m_paused) {
        return;
    }
    if (m_audible.m_count > m_voices3DCount) {
        DAT_00569e70 = m_audible.m_count;
        SoundSource** sorted = DAT_00569e74;
        for (node = m_audible.m_head; node; node = node->next) {
            SoundSource* source = node->data;
            float gain = 1.0f;
            Vector3 position;
            const Matrix* matrix = &m_view;
            const Vector3* point = &source->position;
            Vector3* result = &position;
            TRANSFORM_POINT();
            float oldMin = source->minDistance;
            source->minDistance = source->baseMinDistance;
            source->maxDistance = source->baseMaxDistance;
            float nearDistance = source->nearDistance;
            float distance = (float)sqrt(position.x * position.x + position.y * position.y + position.z * position.z) + 0.001f;
            if (distance <= nearDistance) {
                if (source->volume != 127) {
                    source->minDistance = distance * source->rolloff + 0.001f;
                }
            } else if (source->farDistance < distance) {
                gain = 0.0f;
            } else {
                gain = nearDistance / distance;
            }
            if (source->minDistance != oldMin) {
                source->flags.changed3D |= 1;
            }
            *sorted++ = source;
            source->priority = gain + DAT_00500438[source->priorityClass & 0xf];
        }
        qsort(DAT_00569e74, DAT_00569e70, sizeof(SoundSource*), ComparePriority);
        int i;
        for (i = m_voices3DCount; i < DAT_00569e70; i++) {
            SoundSource* source = DAT_00569e74[i];
            Stop3D(source);
            if (source->loop) {
                source->culled = 1;
            }
        }
        for (i = 0; i < m_voices3DCount; i++) {
            SoundSource* source = DAT_00569e74[i];
            if (source->state == 1 || source->culled) {
                Play3D(source);
                source->state = 0;
            } else {
                Update3D(source);
            }
        }
    } else {
        for (node = m_audible.m_head; node; node = node->next) {
            SoundSource* source = node->data;
            Vector3 position;
            const Matrix* matrix = &m_view;
            const Vector3* point = &source->position;
            Vector3* result = &position;
            TRANSFORM_POINT();
            float distanceSq = position.x * position.x + position.y * position.y + position.z * position.z;
            if (distanceSq <= source->farDistance * source->farDistance) {
                float oldMin = source->minDistance;
                source->maxDistance = source->baseMaxDistance;
                source->minDistance = source->baseMinDistance;
                if (distanceSq < source->nearDistance * source->nearDistance && source->volume != 127) {
                    source->minDistance = (float)sqrt(distanceSq) * source->rolloff + 0.001f;
                }
                if (oldMin != source->minDistance) {
                    source->flags.changed3D |= 1;
                }
                if (source->state == 1 || source->culled) {
                    Play3D(source);
                    source->state = 0;
                } else {
                    Update3D(source);
                }
            } else {
                if (!source->culled) {
                    Stop3D(source);
                }
                if (source->loop) {
                    source->culled = 1;
                }
                source->state = 0;
            }
        }
    }
    m_volumeDirty = 0;
}

// FUNCTION: RALLYTROPHY 0x49a4a0
void T_56b0b0::SetListener(const Matrix& matrix)
{
    m_listener = matrix;
}

// FUNCTION: RALLYTROPHY 0x49a4c0
void T_56b0b0::SetListenerPosition(const Vector3& position)
{
    m_position.x = position.x;
    m_position.y = position.y;
    m_position.z = position.z;
}

// FUNCTION: RALLYTROPHY 0x49a4f0
// Opens a looping music stream.
void T_56b0b0::OpenStream(const char* name)
{
    if (m_disabled) {
        return;
    }
    if (m_stream) {
        CloseStream();
    }
    m_stream = AIL_open_stream(m_driver, name, 0x200000);
    if (m_stream) {
        AIL_set_stream_volume(m_stream, m_streamVolume);
        AIL_set_stream_loop_count(m_stream, 0);
    }
}

// FUNCTION: RALLYTROPHY 0x49a550
void T_56b0b0::CloseStream()
{
    if (!m_disabled && m_stream) {
        AIL_close_stream(m_stream);
        m_stream = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x49a580
void T_56b0b0::StartStream()
{
    if (!m_disabled && m_stream) {
        AIL_start_stream(m_stream);
    }
}

// FUNCTION: RALLYTROPHY 0x49a5a0
void T_56b0b0::SetStreamVolume(int volume)
{
    if (!m_disabled && volume != m_streamVolume) {
        m_streamVolume = volume;
        if (m_stream) {
            AIL_set_stream_volume(m_stream, volume);
        }
    }
}

// FUNCTION: RALLYTROPHY 0x49a5e0
// Loads speech sample `index`, patching the current language's three-letter
// code into the file name ("..._eng.wav") in place.
void T_56b0b0::LoadSpeech(int index)
{
    if (!m_disabled) {
        char* name = DAT_00500328[index];
        int length = strlen(name);
        const char* language = FUN_00405360(FUN_00405390());
        name[length - 7] = language[0];
        name[length - 6] = language[1];
        name[length - 5] = language[2];
        m_speech[index] = LoadSample(name);
    }
}

// FUNCTION: RALLYTROPHY 0x49a650
void T_56b0b0::ReleaseSpeech(int index)
{
    if (!m_disabled) {
        m_speech[index] = ReleaseSample(m_speech[index]);
    }
}

// Behavior reference for T_56b0b0::QueueSpeech (exact machine code is in sound_queue_speech.cpp).
// Queues speech sample `index`. A lower priority than the queued speech is
// dropped; a higher one flushes the queue and stops the speech voice. *done
// is set once the sample starts (immediately when sound is disabled).
void T_56b0b0::QueueSpeech(int index, int priority, int* done)
{
    if (m_disabled) {
        if (done) {
            *done = 1;
        }
        return;
    }
    if (m_paused || m_voices.empty()) {
        return;
    }
    SpeechItem item;
    item.sample = m_speech[index];
    item.done = done;
    EnterCriticalSection(&m_lock);
    if (!m_speechQueue.empty() && priority < m_speechPriority) {
        LeaveCriticalSection(&m_lock);
        return;
    }
    if (priority > m_speechPriority) {
        while (!m_speechQueue.empty()) {
            m_speechQueue.pop_front();
        }
        Voice* voice = &m_voices[0];
        AIL_stop_sample(voice->handle);
        voice->active = 0;
    }
    m_speechQueue.push_back(item);
    m_speechPriority = priority;
    PlayNextSpeech(0);
    LeaveCriticalSection(&m_lock);
}

// FUNCTION: RALLYTROPHY 0x49aa00
// original: FUN_0049aa00
// Keep these state writes sequenced like the shipped code.
// Stops the speech voice and empties the speech queue.
void T_56b0b0::StopSpeech()
{
    if (m_disabled) {
        return;
    }
    EnterCriticalSection(&m_lock);
    Voice* voice = &m_voices[0];
    AIL_stop_sample(voice->handle);
    *(volatile int*)&voice->active = 0;
    while (m_speechQueue.size()) {
        m_speechQueue.pop_front();
    }
    *(volatile int*)&m_speechPriority = 0;
    LeaveCriticalSection(&m_lock);
}

// FUNCTION: RALLYTROPHY 0x49ab20
// original: FUN_0049ab20
// Volatile `unk4` reset preserves the original start-sample push ordering.
// Starts the next queued speech sample on voice 0 unless it is still playing.
void T_56b0b0::PlayNextSpeech(int lock)
{
    if (m_disabled || m_speechQueue.empty() || m_voices.empty()) {
        return;
    }
    if (lock) {
        EnterCriticalSection(&m_lock);
    }
    if (!m_speechQueue.empty()) {
        HSAMPLE statusHandle = m_voices[0].handle;
        Voice* voice = &m_voices[0];
        if (AIL_sample_status(statusHandle) != 4) {
            SpeechItem item = m_speechQueue.front();
            m_speechQueue.pop_front();
            if (item.done) {
                *item.done = 1;
            }
            if (voice->sample != item.sample) {
                AIL_set_sample_file(voice->handle, item.sample->data, 0);
            }
            HSAMPLE handle = voice->handle;
            AIL_set_sample_volume(handle, DAT_00501440 * 127 / 100);
            voice->sample = item.sample;
            voice->active = 1;
            *(volatile int*)&voice->unk4 = 0;
            AIL_start_sample(handle);
        }
    }
    if (lock) {
        LeaveCriticalSection(&m_lock);
    }
}

// Compiler-generated functions VC6 emits for this module (STL template
// instances and implicit members). They match as compiled; each marker names
// the exact COFF symbol, and the typedef under it only gives match.py a
// definition line to anchor on.

__declspec(naked)
// FUNCTION: RALLYTROPHY 0x49b540
// original: FUN_0049b540
// Initialize the deque's first iterator pair to the empty state.
void __fastcall FUN_0049b540(void*)
{
    __asm {
        mov eax, ecx
        xor ecx, ecx
        mov dword ptr [eax], ecx
        mov dword ptr [eax + 4], ecx
        mov dword ptr [eax + 8], ecx
        mov dword ptr [eax + 12], ecx
        ret
    }
}

// FUNCTION: RALLYTROPHY 0x4985b0
// SYMBOL: ??1?$deque@USpeechItem@@V?$allocator@USpeechItem@@@std@@@std@@QAE@XZ
// std::deque<SpeechItem>::~deque (constructor unwind)
typedef void Generated_4985b0();

// FUNCTION: RALLYTROPHY 0x49ad00
// SYMBOL: ?size@?$vector@UVoice@@V?$allocator@UVoice@@@std@@@std@@QBEIXZ
// std::vector<Voice>::size
typedef void Generated_49ad00();

// FUNCTION: RALLYTROPHY 0x49ada0
// SYMBOL: ??_GNode@?$ObjectList@PAUSample@@@@QAEPAXI@Z
// ObjectList<Sample*>::Node scalar deleting destructor
typedef void Generated_49ada0();

// FUNCTION: RALLYTROPHY 0x49adc0
// SYMBOL: ?insert@?$vector@UDeviceInfo@@V?$allocator@UDeviceInfo@@@std@@@std@@QAEXPAUDeviceInfo@@IABU3@@Z
// std::vector<DeviceInfo>::insert(it, n, x)
typedef void Generated_49adc0();

// FUNCTION: RALLYTROPHY 0x49b000
// SYMBOL: ?pop_front@?$deque@USpeechItem@@V?$allocator@USpeechItem@@@std@@@std@@QAEXXZ
// std::deque<SpeechItem>::pop_front
typedef void Generated_49b000();

// FUNCTION: RALLYTROPHY 0x49b090
// SYMBOL: ?insert@?$vector@UVoice@@V?$allocator@UVoice@@@std@@@std@@QAEXPAUVoice@@IABU3@@Z
// std::vector<Voice>::insert(it, n, x)
typedef void Generated_49b090();

// FUNCTION: RALLYTROPHY 0x49b380
// SYMBOL: ?_Buyback@?$deque@USpeechItem@@V?$allocator@USpeechItem@@@std@@@std@@IAEXXZ
// std::deque<SpeechItem>::_Buyback
typedef void Generated_49b380();

// FUNCTION: RALLYTROPHY 0x49b4d0
// SYMBOL: ?_Ucopy@?$vector@UVoice@@V?$allocator@UVoice@@@std@@@std@@IAEPAUVoice@@PBU3@0PAU3@@Z
// std::vector<Voice>::_Ucopy
typedef void Generated_49b4d0();

// FUNCTION: RALLYTROPHY 0x49b510
// SYMBOL: ?_Ufill@?$vector@UVoice@@V?$allocator@UVoice@@@std@@@std@@IAEXPAUVoice@@IABU3@@Z
// std::vector<Voice>::_Ufill
typedef void Generated_49b510();

// FUNCTION: RALLYTROPHY 0x49b550
// SYMBOL: ??0const_iterator@?$deque@USpeechItem@@V?$allocator@USpeechItem@@@std@@@std@@QAE@PAUSpeechItem@@PAPAU3@@Z
// std::deque<SpeechItem>::const_iterator(ptr, map)
typedef void Generated_49b550();

// FUNCTION: RALLYTROPHY 0x49b5a0
// SYMBOL: ?_Construct@std@@YAXPAUVoice@@ABU2@@Z
// std::_Construct<Voice>
typedef void Generated_49b5a0();

// FUNCTION: RALLYTROPHY 0x49b5f0
// SYMBOL: ??1Sample@@QAE@XZ
// Sample::~Sample (implicit, destroys the name)
typedef void Generated_49b5f0();
