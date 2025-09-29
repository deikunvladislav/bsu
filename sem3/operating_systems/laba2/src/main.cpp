#include "shared_data.h"
#include "utils.h"
#include "threads.h"
#include <iostream>
#include <windows.h>
#include <stdexcept>

int main() {
    try {
        SharedData data;
        readInput(data.numbers);

        data.hMinMaxThread = CreateThread(nullptr, 0, findMinMax, &data, 0, nullptr);
        if (data.hMinMaxThread == nullptr) {
            throw std::runtime_error("Failed to create min_max thread");
        }

        data.hAverageThread = CreateThread(nullptr, 0, calculateAverage, &data, 0, nullptr);
        if (data.hAverageThread == nullptr) {
            CloseHandle(data.hMinMaxThread);
            throw std::runtime_error("Failed to create average thread");
        }

        WaitForSingleObject(data.hMinMaxThread, INFINITE);
        WaitForSingleObject(data.hAverageThread, INFINITE);

        CloseHandle(data.hMinMaxThread);
        CloseHandle(data.hAverageThread);

        replaceMinMaxWithAverage(data);
        printArray(data.numbers);

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}