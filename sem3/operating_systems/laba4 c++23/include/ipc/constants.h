#pragma once
#include <cstddef>

namespace core {
    inline constexpr std::size_t kMaxMessageLength = 20;
}

namespace ipc {
    inline constexpr unsigned kDefaultWaitMs = 10000; 
    inline constexpr const char* kMutexSuffix = "_mutex";
    inline constexpr const char* kMessageEventSuffix = "_event";
    inline constexpr const char* kReadyEventPrefix = "_ready_";
}