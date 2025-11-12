#ifndef LAB4_SYNC_WIN_SYNC_H
#define LAB4_SYNC_WIN_SYNC_H

#include <Windows.h>
#include <string>
#include <vector>
#include "ipc/win_handle.h"

namespace ipc {

    struct ReadyEvent {
        WinHandle handle;
        std::string name;
    };

    struct MessageEvent {
        WinHandle handle;
        std::string name;
    };

    struct FileMutex {
        WinHandle handle;
        std::string name;
    };

    class SyncObjects {
    public:
        bool create_for_receiver(const std::string& name_prefix, int sender_count);
        bool open_for_sender(const std::string& name_prefix);

        bool wait_all_ready(unsigned int timeout_ms);

        bool wait_message(unsigned int timeout_ms);
        bool signal_message();

        bool lock_file(unsigned int timeout_ms);
        void unlock_file();

    private:
        std::vector<ReadyEvent> ready_events_;
        MessageEvent message_event_;
        FileMutex file_mutex_;
    };

}

#endif