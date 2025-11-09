#include <windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdio>
#include "generated_tests.h"

static int RunProcessCaptureOutput(const std::string& exePath, std::string& outText) {
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    HANDLE outRead = NULL;
    HANDLE outWrite = NULL;
    if (!CreatePipe(&outRead, &outWrite, &saAttr, 0)) {
        outText = "CreatePipe failed";
        return -1;
    }
    SetHandleInformation(outRead, HANDLE_FLAG_INHERIT, 0);

    PROCESS_INFORMATION pi;
    STARTUPINFOA si;
    ZeroMemory(&pi, sizeof(pi));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.hStdError = outWrite;
    si.hStdOutput = outWrite;
    si.dwFlags |= STARTF_USESTDHANDLES;

    std::string cmd = exePath;
    BOOL ok = CreateProcessA(
        NULL,
        const_cast<LPSTR>(cmd.c_str()),
        NULL,
        NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL,
        NULL,
        &si,
        &pi
    );

    CloseHandle(outWrite);

    if (!ok) {
        CloseHandle(outRead);
        std::ostringstream ss;
        ss << "CreateProcess failed for: " << exePath << " (err=" << GetLastError() << ")";
        outText = ss.str();
        return -1;
    }

    const DWORD bufSize = 4096;
    char buffer[bufSize];
    DWORD bytesRead = 0;
    outText.clear();

    for (;;) {
        BOOL r = ReadFile(outRead, buffer, bufSize, &bytesRead, NULL);
        if (!r || bytesRead == 0) break;
        outText.append(buffer, buffer + bytesRead);
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exitCode = 0;
    GetExitCodeProcess(pi.hProcess, &exitCode);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(outRead);

    return static_cast<int>(exitCode);
}

int main() {
    int total = 0;
    int passed = 0;

    for (int i = 0; i < kGeneratedTestsCount; ++i) {
        const char* path = kGeneratedTests[i];
        if (path == nullptr || path[0] == '\0') continue;
        ++total;
        std::string output;
        std::cerr << "Running test: " << path << " ...\n";
        int code = RunProcessCaptureOutput(path, output);
        std::cerr << "----- begin output -----\n" << output << "----- end output -----\n";
        if (code == 0) {
            std::cerr << "Result: PASSED\n\n";
            ++passed;
        }
        else {
            std::cerr << "Result: FAILED (code=" << code << ")\n\n";
        }
    }

    std::cout << "Ran " << total << " tests. Passed: " << passed << ". Failed: " << (total - passed) << ".\n";

    return (passed == total) ? 0 : 1;
}