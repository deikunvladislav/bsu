#ifndef OS_LAB5_FILE_STORAGE_HPP
#define OS_LAB5_FILE_STORAGE_HPP

#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <optional>
#include <filesystem>
#include "employee.hpp"

class FileStorage {
public:
    explicit FileStorage(std::string path)
        : path_(std::move(path)) {
    }

    const std::string& path() const noexcept { return path_; }

    void create_with_employees(const std::vector<Employee>& employees) {
        std::ofstream ofs(path_, std::ios::binary | std::ios::trunc);
        if (!ofs) throw std::runtime_error("Cannot create file: " + path_);
        for (const auto& e : employees) write_record(ofs, e);
    }

    std::vector<Employee> read_all() const {
        std::ifstream ifs(path_, std::ios::binary);
        if (!ifs) throw std::runtime_error("Cannot open file for reading: " + path_);
        std::vector<Employee> out;
        Employee e{};
        while (read_record(ifs, e)) out.push_back(e);
        return out;
    }

    std::optional<Employee> read_by_id(int32_t id) const {
        std::ifstream ifs(path_, std::ios::binary);
        if (!ifs) throw std::runtime_error("Cannot open file for reading: " + path_);
        Employee e{};
        while (read_record(ifs, e)) {
            if (e.num == id) return e;
        }
        return std::nullopt;
    }

    bool update_by_id(const Employee& updated) {
        std::fstream fs(path_, std::ios::binary | std::ios::in | std::ios::out);
        if (!fs) throw std::runtime_error("Cannot open file for update: " + path_);
        Employee e{};
        while (fs && fs.read(reinterpret_cast<char*>(&e.num), sizeof(e.num))) {
            fs.read(reinterpret_cast<char*>(e.name.data()), e.name.size());
            fs.read(reinterpret_cast<char*>(&e.hours), sizeof(e.hours));
            if (e.num == updated.num) {
                const std::streamoff record_size = static_cast<std::streamoff>(Employee::serialized_size());
                fs.seekp(fs.tellg() - record_size);
                write_record(fs, updated);
                return true;
            }
        }
        return false;
    }

    static void print_to_console(const std::vector<Employee>& list, std::ostream& os) {
        for (const auto& e : list) {
            os << "ID=" << e.num << " name='" << e.get_name() << "' hours=" << e.hours << '\n';
        }
    }

private:
    template<typename Stream>
    static void write_record(Stream& s, const Employee& e) {
        s.write(reinterpret_cast<const char*>(&e.num), sizeof(e.num));
        s.write(reinterpret_cast<const char*>(e.name.data()), e.name.size());
        s.write(reinterpret_cast<const char*>(&e.hours), sizeof(e.hours));
        if (!s) throw std::runtime_error("Write failure");
    }

    static bool read_record(std::istream& s, Employee& e) {
        if (!s.read(reinterpret_cast<char*>(&e.num), sizeof(e.num))) return false;
        s.read(reinterpret_cast<char*>(e.name.data()), e.name.size());
        s.read(reinterpret_cast<char*>(&e.hours), sizeof(e.hours));
        return static_cast<bool>(s);
    }

    std::string path_;
};

#endif