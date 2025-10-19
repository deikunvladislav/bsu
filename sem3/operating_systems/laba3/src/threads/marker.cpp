#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include <threads/marker.hpp>
#include <core/shared_array.hpp>
#include <sync/sync_win.hpp>

Marker::Marker(int id, SharedArray* arr, CriticalSection* cs, Event* start)
    : id_(id), arr_(arr), cs_(cs), start_(start),
    blocked_(true, false), cont_(true, false), term_(true, false), running_(true) {
}

void Marker::run() {
    start_->wait();
    srand(static_cast<unsigned>(id_));
    while (running_.load()) {
        int r = rand();
        std::size_t idx = static_cast<std::size_t>(r >= 0 ? r : -r) % arr_->size();

        cs_->lock();
        if (arr_->get(idx) == 0) {
            cs_->unlock();
            Sleep(5);
            cs_->lock();
            arr_->set(idx, id_);
            marked_.push_back(idx);
            cs_->unlock();
            Sleep(5);
            continue;
        }
        cs_->unlock();

        std::cout << "[Marker " << id_ << "] blocked at index " << idx
            << " (marked=" << marked_.size() << ")\n";

        blocked_.set();

        HANDLE waits[2] = { cont_.native(), term_.native() };
        DWORD res = WaitForMultipleObjects(2, waits, FALSE, INFINITE);
        if (res == WAIT_OBJECT_0 + 1) {
            cs_->lock();
            for (auto p : marked_) {
                if (arr_->get(p) == id_) arr_->set(p, 0);
            }
            cs_->unlock();
            running_.store(false);
        }
        else {
            cont_.reset();
        }
    }
}

void Marker::signalContinue() { cont_.set(); }
void Marker::signalTerminate() { term_.set(); }
void Marker::waitBlocked() { blocked_.wait(); }
void Marker::resetBlocked() { blocked_.reset(); }
bool Marker::isRunning() const { return running_.load(); }