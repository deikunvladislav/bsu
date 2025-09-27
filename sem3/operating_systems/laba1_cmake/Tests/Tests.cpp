#include <gtest/gtest.h>
#include "../common/utils.h"
#include <fstream>
#include <cstdio>
#include <sstream>
using namespace std;

class EmployeeTests : public ::testing::Test {
protected:
    employee e{ 42, "Test", 8.5 };
};

TEST_F(EmployeeTests, SalaryCalculationIsCorrect) {
    EXPECT_DOUBLE_EQ(e.salary(10.0), 85.0);
}

TEST_F(EmployeeTests, ToStringFormatsCorrectly) {
    EXPECT_EQ(e.to_string(10.0), "42\tTest\t8.5\t85.00");
}

TEST_F(EmployeeTests, EqualityOperatorWorks) {
    employee e2{ 42, "Test", 8.5 };
    EXPECT_TRUE(e == e2);
}

TEST_F(EmployeeTests, InequalityDetected) {
    employee e2{ 43, "Test", 8.5 };
    EXPECT_FALSE(e == e2);
}

class FileIOTests : public ::testing::Test {
protected:
    string binFile = "test_io.bin";
    string reportFile = "test_io.txt";
    vector<employee> sample;

    void SetUp() override {
        sample = {
            {1, "Ivan", 10.0},
            {2, "Anna", 12.5}
        };
    }

    void TearDown() override {
        remove(binFile.c_str());
        remove(reportFile.c_str());
    }
};

TEST_F(FileIOTests, WriteAndReadBinaryFile) {
    io::writeBinaryFile(binFile, sample);
    auto result = io::readBinaryFile(binFile);
    EXPECT_EQ(result, sample);
}

TEST_F(FileIOTests, ReadEmptyBinaryFileReturnsEmptyVector) {
    ofstream ofs(binFile, ios::binary);
    ofs.close();
    auto result = io::readBinaryFile(binFile);
    EXPECT_TRUE(result.empty());
}

TEST_F(FileIOTests, ReadNonexistentBinaryFileThrows) {
    remove(binFile.c_str());
    EXPECT_THROW(io::readBinaryFile(binFile), runtime_error);
}

TEST_F(FileIOTests, WriteReportGeneratesCorrectContent) {
    io::writeBinaryFile(binFile, sample);
    io::writeReport(binFile, reportFile, 5.0);

    ifstream ifs(reportFile);
    ASSERT_TRUE(ifs.is_open());

    string line;
    vector<string> lines;
    while (getline(ifs, line)) {
        lines.push_back(line);
    }

    ASSERT_GE(lines.size(), 3);
    EXPECT_EQ(lines[0], "Report based on file: " + binFile );
    EXPECT_EQ(lines[1], "Number\tName\tHours\tSalary");
    EXPECT_NE(lines[2].find("Ivan"), string::npos);
    EXPECT_NE(lines[3].find("Anna"), string::npos);
}