#pragma once
#include <vector>
#include <atomic>
#include <sync/sync_win.hpp>
#include <core/shared_array.hpp>

class Marker {
public:
    Marker(int id, SharedArray* arr, CriticalSection* cs, Event* start);
    void run();
    void signalContinue();
    void signalTerminate();
    void waitBlocked();
    void resetBlocked();
    bool isRunning() const;
    int id() const noexcept { return id_; }
private:
    int id_;
    SharedArray* arr_;
    CriticalSection* cs_;
    Event* start_;
    Event blocked_;
    Event cont_;
    Event term_;
    std::atomic<bool> running_;
    std::vector<std::size_t> marked_;
};
