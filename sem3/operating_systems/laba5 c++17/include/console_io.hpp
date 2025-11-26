#ifndef OS_LAB5_CONSOLE_IO_HPP
#define OS_LAB5_CONSOLE_IO_HPP

#include <iostream>
#include <limits>
#include <string>
#include "employee.hpp"

namespace Console {

    inline int32_t prompt_int(const char* label) {
        std::cout << label;
        int64_t v = 0;
        constexpr int32_t MIN_INT32 = std::numeric_limits<int32_t>::min();
        constexpr int32_t MAX_INT32 = std::numeric_limits<int32_t>::max();
        while (!(std::cin >> v) || v < MIN_INT32 || v > MAX_INT32) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a valid integer value: ";
        }
        return static_cast<int32_t>(v);
    }

    inline double prompt_double(const char* label) {
        std::cout << label;
        double v = 0.0;
        while (!(std::cin >> v)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Enter a valid number: ";
        }
        return v;
    }

    inline std::string prompt_string(const char* label, std::size_t max_len) {
        std::cout << label;
        std::string s;
        std::cin >> s;
        if (s.size() > max_len) {
            s = s.substr(0, max_len);
        }
        return s;
    }

    inline Employee prompt_employee() {
        Employee e{};
        e.num = prompt_int("ID: ");
        const auto name = prompt_string("Name (up to 10 characters): ", 10);
        e.set_name(name);
        e.hours = prompt_double("Hours: ");
        return e;
    }

    inline void print_employee(const Employee& e) {
        std::cout << "ID=" << e.num << " name='" << e.get_name() << "' hours=" << e.hours << '\n';
    }

}

#endif