#include <gtest/gtest.h>
#include "util/string_trim.h"

TEST(UtilTest, TrimCopy) {
    std::string s = "   hello world   ";
    EXPECT_EQ(util::trim_copy(s), "hello world");
}

TEST(UtilTest, TrimEmpty) {
    std::string s = "   ";
    EXPECT_EQ(util::trim_copy(s), "");
}

TEST(UtilTest, TrimNoSpaces) {
    std::string s = "abc";
    EXPECT_EQ(util::trim_copy(s), "abc");
}