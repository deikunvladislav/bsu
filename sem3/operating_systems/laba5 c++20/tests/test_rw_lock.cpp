#include <gtest/gtest.h>
#include "rw_lock.hpp"
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>

TEST(test_rw_lock, TryLockReadWrite) {
    PerRecordRWLock lock;
    int32_t id = 100;

    EXPECT_TRUE(lock.try_lock_read(id));
    lock.unlock_read(id);

    EXPECT_TRUE(lock.try_lock_write(id));
    lock.unlock_write(id);
}

TEST(test_rw_lock_concurrent, ReadersBlockWriter) {
    PerRecordRWLock lock;
    int32_t id = 555;

    const int readers = 4;
    std::atomic<int> started{ 0 };
    std::atomic<int> done{ 0 };
    std::atomic<bool> writer_acquired{ false };

    std::vector<std::thread> ths;
    for (int i = 0; i < readers; ++i) {
        ths.emplace_back([&] {
            if (lock.try_lock_read(id)) {
                started.fetch_add(1);
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
                lock.unlock_read(id);
                done.fetch_add(1);
            }
            });
    }

    while (started.load() < readers) std::this_thread::sleep_for(std::chrono::milliseconds(10));

    bool got = lock.try_lock_write(id);
    if (got) {
        lock.unlock_write(id);
        writer_acquired.store(true);
    }
    else {
        writer_acquired.store(false);
    }

    for (auto& t : ths) if (t.joinable()) t.join();

    bool got_after = lock.try_lock_write(id);
    if (got_after) lock.unlock_write(id);

    EXPECT_FALSE(writer_acquired.load());
    EXPECT_TRUE(got_after);
}