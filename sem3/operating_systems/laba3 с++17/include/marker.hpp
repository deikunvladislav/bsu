#pragma once

#include "shared_array.hpp"
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <cstddef>
#include <optional>

class Marker {
public:
    Marker(int id, SharedArray* arr,
        std::condition_variable* start_cv,
        std::mutex* start_mtx,
        bool* start_flag);
    void start();
    void join();
    void signalTerminate();
    void signalContinue();
    void waitBlocked();
    int id() const noexcept { return id_; }
    std::size_t markedCount() const noexcept { return marked_count_.load(); }
    std::optional<std::size_t> lastConflictIndex() const;

private:
    int id_;
    SharedArray* arr_;
    std::condition_variable* start_cv_;
    std::mutex* start_mtx_;
    bool* start_flag_;
    std::thread worker_;
    std::atomic<bool> terminate_{ false };
    std::atomic<bool> continue_signal_{ false };
    std::atomic<std::size_t> marked_count_{ 0 };
    std::atomic<bool> blocked_{ false };
    std::atomic<bool> finished_{ false };
    std::atomic<std::size_t> last_conflict_{ static_cast<std::size_t>(-1) };
    std::mutex block_mtx_;
    std::condition_variable block_cv_;
    void run();
};