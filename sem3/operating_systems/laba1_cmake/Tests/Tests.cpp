#include <gtest/gtest.h>
#include "employee.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <vector>

employee make(int num, const char* name, double hours) {
    employee e{};
    e.num = num;
    std::memset(e.name, 0, sizeof(e.name));
    std::strncpy(e.name, name, sizeof(e.name) - 1);
    e.hours = hours;
    return e;
}

TEST(EmployeeIO, BinaryReadWritePreservesData) {
    employee original = make(42, "TestUser", 1234.56);
    std::ofstream out("test.bin", std::ios::binary);
    original.write(out);
    out.close();

    std::ifstream in("test.bin", std::ios::binary);
    employee loaded;
    loaded.read(in);
    in.close();

    std::remove("test.bin");
    EXPECT_EQ(original, loaded);
}

TEST(EmployeeIO, TextWriteMatchesStreamOutput) {
    employee e = make(1, "Alice", 5000.0);
    std::ostringstream oss;
    e.writeText(oss);
    EXPECT_EQ(oss.str(), "1\tAlice\t5000\n");
}

TEST(EmployeeOperators, StreamInputParsesCorrectly) {
    std::istringstream iss("99 John 12.5");
    employee e;
    iss >> e;
    EXPECT_EQ(e.num, 99);
    EXPECT_STREQ(e.name, "John");
    EXPECT_DOUBLE_EQ(e.hours, 12.5);
}

TEST(EmployeeOperators, StreamOutputFormatsCorrectly) {
    employee e = make(2, "Bob", 6000.0);
    std::ostringstream oss;
    oss << e;
    EXPECT_EQ(oss.str(), "2\tBob\t6000");
}

TEST(EmployeeEquality, OperatorEqualsWorks) {
    employee a = make(1, "X", 100.0);
    employee b = make(1, "X", 100.0);
    EXPECT_TRUE(a == b);
}

TEST(EmployeeEquality, OperatorNotEqualsWorks) {
    employee a = make(1, "X", 100.0);
    employee b = make(2, "Y", 200.0);
    EXPECT_FALSE(a == b);
}

TEST(EmployeeBinaryIO, WriteAndReadViaStream) {
    employee original = make(123, "StreamGuy", 42.42);
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    original.write(ss);
    employee loaded;
    loaded.read(ss);
    EXPECT_EQ(original, loaded);
}

TEST(EmployeeBinaryIO, WriteProducesExactSize) {
    employee e = make(1, "SizeTest", 1.0);
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    e.write(ss);
    std::string buf = ss.str();
    EXPECT_EQ(buf.size(), sizeof(employee));
}

TEST(EmployeeBinaryIO, ReadParsesRawBytesCorrectly) {
    employee expected = make(7, "RawTest", 3.1);
    std::stringstream ss(std::ios::in | std::ios::out | std::ios::binary);
    ss.write(reinterpret_cast<const char*>(&expected), sizeof(employee));
    ss.seekg(0);
    employee actual;
    actual.read(ss);
    EXPECT_EQ(actual, expected);
}

TEST(EmployeeReport, GeneratesCorrectFormattedReport) {
    const std::string binFile = "test_report.bin";
    const std::string txtFile = "test_report.txt";
    const double rate = 100.0;

    std::vector<employee> data = {
        make(1, "Alice", 5.0),
        make(2, "Bob", 8.0)
    };

    {
        std::ofstream out(binFile, std::ios::binary);
        for (const auto& e : data) {
            out.write(reinterpret_cast<const char*>(&e), sizeof(employee));
        }
    }

    employee::writeReport(binFile, txtFile, rate);

    std::ifstream in(txtFile);
    ASSERT_TRUE(in.is_open());

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }

    EXPECT_GE(lines.size(), 4);
    EXPECT_EQ(lines[0], "Report based on file: " + binFile);
    EXPECT_EQ(lines[1], "Number\tName\tHours\tSalary");

    EXPECT_TRUE(lines[2].find("Alice") != std::string::npos);
    EXPECT_TRUE(lines[2].find("500") != std::string::npos);
    EXPECT_TRUE(lines[3].find("Bob") != std::string::npos);
    EXPECT_TRUE(lines[3].find("800") != std::string::npos);

    std::remove(binFile.c_str());
    std::remove(txtFile.c_str());
}

TEST(EmployeeReport, HandlesEmptyBinaryFile) {
    const std::string binFile = "empty.bin";
    const std::string txtFile = "empty_report.txt";
    const double rate = 50.0;

    {
        std::ofstream out(binFile, std::ios::binary);
    }

    EXPECT_NO_THROW(employee::writeReport(binFile, txtFile, rate));

    std::ifstream in(txtFile);
    ASSERT_TRUE(in.is_open());

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(in, line)) {
        lines.push_back(line);
    }

    EXPECT_EQ(lines.size(), 2);
    EXPECT_EQ(lines[0], "Report based on file: " + binFile);
    EXPECT_EQ(lines[1], "Number\tName\tHours\tSalary");

    std::remove(binFile.c_str());
    std::remove(txtFile.c_str());
}

TEST(EmployeeReport, ThrowsOnMissingBinaryFile) {
    const std::string binFile = "nonexistent.bin";
    const std::string txtFile = "should_not_exist.txt";
    const double rate = 75.0;

    EXPECT_THROW(employee::writeReport(binFile, txtFile, rate), std::runtime_error);
}