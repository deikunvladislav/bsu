#ifndef LAB4_SYNC_RING_QUEUE_FILE_H
#define LAB4_SYNC_RING_QUEUE_FILE_H

#include <cstddef>
#include <string>
#include "core/byte_io.h"

namespace core {

    static constexpr std::size_t kMaxMessageLength = 20;

    struct FileHeader {
        std::uint32_t capacity;
        std::uint32_t head;
        std::uint32_t tail;
        std::uint32_t count;
    };

    class RingQueueFile {
    public:
        static bool initialize(const std::string& path, std::size_t capacity, std::string& err);

        explicit RingQueueFile(const std::string& path);

        bool ok() const;
        bool is_full() const;
        bool is_empty() const;
        std::string error() const;

        bool try_push(const std::string& msg);
        bool try_pop(std::string& out);

        std::size_t capacity() const;

    private:
        std::string path_;
        ByteIO io_;
        std::string last_error_;

        static constexpr std::size_t kHeaderSize = sizeof(FileHeader);

        std::size_t record_offset(std::size_t index) const;
        bool read_header(FileHeader& h) const;
        bool write_header(const FileHeader& h);
        bool write_record(std::size_t index, const std::string& msg);
        bool read_record(std::size_t index, std::string& msg) const;
    };

} 

#endif