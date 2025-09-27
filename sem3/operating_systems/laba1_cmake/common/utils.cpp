#include "utils.h"
#include <fstream>
#include <stdexcept>
#include <windows.h>
#include <iostream>

namespace io {

    std::vector<employee> readBinaryFile(const std::string& filename) {
        std::ifstream ifs(filename, std::ios::binary);
        if (!ifs) throw std::runtime_error("Cannot open binary file for reading");

        std::vector<employee> result;
        employee temp;
        while (ifs.read(reinterpret_cast<char*>(&temp), sizeof(employee))) {
            result.push_back(temp);
        }
        return result;
    }

    void writeBinaryFile(const std::string& filename, const std::vector<employee>& data) {
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs) throw std::runtime_error("Cannot open binary file for writing");

        for (const auto& e : data) {
            ofs.write(reinterpret_cast<const char*>(&e), sizeof(employee));
        }
    }

    void writeReport(const std::string& binFile, const std::string& reportFile, double rate) {
        auto data = readBinaryFile(binFile);
        std::ofstream ofs(reportFile);
        if (!ofs) throw std::runtime_error("Cannot open report file");

        ofs << "Report based on file: " << binFile << "\n";
        ofs << "Number\tName\tHours\tSalary\n";
        for (const auto& e : data) {
            ofs << e.to_string(rate) << '\n';
        }
    }

    void runProcess(const std::string& exe, const std::string& args) {
        std::string command = exe + " " + args;
        STARTUPINFOA si = { sizeof(si) };
        PROCESS_INFORMATION pi;

        if (!CreateProcessA(nullptr, command.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            std::cerr << "Failed to start process: " << exe << '\n';
            exit(1);
        }

        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }

}