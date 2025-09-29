#include <gtest/gtest.h>
#include "shared_data.h"
#include "threads.h"

TEST(MinMaxTest, FindsCorrectValues) {
    SharedData data;
    data.numbers = { 7, 2, 9, 4 };

    findMinMax(&data);
    EXPECT_EQ(data.minValue, 2);
    EXPECT_EQ(data.maxValue, 9);
}

TEST(MinMaxTest, EmptyArrayReturnsError) {
    SharedData data;
    data.numbers = {};

    DWORD result = findMinMax(&data);
    EXPECT_EQ(result, 1);
}