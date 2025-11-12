#ifndef LAB4_SYNC_UTIL_STRING_TRIM_H
#define LAB4_SYNC_UTIL_STRING_TRIM_H

#include <string>
#include <algorithm>
#include <cctype>

namespace util {

    inline std::string trim_copy(const std::string& s) {
        auto first = std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); });
        auto last = std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base();
        if (first >= last) return std::string();
        return std::string(first, last);
    }

}

#endif