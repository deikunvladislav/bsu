#pragma once

#include <vector>
#include <windows.h>

struct SharedData {
    std::vector<int> numbers;
    int minValue;
    int maxValue;
    double averageValue;

    HANDLE hMinMaxThread;
    HANDLE hAverageThread;

    SharedData()
        : minValue(0), maxValue(0), averageValue(0.0),
        hMinMaxThread(NULL), hAverageThread(NULL) {
    }
};