#pragma once

#include <vector>
#include <windows.h>

struct SharedData {
    std::vector<int> numbers;
    int minValue = 0;
    int maxValue = 0;
    double averageValue = 0.0;

    HANDLE hMinMaxThread = nullptr;
    HANDLE hAverageThread = nullptr;
};