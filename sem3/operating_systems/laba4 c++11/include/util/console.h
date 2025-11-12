#ifndef LAB4_SYNC_UTIL_CONSOLE_H
#define LAB4_SYNC_UTIL_CONSOLE_H

#include <string>
#include <iostream>

namespace util {

    inline std::string read_line() {
        std::string s;
        std::getline(std::cin, s);
        return s;
    }

    inline void write_line(const std::string& s) {
        std::cout << s << std::endl;
    }

}

#endif