#define NOMINMAX
#include "shared_array.hpp"
#include "marker.hpp"
#include <windows.h>
#include <iostream>
#include <boost/make_shared.hpp>

int main() {
    SharedArray arr(10);
    HANDLE startEvent = CreateEventA(NULL, TRUE, FALSE, NULL);

    Marker::Ptr m1 = boost::make_shared<Marker>(1, arr, startEvent);
    Marker::Ptr m2 = boost::make_shared<Marker>(2, arr, startEvent);

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