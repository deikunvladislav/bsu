#include "utils.h"
#include <iostream>
#include <limits>
#include <stdexcept>

void readInput(std::vector<int>& numbers) {
    std::cout << "Enter array size: ";
    int size;
    std::cin >> size;

    if (size <= 0 || size > 1000) {
        throw std::invalid_argument("Invalid array size");
    }

    numbers.resize(size);
    std::cout << "Enter " << size << " integers:\n";
    for (int i = 0; i < size; ++i) {
        std::cin >> numbers[i];
        if (std::cin.fail()) {
            throw std::invalid_argument("Invalid input");
        }
    }
}

void replaceMinMaxWithAverage(SharedData& data) {
    for (auto& x : data.numbers) {
        if (x == data.minValue || x == data.maxValue) {
            x = static_cast<int>(data.averageValue);
        }
    }
}


void printArray(const std::vector<int>& numbers) {
    std::cout << "Modified array: ";
    for (size_t i = 0; i < numbers.size(); ++i) {
        std::cout << numbers[i] << " ";
    }
    std::cout << "\n";
}