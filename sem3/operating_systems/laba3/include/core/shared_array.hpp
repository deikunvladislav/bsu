#pragma once
#include <vector>
#include <cstddef>

class SharedArray {
public:
    explicit SharedArray(std::size_t n) : data_(n, 0) {}
    std::size_t size() const noexcept { return data_.size(); }
    int get(std::size_t i) const { return data_.at(i); }
    void set(std::size_t i, int v) { data_.at(i) = v; }
    const std::vector<int>& raw() const noexcept { return data_; }
private:
    std::vector<int> data_;
};
