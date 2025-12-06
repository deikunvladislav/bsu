#include <gtest/gtest.h>
#include "file_storage.hpp"
#include <filesystem>

TEST(test_file_storage, CreateAndReadAll) {
    const std::string path = "test_data.bin";
    FileStorage fs(path);
    std::vector<Employee> emps = { {1, {}, 5.0}, {2, {}, 8.0} };
    emps[0].set_name("Alice");
    emps[1].set_name("Bob");

    fs.create_with_employees(emps);
    auto read = fs.read_all();
    EXPECT_EQ(read.size(), 2);
    EXPECT_EQ(read[0].get_name(), "Alice");
    EXPECT_EQ(read[1].get_name(), "Bob");

    std::filesystem::remove(path);
}

TEST(test_file_storage, UpdateById) {
    const std::string path = "test_update.bin";
    FileStorage fs(path);
    Employee e{ 42, {}, 3.0 };
    e.set_name("Old");
    fs.create_with_employees({ e });

    e.set_name("New");
    e.hours = 9.5;
    EXPECT_TRUE(fs.update_by_id(e));

    auto updated = fs.read_by_id(42);
    ASSERT_TRUE(updated.has_value());
    EXPECT_EQ(updated->get_name(), "New");
    EXPECT_EQ(updated->hours, 9.5);

    std::filesystem::remove(path);
}