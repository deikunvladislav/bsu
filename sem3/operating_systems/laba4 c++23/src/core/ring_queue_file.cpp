#include "core/ring_queue_file.h"
#include "core/byte_io.h"
#include <format>
#include <array>
#include <cstring>

namespace core {

    static FileHeader read_header(ByteIO& io, std::string& err) {
        auto hdr_res = io.read_at(0, sizeof(FileHeader));
        if (!hdr_res) { err = hdr_res.error(); return {}; }
        FileHeader hdr{};
        std::memcpy(&hdr, hdr_res->data(), sizeof(FileHeader));
        return hdr;
    }

    static std::expected<void, std::string> write_header(ByteIO& io, const FileHeader& hdr) {
        std::string_view sv(reinterpret_cast<const char*>(&hdr), sizeof(FileHeader));
        auto wr = io.write_at(0, sv);
        if (!wr) return std::unexpected(wr.error());
        return {};
    }

    std::expected<void, std::string> RingQueueFile::initialize(std::string_view file, std::size_t capacity) {
        ByteIO io(std::string(file), true);
        if (!io.good()) return std::unexpected(io.error());

        FileHeader hdr{ capacity, 0, 0, 0 };
        auto wr = write_header(io, hdr);
        if (!wr) return std::unexpected(wr.error());

        const std::size_t total_size = sizeof(FileHeader) + capacity * kMaxMessageLength;
        auto rsz = io.resize(total_size);
        if (!rsz) return std::unexpected(rsz.error());

        std::string zeros(kMaxMessageLength, '\0');
        for (std::size_t i = 0; i < capacity; ++i) {
            std::size_t offset = sizeof(FileHeader) + i * kMaxMessageLength;
            auto w = io.write_at(offset, zeros);
            if (!w) return std::unexpected(w.error());
        }
        return {};
    }

    RingQueueFile::RingQueueFile(std::string_view file) : file_(file) {
        ByteIO io(std::string(file), false);
        ok_ = io.good();
        if (!ok_) last_error_ = io.error();
    }

    bool RingQueueFile::ok() const noexcept { return ok_; }
    std::string RingQueueFile::error() const noexcept { return last_error_; }

    std::expected<void, std::string> RingQueueFile::try_push(std::string_view msg) {
        if (msg.size() >= kMaxMessageLength) return std::unexpected("Message too long");

        ByteIO io(file_, false);
        std::string err;
        FileHeader hdr = read_header(io, err);
        if (!err.empty()) return std::unexpected(err);

        if (hdr.count == hdr.capacity) return std::unexpected("Queue full");

        std::array<char, kMaxMessageLength> slot{};
        std::memset(slot.data(), 0, slot.size());
        std::memcpy(slot.data(), msg.data(), msg.size());

        std::string_view sv(slot.data(), slot.size());
        std::size_t offset = sizeof(FileHeader) + hdr.tail * kMaxMessageLength;
        auto wr = io.write_at(offset, sv);
        if (!wr) return std::unexpected(wr.error());

        hdr.tail = (hdr.tail + 1) % hdr.capacity;
        hdr.count++;
        auto wh = write_header(io, hdr);
        if (!wh) return std::unexpected(wh.error());

        return {};
    }

    std::expected<std::string, std::string> RingQueueFile::try_pop() {
        ByteIO io(file_, false);
        std::string err;
        FileHeader hdr = read_header(io, err);
        if (!err.empty()) return std::unexpected(err);

        if (hdr.count == 0) return std::unexpected("Queue empty");

        std::size_t offset = sizeof(FileHeader) + hdr.head * kMaxMessageLength;
        auto msg_res = io.read_at(offset, kMaxMessageLength);
        if (!msg_res) return std::unexpected(msg_res.error());

        const std::string& raw = *msg_res;
        std::size_t cut = raw.find('\0');
        std::string trimmed = (cut == std::string::npos) ? raw : raw.substr(0, cut);

        hdr.head = (hdr.head + 1) % hdr.capacity;
        hdr.count--;
        auto wh = write_header(io, hdr);
        if (!wh) return std::unexpected(wh.error());

        return trimmed;
    }

    bool RingQueueFile::is_full() const {
        ByteIO io(file_, false);
        std::string err;
        FileHeader hdr = read_header(io, err);
        if (!err.empty()) return false;
        return hdr.count == hdr.capacity;
    }

    bool RingQueueFile::is_empty() const {
        ByteIO io(file_, false);
        std::string err;
        FileHeader hdr = read_header(io, err);
        if (!err.empty()) return true;
        return hdr.count == 0;
    }

}