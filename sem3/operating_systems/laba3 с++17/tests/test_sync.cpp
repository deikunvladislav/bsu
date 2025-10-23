#include <gtest/gtest.h>
#include <thread>
#include <mutex>
#include <condition_variable>

TEST(SyncTest, MutexGuards) {
    std::mutex m;
    int counter = 0;

    auto worker = [&](int n) {
        for (int i = 0; i < n; ++i) {
            std::lock_guard<std::mutex> lk(m);
            counter++;
        }
        };

    std::thread t1(worker, 1000);
    std::thread t2(worker, 1000);
    t1.join();
    t2.join();

    EXPECT_EQ(counter, 2000);
}

TEST(SyncTest, ConditionVariableSignals) {
    std::mutex m;
    std::condition_variable cv;
    bool ready = false;

    std::thread t([&] {
        std::unique_lock<std::mutex> lk(m);
        cv.wait(lk, [&] { return ready; });
        EXPECT_TRUE(ready);
        });

    {
        std::lock_guard<std::mutex> lk(m);
        ready = true;
    }
    cv.notify_one();
    t.join();
}