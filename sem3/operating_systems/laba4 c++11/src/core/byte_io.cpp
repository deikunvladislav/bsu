#include "core/byte_io.h"
#include <ios>
#include <vector>
#include <fstream>

namespace core {

    ByteIO::ByteIO(const std::string& path, bool create) : path_(path) {
        std::ios::openmode mode = std::ios::binary | std::ios::in | std::ios::out;
        if (create) {
            file_.open(path_, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);
            if (!file_.is_open()) {
                last_error_ = "Не удалось создать файл: " + path_;
                return;
            }
        }
        else {
            file_.open(path_, mode);
            if (!file_.is_open()) {
                last_error_ = "Не удалось открыть файл: " + path_;
                return;
            }
        }
    }

    ByteIO::~ByteIO() {
        if (file_.is_open()) {
            file_.flush();
            file_.close();
        }
    }

    bool ByteIO::good() const {
        return file_.good() || file_.is_open();
    }

    std::string ByteIO::error() const {
        return last_error_;
    }

    bool ByteIO::write_at(std::size_t offset, const void* data, std::size_t size) {
        file_.seekp(static_cast<std::streamoff>(offset), std::ios::beg);
        if (!file_) { last_error_ = "Ошибка seekp"; return false; }
        file_.write(reinterpret_cast<const char*>(data), static_cast<std::streamsize>(size));
        if (!file_) { last_error_ = "Ошибка записи"; return false; }
        file_.flush();
        return true;
    }

    bool ByteIO::read_at(std::size_t offset, void* data, std::size_t size) const {
        file_.seekg(static_cast<std::streamoff>(offset), std::ios::beg);
        if (!file_) { last_error_ = "seekg failed"; return false; }
        file_.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
        if (!file_) { last_error_ = "read failed"; return false; }
        return true;
    }

    bool ByteIO::resize(std::size_t new_size) {
        if (!file_.is_open()) { last_error_ = "Файл не открыт"; return false; }
        std::size_t current = file_size();
        if (new_size == current) return true;
        std::vector<char> zeros(1, 0);
        return write_at(new_size - 1, zeros.data(), zeros.size());
    }

    std::size_t ByteIO::file_size() const {
        std::ifstream in(path_, std::ios::binary | std::ios::ate);
        return in ? static_cast<std::size_t>(in.tellg()) : 0U;
    }

}