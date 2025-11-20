#include <gtest/gtest.h>
#include "core/ring_queue_file.h"

TEST(RingQueueFileTest, InitializeAndPushPop) {
    auto init = core::RingQueueFile::initialize("queue_test.bin", 4);
    ASSERT_TRUE(init.has_value());
    core::RingQueueFile q("queue_test.bin");
    ASSERT_TRUE(q.ok());
    EXPECT_TRUE(q.try_push("msg1").has_value());
    EXPECT_TRUE(q.try_push("msg2").has_value());
    auto out1 = q.try_pop();
    ASSERT_TRUE(out1.has_value());
    EXPECT_EQ(*out1, "msg1");
    auto out2 = q.try_pop();
    ASSERT_TRUE(out2.has_value());
    EXPECT_EQ(*out2, "msg2");
}

TEST(RingQueueFileTest, FullAndEmpty) {
    auto init = core::RingQueueFile::initialize("queue_full.bin", 2);
    ASSERT_TRUE(init.has_value());
    core::RingQueueFile q("queue_full.bin");
    ASSERT_TRUE(q.ok());
    EXPECT_TRUE(q.try_push("a").has_value());
    EXPECT_TRUE(q.try_push("b").has_value());
    EXPECT_TRUE(q.is_full());
    auto out = q.try_pop();
    ASSERT_TRUE(out.has_value());
    EXPECT_FALSE(q.is_empty());
}

TEST(RingQueueFileTest, PopFromEmpty) {
    auto init = core::RingQueueFile::initialize("queue_empty.bin", 2);
    ASSERT_TRUE(init.has_value());
    core::RingQueueFile q("queue_empty.bin");
    ASSERT_TRUE(q.ok());
    auto out = q.try_pop();
    EXPECT_FALSE(out.has_value());
    EXPECT_TRUE(q.is_empty());
}

TEST(RingQueueFileTest, PushTooLongMessage) {
    auto init = core::RingQueueFile::initialize("queue_long.bin", 2);
    ASSERT_TRUE(init.has_value());
    core::RingQueueFile q("queue_long.bin");
    ASSERT_TRUE(q.ok());
    std::string longmsg(20, 'x');
    auto res = q.try_push(longmsg);
    EXPECT_FALSE(res.has_value());
}

TEST(RingQueueFileTest, BoundaryLengthAccepted) {
    auto init = core::RingQueueFile::initialize("queue_19.bin", 2);
    ASSERT_TRUE(init.has_value());
    core::RingQueueFile q("queue_19.bin");
    ASSERT_TRUE(q.ok());
    std::string msg(19, 'a');
    auto res = q.try_push(msg);
    EXPECT_TRUE(res.has_value());
    auto out = q.try_pop();
    ASSERT_TRUE(out.has_value());
    EXPECT_EQ(*out, msg);
}

TEST(RingQueueFileTest, FifoAcrossWrap) {
    auto init = core::RingQueueFile::initialize("queue_wrap.bin", 3);
    ASSERT_TRUE(init.has_value());
    core::RingQueueFile q("queue_wrap.bin");
    ASSERT_TRUE(q.ok());
    EXPECT_TRUE(q.try_push("m1").has_value());
    EXPECT_TRUE(q.try_push("m2").has_value());
    EXPECT_TRUE(q.try_push("m3").has_value());
    auto out1 = q.try_pop(); ASSERT_TRUE(out1.has_value()); EXPECT_EQ(*out1, "m1");
    EXPECT_TRUE(q.try_push("m4").has_value());
    auto out2 = q.try_pop(); ASSERT_TRUE(out2.has_value()); EXPECT_EQ(*out2, "m2");
    auto out3 = q.try_pop(); ASSERT_TRUE(out3.has_value()); EXPECT_EQ(*out3, "m3");
    auto out4 = q.try_pop(); ASSERT_TRUE(out4.has_value()); EXPECT_EQ(*out4, "m4");
}