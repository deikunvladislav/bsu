#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <thread>
#include <windows.h>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <set>
#include "protocol.hpp"
#include "pipe.hpp"
#include "file_storage.hpp"
#include "rw_lock.hpp"
#include "console_io.hpp"

namespace {
    std::atomic<int> active_clients{ 0 };
    int max_clients = 0;
    std::atomic<bool> session_active{ false };
    std::atomic<bool> shutdown_requested{ false };

    std::mutex mtx;
    std::condition_variable cv_zero;

    std::mutex index_mtx;
    std::set<int> free_indices;

    constexpr const char* prompt_file() { return "Enter binary file name: "; }
    constexpr const char* prompt_n() { return "Enter number of records: "; }

    std::vector<Employee> read_initial_employees() {
        const int32_t n = Console::prompt_int(prompt_n());
        std::vector<Employee> v;
        v.reserve(static_cast<std::size_t>(n));
        for (int32_t i = 0; i < n; ++i) {
            std::cout << "Record #" << (i + 1) << '\n';
            v.push_back(Console::prompt_employee());
        }
        return v;
    }

    void print_file(const FileStorage& fs) {
        const auto all = fs.read_all();
        std::cout << "File contents:\n";
        FileStorage::print_to_console(all, std::cout);
    }

    void serve_client(FileStorage* storage, PerRecordRWLock* locks, NamedPipeServer server, int client_index) {
        Proto::Response hello{};
        hello.ok = true;
        std::snprintf(hello.message, sizeof(hello.message),
            "Connected. Client #%d. Active clients: %d", client_index, active_clients.load());
        server.write_response(hello);

        std::cout << "Client " << client_index << " connected. Active clients: " << active_clients.load() << '\n';

        bool running = true;
        while (running) {
            if (shutdown_requested.load()) {
                Proto::Response resp{};
                resp.ok = false;
                std::snprintf(resp.message, sizeof(resp.message), "Shutdown");
                server.write_response(resp);
                break;
            }

            Proto::Request req{};
            if (!server.read_request(req)) break;

            Proto::Response resp{};
            resp.ok = false;

            switch (req.op) {
            case Proto::Op::ReadBegin:
                if (!locks->try_lock_read(req.id)) {
                    std::snprintf(resp.message, sizeof(resp.message), "Record is locked by writer");
                    server.write_response(resp);
                    break;
                }
                if (auto rec = storage->read_by_id(req.id)) {
                    resp.ok = true;
                    resp.payload = *rec;
                    std::snprintf(resp.message, sizeof(resp.message), "Read OK");
                }
                else {
                    locks->unlock_read(req.id);
                    std::snprintf(resp.message, sizeof(resp.message), "Record not found");
                }
                server.write_response(resp);
                break;

            case Proto::Op::ReadEnd:
                locks->unlock_read(req.id);
                resp.ok = true;
                std::snprintf(resp.message, sizeof(resp.message), "Read ended");
                server.write_response(resp);
                break;

            case Proto::Op::WriteStart:
                if (!locks->try_lock_write(req.id)) {
                    std::snprintf(resp.message, sizeof(resp.message), "Record is locked by readers/writer");
                    server.write_response(resp);
                    break;
                }
                if (auto rec = storage->read_by_id(req.id)) {
                    resp.ok = true;
                    resp.payload = *rec;
                    std::snprintf(resp.message, sizeof(resp.message), "Write lock acquired");
                }
                else {
                    resp.ok = false;
                    std::snprintf(resp.message, sizeof(resp.message), "Record not found, lock held");
                }
                server.write_response(resp);
                break;

            case Proto::Op::WriteCommit:
                if (storage->update_by_id(req.payload)) {
                    resp.ok = true;
                    std::snprintf(resp.message, sizeof(resp.message), "Commit OK");
                }
                else {
                    std::snprintf(resp.message, sizeof(resp.message), "Commit failed");
                }
                server.write_response(resp);
                break;

            case Proto::Op::Release:
                locks->unlock_write(req.id);
                resp.ok = true;
                std::snprintf(resp.message, sizeof(resp.message), "Write lock released");
                server.write_response(resp);
                break;

            case Proto::Op::Exit:
                resp.ok = true;
                std::snprintf(resp.message, sizeof(resp.message), "Bye");
                server.write_response(resp);
                running = false;
                break;

            default:
                std::snprintf(resp.message, sizeof(resp.message), "Unknown operation");
                server.write_response(resp);
                break;
            }
        }

        server.disconnect();

        {
            std::lock_guard<std::mutex> g(index_mtx);
            free_indices.insert(client_index);
        }

        int prev = active_clients.fetch_sub(1) - 1;
        std::cout << "Client " << client_index << " disconnected. Active clients: " << prev << '\n';

        if (prev == 0) {
            std::lock_guard<std::mutex> lk(mtx);
            cv_zero.notify_one();
        }
    }

