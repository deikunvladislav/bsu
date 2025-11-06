#pragma once

#include <vector>
#include <windows.h>

/*
 * SharedData is used to pass information between threads.
 * It contains both computational results and thread handles.
 * Note: In unit tests, only a subset of these fields is used.
 * The thread-related members (HANDLEs) are included for consistency
 * with the multithreaded execution in the main application.
 */

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