#include "ipc/process_launcher.h"
#include <Windows.h>
#include <format>
#include <filesystem>
#include <vector>
#include <string>

namespace ipc {
    static std::string win_last_error(const char* msg) {
        DWORD code = ::GetLastError();
        return std::format("{} (WinErr {})", msg, code);
    }

    std::expected<std::vector<ChildProcess>, std::string>
        ProcessLauncher::launch_senders(int count,
            std::string_view sender_exe_path,
            std::string_view file_path,
            std::string_view sync_prefix) {
        if (count < 0) return std::unexpected("Sender count cannot be negative");
        std::vector<ChildProcess> children;
        children.reserve(count);

        std::filesystem::path exe(sender_exe_path);
        if (!std::filesystem::exists(exe)) {
            return std::unexpected("Sender executable not found: " + std::string(sender_exe_path));
        }

        for (int i = 0; i < count; ++i) {
            std::string cmd = std::format("\"{}\" \"{}\" \"{}\" {}", sender_exe_path, file_path, sync_prefix, i);

            STARTUPINFOA si{};
            si.cb = sizeof(si);
            PROCESS_INFORMATION pi{};
            std::vector<char> mutable_cmd(cmd.begin(), cmd.end());
            mutable_cmd.push_back('\0');

            BOOL ok = ::CreateProcessA(
                nullptr,
                mutable_cmd.data(),
                nullptr,
                nullptr,
                FALSE,
                CREATE_NEW_CONSOLE,
                nullptr,
                nullptr,
                &si,
                &pi
            );

            if (!ok) {
                return std::unexpected(win_last_error("CreateProcessA failed"));
            }

            ::CloseHandle(pi.hThread);
            ChildProcess child;
            child.pid = pi.dwProcessId;
            child.process.reset(pi.hProcess);
            children.push_back(std::move(child));
        }

        return children;
    }
}