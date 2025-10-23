#include <gtest/gtest.h>
#include "marker.hpp"
#include "shared_array.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>

struct MarkerTestEnv {
    SharedArray arr;
    std::mutex start_mtx;
    std::condition_variable start_cv;
    bool start_flag{ false };

    MarkerTestEnv(size_t n) : arr(n) {}
};

TEST(MarkerTest, SingleMarkerTerminatesAndCleans) {
    MarkerTestEnv env(10);
    Marker m1(1, &env.arr, &env.start_cv, &env.start_mtx, &env.start_flag);

    m1.start();
    {
        std::lock_guard<std::mutex> lk(env.start_mtx);
        env.start_flag = true;
    }
    env.start_cv.notify_all();

    m1.waitBlocked();

    m1.signalTerminate();
    m1.join();

    for (size_t i = 0; i < env.arr.size(); ++i) {
        EXPECT_EQ(env.arr.get(i), 0);
    }
}

TEST(MarkerTest, TwoMarkersOneTerminatesOtherContinues) {
    MarkerTestEnv env(20);
    Marker m1(1, &env.arr, &env.start_cv, &env.start_mtx, &env.start_flag);
    Marker m2(2, &env.arr, &env.start_cv, &env.start_mtx, &env.start_flag);

    m1.start();
    m2.start();
    {
        std::lock_guard<std::mutex> lk(env.start_mtx);
        env.start_flag = true;
    }
    env.start_cv.notify_all();

    m1.waitBlocked();
    m2.waitBlocked();

    m1.signalTerminate();
    m1.join();

    m2.signalContinue();

    m2.waitBlocked();
    m2.signalTerminate();
    m2.join();

    for (size_t i = 0; i < env.arr.size(); ++i) {
        EXPECT_TRUE(env.arr.get(i) == 0 || env.arr.get(i) == 2);
    }
}