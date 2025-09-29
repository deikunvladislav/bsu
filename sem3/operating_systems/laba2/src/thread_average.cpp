#include "shared_data.h"
#include <windows.h>
#include <iostream>

DWORD WINAPI calculateAverage(LPVOID lpParam) {
    SharedData* data = static_cast<SharedData*>(lpParam);
    if (!data || data->numbers.empty()) return 1;

    long long sum = 0;
    for (int num : data->numbers) {
        sum += num;
        Sleep(12);
    }

    data->averageValue = static_cast<double>(sum) / data->numbers.size();
    std::cout << "Average: " << data->averageValue << "\n";
    return 0;
}