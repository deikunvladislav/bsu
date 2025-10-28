#pragma once
#include <stdexcept>
#include <string>

namespace app {
    struct AppError : public std::runtime_error {
        explicit AppError(const std::string& msg) : std::runtime_error(msg) {}
    };
    struct InvalidArgumentError : public AppError {
        explicit InvalidArgumentError(const std::string& msg) : AppError(msg) {}
    };
}