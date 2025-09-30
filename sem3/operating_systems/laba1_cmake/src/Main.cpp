#include "utils.h"
#include <iostream>
#include <string>

int main() {
    std::string binFile, reportFile;
    int count;
    double rate;
    std::cout << "Enter binary file name: ";
    std::cin >> binFile;
    std::cout << "Enter number of employees: ";
    std::cin >> count;
    utils::runProcess("Creator.exe", binFile + " " + std::to_string(count));
    std::cout << "Enter report file name: ";
    std::cin >> reportFile;
    std::cout << "Enter hourly rate: ";
    std::cin >> rate;
    utils::runProcess("Reporter.exe", binFile + " " + reportFile + " " + std::to_string(rate));
    return 0;
}