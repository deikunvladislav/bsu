#pragma once
#include <string>
#include <vector>
#include "employee.h"

namespace io {
    std::vector<employee> readBinaryFile(const std::string& filename);
    void writeBinaryFile(const std::string& filename, const std::vector<employee>& data);
    void writeReport(const std::string& binFile, const std::string& reportFile, double rate);
    void runProcess(const std::string& exe, const std::string& args); 
}