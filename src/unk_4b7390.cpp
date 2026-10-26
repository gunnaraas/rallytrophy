// Timer, string conversion and registry helpers (0x4b7390-0x4b777f).
#include "rallytrophy.h"

// FUNCTION: RALLYTROPHY 0x4b7390
void T_56b0bc::ClearKeys()
{
    memset(DAT_0057ba28, 0, sizeof(DAT_0057ba28));
}

// FUNCTION: RALLYTROPHY 0x4b73b0
void FUN_004b73b0(int value)
{
    DAT_0057bc58 = value;
}

// 0x4b73c0-0x4b751f: std::deque instances emitted in input_deque.cpp.

// FUNCTION: RALLYTROPHY 0x4b7520
Timer::Timer()
{
    if (s_refCount == 0) {
        TIMECAPS caps;
        UINT period = 1;
        if (timeGetDevCaps(&caps, sizeof(caps)) == TIMERR_NOERROR) {
            period = min(max(caps.wPeriodMin, 1), caps.wPeriodMax);
            timeBeginPeriod(period);
        }
        s_period = period;
        s_hasPerfCounter = QueryPerformanceFrequency(&s_frequency);
    }
    s_refCount++;
    Reset();
}

// FUNCTION: RALLYTROPHY 0x4b75a0
Timer::~Timer()
{
    if (--s_refCount == 0) {
        timeEndPeriod(s_period);
    }
}

// FUNCTION: RALLYTROPHY 0x4b75c0
void Timer::Update()
{
    DWORD prev = m_time;
    m_time = timeGetTime() - m_start;
    m_delta = m_time - prev;
    m_deltaSec = m_delta * 0.001f;
}

// FUNCTION: RALLYTROPHY 0x4b7600
void Timer::Reset()
{
    m_time = 0;
    m_deltaSec = 0;
    m_delta = 0;
    m_start = timeGetTime();
}

// FUNCTION: RALLYTROPHY 0x4b7620
__int64 Timer::GetCycles()
{
    __int64 cycles;
    __asm {
        xor eax, eax
        _emit 0x0f // rdtsc
        _emit 0x31
        mov dword ptr [cycles], eax
        mov dword ptr [cycles + 4], edx
        xor edx, edx
    }
    return cycles;
}

// FUNCTION: RALLYTROPHY 0x4b7670
void FUN_004b7670(WCHAR* dst, const char* src, int len)
{
    if (dst && src) {
        if (len == -1) {
            len = strlen(src) + 1;
        }
        MultiByteToWideChar(CP_ACP, 0, src, -1, dst, len - 1);
        dst[len - 1] = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x4b76c0
void FUN_004b76c0(char* dst, const WCHAR* src, int len)
{
    if (dst && src) {
        if (len == -1) {
            len = wcslen(src) + 1;
        }
        WideCharToMultiByte(CP_ACP, 0, src, -1, dst, len - 1, NULL, NULL);
        dst[len - 1] = 0;
    }
}

// FUNCTION: RALLYTROPHY 0x4b7710
void FUN_004b7710(WCHAR* dst, const char* src, int len)
{
    if (dst && src) {
        FUN_004b7670(dst, src, len);
    }
}

// FUNCTION: RALLYTROPHY 0x4b7730
void FUN_004b7730(char* dst, const WCHAR* src, int len)
{
    if (dst && src) {
        FUN_004b76c0(dst, src, len);
    }
}

// FUNCTION: RALLYTROPHY 0x4b7750
HRESULT FUN_004b7750(HKEY key, const char* name, const char* value)
{
    if (RegSetValueExA(key, name, 0, REG_SZ, (const BYTE*)value, strlen(value) + 1)) {
        return E_FAIL;
    }
    return S_OK;
}
