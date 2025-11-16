#include <gtest/gtest.h>
#include "core/byte_io.h"

TEST(ByteIOTest, WriteAndReadAt) {
    core::ByteIO io("test_io.bin", true);
    ASSERT_TRUE(io.good());

    const char data[] = "hello";
    EXPECT_TRUE(io.write_at(0, data, sizeof(data)));

    char buf[6] = {};
    EXPECT_TRUE(io.read_at(0, buf, sizeof(data)));
    EXPECT_STREQ(buf, "hello");
}

TEST(ByteIOTest, ResizeFile) {
    core::ByteIO io("test_resize.bin", true);
    ASSERT_TRUE(io.good());

    EXPECT_TRUE(io.resize(128));
    EXPECT_GE(io.file_size(), 128u);
}

TEST(ByteIOTest, ErrorOnInvalidRead) {
    core::ByteIO io("test_err.bin", true);
    ASSERT_TRUE(io.good());

    char buf[10];
    EXPECT_FALSE(io.read_at(9999, buf, sizeof(buf)));
    EXPECT_FALSE(io.error().empty());
}