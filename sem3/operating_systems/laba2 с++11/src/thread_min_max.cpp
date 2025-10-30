#include "shared_data.h"
#include <windows.h>
#include <iostream>

DWORD WINAPI findMinMax(LPVOID lpParam) {
    SharedData* data = static_cast<SharedData*>(lpParam);
    if (!data || data->numbers.empty()) return 1;

    int min = data->numbers[0];
    int max = data->numbers[0];

    for (size_t i = 1; i < data->numbers.size(); ++i) {
        if (data->numbers[i] < min) min = data->numbers[i];
        if (data->numbers[i] > max) max = data->numbers[i];
        Sleep(7);
    }

    data->minValue = min;
    data->maxValue = max;

    std::cout << "Min: " << min << ", Max: " << max << "\n";
    return 0;
}