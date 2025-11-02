#include <gtest/gtest.h>
#include "shared_data.h"
#include "threads.h"

TEST(AverageTest, CalculatesCorrectAverage) {
    SharedData data;
    data.numbers = { 2, 4, 6, 8 };

    calculateAverage(data);

    EXPECT_DOUBLE_EQ(data.averageValue, 5.0);
}

TEST(AverageTest, HandlesSingleElement) {
    SharedData data;
    data.numbers = { 10 };

    calculateAverage(data);

    EXPECT_DOUBLE_EQ(data.averageValue, 10.0);
}

TEST(AverageTest, HandlesEmptyArray) {
    SharedData data;
    data.numbers = {};

    EXPECT_NO_THROW(calculateAverage(data));
    EXPECT_DOUBLE_EQ(data.averageValue, 0.0);
}