#include <gtest/gtest.h>
#include "core/ring_queue_file.h"

TEST(RingQueueFileTest, InitializeAndPushPop) {
    std::string err;
    ASSERT_TRUE(core::RingQueueFile::initialize("queue_test.bin", 4, err));

    core::RingQueueFile q("queue_test.bin");
    ASSERT_TRUE(q.ok());

    EXPECT_TRUE(q.try_push("msg1"));
    EXPECT_TRUE(q.try_push("msg2"));

    std::string out;
    EXPECT_TRUE(q.try_pop(out));
    EXPECT_EQ(out, "msg1");

    EXPECT_TRUE(q.try_pop(out));
    EXPECT_EQ(out, "msg2");
}

TEST(RingQueueFileTest, FullAndEmpty) {
    std::string err;
    ASSERT_TRUE(core::RingQueueFile::initialize("queue_full.bin", 2, err));

    core::RingQueueFile q("queue_full.bin");
    ASSERT_TRUE(q.ok());

    EXPECT_TRUE(q.try_push("a"));
    EXPECT_TRUE(q.try_push("b"));
    EXPECT_TRUE(q.is_full());

    std::string out;
    EXPECT_TRUE(q.try_pop(out));
    EXPECT_FALSE(q.is_empty());
}

TEST(RingQueueFileTest, PopFromEmpty) {
    std::string err;
    ASSERT_TRUE(core::RingQueueFile::initialize("queue_empty.bin", 2, err));

    core::RingQueueFile q("queue_empty.bin");
    ASSERT_TRUE(q.ok());

    std::string out;
    EXPECT_FALSE(q.try_pop(out));
    EXPECT_TRUE(q.is_empty());
}

TEST(RingQueueFileTest, PushTooLongMessage) {
    std::string err;
    ASSERT_TRUE(core::RingQueueFile::initialize("queue_long.bin", 2, err));

    core::RingQueueFile q("queue_long.bin");
    ASSERT_TRUE(q.ok());

    std::string longmsg(100, 'x');
    EXPECT_FALSE(q.try_push(longmsg));
}