#include "server_session.hpp"
#include <cstdio>

ServerSession::ServerSession(FileStorage& storage, PerRecordRWLock& locks, ServerState& state, int client_index)
    : storage_(storage), locks_(locks), state_(state), client_index_(client_index) {
}

void ServerSession::on_connect(IServerTransport& transport) {
    Proto::Response hello{};
    hello.ok = true;
    std::snprintf(hello.message, sizeof(hello.message),
        "Connected. Client #%d. Active clients: %d", client_index_, state_.active_clients.load());
    transport.write_response(hello);
}

bool ServerSession::handle_request(const Proto::Request& req, Proto::Response& out) {
    out = Proto::Response{};
    out.ok = false;

    switch (req.op) {
    case Proto::Op::ReadBegin:
        if (!locks_.try_lock_read(req.id)) {
            std::snprintf(out.message, sizeof(out.message), "Record is locked by writer");
            return true;
        }
        if (auto rec = storage_.read_by_id(req.id)) {
            out.ok = true;
            out.payload = *rec;
            std::snprintf(out.message, sizeof(out.message), "Read OK");
        }
        else {
            locks_.unlock_read(req.id);
            std::snprintf(out.message, sizeof(out.message), "Record not found");
        }
        return true;

    case Proto::Op::ReadEnd:
        locks_.unlock_read(req.id);
        out.ok = true;
        std::snprintf(out.message, sizeof(out.message), "Read ended");
        return true;

    case Proto::Op::WriteStart:
        if (!locks_.try_lock_write(req.id)) {
            std::snprintf(out.message, sizeof(out.message), "Record is locked by readers/writer");
            return true;
        }
        if (auto rec = storage_.read_by_id(req.id)) {
            out.ok = true;
            out.payload = *rec;
            std::snprintf(out.message, sizeof(out.message), "Write lock acquired");
        }
        else {
            out.ok = false;
            std::snprintf(out.message, sizeof(out.message), "Record not found, lock held");
        }
        return true;

    case Proto::Op::WriteCommit:
        if (storage_.update_by_id(req.payload)) {
            out.ok = true;
            std::snprintf(out.message, sizeof(out.message), "Commit OK");
        }
        else {
            std::snprintf(out.message, sizeof(out.message), "Commit failed");
        }
        return true;

    case Proto::Op::Release:
        locks_.unlock_write(req.id);
        out.ok = true;
        std::snprintf(out.message, sizeof(out.message), "Write lock released");
        return true;

    case Proto::Op::Exit:
        out.ok = true;
        std::snprintf(out.message, sizeof(out.message), "Bye");
        return true;

    default:
        std::snprintf(out.message, sizeof(out.message), "Unknown operation");
        return true;
    }
}

void ServerSession::process(IServerTransport& transport) {
    bool running = true;
    while (running) {
        if (state_.shutdown_requested.load()) {
            Proto::Response resp{};
            resp.ok = false;
            std::snprintf(resp.message, sizeof(resp.message), "Shutdown");
            transport.write_response(resp);
            break;
        }
        auto reqOpt = transport.read_request();
        if (!reqOpt.has_value()) break;
        const Proto::Request& req = *reqOpt;
        Proto::Response resp{};
        if (!handle_request(req, resp)) {
            resp.ok = false;
            std::snprintf(resp.message, sizeof(resp.message), "Internal error");
        }
        transport.write_response(resp);
        if (req.op == Proto::Op::Exit) running = false;
    }
    transport.disconnect();
}

void ServerSession::on_disconnect() {
    {
        std::lock_guard<std::mutex> g(state_.index_mtx);
        state_.free_indices.insert(client_index_);
    }
    const int prev = state_.active_clients.fetch_sub(1) - 1;
    if (prev == 0) {
        std::lock_guard<std::mutex> lk(state_.mtx);
        state_.cv_zero.notify_one();
    }
}