    void accept_loop(FileStorage* storage, PerRecordRWLock* locks) {
        while (!shutdown_requested.load()) {
            NamedPipeServer server;
            server.create();
            server.wait_for_client();

            if (shutdown_requested.load()) {
                Proto::Response resp{};
                resp.ok = false;
                std::snprintf(resp.message, sizeof(resp.message), "Shutdown");
                server.write_response(resp);
                server.disconnect();
                continue;
            }

            int client_index = -1;
            {
                std::lock_guard<std::mutex> g(index_mtx);
                if (free_indices.empty()) {
                    Proto::Response resp{};
                    resp.ok = false;
                    std::snprintf(resp.message, sizeof(resp.message), "Server is full. Please try again later.");
                    server.write_response(resp);
                    server.disconnect();
                    continue;
                }
                client_index = *free_indices.begin();
                free_indices.erase(client_index);
            }

            active_clients.fetch_add(1);
            session_active.store(true);

            std::thread t([&](NamedPipeServer s, int idx) {
                serve_client(storage, locks, std::move(s), idx);
                }, std::move(server), client_index);
            t.detach();
        }
    }
}

int run_server() {
    try {
        std::cout << "=== Server ===\n";
        std::cout << prompt_file();
        std::string path;
        std::cin >> path;

        FileStorage storage(path);
        const auto init = read_initial_employees();
        storage.create_with_employees(init);
        print_file(storage);

        max_clients = Console::prompt_int("Enter number of client processes: ");
        if (max_clients < 1 || max_clients > static_cast<int32_t>(Proto::MAX_CLIENTS)) {
            std::cerr << "Client count must be in [1.." << Proto::MAX_CLIENTS << "]\n";
            return 1;
        }

        {
            std::lock_guard<std::mutex> g(index_mtx);
            for (int i = 1; i <= max_clients; ++i)
                free_indices.insert(i);
        }

        PerRecordRWLock locks;
        std::thread accept_thread([&] { accept_loop(&storage, &locks); });

        while (!shutdown_requested.load()) {
            std::unique_lock<std::mutex> lk(mtx);
            cv_zero.wait(lk, [&] {
                return shutdown_requested.load() || (session_active.load() && active_clients.load() == 0);
                });
            lk.unlock();

            if (shutdown_requested.load()) break;

            if (session_active.load() && active_clients.load() == 0) {
                std::cout << "\n=== All clients disconnected ===\n";
                std::cout << "Modified file:\n";
                print_file(storage);
                std::cout << "\nType 'quit' to terminate server: ";
                std::string cmd;
                std::cin >> cmd;
                if (cmd == "quit") {
                    shutdown_requested.store(true);
                    std::cout << "Server is shutting down...\n";
                    std::cout << "Press Enter to close the window...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                else {
                    session_active.store(false);
                }
            }
        }

        shutdown_requested.store(true);
        if (accept_thread.joinable()) accept_thread.join();

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Server error: " << ex.what() << '\n';
        return 1;
    }
}