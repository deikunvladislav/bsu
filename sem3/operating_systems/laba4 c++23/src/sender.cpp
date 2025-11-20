#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "core/ring_queue_file.h"
#include "ipc/win_sync.h"
#include "ipc/constants.h"
#include "util/console.h"

static void print_usage() {
    std::cout << "Usage: sender.exe <file_path> <sync_prefix> <sender_id>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        print_usage();
        util::wait_key_to_close();
        return 1;
    }

    const std::string file_path = argv[1];
    const std::string sync_prefix = argv[2];
    int sender_id = -1;
    try { sender_id = std::stoi(argv[3]); }
    catch (...) { sender_id = -1; }
    if (sender_id < 0) {
        std::cout << "Invalid sender id" << std::endl;
        util::wait_key_to_close();
        return 1;
    }

    ipc::SyncObjects sync;
    auto open = sync.open_for_sender(sync_prefix);
    if (!open) {
        std::cout << "Sync open error: " << open.error() << std::endl;
        util::wait_key_to_close();
        return 1;
    }

    core::RingQueueFile queue(file_path);
    if (!queue.ok()) {
        std::cout << "Open queue error: " << queue.error() << std::endl;
        util::wait_key_to_close();
        return 1;
    }

    auto rd = sync.signal_ready(sender_id);
    if (!rd) {
        std::cout << "Signal ready error: " << rd.error() << std::endl;
        util::wait_key_to_close();
        return 1;
    }

    std::cout << "Sender " << sender_id << " ready. Commands: send | exit" << std::endl;
    for (;;) {
        std::string cmd;
        if (!std::getline(std::cin, cmd)) break;

        if (cmd == "exit") {
            std::cout << "Sender " << sender_id << " exiting..." << std::endl;
            break;
        }
        else if (cmd == "send") {
            while (queue.is_full()) {
                std::cout << "Queue full, waiting..." << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(3000));
            }

            std::cout << "Enter message (<20 chars):" << std::endl;
            std::string msg;
            if (!std::getline(std::cin, msg)) break;

            if (msg.size() >= core::RingQueueFile::kMaxMessageLength) {
                std::cout << "Message too long" << std::endl;
                continue;
            }

            auto lock = sync.lock_file(ipc::kDefaultWaitMs);
            if (!lock) {
                std::cout << "Lock error: " << lock.error() << std::endl;
                continue;
            }

            auto wr = queue.try_push(msg);
            sync.unlock_file();

            if (!wr) {
                std::cout << "Push error: " << wr.error() << std::endl;
                continue;
            }

            auto sig = sync.signal_message();
            if (!sig) {
                std::cout << "Signal error: " << sig.error() << std::endl;
            }
            else {
                std::cout << "Sent" << std::endl;
            }
        }
        else {
            std::cout << "Unknown command. Commands: send | exit" << std::endl;
        }
    }

    util::wait_key_to_close();
    return 0;
}