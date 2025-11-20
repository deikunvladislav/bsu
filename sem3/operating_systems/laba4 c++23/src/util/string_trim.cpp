#include "util/string_trim.h"
#include <ranges>

namespace util {

    std::string trim_copy(std::string_view s) {
        auto is_space = [](char c) { return std::isspace(static_cast<unsigned char>(c)); };
        auto first = std::ranges::find_if_not(s, is_space);
        auto last = std::ranges::find_if_not(s | std::views::reverse, is_space).base();
        if (first >= last) return {};
        return std::string(first, last);
    }

}