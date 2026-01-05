#include "shared_data.h"
#include "utils.h"
#include "threads.h"
#include <iostream>
#include <windows.h>
#include <stdexcept>
#include <conio.h>

int main() {
    try {
        SharedData data;
        readInput(data.numbers);

        data.hMinMaxThread = CreateThread(NULL, 0, findMinMax, &data, 0, NULL);
        if (data.hMinMaxThread == NULL) {
            throw std::runtime_error("Failed to create min_max thread");
        }

        data.hAverageThread = CreateThread(NULL, 0, calculateAverage, &data, 0, NULL);
        if (data.hAverageThread == NULL) {
            CloseHandle(data.hMinMaxThread);
            throw std::runtime_error("Failed to create average thread");
        }

        WaitForSingleObject(data.hMinMaxThread, INFINITE);
        WaitForSingleObject(data.hAverageThread, INFINITE);

        CloseHandle(data.hMinMaxThread);
        CloseHandle(data.hAverageThread);

        replaceMinMaxWithAverage(data);
        printArray(data.numbers);

        std::cout << "\nPress any key to exit...";
        _getch();

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}