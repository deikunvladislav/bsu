#pragma once
#include <Windows.h>

namespace ipc {
    struct unique_handle {
        HANDLE h{ nullptr };
        unique_handle() = default;
        explicit unique_handle(HANDLE v) : h(v) {}
        unique_handle(const unique_handle&) = delete;
        unique_handle& operator=(const unique_handle&) = delete;
        unique_handle(unique_handle&& other) noexcept : h(other.h) { other.h = nullptr; }
        unique_handle& operator=(unique_handle&& other) noexcept {
            if (this != &other) {
                reset();
                h = other.h;
                other.h = nullptr;
            }
            return *this;
        }
        ~unique_handle() { reset(); }
        void reset(HANDLE v = nullptr) { if (h) ::CloseHandle(h); h = v; }
        [[nodiscard]] HANDLE get() const noexcept { return h; }
        [[nodiscard]] bool valid() const noexcept { return h != nullptr; }
        explicit operator bool() const noexcept { return valid(); }
    };
}