#ifndef OS_LAB5_HANDLE_HPP
#define OS_LAB5_HANDLE_HPP

#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>

class Handle {
public:
    Handle() noexcept : h_(nullptr) {}
    explicit Handle(HANDLE h) noexcept : h_(h) {}

    Handle(const Handle&) = delete;
    Handle& operator=(const Handle&) = delete;

    Handle(Handle&& other) noexcept : h_(other.h_) {
        other.h_ = nullptr;
    }

    Handle& operator=(Handle&& other) noexcept {
        if (this != &other) {
            reset();
            h_ = other.h_;
            other.h_ = nullptr;
        }
        return *this;
    }

    ~Handle() { reset(); }

    HANDLE get() const noexcept { return h_; }

    bool valid() const noexcept {
        return h_ != nullptr && h_ != INVALID_HANDLE_VALUE;
    }

    void reset(HANDLE nh = nullptr) noexcept {
        if (valid()) {
            CloseHandle(h_);
        }
        h_ = nh;
    }

private:
    HANDLE h_;
};

#endif