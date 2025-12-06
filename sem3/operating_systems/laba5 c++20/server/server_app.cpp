#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <set>
#include <limits>
#include "protocol.hpp"
#include "pipe.hpp"
#include "file_storage.hpp"
#include "rw_lock.hpp"
#include "console_io.hpp"
#include "server_session.hpp"

namespace {
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

    void serve_client(FileStorage& storage, PerRecordRWLock& locks, ServerState& state,
        NamedPipeServer server, int client_index)
    {
        ServerSession session(storage, locks, state, client_index);
        session.on_connect(server);
        std::cout << "Client " << client_index << " connected. Active clients: " << state.active_clients.load() << '\n';
        session.process(server);
        session.on_disconnect();
        std::cout << "Client " << client_index << " disconnected. Active clients: " << state.active_clients.load() << '\n';
    }

    void accept_loop(FileStorage& storage, PerRecordRWLock& locks, ServerState& state) {
        while (!state.shutdown_requested.load()) {
            NamedPipeServer server;
            server.create();
            server.wait_for_client();

            if (state.shutdown_requested.load()) {
                Proto::Response resp{};
                resp.ok = false;
                std::snprintf(resp.message, sizeof(resp.message), "Shutdown");
                server.write_response(resp);
                server.disconnect();
                continue;
            }

            int client_index = -1;
            {
                std::lock_guard<std::mutex> g(state.index_mtx);
                if (state.free_indices.empty()) {
                    Proto::Response resp{};
                    resp.ok = false;
                    std::snprintf(resp.message, sizeof(resp.message), "Server is full. Please try again later.");
                    server.write_response(resp);
                    server.disconnect();
                    continue;
                }
                client_index = *state.free_indices.begin();
                state.free_indices.erase(client_index);
            }

            state.active_clients.fetch_add(1);
            state.session_active.store(true);

            std::thread t([&](NamedPipeServer s, int idx) {
                serve_client(storage, locks, state, std::move(s), idx);
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

        ServerState state{};
        state.max_clients = Console::prompt_int("Enter number of client processes: ");
        if (state.max_clients < 1 || state.max_clients > static_cast<int32_t>(Proto::MAX_CLIENTS)) {
            std::cerr << "Client count must be in [1.." << Proto::MAX_CLIENTS << "]\n";
            return 1;
        }

        {
            std::lock_guard<std::mutex> g(state.index_mtx);
            for (int i = 1; i <= state.max_clients; ++i) state.free_indices.insert(i);
        }

        PerRecordRWLock locks;
        std::thread accept_thread([&] { accept_loop(storage, locks, state); });

        while (!state.shutdown_requested.load()) {
            std::unique_lock<std::mutex> lk(state.mtx);
            state.cv_zero.wait(lk, [&] {
                return state.shutdown_requested.load() || (state.session_active.load() && state.active_clients.load() == 0);
                });
            lk.unlock();

            if (state.shutdown_requested.load()) break;

            if (state.session_active.load() && state.active_clients.load() == 0) {
                std::cout << "\n=== All clients disconnected ===\n";
                std::cout << "Modified file:\n";
                print_file(storage);
                std::cout << "\nType 'quit' to terminate server: ";
                std::string cmd;
                std::cin >> cmd;
                if (cmd == "quit") {
                    state.shutdown_requested.store(true);
                    std::cout << "Server is shutting down...\n";
                    std::cout << "Press Enter to close the window...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.get();
                    break;
                }
                else {
                    state.session_active.store(false);
                }
            }
        }

        state.shutdown_requested.store(true);
        if (accept_thread.joinable()) accept_thread.join();

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Server error: " << ex.what() << '\n';
        return 1;
    }
}