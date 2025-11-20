#include <iostream>
#include <string>
#include <vector>
#include "core/ring_queue_file.h"
#include "ipc/process_launcher.h"
#include "ipc/win_sync.h"
#include "ipc/constants.h"
#include "util/console.h"

static std::string derive_prefix_from_filename(const std::string& file) {
    auto pos = file.find_last_of("/\\");
    std::string base = (pos == std::string::npos) ? file : file.substr(pos + 1);
    auto dot = base.find_last_of('.');
    if (dot != std::string::npos) base = base.substr(0, dot);
    return "sync_" + base;
}

int main() {
    std::cout << "Receiver: enter binary file name:" << std::endl;
    std::string file_name;
    std::getline(std::cin, file_name);
    if (file_name.empty()) {
        std::cout << "Invalid file name" << std::endl;
        return 1;
    }

    std::cout << "Receiver: enter capacity (number of records):" << std::endl;
    std::string cap_str;
    std::getline(std::cin, cap_str);
    std::size_t capacity = 0;
    try { capacity = static_cast<std::size_t>(std::stoull(cap_str)); }
    catch (...) { capacity = 0; }
    if (capacity == 0) {
        std::cout << "Capacity must be > 0" << std::endl;
        return 1;
    }

    std::cout << "Receiver: enter number of sender processes:" << std::endl;
    std::string cnt_str;
    std::getline(std::cin, cnt_str);
    int sender_count = 0;
    try { sender_count = std::stoi(cnt_str); }
    catch (...) { sender_count = -1; }
    if (sender_count < 0) {
        std::cout << "Invalid sender count" << std::endl;
        return 1;
    }

    auto init = core::RingQueueFile::initialize(file_name, capacity);
    if (!init) {
        std::cout << "Initialize error: " << init.error() << std::endl;
        util::wait_key_to_close();
        return 1;
    }

    std::string prefix = derive_prefix_from_filename(file_name);
    ipc::SyncObjects sync;
    auto sres = sync.create_for_receiver(prefix, sender_count);
    if (!sres) {
        std::cout << "Sync create error: " << sres.error() << std::endl;
        util::wait_key_to_close();
        return 1;
    }

    std::cout << "Path to sender.exe (leave empty for sender.exe in current directory):" << std::endl;
    std::string sender_path;
    std::getline(std::cin, sender_path);
    if (sender_path.empty()) sender_path = "sender.exe";

    auto children = ipc::ProcessLauncher::launch_senders(sender_count, sender_path, file_name, prefix);
    if (!children) {
        std::cout << "Launch error: " << children.error() << std::endl;
        util::wait_key_to_close();
        return 1;
    }
    auto procs = std::move(children.value());

    if (sender_count > 0) {
        std::cout << "Waiting for all senders to be ready..." << std::endl;
        auto ready = sync.wait_all_ready(ipc::kDefaultWaitMs);
        if (!ready) {
            std::cout << "Ready wait failed: " << ready.error() << std::endl;
            util::wait_key_to_close();
            return 1;
        }
    }

    core::RingQueueFile queue(file_name);
    if (!queue.ok()) {
        std::cout << "Open queue error: " << queue.error() << std::endl;
        util::wait_key_to_close();
        return 1;
    }

    std::cout << "Receiver ready. Commands: read | quit" << std::endl;
    for (;;) {
        std::string cmd;
        if (!std::getline(std::cin, cmd)) break;

        if (cmd == "quit") {
            std::cout << "Receiver exiting..." << std::endl;
            for (auto& child : procs) {
                if (child.process) {
                    ::TerminateProcess(child.process.get(), 0);
                    child.process.reset();
                }
            }
            break;
        }
        else if (cmd == "read") {
            if (queue.is_empty()) {
                std::cout << "Queue empty, waiting for message..." << std::endl;
                auto waited = sync.wait_message(ipc::kDefaultWaitMs);
                if (!waited) {
                    std::cout << "Message wait failed: " << waited.error() << std::endl;
                    continue;
                }
            }
            auto lock = sync.lock_file(ipc::kDefaultWaitMs);
            if (!lock) {
                std::cout << "Lock error: " << lock.error() << std::endl;
                continue;
            }
            auto msg = queue.try_pop();
            sync.unlock_file();
            if (msg) {
                std::cout << "Message: " << *msg << std::endl;
            }
            else {
                std::cout << "Pop error: " << msg.error() << std::endl;
            }
        }
        else {
            std::cout << "Unknown command. Commands: read | quit" << std::endl;
        }
    }

    util::wait_key_to_close();
    return 0;
}