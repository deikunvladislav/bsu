#pragma once
#define NOMINMAX
#include "shared_array.hpp"
#include <windows.h>
#include <cstddef>
#include <string>

class Marker {
public:
    Marker(int id, SharedArray& arr, HANDLE startEvent);
    ~Marker();

    bool start();
    void join();
    void signalTerminate();
    void signalContinue();
    void waitBlocked();

    int id() const { return id_; }
    std::size_t markedCount() const { return markedCount_; }
    std::size_t lastConflictIndex() const { return lastConflictIndex_; }

    Marker(const Marker&);
    Marker& operator=(const Marker&);

private:
    static DWORD WINAPI ThreadProcStatic(LPVOID param);
    DWORD run();
    void notifyBlocked();
    bool waitForContinueOrTerminate();

    int id_;
    SharedArray& arr_;
    HANDLE startEvent_;
    HANDLE threadHandle_;
    HANDLE blockedEvent_;
    HANDLE continueEvent_;
    HANDLE terminateEvent_;
    volatile bool finished_;
    std::size_t markedCount_;
    std::size_t lastConflictIndex_;
};