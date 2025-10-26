#pragma once
#include "shared_array.hpp"
#include <iostream>
#include <iomanip>
#include <string>

inline void printArray(const SharedArray& arr) {
    const std::size_t n = arr.size();
    for (std::size_t i = 0; i < n; ++i) {
        std::cout << std::setw(2) << arr.get(i) << ' ';
    }
    std::cout << '\n';
}

inline void printState(const SharedArray& arr, const std::string& msg) {
    std::cout << msg << '\n';
    printArray(arr);
    std::cout << std::string(40, '-') << '\n';
}