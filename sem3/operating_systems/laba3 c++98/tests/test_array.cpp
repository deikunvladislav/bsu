#include "shared_array.hpp"
#include <iostream>

int main() {
    SharedArray arr(5);
    bool ok = true;

    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr.get(i) != 0) ok = false;
    }

    arr.set(1, 42);
    if (arr.get(1) != 42) ok = false;

    try {
        arr.get(100);
        ok = false;
    }
    catch (const std::out_of_range&) {
    }

    try {
        arr.set(100, 1);
        ok = false;
    }
    catch (const std::out_of_range&) {
    }

    if (ok) {
        std::cout << "SharedArray tests passed\n";
        return 0;
    }
    else {
        std::cout << "SharedArray tests failed\n";
        return 1;
    }
}