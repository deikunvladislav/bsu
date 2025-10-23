#pragma once

#include <vector>
#include <stdexcept>
#include <cstddef>

class SharedArray {
public:
    explicit SharedArray(std::size_t n) : data_(n, 0) {}

    std::size_t size() const noexcept { return data_.size(); }

    int get(std::size_t i) const {
        if (i >= data_.size()) throw std::out_of_range("SharedArray::get index");
        return data_[i];
    }

    void set(std::size_t i, int value) {
        if (i >= data_.size()) throw std::out_of_range("SharedArray::set index");
        data_[i] = value;
    }

private:
    std::vector<int> data_;
};