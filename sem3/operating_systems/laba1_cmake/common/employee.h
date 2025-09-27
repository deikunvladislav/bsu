#pragma once
#include <string>
#include <sstream>
#include <iomanip>

struct employee {
    int num;
    char name[10];
    double hours;

    double salary(double rate) const {
        return hours * rate;
    }

    std::string to_string(double rate) const {
        std::ostringstream oss;
        oss << num << '\t' << name << '\t' << hours << '\t'
            << std::fixed << std::setprecision(2) << salary(rate);
        return oss.str();
    }

    bool operator==(const employee& other) const {
        return num == other.num && std::string(name) ==std::string(other.name) && hours == other.hours;
    }
};