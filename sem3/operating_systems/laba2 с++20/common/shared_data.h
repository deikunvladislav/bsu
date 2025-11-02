#pragma once
#include <vector>

struct SharedData {
    std::vector<int> numbers;
    int minValue = 0;
    int maxValue = 0;
    double averageValue = 0.0;
};