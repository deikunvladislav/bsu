#include "marker.hpp"
#include "constants.hpp"
#include <windows.h>
#include <iostream>
#include <cstdlib>

Marker::Marker(int id, SharedArray& arr, HANDLE startEvent)
    : id_(id),
    arr_(arr),
    startEvent_(startEvent),
    threadHandle_(NULL),
    blockedEvent_(NULL),
    continueEvent_(NULL),
    terminateEvent_(NULL),
    finished_(false),
    markedCount_(0),
    lastConflictIndex_(static_cast<std::size_t>(-1)) {
    blockedEvent_ = CreateEventA(NULL, FALSE, FALSE, NULL);
    continueEvent_ = CreateEventA(NULL, TRUE, FALSE, NULL);
    terminateEvent_ = CreateEventA(NULL, TRUE, FALSE, NULL);
}

Marker::~Marker() {
    if (threadHandle_ != NULL) {
        SetEvent(terminateEvent_);
        WaitForSingleObject(threadHandle_, INFINITE);
        CloseHandle(threadHandle_);
        threadHandle_ = NULL;
    }
    if (terminateEvent_ != NULL) { CloseHandle(terminateEvent_); terminateEvent_ = NULL; }
    if (continueEvent_ != NULL) { CloseHandle(continueEvent_);  continueEvent_ = NULL; }
    if (blockedEvent_ != NULL) { CloseHandle(blockedEvent_);   blockedEvent_ = NULL; }
}

bool Marker::start() {
    if (threadHandle_ != NULL) return true;
    DWORD tid = 0;
    threadHandle_ = CreateThread(NULL, 0, &Marker::ThreadProcStatic, this, 0, &tid);
    return threadHandle_ != NULL;
}

void Marker::join() {
    if (threadHandle_ != NULL) {
        WaitForSingleObject(threadHandle_, INFINITE);
        CloseHandle(threadHandle_);
        threadHandle_ = NULL;
        finished_ = true;
    }
}

void Marker::signalTerminate() {
    SetEvent(terminateEvent_);
    SetEvent(continueEvent_);
}

void Marker::signalContinue() {
    SetEvent(continueEvent_);
}

void Marker::waitBlocked() {
    WaitForSingleObject(blockedEvent_, INFINITE);
}

DWORD WINAPI Marker::ThreadProcStatic(LPVOID param) {
    Marker* self = reinterpret_cast<Marker*>(param);
    return self->run();
}

DWORD Marker::run() {
    if (WaitForSingleObject(startEvent_, INFINITE) != WAIT_OBJECT_0) {
        return 1;
    }
    std::srand(static_cast<unsigned int>(id_));
    const std::size_t N = arr_.size();
    for (;;) {
        if (WaitForSingleObject(terminateEvent_, 0) == WAIT_OBJECT_0) {
            break;
        }
        std::size_t idx = static_cast<std::size_t>(std::rand()) % N;
        if (arr_.get(idx) == 0) {
            Sleep(kPreWriteSleepMs);
            arr_.set(idx, id_);
            ++markedCount_;
            Sleep(kPostWriteSleepMs);
        }
        else {
            lastConflictIndex_ = idx;
            std::cout << "Marker " << id_
                << " blocked; marked=" << markedCount_
                << "; conflict_index=" << idx << "\n";
            notifyBlocked();
            if (!waitForContinueOrTerminate()) {
                break;
            }
            ResetEvent(continueEvent_);
        }
        Sleep(kLoopBackoffMs);
    }
    const std::size_t M = arr_.size();
    for (std::size_t i = 0; i < M; ++i) {
        if (arr_.get(i) == id_) {
            arr_.set(i, 0);
        }
    }
    finished_ = true;
    return 0;
}

void Marker::notifyBlocked() {
    SetEvent(blockedEvent_);
}

bool Marker::waitForContinueOrTerminate() {
    HANDLE events[2];
    events[0] = terminateEvent_;
    events[1] = continueEvent_;
    DWORD w = WaitForMultipleObjects(2, events, FALSE, INFINITE);
    if (w == WAIT_OBJECT_0) return false;
    if (w == WAIT_OBJECT_0 + 1) return true;
    return false;
}