#ifndef OS_LAB5_SERVER_SESSION_HPP
#define OS_LAB5_SERVER_SESSION_HPP

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <set>
#include "protocol.hpp"
#include "pipe.hpp"
#include "file_storage.hpp"
#include "rw_lock.hpp"

struct ServerState {
    std::atomic<int>  active_clients{ 0 };
    std::atomic<bool> session_active{ false };
    std::atomic<bool> shutdown_requested{ false };
    std::mutex mtx;
    std::condition_variable cv_zero;
    std::mutex index_mtx;
    std::set<int> free_indices;
    int max_clients = 0;
};

class ServerSession {
public:
    ServerSession(FileStorage& storage, PerRecordRWLock& locks, ServerState& state, int client_index);
    void on_connect(IServerTransport& transport);
    void process(IServerTransport& transport);
    void on_disconnect();

    bool handle_request(const Proto::Request& req, Proto::Response& out);

private:
    FileStorage& storage_;
    PerRecordRWLock& locks_;
    ServerState& state_;
    int client_index_;
};

#endif