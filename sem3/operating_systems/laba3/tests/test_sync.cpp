#include <gtest/gtest.h>
#include <thread>
#include "sync/sync_win.hpp"

TEST(SyncTest, CriticalSectionGuards) {
    CriticalSection cs;
    int counter = 0;

    auto worker = [&](int n) {
        for (int i = 0; i < n; ++i) {
            cs.lock();
            counter++;
            cs.unlock();
        }
        };

    std::thread t1(worker, 1000);
    std::thread t2(worker, 1000);
    t1.join();
    t2.join();

    EXPECT_EQ(counter, 2000);
}

TEST(SyncTest, EventSignals) {
    Event ev(true, false);
    bool flag = false;

    std::thread t([&] {
        ev.wait();
        flag = true;
        });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_FALSE(flag);

    ev.set();
    t.join();

    EXPECT_TRUE(flag);
}