#include "employee.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Usage: Creator <binary_file> <count>\n";
        return 1;
    }
    std::string binFile = argv[1];
    int count = std::stoi(argv[2]);
    std::vector<employee> employees;
    for (int i = 0; i < count; ++i) {
        employee e;
        std::cout << "Enter employee #" << (i + 1) << " (num name hours): ";
        std::cin >> e;
        employees.push_back(e);
    }
    std::ofstream out(binFile, std::ios::binary);
    for (const auto& e : employees) {
        e.write(out);
    }
    return 0;
}