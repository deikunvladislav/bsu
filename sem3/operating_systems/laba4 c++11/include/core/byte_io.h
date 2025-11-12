#ifndef LAB4_SYNC_BYTE_IO_H
#define LAB4_SYNC_BYTE_IO_H

#include <fstream>
#include <cstddef>
#include <string>

namespace core {

    class ByteIO {
    public:
        ByteIO(const std::string& path, bool create);
        ~ByteIO();

        bool good() const;
        std::string error() const;

        bool write_at(std::size_t offset, const void* data, std::size_t size);
        bool read_at(std::size_t offset, void* data, std::size_t size) const;

        bool resize(std::size_t new_size);
        std::size_t file_size() const;

    private:
        std::string path_;
        mutable std::fstream file_;
        mutable std::string last_error_;
    };

} 

#endif