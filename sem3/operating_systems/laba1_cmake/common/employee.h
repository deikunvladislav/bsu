#ifndef EMPLOYEE_H
#define EMPLOYEE_H
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <stdexcept>

struct employee {
    int num;
    char name[10];
    double hours;

    friend std::istream& operator>>(std::istream& in, employee& e) {
        in >> e.num;
        std::string temp;
        in >> temp;
        std::memset(e.name, 0, sizeof(e.name));
        std::strncpy(e.name, temp.c_str(), sizeof(e.name) - 1);
        in >> e.hours;
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const employee& e) {
        out << e.num << '\t' << e.name << '\t' << e.hours;
        return out;
    }

    void write(std::ostream& out) const {
        out.write(reinterpret_cast<const char*>(this), sizeof(employee));
    }

    void read(std::istream& in) {
        in.read(reinterpret_cast<char*>(this), sizeof(employee));
    }

    void writeText(std::ostream& out) const {
        out << *this << '\n';
    }

    static void writeReport(const std::string& binFile, const std::string& reportFile, double rate) {
        std::ifstream in(binFile, std::ios::binary);
        if (!in) throw std::runtime_error("Cannot open binary file for reading");
        std::ofstream out(reportFile);
        if (!out) throw std::runtime_error("Cannot open report file for writing");
        out << "Report based on file: " << binFile << "\n";
        out << "Number\tName\tHours\tSalary\n";
        employee e;
        while (in.read(reinterpret_cast<char*>(&e), sizeof(employee))) {
            out << e << '\t' << e.hours * rate << '\n';
        }
    }

    bool operator==(const employee& other) const {
        return num == other.num &&
            std::strncmp(name, other.name, sizeof(name)) == 0 &&
            hours == other.hours;
    }
};
#endif 