#include <string>
#include <vector>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <limits>
#include "core/ring_queue_file.h"
#include "ipc/win_sync.h"
#include "ipc/process_launcher.h"
#include "util/string_trim.h"
#include "util/console.h"

static constexpr unsigned int kWaitReadyTimeoutMs = 15000;
static constexpr unsigned int kMutexAcquireTimeoutMs = 15000;

static bool parse_positive_int(const std::string& s, int& out) {
    try { int v = std::stoi(s); if (v > 0) { out = v; return true; } }
    catch (...) {}
    return false;
}

static void wait_key_to_close() {
    util::write_line("Press any key to close...");
    _getch();
}

int main() {
    using util::write_line;

    write_line("Receiver: enter binary file name:");
    std::string file_name = util::trim_copy(util::read_line());
    if (file_name.empty()) { write_line("Error: empty file name"); wait_key_to_close(); return 1; }

    write_line("Receiver: enter number of records:");
    std::string cap_s = util::trim_copy(util::read_line());
    int cap_i = 0;
    if (!parse_positive_int(cap_s, cap_i)) { write_line("Error: invalid number"); wait_key_to_close(); return 1; }

    std::string err;
    if (!core::RingQueueFile::initialize(file_name, static_cast<std::size_t>(cap_i), err)) {
        write_line("File initialization error: " + err);
        wait_key_to_close();
        return 1;
    }
    write_line("Queue file created.");

    write_line("Receiver: enter number of sender processes:");
    std::string senders_s = util::trim_copy(util::read_line());
    int sender_count = 0;
    if (!parse_positive_int(senders_s, sender_count)) { write_line("Error: invalid number"); wait_key_to_close(); return 1; }

    ipc::SyncObjects sync;
    if (!sync.create_for_receiver(file_name, sender_count)) {
        write_line("Error creating sync objects");
        wait_key_to_close();
        return 1;
    }

    std::vector<ipc::ChildProcess> children;
    if (!ipc::ProcessLauncher::launch_senders(sender_count, "sender.exe", file_name, file_name, children, err)) {
        write_line("Error launching senders: " + err);
        wait_key_to_close();
        return 1;
    }
    write_line("Senders launched.");

    write_line("Waiting for all senders to be ready...");
    if (!sync.wait_all_ready(kWaitReadyTimeoutMs)) {
        write_line("Not all senders are ready.");
        wait_key_to_close();
        return 1;
    }
    write_line("All senders are ready.");

    core::RingQueueFile queue(file_name);
    if (!queue.ok()) {
        write_line("Queue open error: " + queue.error());
        wait_key_to_close();
        return 1;
    }

    for (;;) {
        write_line("Receiver command: [read | quit]");
        std::string cmd = util::trim_copy(util::read_line());
        if (cmd == "quit") {
            write_line("Receiver exiting...");
            for (auto& child : children) {
                if (child.handle) {
                    TerminateProcess(child.handle, 0);
                    CloseHandle(child.handle);
                }
            }
            wait_key_to_close();
            return 0;
        }
        else if (cmd == "read") {
            std::string msg;
            bool popped = false;

            if (!sync.lock_file(kMutexAcquireTimeoutMs)) {
                write_line("File lock error");
                continue;
            }
            popped = queue.try_pop(msg);
            sync.unlock_file();

            if (!popped) {
                write_line("Queue empty. Waiting for message...");
                if (!sync.wait_message(INFINITE)) {
                    write_line("Message wait error");
                    continue;
                }
                if (!sync.lock_file(kMutexAcquireTimeoutMs)) {
                    write_line("File lock error");
                    continue;
                }
                popped = queue.try_pop(msg);
                sync.unlock_file();
                if (!popped) continue;
            }

            write_line("Received: " + msg);
        }
        else {
            write_line("Unknown command.");
        }
    }
}