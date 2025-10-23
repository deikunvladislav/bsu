#include <gtest/gtest.h>
#include "shared_array.hpp"

TEST(SharedArrayTest, Initialization) {
    SharedArray arr(5);
    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_EQ(arr.get(i), 0);
    }
}

TEST(SharedArrayTest, SetAndGet) {
    SharedArray arr(3);
    arr.set(1, 42);
    EXPECT_EQ(arr.get(1), 42);
}

TEST(SharedArrayTest, OutOfRangeThrows) {
    SharedArray arr(2);
    EXPECT_THROW(arr.get(5), std::out_of_range);
    EXPECT_THROW(arr.set(5, 1), std::out_of_range);
}