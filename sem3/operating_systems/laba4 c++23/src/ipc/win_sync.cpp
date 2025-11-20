#include "ipc/win_sync.h"
#include "ipc/constants.h"
#include <Windows.h>
#include <string>
#include <format>
#include <vector>

namespace ipc {
    std::string SyncObjects::win_last_error(const char* msg) {
        DWORD code = ::GetLastError();
        return std::format("{} (WinErr {})", msg, code);
    }

    std::string SyncObjects::ready_event_name(const std::string& prefix, int id) {
        return prefix + ipc::kReadyEventPrefix + std::to_string(id);
    }

    std::string SyncObjects::mutex_name(const std::string& prefix) {
        return prefix + ipc::kMutexSuffix;
    }

    std::string SyncObjects::message_event_name(const std::string& prefix) {
        return prefix + ipc::kMessageEventSuffix;
    }

    std::expected<void, std::string> SyncObjects::create_for_receiver(std::string_view pfx, int senders) {
        prefix_ = std::string(pfx);
        sender_count_ = senders;
        ready_events_.clear();

        HANDLE m = ::CreateMutexA(nullptr, FALSE, mutex_name(prefix_).c_str());
        if (!m) return std::unexpected(win_last_error("CreateMutex failed"));
        mutex_.reset(m);

        HANDLE e = ::CreateEventA(nullptr, FALSE, FALSE, message_event_name(prefix_).c_str());
        if (!e) return std::unexpected(win_last_error("CreateEvent (message) failed"));
        event_message_.reset(e);

        for (int i = 0; i < sender_count_; ++i) {
            HANDLE h = ::CreateEventA(nullptr, TRUE, FALSE, ready_event_name(prefix_, i).c_str());
            if (!h) return std::unexpected(win_last_error("CreateEvent (ready) failed"));
            ready_events_.emplace_back(h);
        }

        return {};
    }

    std::expected<void, std::string> SyncObjects::open_for_sender(std::string_view pfx) {
        prefix_ = std::string(pfx);

        HANDLE m = ::OpenMutexA(SYNCHRONIZE, FALSE, mutex_name(prefix_).c_str());
        if (!m) return std::unexpected(win_last_error("OpenMutex failed"));
        mutex_.reset(m);

        HANDLE e = ::OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, message_event_name(prefix_).c_str());
        if (!e) return std::unexpected(win_last_error("OpenEvent (message) failed"));
        event_message_.reset(e);

        return {};
    }

    std::expected<void, std::string> SyncObjects::lock_file(unsigned int timeout_ms) {
        if (!mutex_) return std::unexpected("Mutex not initialized");
        DWORD w = ::WaitForSingleObject(mutex_.get(), timeout_ms);
        if (w == WAIT_OBJECT_0) return {};
        if (w == WAIT_TIMEOUT) return std::unexpected("Mutex wait timeout");
        return std::unexpected(win_last_error("WaitForSingleObject (mutex) failed"));
    }

    void SyncObjects::unlock_file() {
        if (mutex_) ::ReleaseMutex(mutex_.get());
    }

    std::expected<void, std::string> SyncObjects::wait_message(unsigned int timeout_ms) {
        if (!event_message_) return std::unexpected("Message event not initialized");
        DWORD w = ::WaitForSingleObject(event_message_.get(), timeout_ms);
        if (w == WAIT_OBJECT_0) return {};
        if (w == WAIT_TIMEOUT) return std::unexpected("Message wait timeout");
        return std::unexpected(win_last_error("WaitForSingleObject (message) failed"));
    }

    std::expected<void, std::string> SyncObjects::signal_message() {
        if (!event_message_) return std::unexpected("Message event not initialized");
        if (::SetEvent(event_message_.get())) return {};
        return std::unexpected(win_last_error("SetEvent (message) failed"));
    }

    std::expected<void, std::string> SyncObjects::wait_all_ready(unsigned int timeout_ms) {
        if (ready_events_.empty()) return std::unexpected("Ready events not initialized");

        std::vector<HANDLE> hs;
        hs.reserve(ready_events_.size());
        for (auto& uh : ready_events_) hs.push_back(uh.get());

        DWORD w = ::WaitForMultipleObjects(static_cast<DWORD>(hs.size()), hs.data(), TRUE, timeout_ms);
        if (w >= WAIT_OBJECT_0 && w < WAIT_OBJECT_0 + hs.size()) return {};
        if (w == WAIT_TIMEOUT) return std::unexpected("Ready events wait timeout");
        return std::unexpected(win_last_error("WaitForMultipleObjects failed"));
    }

    std::expected<void, std::string> SyncObjects::signal_ready(int sender_id) {
        std::string name = ready_event_name(prefix_, sender_id);
        HANDLE h = ::OpenEventA(EVENT_MODIFY_STATE, FALSE, name.c_str());
        if (!h) return std::unexpected(win_last_error("OpenEvent (ready) failed"));
        bool ok = (::SetEvent(h) != 0);
        ::CloseHandle(h);
        if (ok) return {};
        return std::unexpected(win_last_error("SetEvent (ready) failed"));
    }
}