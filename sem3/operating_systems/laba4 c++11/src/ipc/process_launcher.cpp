#include "ipc/process_launcher.h"
#include <Windows.h>
#include <sstream>
#include <cstdlib>

namespace ipc {

    bool ProcessLauncher::launch_senders(
        int count,
        const std::string& sender_exe_path,
        const std::string& file_path,
        const std::string& sync_prefix,
        std::vector<ChildProcess>& out_children,
        std::string& err
    ) {
        out_children.clear();
        for (int i = 0; i < count; ++i) {
            std::ostringstream cmd;
            cmd << "\"" << sender_exe_path << "\""
                << " " << "\"" << file_path << "\""
                << " " << "\"" << sync_prefix << "\""
                << " " << i;

            STARTUPINFOA si;
            PROCESS_INFORMATION pi;
            ZeroMemory(&si, sizeof(si));
            ZeroMemory(&pi, sizeof(pi));
            si.cb = sizeof(si);

            std::string cmd_str = cmd.str();
            char* mutable_cmd = _strdup(cmd_str.c_str());

            BOOL ok = CreateProcessA(
                NULL,
                mutable_cmd,
                NULL,
                NULL,
                FALSE,
                CREATE_NEW_CONSOLE,
                NULL,
                NULL,
                &si,
                &pi
            );
            free(mutable_cmd);

            if (!ok) {
                err = "Failed to launch sender " + std::to_string(i);
                return false;
            }

            CloseHandle(pi.hThread);
            out_children.push_back({ pi.dwProcessId, pi.hProcess });
        }
        return true;
    }

}