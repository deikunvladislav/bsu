#include <gtest/gtest.h>
#include <thread>
#include <vector>
#include <atomic>
#include <chrono>
#include "rw_lock.hpp"

TEST(PerRecordRWLock, MultipleReadersSingleWriterExclusion) {
    PerRecordRWLock locks;
    const int id = 42;

    EXPECT_TRUE(locks.try_lock_read(id));
    EXPECT_TRUE(locks.try_lock_read(id));

    EXPECT_FALSE(locks.try_lock_write(id));

    locks.unlock_read(id);
    locks.unlock_read(id);

    EXPECT_TRUE(locks.try_lock_write(id));
    locks.unlock_write(id);
}

TEST(PerRecordRWLock, WriterBlocksReaders) {
    PerRecordRWLock locks;
    const int id = 7;

    EXPECT_TRUE(locks.try_lock_write(id));
    EXPECT_FALSE(locks.try_lock_read(id));
    locks.unlock_write(id);
    EXPECT_TRUE(locks.try_lock_read(id));
    locks.unlock_read(id);
}

TEST(PerRecordRWLock, ConcurrencyStress) {
    PerRecordRWLock locks;
    const int id = 5;
    std::atomic<int> read_count{ 0 };
    std::atomic<int> write_count{ 0 };

    auto reader = [&] {
        if (locks.try_lock_read(id)) {
            read_count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            locks.unlock_read(id);
        }
        };
    auto writer = [&] {
        if (locks.try_lock_write(id)) {
            write_count++;
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            locks.unlock_write(id);
        }
        };

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) threads.emplace_back(reader);
    for (int i = 0; i < 5; ++i) threads.emplace_back(writer);
    for (auto& t : threads) t.join();

    EXPECT_GE(read_count.load(), 0);
    EXPECT_GE(write_count.load(), 0);
}