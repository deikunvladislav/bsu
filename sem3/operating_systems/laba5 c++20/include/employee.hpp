#ifndef OS_LAB5_EMPLOYEE_HPP
#define OS_LAB5_EMPLOYEE_HPP

#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

struct Employee {
    int32_t num = 0;
    std::array<char, 10> name{};
    double hours = 0.0;

    static constexpr std::size_t serialized_size() noexcept {
        return sizeof(int32_t) + 10 * sizeof(char) + sizeof(double);
    }

    void set_name(std::string_view s) noexcept {
        std::array<char, 10> tmp{};
        const std::size_t n = s.size() < 10 ? s.size() : 10;
        std::memcpy(tmp.data(), s.data(), n);
        name = tmp;
    }

    std::string get_name() const {
        return std::string(name.data(), strnlen(name.data(), name.size()));
    }

    friend bool operator==(const Employee& a, const Employee& b) noexcept {
        return a.num == b.num && a.get_name() == b.get_name() && a.hours == b.hours;
    }
};

#endif