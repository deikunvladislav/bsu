#include "employee.h"
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage: Reporter <binary_file> <report_file> <rate>\n";
        return 1;
    }
    try {
        employee::writeReport(argv[1], argv[2], std::stod(argv[3]));
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
    return 0;
}