#include <gtest/gtest.h>
#include "employee.hpp"

TEST(Employee, NameClippingAndRetrieval) {
    Employee e{};
    e.set_name("ABCDEFGHIJK");
    EXPECT_EQ(e.get_name(), "ABCDEFGHIJ");
    e.set_name("Ivan");
    EXPECT_EQ(e.get_name(), "Ivan");
    EXPECT_EQ(e.get_name().size(), 4u);
}

TEST(Employee, SerializedSize) {
    EXPECT_EQ(Employee::serialized_size(),
        sizeof(int32_t) + 10 * sizeof(char) + sizeof(double));
}