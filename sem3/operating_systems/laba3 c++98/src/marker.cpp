#include "marker.hpp"
#include "constants.hpp"
#include <windows.h>
#include <iostream>
#include <cstdio>
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
    lastConflictIndex_(static_cast<std::size_t>(-1)),
    shouldCheckSameIndex_(false) {
    blockedEvent_ = CreateEventA(NULL, TRUE, FALSE, NULL);
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
}

void Marker::signalContinue() {
    SetEvent(continueEvent_);
}

void Marker::waitBlocked() {
    WaitForSingleObject(blockedEvent_, INFINITE);
}

void Marker::resetBlocked() {
    ResetEvent(blockedEvent_);
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

        std::size_t idx;
        bool checkingSameIndexForBlock = false;
        if (shouldCheckSameIndex_) {
            idx = lastConflictIndex_;
            shouldCheckSameIndex_ = false;
            checkingSameIndexForBlock = true;
        }
        else {
            idx = static_cast<std::size_t>(std::rand()) % N;
        }

        int currentValue = arr_.get(idx);
        if (currentValue == 0 && !checkingSameIndexForBlock) {
            Sleep(kPreWriteSleepMs);
            arr_.set(idx, id_);
            ++markedCount_;
            Sleep(kPostWriteSleepMs);
        }
        else {
            lastConflictIndex_ = idx;

            Sleep(10);
            printf("Marker %d blocked; marked=%d; conflict_index=%d\n",
                id_, (int)markedCount_, (int)idx);
            fflush(stdout);

            SetEvent(blockedEvent_);

            HANDLE events[2] = { terminateEvent_, continueEvent_ };
            DWORD waitResult = WaitForMultipleObjects(2, events, FALSE, INFINITE);

            if (waitResult == WAIT_OBJECT_0) {
                break;
            }
            else if (waitResult == WAIT_OBJECT_0 + 1) {
                ResetEvent(continueEvent_);
                ResetEvent(blockedEvent_);
                shouldCheckSameIndex_ = true;
            }
        }

        Sleep(kLoopBackoffMs);
    }

    for (std::size_t i = 0; i < arr_.size(); ++i) {
        if (arr_.get(i) == id_) {
            arr_.set(i, 0);
        }
    }

    finished_ = true;
    return 0;
}