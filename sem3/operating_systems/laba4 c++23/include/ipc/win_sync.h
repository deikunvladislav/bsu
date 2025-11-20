#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <expected>
#include <Windows.h>
#include "ipc/win_handle.h"

namespace ipc {
    class SyncObjects {
    public:
        SyncObjects() = default;

        [[nodiscard]] std::expected<void, std::string> create_for_receiver(std::string_view prefix, int sender_count);
        [[nodiscard]] std::expected<void, std::string> open_for_sender(std::string_view prefix);

        [[nodiscard]] std::expected<void, std::string> lock_file(unsigned int timeout_ms);
        void unlock_file();

        [[nodiscard]] std::expected<void, std::string> wait_message(unsigned int timeout_ms);
        [[nodiscard]] std::expected<void, std::string> signal_message();

        [[nodiscard]] std::expected<void, std::string> wait_all_ready(unsigned int timeout_ms);
        [[nodiscard]] std::expected<void, std::string> signal_ready(int sender_id);

        [[nodiscard]] std::string prefix() const { return prefix_; }
        [[nodiscard]] int sender_count() const { return sender_count_; }

    private:
        std::string prefix_;
        int sender_count_{ 0 };
        ipc::unique_handle mutex_;
        ipc::unique_handle event_message_;
        std::vector<ipc::unique_handle> ready_events_;

        static std::string win_last_error(const char* msg);
        static std::string ready_event_name(const std::string& prefix, int id);
        static std::string mutex_name(const std::string& prefix);
        static std::string message_event_name(const std::string& prefix);
    };
}