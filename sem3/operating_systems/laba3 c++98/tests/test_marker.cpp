#define NOMINMAX
#include "shared_array.hpp"
#include "marker.hpp"
#include <windows.h>
#include <iostream>

int main() {
    SharedArray arr(10);
    HANDLE startEvent = CreateEventA(NULL, TRUE, FALSE, NULL);

    Marker* m1 = new Marker(1, arr, startEvent);
    Marker* m2 = new Marker(2, arr, startEvent);

    m1->start();
    m2->start();

    SetEvent(startEvent);

    m1->waitBlocked();
    m2->waitBlocked();

    m1->signalTerminate();
    m1->join();

    m2->signalContinue();
    m2->waitBlocked();
    m2->signalTerminate();
    m2->join();

    bool ok = true;
    for (size_t i = 0; i < arr.size(); ++i) {
        int v = arr.get(i);
        if (!(v == 0 || v == 2)) {
            ok = false;
        }
    }

    delete m1;
    delete m2;
    CloseHandle(startEvent);

    if (ok) {
        std::cout << "Marker test passed\n";
        return 0;
    }
    else {
        std::cout << "Marker test failed\n";
        return 1;
    }
}