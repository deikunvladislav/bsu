#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include "shared_data.h"
#include "threads.h"

TEST(InputOutputTest, ReadsAndPrintsArray) {
    std::istringstream input("3\n1 2 3\n");
    std::ostringstream output;

    std::streambuf* cinBuf = std::cin.rdbuf();
    std::streambuf* coutBuf = std::cout.rdbuf();
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());

    SharedData data;
    int n;
    std::cin >> n;
    data.numbers.resize(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> data.numbers[i];
    }

    std::cout << "Result: ";
    for (int x : data.numbers) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    std::cin.rdbuf(cinBuf);
    std::cout.rdbuf(coutBuf);

    EXPECT_EQ(data.numbers, (std::vector<int>{1, 2, 3}));
    EXPECT_NE(output.str().find("Result: 1 2 3"), std::string::npos);
}