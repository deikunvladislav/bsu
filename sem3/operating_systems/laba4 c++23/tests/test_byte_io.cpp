#include <gtest/gtest.h>
#include "core/byte_io.h"

TEST(ByteIOTest, WriteAndReadAt) {
    core::ByteIO io("test_io.bin", true);
    ASSERT_TRUE(io.good());
    auto res = io.write_at(0, "hello");
    EXPECT_TRUE(res.has_value());
    auto read = io.read_at(0, 5);
    ASSERT_TRUE(read.has_value());
    EXPECT_EQ(*read, "hello");
}

TEST(ByteIOTest, ResizeFile) {
    core::ByteIO io("test_resize.bin", true);
    ASSERT_TRUE(io.good());
    auto res = io.resize(128);
    EXPECT_TRUE(res.has_value());
    EXPECT_GE(io.file_size(), 128u);
}

TEST(ByteIOTest, ErrorOnInvalidRead) {
    core::ByteIO io("test_err.bin", true);
    ASSERT_TRUE(io.good());
    auto read = io.read_at(9999, 10);
    EXPECT_FALSE(read.has_value());
}

TEST(ByteIOTest, CreateIfMissingFalse) {
    core::ByteIO io("no_such_file.bin", false);
    EXPECT_FALSE(io.good());
}