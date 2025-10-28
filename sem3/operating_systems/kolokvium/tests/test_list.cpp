#include <gtest/gtest.h>
#include "singly_list.hpp"

TEST(List, Reverse) {
    auto head = app::list::from_vector(std::vector<int>{1, 2, 3});
    auto rev = app::list::reverse_recursive<int>(std::move(head));
    auto out = app::list::to_vector(rev);
    EXPECT_EQ(out, (std::vector<int>{3, 2, 1}));
}

TEST(List, ReverseEmpty) {
    app::list::NodePtr<int> head;
    auto rev = app::list::reverse_recursive<int>(std::move(head));
    EXPECT_TRUE(app::list::to_vector(rev).empty());
}

TEST(List, ReverseSingle) {
    auto head = app::list::from_vector(std::vector<int>{42});
    auto rev = app::list::reverse_recursive<int>(std::move(head));
    EXPECT_EQ(app::list::to_vector(rev), (std::vector<int>{42}));
}

TEST(List, ReverseTwoElements) {
    auto head = app::list::from_vector(std::vector<int>{10, 20});
    auto rev = app::list::reverse_recursive<int>(std::move(head));
    EXPECT_EQ(app::list::to_vector(rev), (std::vector<int>{20, 10}));
}

TEST(List, ReverseTwiceIdentity) {
    auto head = app::list::from_vector(std::vector<int>{1, 2, 3, 4});
    auto rev1 = app::list::reverse_recursive<int>(std::move(head));
    auto rev2 = app::list::reverse_recursive<int>(std::move(rev1));
    EXPECT_EQ(app::list::to_vector(rev2), (std::vector<int>{1, 2, 3, 4}));
}