#include <gtest/gtest.h>
#include "dedup.hpp"

TEST(Dedup, Stable) {
    std::vector<int> in{ 3,3,2,1,2 };
    auto out = app::algo::stable_unique(in);
    std::vector<int> exp{ 3,2,1 };
    EXPECT_EQ(out, exp);
}

TEST(Dedup, Empty) {
    std::vector<int> in;
    auto out = app::algo::stable_unique(in);
    EXPECT_TRUE(out.empty());
}

TEST(Dedup, AllDuplicates) {
    std::vector<int> in(5, 7);
    auto out = app::algo::stable_unique(in);
    EXPECT_EQ(out, (std::vector<int>{7}));
}

TEST(Dedup, NegativesAndZeros) {
    std::vector<int> in{ 0, -1, 0, -1, -2, -2, 3 };
    auto out = app::algo::stable_unique(in);
    EXPECT_EQ(out, (std::vector<int>{0, -1, -2, 3}));
}

TEST(Dedup, Idempotent) {
    std::vector<int> in{ 1,1,2,2,3,3 };
    auto once = app::algo::stable_unique(in);
    auto twice = app::algo::stable_unique(once);
    EXPECT_EQ(once, twice);
}

struct Pair { int a; int b; };
struct PairHash {
    size_t operator()(const Pair& p) const {
        return std::hash<int>()(p.a) ^ (std::hash<int>()(p.b) << 1);
    }
};
struct PairEq {
    bool operator()(const Pair& x, const Pair& y) const {
        return x.a == y.a && x.b == y.b;
    }
};

TEST(Dedup, CustomType) {
    std::vector<Pair> in{ {1,2}, {1,2}, {2,3}, {1,2} };
    auto out = app::algo::stable_unique<Pair, PairHash, PairEq>(in);
    ASSERT_EQ(out.size(), 2);
    EXPECT_EQ(out[0].a, 1); EXPECT_EQ(out[0].b, 2);
    EXPECT_EQ(out[1].a, 2); EXPECT_EQ(out[1].b, 3);
}