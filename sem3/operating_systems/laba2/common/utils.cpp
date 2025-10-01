#include "utils.h"
#include <iostream>
#include <limits>

void readInput(std::vector<int>& numbers) {
    std::cout << "Enter array size: ";
    int size;
    std::cin >> size;

    if (size <= 0 || size > 1000) {
        throw std::invalid_argument("Invalid array size");
    }

    numbers.resize(size);
    std::cout << "Enter " << size << " integers:\n";
    for (int& num : numbers) {
        std::cin >> num;
        if (std::cin.fail()) {
            throw std::invalid_argument("Invalid input");
        }
    }
}

void replaceMinMaxWithAverage(SharedData& data) {
    for (int& num : data.numbers) {
        if (num == data.minValue || num == data.maxValue) {
            num = static_cast<int>(data.averageValue);
        }
    }
}

void printArray(const std::vector<int>& numbers) {
    std::cout << "Modified array: ";
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << "\n";
}