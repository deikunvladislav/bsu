#include <gtest/gtest.h>
#include "shared_data.h"
#include "utils.h"
#include <sstream>

TEST(InputTest, NegativeSizeThrows) {
    SharedData data;
    std::istringstream fakeInput("-5\n1 2 3");
    std::cin.rdbuf(fakeInput.rdbuf());
    EXPECT_THROW(readInput(data.numbers), std::invalid_argument);
}

TEST(InputTest, OversizedArrayThrows) {
    SharedData data;
    std::ostringstream oss;
    oss << "1001\n";
    for (int i = 0; i < 1001; ++i) oss << i << " ";
    std::istringstream fakeInput(oss.str());
    std::cin.rdbuf(fakeInput.rdbuf());
    EXPECT_THROW(readInput(data.numbers), std::invalid_argument);
}

TEST(InputTest, NonIntegerThrows) {
    SharedData data;
    std::istringstream fakeInput("3\n1 two 3");
    std::cin.rdbuf(fakeInput.rdbuf());
    EXPECT_THROW(readInput(data.numbers), std::invalid_argument);
}