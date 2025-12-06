#include <gtest/gtest.h>
#include "protocol.hpp"
#include "pipe.hpp"
#include "file_storage.hpp"
#include "rw_lock.hpp"
#include "server_session.hpp"
#include <filesystem>

class MockServerTransport : public IServerTransport {
public:
    bool first = true;
    Proto::Request req{};
    Proto::Response last{};
    bool disconnected = false;

    bool write_response(const Proto::Response& resp) override {
        last = resp;
        return true;
    }
    std::optional<Proto::Request> read_request() override {
        if (first) { first = false; return req; }
        return std::nullopt;
    }
    void disconnect() override { disconnected = true; }
};

TEST(test_pipe, ServerSessionHandlesReadBegin) {
    const std::string path = "mock_pipe_test.bin";
    FileStorage fs(path);
    Employee e{ 1, {}, 4.0 };
    e.set_name("Alice");
    fs.create_with_employees({ e });

    PerRecordRWLock locks;
    ServerState state;
    ServerSession session(fs, locks, state, 1);

    MockServerTransport transport;
    transport.req.op = Proto::Op::ReadBegin;
    transport.req.id = 1;

    session.on_connect(transport);
    session.process(transport);

    EXPECT_TRUE(transport.last.ok);
    EXPECT_STREQ(transport.last.message, "Read OK");

    std::filesystem::remove(path);
}