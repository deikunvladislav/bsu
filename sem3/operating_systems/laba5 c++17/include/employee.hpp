#ifndef OS_LAB5_EMPLOYEE_HPP
#define OS_LAB5_EMPLOYEE_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <string>

struct Employee {
    int32_t num = 0;
    std::array<char, 10> name{};
    double hours = 0.0;

    static constexpr std::size_t serialized_size() {
        return sizeof(int32_t) + 10 * sizeof(char) + sizeof(double);
    }

    void set_name(const std::string& s) {
        std::array<char, 10> tmp{};
        const std::size_t n = s.size() < 10 ? s.size() : 10;
        std::memcpy(tmp.data(), s.data(), n);
        name = tmp;
    }

    std::string get_name() const {
        return std::string(name.data(), strnlen(name.data(), name.size()));
    }
};

#endif