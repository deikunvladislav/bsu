#pragma once

#include "shared_array.hpp"
#include <iostream>
#include <iomanip>
#include <string>

inline void printArray(const SharedArray& arr) {
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::cout << std::setw(2) << arr.get(i) << ' ';
    }
    std::cout << '\n';
}

inline void printState(const SharedArray& arr, const std::string& msg) {
    std::cout << msg << '\n';
    printArray(arr);
    std::cout << std::string(40, '-') << '\n';
}