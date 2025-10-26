#include "shared_array.hpp"
#include "marker.hpp"
#include "print_utils.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <mutex>
#include <condition_variable>

namespace {

    int readPositiveInt(const std::string& prompt) {
        int v = 0;
        for (;;) {
            std::cout << prompt;
            if (std::cin >> v && v > 0) return v;
            std::cout << "Invalid input. Enter a positive integer.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    int readMarkerId(const std::string& prompt, int maxId) {
        int v = 0;
        for (;;) {
            std::cout << prompt;
            if (std::cin >> v && v >= 1 && v <= maxId) return v;
            std::cout << "Invalid marker id. Valid range: [1.." << maxId << "].\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

} 

int main() {
    try {
        const int arraySize = readPositiveInt("Enter array size: ");
        SharedArray arr(static_cast<std::size_t>(arraySize));

        const int markersCount = readPositiveInt("Enter number of markers: ");

        std::mutex start_mtx;
        std::condition_variable start_cv;
        bool start_flag = false;

        std::vector<std::unique_ptr<Marker>> markers;
        markers.reserve(static_cast<std::size_t>(markersCount));
        for (int id = 1; id <= markersCount; ++id) {
            markers.emplace_back(std::make_unique<Marker>(id, &arr, &start_cv, &start_mtx, &start_flag));
        }

        for (auto& m : markers) m->start();

        {
            std::lock_guard<std::mutex> lk(start_mtx);
            start_flag = true;
        }
        start_cv.notify_all();

        std::vector<bool> alive(static_cast<std::size_t>(markersCount), true);
        int aliveCount = markersCount;

        while (aliveCount > 0) {
            for (std::size_t i = 0; i < markers.size(); ++i) {
                if (!alive[i]) continue;
                markers[i]->waitBlocked();
            }

            printState(arr, "All active markers blocked");

            const int toTerminateId =
                readMarkerId("Enter marker id to terminate: ", markersCount);

            if (!alive[static_cast<std::size_t>(toTerminateId - 1)]) {
                std::cout << "Marker " << toTerminateId << " already terminated.\n";
                continue;
            }

            markers[static_cast<std::size_t>(toTerminateId - 1)]->signalTerminate();
            markers[static_cast<std::size_t>(toTerminateId - 1)]->join();
            alive[static_cast<std::size_t>(toTerminateId - 1)] = false;
            --aliveCount;

            printState(arr, std::string("Marker ") + std::to_string(toTerminateId) + " terminated");

            for (std::size_t i = 0; i < markers.size(); ++i) {
                if (!alive[i]) continue;
                markers[i]->signalContinue();
            }
        }

        std::cout << "All markers have finished.\n";

        printState(arr, "Final array state (all zeros):");

        std::cout << "Press Enter to exit...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin.get();

        return 0;
    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }
}