#include "util/console.h"
#include <iostream>
#include <limits>

namespace util {
    void write_line(std::string_view s) {
        std::cout << s << std::endl;
    }

    std::string read_line() {
        std::string line;
        std::getline(std::cin, line);
        return line;
    }

    void wait_key_to_close() {
        std::cout << "Press Enter to close..." << std::endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}