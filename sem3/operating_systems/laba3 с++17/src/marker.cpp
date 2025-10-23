#include "marker.hpp"
#include "constants.hpp"

#include <random>
#include <chrono>
#include <iostream>

Marker::Marker(int id, SharedArray* arr,
    std::condition_variable* start_cv,
    std::mutex* start_mtx,
    bool* start_flag)
    : id_(id),
    arr_(arr),
    start_cv_(start_cv),
    start_mtx_(start_mtx),
    start_flag_(start_flag) {
}

void Marker::start() {
    worker_ = std::thread(&Marker::run, this);
}

void Marker::join() {
    if (worker_.joinable()) worker_.join();
}

void Marker::signalTerminate() {
    terminate_.store(true, std::memory_order_relaxed);
    {
        std::lock_guard<std::mutex> lk(block_mtx_);
        blocked_.store(false, std::memory_order_relaxed);
    }
    block_cv_.notify_all();
}

void Marker::signalContinue() {
    continue_signal_.store(true, std::memory_order_relaxed);
    {
        std::lock_guard<std::mutex> lk(block_mtx_);
        blocked_.store(false, std::memory_order_relaxed);
    }
    block_cv_.notify_all();
}

void Marker::waitBlocked() {
    std::unique_lock<std::mutex> lk(block_mtx_);
    block_cv_.wait(lk, [&] { return blocked_.load(std::memory_order_relaxed); });
}

std::optional<std::size_t> Marker::lastConflictIndex() const {
    auto v = last_conflict_.load(std::memory_order_relaxed);
    if (v == static_cast<std::size_t>(-1)) return std::nullopt;
    return v;
}

void Marker::run() {
    {
        std::unique_lock<std::mutex> lk(*start_mtx_);
        start_cv_->wait(lk, [&] { return *start_flag_; });
    }

    std::mt19937 rng(static_cast<std::mt19937::result_type>(id_));
    std::uniform_int_distribution<std::size_t> dist(0, arr_->size() - 1);

    while (!terminate_.load(std::memory_order_relaxed)) {
        std::size_t idx = dist(rng);

        if (arr_->get(idx) == 0) {
            std::this_thread::sleep_for(kPreWriteSleep);
            arr_->set(idx, id_);
            marked_count_.fetch_add(1, std::memory_order_relaxed);
            std::this_thread::sleep_for(kPostWriteSleep);
        }
        else {
            last_conflict_.store(idx, std::memory_order_relaxed);
            
            std::cout << "Marker " << id_
                << " blocked; marked=" << marked_count_.load()
                << "; conflict_index=" << idx << '\n';

            {
                std::lock_guard<std::mutex> lk(block_mtx_);
                blocked_.store(true, std::memory_order_relaxed);
            }
            block_cv_.notify_all();
            std::unique_lock<std::mutex> lk(block_mtx_);
            block_cv_.wait(lk, [&] {
                return terminate_.load(std::memory_order_relaxed) ||
                    continue_signal_.load(std::memory_order_relaxed);
                });

            if (terminate_.load(std::memory_order_relaxed)) break;
            continue_signal_.store(false, std::memory_order_relaxed);
        }

        std::this_thread::sleep_for(kLoopBackoff);
    }
    for (std::size_t i = 0; i < arr_->size(); ++i) {
        if (arr_->get(i) == id_) arr_->set(i, 0);
    }
    finished_.store(true, std::memory_order_relaxed);
}