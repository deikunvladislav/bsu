#include <numeric>
#include <thread>
#include <chrono>
#include <iostream>
#include "shared_data.h"

void calculateAverage(SharedData& data) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    if (!data.numbers.empty()) {
        double sum = std::accumulate(data.numbers.begin(), data.numbers.end(), 0.0);
        data.averageValue = sum / data.numbers.size();
        std::cout << "Thread Average: average = " << data.averageValue << "\n";
    }
}