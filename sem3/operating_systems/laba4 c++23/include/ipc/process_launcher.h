#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <expected>
#include "ipc/win_handle.h"

namespace ipc {
    struct ChildProcess {
        unsigned long pid{};
        ipc::unique_handle process{};
    };

    class ProcessLauncher {
    public:
        [[nodiscard]] static std::expected<std::vector<ChildProcess>, std::string>
            launch_senders(int count,
                std::string_view sender_exe_path,
                std::string_view file_path,
                std::string_view sync_prefix);
    };
}