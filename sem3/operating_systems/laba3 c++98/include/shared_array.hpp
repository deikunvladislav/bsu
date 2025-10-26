#pragma once
#define NOMINMAX
#include <vector>
#include <stdexcept>
#include <cstddef>
#include <windows.h>

class SharedArray {
public:
    explicit SharedArray(std::size_t n) : data_(n, 0) {
        InitializeCriticalSection(&cs_);
    }

    ~SharedArray() {
        DeleteCriticalSection(&cs_);
    }

    std::size_t size() const { return data_.size(); }

    int get(std::size_t i) const {
        if (i >= data_.size()) throw std::out_of_range("SharedArray::get index");
        EnterCriticalSection(&cs_);
        int v = data_[i];
        LeaveCriticalSection(&cs_);
        return v;
    }

    void set(std::size_t i, int value) {
        if (i >= data_.size()) throw std::out_of_range("SharedArray::set index");
        EnterCriticalSection(&cs_);
        data_[i] = value;
        LeaveCriticalSection(&cs_);
    }

private:
    std::vector<int> data_;
    mutable CRITICAL_SECTION cs_;
};