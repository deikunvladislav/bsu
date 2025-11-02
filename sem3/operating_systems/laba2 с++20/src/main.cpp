#include <iostream>
#include <thread>
#include <vector>
#include "shared_data.h"
#include "threads.h"

int main() {
    SharedData data;

    std::cout << "Enter the number of elements: ";
    int n;
    std::cin >> n;
    data.numbers.resize(n);

    std::cout << "Enter the elements:\n";
    for (int i = 0; i < n; ++i) {
        std::cin >> data.numbers[i];
    }

    std::jthread t1(findMinMax, std::ref(data));
    std::jthread t2(calculateAverage, std::ref(data));

    t1.join();
    t2.join();

    replaceMinMaxWithAverage(data);

    std::cout << "Result: ";
    for (int x : data.numbers) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    return 0;
}