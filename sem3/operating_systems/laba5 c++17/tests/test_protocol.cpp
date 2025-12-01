#include <gtest/gtest.h>
#include "protocol.hpp"

TEST(Protocol, DefaultsAndConstants) {
    Proto::Request req{};
    EXPECT_EQ(req.op, Proto::Op::ReadBegin);
    EXPECT_EQ(req.id, 0);

    Proto::Response resp{};
    EXPECT_FALSE(resp.ok);
    EXPECT_EQ(sizeof(resp.message), 64u);

    EXPECT_EQ(Proto::PIPE_OUT_BUFFER, 1024u);
    EXPECT_EQ(Proto::PIPE_IN_BUFFER, 1024u);
    EXPECT_EQ(Proto::PIPE_TIMEOUT_MS, 5000u);
    EXPECT_EQ(Proto::MAX_CLIENTS, 16u);
}