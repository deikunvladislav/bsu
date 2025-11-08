#pragma once
#define NOMINMAX
#include "shared_array.hpp"
#include <windows.h>
#include <cstddef>
#include <string>
#include <boost/shared_ptr.hpp>

class Marker {
public:
    typedef boost::shared_ptr<Marker> Ptr;

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

private:
    Marker(const Marker&);
    Marker& operator=(const Marker&);

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
