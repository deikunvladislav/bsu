#include "utils.h"
#include <windows.h>
#include <iostream>

namespace utils {
    void runProcess(const std::string& exe, const std::string& args) {
        std::string command = exe + " " + args;
        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;
        if (!CreateProcessA(nullptr, command.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            std::cerr << "Failed to start process: " << exe << '\n';
            exit(1);
        }
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}