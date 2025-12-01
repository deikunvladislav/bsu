#include <gtest/gtest.h>
#include <filesystem>
#include "file_storage.hpp"

static std::string temp_path(const char* name) {
    auto p = std::filesystem::temp_directory_path() / name;
    return p.string();
}

TEST(FileStorage, CreateAndReadAll) {
    const auto path = temp_path("lab5_employees.bin");
    std::filesystem::remove(path);

    FileStorage fs(path);
    std::vector<Employee> init(3);
    init[0].num = 1; init[0].set_name("Ann"); init[0].hours = 10.5;
    init[1].num = 2; init[1].set_name("Bob"); init[1].hours = 12.0;
    init[2].num = 3; init[2].set_name("Cat"); init[2].hours = 7.25;

    fs.create_with_employees(init);

    const auto all = fs.read_all();
    ASSERT_EQ(all.size(), 3u);
    EXPECT_EQ(all[0].num, 1);
    EXPECT_EQ(all[0].get_name(), "Ann");
    EXPECT_DOUBLE_EQ(all[0].hours, 10.5);
    EXPECT_EQ(all[2].get_name(), "Cat");
}

TEST(FileStorage, ReadByIdFoundAndNotFound) {
    const auto path = temp_path("lab5_read_by_id.bin");
    std::filesystem::remove(path);

    FileStorage fs(path);
    Employee a{ 1 }; a.set_name("AAA"); a.hours = 1.0;
    Employee b{ 5 }; b.set_name("BBB"); b.hours = 2.0;
    fs.create_with_employees({ a, b });

    auto got1 = fs.read_by_id(1);
    ASSERT_TRUE(got1.has_value());
    EXPECT_EQ(got1->get_name(), "AAA");
    EXPECT_DOUBLE_EQ(got1->hours, 1.0);

    auto got2 = fs.read_by_id(5);
    ASSERT_TRUE(got2.has_value());
    EXPECT_EQ(got2->get_name(), "BBB");

    auto none = fs.read_by_id(99);
    EXPECT_FALSE(none.has_value());
}

TEST(FileStorage, UpdateByIdInPlace) {
    const auto path = temp_path("lab5_update.bin");
    std::filesystem::remove(path);

    FileStorage fs(path);
    Employee a{ 1 }; a.set_name("AAA"); a.hours = 1.0;
    Employee b{ 2 }; b.set_name("BBB"); b.hours = 2.0;
    fs.create_with_employees({ a, b });

    Employee upd{ 2 }; upd.set_name("B2"); upd.hours = 8.75;
    EXPECT_TRUE(fs.update_by_id(upd));

    auto got = fs.read_by_id(2);
    ASSERT_TRUE(got.has_value());
    EXPECT_EQ(got->get_name(), "B2");
    EXPECT_DOUBLE_EQ(got->hours, 8.75);

    Employee bad{ 999 }; bad.set_name("X"); bad.hours = 0.0;
    EXPECT_FALSE(fs.update_by_id(bad));
}

TEST(FileStorage, ErrorsOnBadPath) {
    std::string bad = std::filesystem::temp_directory_path().string();
    FileStorage fs(bad);
    EXPECT_THROW(fs.create_with_employees({}), std::runtime_error);

    FileStorage fs2("Z:\\this\\path\\does\\not\\exist\\employees.bin");
    EXPECT_THROW(fs2.read_all(), std::runtime_error);
    EXPECT_THROW(fs2.read_by_id(1), std::runtime_error);
    EXPECT_THROW(fs2.update_by_id(Employee{}), std::runtime_error);
}