#include "ipc/win_sync.h"

namespace ipc {

    static WinHandle create_named_event(const std::string& name, BOOL manual_reset, BOOL initial_state) {
        HANDLE h = CreateEventA(NULL, manual_reset, initial_state, name.c_str());
        return WinHandle(h);
    }

    static WinHandle open_named_event(const std::string& name) {
        HANDLE h = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, name.c_str());
        return WinHandle(h);
    }

    static WinHandle create_named_mutex(const std::string& name) {
        HANDLE h = CreateMutexA(NULL, FALSE, name.c_str());
        return WinHandle(h);
    }

    static WinHandle open_named_mutex(const std::string& name) {
        HANDLE h = OpenMutexA(SYNCHRONIZE, FALSE, name.c_str());
        return WinHandle(h);
    }

    bool SyncObjects::create_for_receiver(const std::string& prefix, int sender_count) {
        ready_events_.clear();
        ready_events_.reserve(static_cast<std::size_t>(sender_count));
        for (int i = 0; i < sender_count; ++i) {
            std::string name = prefix + "_ready_" + std::to_string(i);
            WinHandle h = create_named_event(name, TRUE, FALSE);
            if (!h.valid()) return false;
            ready_events_.push_back({ std::move(h), name });
        }
        message_event_.name = prefix + "_message";
        message_event_.handle = create_named_event(message_event_.name, FALSE, FALSE);
        if (!message_event_.handle.valid()) return false;

        file_mutex_.name = prefix + "_file_mutex";
        file_mutex_.handle = create_named_mutex(file_mutex_.name);
        if (!file_mutex_.handle.valid()) return false;

        return true;
    }

    bool SyncObjects::open_for_sender(const std::string& prefix) {
        message_event_.name = prefix + "_message";
        message_event_.handle = open_named_event(message_event_.name);
        if (!message_event_.handle.valid()) return false;

        file_mutex_.name = prefix + "_file_mutex";
        file_mutex_.handle = open_named_mutex(file_mutex_.name);
        if (!file_mutex_.handle.valid()) return false;

        return true;
    }

    bool SyncObjects::wait_all_ready(unsigned int timeout_ms) {
        if (ready_events_.empty()) return true;
        std::vector<HANDLE> hs;
        hs.reserve(ready_events_.size());
        for (const auto& re : ready_events_) hs.push_back(re.handle.get());
        DWORD res = WaitForMultipleObjects(static_cast<DWORD>(hs.size()), hs.data(), TRUE, timeout_ms);
        return res == WAIT_OBJECT_0;
    }

    bool SyncObjects::wait_message(unsigned int timeout_ms) {
        HANDLE h = message_event_.handle.get();
        DWORD res = WaitForSingleObject(h, timeout_ms);
        return res == WAIT_OBJECT_0;
    }

    bool SyncObjects::signal_message() {
        HANDLE h = message_event_.handle.get();
        return SetEvent(h) == TRUE;
    }

    bool SyncObjects::lock_file(unsigned int timeout_ms) {
        HANDLE h = file_mutex_.handle.get();
        DWORD res = WaitForSingleObject(h, timeout_ms);
        return res == WAIT_OBJECT_0;
    }

    void SyncObjects::unlock_file() {
        HANDLE h = file_mutex_.handle.get();
        ReleaseMutex(h);
    }

}