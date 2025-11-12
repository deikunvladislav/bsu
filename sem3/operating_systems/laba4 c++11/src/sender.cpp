#include <string>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <limits>
#include "core/ring_queue_file.h"
#include "ipc/win_sync.h"
#include "util/string_trim.h"
#include "util/console.h"

static bool set_ready_event(const std::string& prefix, int sender_index) {
    std::string name = prefix + "_ready_" + std::to_string(sender_index);
    HANDLE h = OpenEventA(EVENT_MODIFY_STATE | SYNCHRONIZE, FALSE, name.c_str());
    if (h == NULL) return false;
    BOOL ok = SetEvent(h);
    CloseHandle(h);
    return ok == TRUE;
}

static constexpr unsigned int kMutexAcquireTimeoutMs = 15000;

static void wait_key_to_close() {
    util::write_line("Press any key to close...");
    _getch();
}

int main(int argc, char* argv[]) {
    using util::write_line;

    if (argc < 4) {
        write_line("Usage: sender <file> <prefix> <index>");
        wait_key_to_close();
        return 1;
    }
    std::string file_path = argv[1];
    std::string prefix = argv[2];
    int sender_index = std::stoi(argv[3]);

    ipc::SyncObjects sync;
    if (!sync.open_for_sender(prefix)) {
        write_line("Error opening sync objects");
        wait_key_to_close();
        return 1;
    }

    if (!set_ready_event(prefix, sender_index)) {
        write_line("Error signaling ready");
        wait_key_to_close();
        return 1;
    }
    write_line("Sender ready.");

    core::RingQueueFile queue(file_path);
    if (!queue.ok()) {
        write_line("Queue open error: " + queue.error());
        wait_key_to_close();
        return 1;
    }

    for (;;) {
        write_line("Sender command: [send | stop]");
        std::string cmd = util::trim_copy(util::read_line());
        if (cmd == "stop") {
            write_line("Sender exiting...");
            wait_key_to_close();
            return 0;
        }
        else if (cmd == "send") {
            bool queue_full = false;

            if (!sync.lock_file(kMutexAcquireTimeoutMs)) {
                write_line("File lock error");
                continue;
            }
            {
                queue_full = queue.is_full();
            }
            sync.unlock_file();

            if (queue_full) {
                write_line("Queue full. Try again later.");
                continue;
            }

            write_line("Enter message (< 20 chars):");
            std::string msg = util::trim_copy(util::read_line());
            if (msg.empty() || msg.size() >= core::kMaxMessageLength) {
                write_line("Error: invalid message length.");
                continue;
            }

            bool pushed = false;

            if (!sync.lock_file(kMutexAcquireTimeoutMs)) {
                write_line("File lock error");
                continue;
            }
            pushed = queue.try_push(msg);
            sync.unlock_file();

            if (!pushed) {
                write_line("Queue full. Try again later.");
                Sleep(200);
                continue;
            }

            if (!sync.signal_message()) {
                write_line("Error signaling message");
            }
            else {
                write_line("Message sent.");
            }
        }
        else {
            write_line("Unknown command.");
        }
    }
}