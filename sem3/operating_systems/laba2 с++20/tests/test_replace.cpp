#include <gtest/gtest.h>
#include "shared_data.h"
#include "threads.h"

TEST(ReplaceTest, ReplacesMinMaxWithAverage) {
    SharedData data;
    data.numbers = { 5, 10, 20, 5, 20 };
    data.minValue = 5;
    data.maxValue = 20;
    data.averageValue = 13.0;

    replaceMinMaxWithAverage(data);

    std::vector<int> expected = { 13, 10, 13, 13, 13 };
    EXPECT_EQ(data.numbers, expected);
}

TEST(ReplaceTest, NoMinMaxNoChange) {
    SharedData data;
    data.numbers = { 1, 2, 3 };
    data.minValue = 100;
    data.maxValue = 200;
    data.averageValue = 50.0;

    replaceMinMaxWithAverage(data);

    std::vector<int> expected = { 1, 2, 3 };
    EXPECT_EQ(data.numbers, expected);
}

TEST(ReplaceTest, EmptyArrayNoCrash) {
    SharedData data;
    data.numbers = {};
    data.minValue = 0;
    data.maxValue = 0;
    data.averageValue = 0.0;

    EXPECT_NO_THROW(replaceMinMaxWithAverage(data));
    EXPECT_TRUE(data.numbers.empty());
}