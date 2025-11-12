#ifndef LAB4_SYNC_PROCESS_LAUNCHER_H
#define LAB4_SYNC_PROCESS_LAUNCHER_H

#include <string>
#include <vector>

namespace ipc {

    struct ChildProcess {
        unsigned long pid;
        void* handle;
    };

    class ProcessLauncher {
    public:
        static bool launch_senders(
            int count,
            const std::string& sender_exe_path,
            const std::string& file_path,
            const std::string& sync_prefix,
            std::vector<ChildProcess>& out_children,
            std::string& err
        );
    };

}

#endif