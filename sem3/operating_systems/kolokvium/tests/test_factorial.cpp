#include <gtest/gtest.h>
#include "factorial.hpp"

TEST(Factorial, FirstFive) {
    auto v = app::math::first_n_factorials(5);
    EXPECT_EQ(v[0].to_string(), "1");
    EXPECT_EQ(v[1].to_string(), "2");
    EXPECT_EQ(v[2].to_string(), "6");
    EXPECT_EQ(v[3].to_string(), "24");
    EXPECT_EQ(v[4].to_string(), "120");
}

TEST(Factorial, ThrowsOnZero) {
    EXPECT_THROW(app::math::first_n_factorials(0), app::InvalidArgumentError);
}

TEST(Factorial, Single) {
    auto v = app::math::first_n_factorials(1);
    ASSERT_EQ(v.size(), 1);
    EXPECT_EQ(v[0].to_string(), "1");
}

TEST(Factorial, LargeFormatting25) {
    auto v = app::math::first_n_factorials(25);
    auto s = v.back().to_string();
    ASSERT_EQ(s, "15511210043330985984000000");
}

TEST(Factorial, MulZeroKeepsZero) {
    app::math::BigInt z(0);
    z.mul(0);
    EXPECT_EQ(z.to_string(), "0");
}

TEST(Factorial, TooManyFactorials) {
    EXPECT_THROW(app::math::first_n_factorials(app::math::MAX_FACTORIALS + 1),
        app::InvalidArgumentError);
}

TEST(Factorial, AtLimit) {
    auto v = app::math::first_n_factorials(app::math::MAX_FACTORIALS);
    ASSERT_EQ(v.size(), app::math::MAX_FACTORIALS);
    EXPECT_EQ(v[0].to_string(), "1");
}

TEST(Factorial, JustBelowLimit) {
    auto v = app::math::first_n_factorials(app::math::MAX_FACTORIALS - 1);
    ASSERT_EQ(v.size(), app::math::MAX_FACTORIALS - 1);
}