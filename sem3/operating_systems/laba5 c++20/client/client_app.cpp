#include <iostream>
#include <string>
#include <limits>
#include "protocol.hpp"
#include "pipe.hpp"
#include "console_io.hpp"

static int safe_read_int() {
    int v;
    if (!(std::cin >> v)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    return v;
}

static void wait_enter(const char* prompt = "Press Enter to continue...") {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << prompt;
    std::cin.get();
}

static bool recv(NamedPipeClient& pipe, Proto::Response& out) {
    auto respOpt = pipe.receive_response();
    if (!respOpt.has_value()) {
        std::cerr << "[Client] Connection lost. Server closed.\n";
        return false;
    }
    const auto& resp = *respOpt;
    out = resp;
    if (std::string(resp.message) == "Shutdown") {
        std::cout << "[Client] Server is shutting down.\n";
        return false;
    }
    return true;
}

int run_client() {
    try {
        std::cout << "=== Client ===\n";

        NamedPipeClient pipe;
        if (!pipe.try_connect()) {
            std::cerr << "[Client] Server is busy. Please try again later.\n";
            wait_enter("Press Enter to exit...");
            return 1;
        }

        if (auto probeOpt = pipe.receive_response()) {
            const auto& p = *probeOpt;
            if (!p.ok) {
                if (std::string(p.message) == "Shutdown") {
                    std::cout << "[Client] Server is shutting down.\n";
                    return 0;
                }
                std::cerr << "[Client] " << p.message << '\n';
                wait_enter("Press Enter to exit...");
                return 1;
            }
            else {
                std::cout << "[Client] " << p.message << '\n';
            }
        }
        else {
            std::cerr << "[Client] Failed to receive initial response. Server closed.\n";
            wait_enter("Press Enter to exit...");
            return 1;
        }

        while (true) {
            std::cout << "\n=== Client Menu ===\n";
            std::cout << "1) Modify record\n";
            std::cout << "2) Read record\n";
            std::cout << "3) Exit\n";
            std::cout << "Choose option: ";
            const int opt = safe_read_int();
            if (opt == -1) {
                std::cout << "[Client] Invalid option.\n";
                continue;
            }

            if (opt == 1) {
                const int32_t id = Console::prompt_int("Enter ID to modify: ");
                Proto::Request req{};
                req.op = Proto::Op::WriteStart;
                req.id = id;
                pipe.send_request(req);

                Proto::Response resp{};
                if (!recv(pipe, resp)) {
                    wait_enter("Press Enter to exit...");
                    return 1;
                }
                if (!resp.ok) {
                    std::cout << "[Client] " << resp.message << '\n';
                    wait_enter();
                    continue;
                }

                std::cout << "[Client] " << resp.message << '\n';
                Console::print_employee(resp.payload);

                const auto name = Console::prompt_string("New name (up to 10 chars): ", 10);
                const double hrs = Console::prompt_double("New hours: ");
                resp.payload.set_name(name);
                resp.payload.hours = hrs;

                std::cout << "Press 'y' to commit changes, any other key to cancel: ";
                std::string confirm;
                std::cin >> confirm;
                if (confirm == "y" || confirm == "Y") {
                    Proto::Request commit{};
                    commit.op = Proto::Op::WriteCommit;
                    commit.id = id;
                    commit.payload = resp.payload;
                    pipe.send_request(commit);

                    if (!recv(pipe, resp)) {
                        wait_enter("Press Enter to exit...");
                        return 1;
                    }
                    std::cout << "[Client] " << resp.message << '\n';
                }

                Proto::Request release{};
                release.op = Proto::Op::Release;
                release.id = id;
                pipe.send_request(release);

                if (!recv(pipe, resp)) {
                    wait_enter("Press Enter to exit...");
                    return 1;
                }
                std::cout << "[Client] " << resp.message << '\n';

                wait_enter();
            }
            else if (opt == 2) {
                const int32_t id = Console::prompt_int("Enter ID to read: ");

                Proto::Request req{};
                req.op = Proto::Op::ReadBegin;
                req.id = id;
                pipe.send_request(req);

                Proto::Response resp{};
                if (!recv(pipe, resp)) {
                    wait_enter("Press Enter to exit...");
                    return 1;
                }
                if (!resp.ok) {
                    std::cout << "[Client] " << resp.message << '\n';
                    wait_enter();
                    continue;
                }

                std::cout << "[Client] " << resp.message << '\n';
                Console::print_employee(resp.payload);

                wait_enter("Press Enter to end reading process...");

                Proto::Request end{};
                end.op = Proto::Op::ReadEnd;
                end.id = id;
                pipe.send_request(end);

                if (!recv(pipe, resp)) {
                    wait_enter("Press Enter to exit...");
                    return 1;
                }
                std::cout << "[Client] " << resp.message << '\n';
            }
            else if (opt == 3) {
                Proto::Request req{};
                req.op = Proto::Op::Exit;
                pipe.send_request(req);

                if (auto respOpt = pipe.receive_response()) {
                    const auto& resp = *respOpt;
                    if (std::string(resp.message) == "Shutdown") {
                        std::cout << "[Client] Server is shutting down.\n";
                    }
                    else {
                        std::cout << "[Client] " << resp.message << '\n';
                    }
                }
                else {
                    std::cout << "[Client] Bye\n";
                }
                break;
            }
            else {
                std::cout << "[Client] Invalid option.\n";
            }
        }

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "[Client] Error: " << ex.what() << '\n';
        return 1;
    }
}