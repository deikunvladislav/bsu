#pragma once
#include <Windows.h>
#include "win_error.hpp"

class CriticalSection {
public:
    CriticalSection() { InitializeCriticalSection(&cs_); }
    ~CriticalSection() { DeleteCriticalSection(&cs_); }
    void lock() { EnterCriticalSection(&cs_); }
    void unlock() { LeaveCriticalSection(&cs_); }
private:
    CRITICAL_SECTION cs_{};
};

class Event {
public:
    Event(bool manualReset, bool initialState) {
        handle_ = CreateEventA(nullptr, manualReset, initialState, nullptr);
        if (!handle_) throw WinApiError("CreateEventA");
    }
    ~Event() { if (handle_) CloseHandle(handle_); }
    void set() { if (!SetEvent(handle_)) throw WinApiError("SetEvent"); }
    void reset() { if (!ResetEvent(handle_)) throw WinApiError("ResetEvent"); }
    DWORD wait(DWORD ms = INFINITE) {
        DWORD res = WaitForSingleObject(handle_, ms);
        if (res == WAIT_FAILED) throw WinApiError("WaitForSingleObject");
        return res;
    }
    HANDLE native() const { return handle_; }
private:
    HANDLE handle_{ nullptr };
};
