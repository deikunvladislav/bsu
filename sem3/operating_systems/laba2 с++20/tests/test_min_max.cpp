#include <gtest/gtest.h>
#include "shared_data.h"
#include "threads.h"

TEST(MinMaxTest, FindsCorrectMinAndMax) {
    SharedData data;
    data.numbers = { 7, 2, 9, 4, 5 };

    findMinMax(data);

    EXPECT_EQ(data.minValue, 2);
    EXPECT_EQ(data.maxValue, 9);
}

TEST(MinMaxTest, HandlesSingleElement) {
    SharedData data;
    data.numbers = { 42 };

    findMinMax(data);

    EXPECT_EQ(data.minValue, 42);
    EXPECT_EQ(data.maxValue, 42);
}

TEST(MinMaxTest, HandlesEmptyArray) {
    SharedData data;
    data.numbers = {};

    EXPECT_NO_THROW(findMinMax(data));
    EXPECT_EQ(data.minValue, 0);
    EXPECT_EQ(data.maxValue, 0);
}