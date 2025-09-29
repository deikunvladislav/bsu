#include <gtest/gtest.h>
#include "shared_data.h"
#include "threads.h"

TEST(AverageTest, CorrectCalculation) {
    SharedData data;
    data.numbers = { 10, 20, 30, 40 };

    calculateAverage(&data);
    EXPECT_DOUBLE_EQ(data.averageValue, 25.0);
}

TEST(AverageTest, EmptyArrayReturnsError) {
    SharedData data;
    data.numbers = {};

    DWORD result = calculateAverage(&data);
    EXPECT_EQ(result, 1);
}