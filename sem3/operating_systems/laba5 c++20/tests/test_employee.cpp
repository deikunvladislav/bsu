#include <gtest/gtest.h>
#include "employee.hpp"

TEST(test_employee, NameTruncation) {
    Employee e;
    e.set_name("VeryLongNameExceedingLimit");
    EXPECT_EQ(e.get_name(), "VeryLongNa");
}

TEST(test_employee, EqualityOperator) {
    Employee a{ 1, {}, 10.5 }, b{ 1, {}, 10.5 };
    a.set_name("John");
    b.set_name("John");
    EXPECT_TRUE(a == b);
}