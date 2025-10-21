#include <gtest/gtest.h>
#include <thread>
#include "threads/marker.hpp"
#include "core/shared_array.hpp"
#include "sync/sync_win.hpp"

TEST(MarkerTest, MarksAndTerminates) {
    SharedArray arr(10);
    CriticalSection cs;
    Event start(true, false);

    Marker marker(1, &arr, &cs, &start);
    std::thread t([&] { marker.run(); });

    start.set();

    marker.waitBlocked();
    marker.resetBlocked();

    marker.signalTerminate();
    t.join();

    for (size_t i = 0; i < arr.size(); ++i) {
        EXPECT_TRUE(arr.get(i) == 0 || arr.get(i) != 1);
    }
}

TEST(MarkerTest, MultipleMarkers) {
    SharedArray arr(20);
    CriticalSection cs;
    Event start(true, false);

    Marker m1(1, &arr, &cs, &start);
    Marker m2(2, &arr, &cs, &start);

    std::thread t1([&] { m1.run(); });
    std::thread t2([&] { m2.run(); });

    start.set();

    m1.waitBlocked();
    m2.waitBlocked();

    m1.signalTerminate();
    t1.join();

    m2.signalTerminate();
    t2.join();

    for (size_t i = 0; i < arr.size(); ++i) {
        int v = arr.get(i);
        EXPECT_TRUE(v == 0 || v == 1 || v == 2);
    }
}

TEST(MarkerTest, FullArrayBlocks) {
    SharedArray arr(1);
    CriticalSection cs;
    Event start(true, false);

    Marker m1(1, &arr, &cs, &start);
    std::thread t([&] { m1.run(); });

    start.set();

    m1.waitBlocked();
    m1.signalTerminate();
    t.join();

    EXPECT_EQ(arr.get(0), 0);
}