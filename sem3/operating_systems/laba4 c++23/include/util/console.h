#pragma once
#include <string>
#include <string_view>

namespace util {
    void write_line(std::string_view s);
    std::string read_line();
    void wait_key_to_close();
}