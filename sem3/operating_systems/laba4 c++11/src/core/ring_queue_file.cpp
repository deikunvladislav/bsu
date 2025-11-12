#include "core/ring_queue_file.h"
#include <cstring>

namespace core {

    bool RingQueueFile::initialize(const std::string& path, std::size_t capacity, std::string& err) {
        if (capacity == 0U) {
            err = "Ёмкость должна быть больше 0";
            return false;
        }
        ByteIO io(path, true);
        if (!io.good()) {
            err = io.error();
            return false;
        }
        const std::size_t record_size = kMaxMessageLength;
        const std::size_t total_size = kHeaderSize + capacity * record_size;
        if (!io.resize(total_size)) {
            err = io.error();
            return false;
        }
        FileHeader hdr;
        hdr.capacity = static_cast<std::uint32_t>(capacity);
        hdr.head = 0;
        hdr.tail = 0;
        hdr.count = 0;
        if (!io.write_at(0, &hdr, kHeaderSize)) {
            err = io.error();
            return false;
        }
        return true;
    }

    RingQueueFile::RingQueueFile(const std::string& path) : path_(path), io_(path, false) {
        if (!io_.good()) {
            last_error_ = io_.error();
        }
    }

    bool RingQueueFile::ok() const {
        return last_error_.empty();
    }

    bool RingQueueFile::is_full() const {
        FileHeader h;
        if (!read_header(h)) {
            return false; 
        }
        return h.count == h.capacity;
    }

    bool RingQueueFile::is_empty() const {
        FileHeader h;
        if (!read_header(h)) {
            return true;
        }
        return h.count == 0;
    }

    std::string RingQueueFile::error() const {
        return last_error_;
    }

    std::size_t RingQueueFile::capacity() const {
        FileHeader h;
        if (!read_header(h)) return 0U;
        return static_cast<std::size_t>(h.capacity);
    }

    bool RingQueueFile::try_push(const std::string& msg) {
        if (msg.size() == 0 || msg.size() >= kMaxMessageLength) {
            last_error_ = "Недопустимая длина сообщения";
            return false;
        }
        FileHeader h;
        if (!read_header(h)) return false;
        if (h.count == h.capacity) {
            return false;
        }
        if (!write_record(h.tail, msg)) return false;
        h.tail = (h.tail + 1U) % h.capacity;
        h.count += 1U;
        return write_header(h);
    }

    bool RingQueueFile::try_pop(std::string& out) {
        FileHeader h;
        if (!read_header(h)) return false;
        if (h.count == 0U) {
            return false;
        }
        if (!read_record(h.head, out)) return false;
        h.head = (h.head + 1U) % h.capacity;
        h.count -= 1U;
        return write_header(h);
    }

    std::size_t RingQueueFile::record_offset(std::size_t index) const {
        return kHeaderSize + index * kMaxMessageLength;
    }

    bool RingQueueFile::read_header(FileHeader& h) const {
        if (!io_.read_at(0, &h, kHeaderSize)) {
            const_cast<RingQueueFile*>(this)->last_error_ = io_.error();
            return false;
        }
        return true;
    }

    bool RingQueueFile::read_record(std::size_t index, std::string& msg) const {
        std::size_t off = record_offset(index);
        char buf[kMaxMessageLength] = { 0 };
        if (!io_.read_at(off, buf, kMaxMessageLength)) {
            const_cast<RingQueueFile*>(this)->last_error_ = io_.error();
            return false;
        }
        std::size_t len = 0;
        while (len < kMaxMessageLength && buf[len] != '\0') { ++len; }
        msg.assign(buf, buf + len);
        return true;
    }

    bool RingQueueFile::write_header(const FileHeader& h) {
        if (!io_.write_at(0, &h, kHeaderSize)) {
            last_error_ = io_.error();
            return false;
        }
        return true;
    }

    bool RingQueueFile::write_record(std::size_t index, const std::string& msg) {
        const std::size_t off = record_offset(index);
        char buf[kMaxMessageLength] = { 0 };
        std::memcpy(buf, msg.data(), msg.size());
        return io_.write_at(off, buf, kMaxMessageLength);
    }

}