#ifndef LAB4_SYNC_WIN_HANDLE_H
#define LAB4_SYNC_WIN_HANDLE_H

#include <Windows.h>

namespace ipc {

    class WinHandle {
    public:
        WinHandle() : h_(NULL) {}
        explicit WinHandle(HANDLE h) : h_(h) {}
        ~WinHandle() { close(); }

        WinHandle(const WinHandle&) = delete;
        WinHandle& operator=(const WinHandle&) = delete;

        WinHandle(WinHandle&& other) noexcept : h_(other.h_) { other.h_ = NULL; }
        WinHandle& operator=(WinHandle&& other) noexcept {
            if (this != &other) {
                close();
                h_ = other.h_;
                other.h_ = NULL;
            }
            return *this;
        }

        HANDLE get() const { return h_; }
        bool valid() const { return h_ != NULL; }

        void reset(HANDLE h) {
            close();
            h_ = h;
        }

        void close() {
            if (h_ != NULL) {
                CloseHandle(h_);
                h_ = NULL;
            }
        }

    private:
        HANDLE h_;
    };

}

#endif