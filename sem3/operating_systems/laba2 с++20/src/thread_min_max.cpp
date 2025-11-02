#include <algorithm>
#include <ranges>
#include <thread>
#include <chrono>
#include <iostream>
#include "shared_data.h"

void findMinMax(SharedData& data) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if (!data.numbers.empty()) {
        auto [minIt, maxIt] = std::ranges::minmax_element(data.numbers);
        data.minValue = *minIt;
        data.maxValue = *maxIt;
        std::cout << "Thread MinMax: min = " << data.minValue
            << ", max = " << data.maxValue << "\n";
    }
}