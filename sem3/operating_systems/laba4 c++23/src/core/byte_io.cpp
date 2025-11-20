#include "core/byte_io.h"
#include <fstream>
#include <vector>

namespace core {

    ByteIO::ByteIO(std::filesystem::path path, bool create_if_missing)
        : file_path_(std::move(path)) {
        if (create_if_missing) {
            std::ofstream ofs(file_path_, std::ios::binary | std::ios::app);
        }
        ok_ = std::filesystem::exists(file_path_);
        if (!ok_) last_error_ = "File does not exist";
    }

    bool ByteIO::good() const noexcept { return ok_; }
    std::string ByteIO::error() const noexcept { return last_error_; }

    std::expected<void, std::string> ByteIO::write_at(std::size_t offset, std::string_view data) {
        std::fstream fs(file_path_, std::ios::in | std::ios::out | std::ios::binary);
        if (!fs) return std::unexpected("Cannot open file for writing");
        fs.seekp(offset);
        fs.write(data.data(), static_cast<std::streamsize>(data.size()));
        if (!fs) return std::unexpected("Write error");
        return {};
    }

    std::expected<std::string, std::string> ByteIO::read_at(std::size_t offset, std::size_t size) {
        std::ifstream ifs(file_path_, std::ios::binary);
        if (!ifs) return std::unexpected("Cannot open file for reading");
        ifs.seekg(offset);
        std::vector<char> buf(size);
        ifs.read(buf.data(), static_cast<std::streamsize>(size));
        if (!ifs) return std::unexpected("Read error");
        return std::string(buf.begin(), buf.end());
    }

    std::expected<void, std::string> ByteIO::resize(std::size_t new_size) {
        try {
            std::filesystem::resize_file(file_path_, new_size);
        }
        catch (const std::exception& e) {
            return std::unexpected(e.what());
        }
        return {};
    }

    std::size_t ByteIO::file_size() const noexcept {
        try {
            return std::filesystem::file_size(file_path_);
        }
        catch (...) {
            return 0;
        }
    }

}