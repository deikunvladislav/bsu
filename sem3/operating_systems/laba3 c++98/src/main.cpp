#define NOMINMAX
#include "shared_array.hpp"
#include "marker.hpp"
#include "print_utils.hpp"
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <sstream>
#include <boost/make_shared.hpp>

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
    HANDLE startEvent = NULL;
    try {
        const int arraySize = readPositiveInt("Enter array size: ");
        SharedArray arr(static_cast<std::size_t>(arraySize));

        const int markersCount = readPositiveInt("Enter number of markers: ");
        if (markersCount <= 0) {
            std::cout << "No markers to run.\n";
            return 0;
        }

        startEvent = CreateEventA(NULL, TRUE, FALSE, NULL);
        if (startEvent == NULL) {
            std::cerr << "Failed to create start event.\n";
            return 1;
        }

        std::vector<Marker::Ptr> markers;
        markers.reserve(markersCount);

        for (int id = 1; id <= markersCount; ++id) {
            Marker::Ptr m = boost::make_shared<Marker>(id, arr, startEvent);
            if (!m->start()) {
                std::cerr << "Failed to start marker " << id << ".\n";
                for (size_t j = 0; j < markers.size(); ++j) {
                    markers[j]->signalTerminate();
                    markers[j]->join();
                }
                CloseHandle(startEvent);
                return 1;
            }
            markers.push_back(m);
        }

        SetEvent(startEvent);

        std::vector<bool> alive(markersCount, true);
        int aliveCount = markersCount;

        while (aliveCount > 0) {
            for (size_t i = 0; i < markers.size(); ++i) {
                if (!alive[i]) continue;
                markers[i]->waitBlocked();
            }

            printState(arr, "All active markers are blocked");

            const int toTerminateId =
                readMarkerId("Enter marker id to terminate: ", markersCount);

            size_t idx = static_cast<size_t>(toTerminateId - 1);
            if (!alive[idx]) {
                std::cout << "Marker " << toTerminateId << " already terminated.\n";
                continue;
            }

            markers[idx]->signalTerminate();
            markers[idx]->join();
            alive[idx] = false;
            --aliveCount;

            std::ostringstream oss;
            oss << "Marker " << toTerminateId << " terminated";
            printState(arr, oss.str());

            for (size_t i = 0; i < markers.size(); ++i) {
                if (!alive[i]) continue;
                markers[i]->signalContinue();
            }
        }

        std::cout << "All markers have finished.\n";

        if (startEvent != NULL) {
            CloseHandle(startEvent);
            startEvent = NULL;
        }

        return 0;
    }
    catch (...) {
        std::cerr << "Unexpected error.\n";
        if (startEvent != NULL) {
            CloseHandle(startEvent);
            startEvent = NULL;
        }
        return 1;
    }
}