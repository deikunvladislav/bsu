#include <gtest/gtest.h>
#include "console_io.hpp"
#include "file_storage.hpp"
#include <sstream>

TEST(test_console_io, PrintEmployeeFormat) {
    Employee e{ 7, {}, 12.5 };
    e.set_name("TestName");

    std::stringstream ss;
    FileStorage::print_to_console({ e }, ss);
    EXPECT_NE(ss.str().find("TestName"), std::string::npos);
}