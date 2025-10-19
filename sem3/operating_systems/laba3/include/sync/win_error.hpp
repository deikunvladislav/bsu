#pragma once
#include <stdexcept>
#include <string>
#include <Windows.h>

inline std::string formatWinError(DWORD code) {
    LPVOID msgBuf = nullptr;
    DWORD len = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, code, 0,
        reinterpret_cast<LPSTR>(&msgBuf), 0, nullptr);
    std::string msg = len ? std::string(reinterpret_cast<LPSTR>(msgBuf)) : "Unknown error.";
    if (msgBuf) LocalFree(msgBuf);
    return msg;
}

struct WinApiError : std::runtime_error {
    explicit WinApiError(const char* api)
        : std::runtime_error(std::string(api) + " failed: " + formatWinError(GetLastError())) {}
};
