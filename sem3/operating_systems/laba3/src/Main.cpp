#define NOMINMAX
#include <iostream>
#include <vector>
#include <thread>
#include <limits>
#include <memory>
#include <core/shared_array.hpp>
#include <sync/sync_win.hpp>
#include <core/print_utils.hpp>
#include <threads/marker.hpp>

int main() {
    int n, m;
    std::cout << "Enter array size: ";
    std::cin >> n;
    SharedArray arr(n);
    CriticalSection cs;

    std::cout << "Enter markers count: ";
    std::cin >> m;

    Event globalStart(true, false);

    std::vector<std::unique_ptr<Marker>> markers;
    std::vector<std::thread> threads;

    for (int i = 1; i <= m; ++i) {
        auto mk = std::make_unique<Marker>(i, &arr, &cs, &globalStart);
        threads.emplace_back([ptr = mk.get()] { ptr->run(); });
        markers.push_back(std::move(mk));
    }

    globalStart.set();

    int active = m;
    while (active > 0) {
        for (auto& mk : markers) {
            if (mk->isRunning()) {
                mk->waitBlocked();
                mk->resetBlocked();
            }
        }

        printArray(arr);

        int killId;
        std::cout << "Enter marker id to terminate: ";
        std::cin >> killId;

        if (killId >= 1 && killId <= m && markers[killId - 1]->isRunning()) {
            markers[killId - 1]->signalTerminate();
            threads[killId - 1].join();
            active--;
            printArray(arr);
            for (auto& mk : markers) {
                if (mk->isRunning()) mk->signalContinue();
            }
        }
        else {
            std::cout << "Invalid id.\n";
        }
    }

    std::cout << "All markers finished.\n";
    std::cout << "Press Enter to exit...";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();

    return 0;
}
