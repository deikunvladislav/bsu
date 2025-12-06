#ifndef OS_LAB5_PROTOCOL_HPP
#define OS_LAB5_PROTOCOL_HPP

#include <cstdint>
#include "employee.hpp"

namespace Proto {

    enum class Op : uint8_t {
        ReadBegin = 1,
        ReadEnd = 2,
        WriteStart = 3,
        WriteCommit = 4,
        Release = 5,
        Exit = 6,
        Shutdown = 7
    };

    struct Request {
        Op op = Op::ReadBegin;
        int32_t id = 0;
        Employee payload{};
    };

    struct Response {
        bool ok = false;
        Employee payload{};
        char message[64]{};
    };

    static constexpr wchar_t PIPE_NAME[] = LR"(\\.\pipe\employee_pipe)";
    static constexpr uint32_t PIPE_OUT_BUFFER = 1024;
    static constexpr uint32_t PIPE_IN_BUFFER = 1024;
    static constexpr uint32_t PIPE_TIMEOUT_MS = 5000;
    static constexpr uint32_t MAX_CLIENTS = 16;

}

#endif