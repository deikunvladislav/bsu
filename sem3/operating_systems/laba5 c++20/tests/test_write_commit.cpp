#include <gtest/gtest.h>
#include "protocol.hpp"
#include "pipe.hpp"
#include "file_storage.hpp"
#include "rw_lock.hpp"
#include "server_session.hpp"
#include <filesystem>
#include <random>
#include <string>

class MockServerTransportSimple : public IServerTransport {
public:
    std::vector<Proto::Request> requests_sent;
    std::vector<Proto::Response> responses_written;
    bool disconnected = false;

    bool write_response(const Proto::Response& resp) override {
        responses_written.push_back(resp);
        return true;
    }
    std::optional<Proto::Request> read_request() override {
        if (requests_sent.empty()) return std::nullopt;
        auto r = requests_sent.front();
        requests_sent.erase(requests_sent.begin());
        return r;
    }
    void disconnect() override { disconnected = true; }
};

static std::string make_temp_path(const char* base) {
    const auto tmp = std::filesystem::temp_directory_path();
    std::string name = std::to_string(std::random_device{}()) + "_" + base;
    return (tmp / name).string();
}

TEST(test_write_commit, FullFlow) {
    const std::string path = make_temp_path("write_commit.bin");
    FileStorage fs(path);
    Employee e{ 42, {}, 3.0 };
    e.set_name("OldName");
    fs.create_with_employees({ e });

    PerRecordRWLock locks;
    ServerState state;
    ServerSession session(fs, locks, state, 1);

    MockServerTransportSimple transport;
    Proto::Request start{};
    start.op = Proto::Op::WriteStart;
    start.id = 42;

    Proto::Request commit{};
    commit.op = Proto::Op::WriteCommit;
    commit.id = 42;
    Employee modified = e;
    modified.set_name("NewName");
    modified.hours = 12.5;
    commit.payload = modified;

    Proto::Request release{};
    release.op = Proto::Op::Release;
    release.id = 42;

    transport.requests_sent.push_back(start);
    transport.requests_sent.push_back(commit);
    transport.requests_sent.push_back(release);

    session.on_connect(transport);
    session.process(transport);
    session.on_disconnect();

    auto updated = fs.read_by_id(42);
    ASSERT_TRUE(updated.has_value());
    EXPECT_EQ(updated->get_name(), "NewName");
    EXPECT_EQ(updated->hours, 12.5);

    std::filesystem::remove(path);
}