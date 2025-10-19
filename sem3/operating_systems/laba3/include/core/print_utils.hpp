#pragma once
#include <iostream>
#include "shared_array.hpp"

inline void printArray(const SharedArray& arr) {
    std::cout << "Array: ";
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::cout << arr.raw()[i] << (i + 1 == arr.size() ? "" : " ");
    }
    std::cout << std::endl;
}
