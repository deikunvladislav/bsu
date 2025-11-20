#pragma once
#include <string>
#include <string_view>
#include <expected>
#include <cstddef>

namespace core {

    struct FileHeader {
        std::size_t capacity{};
        std::size_t count{};
        std::size_t head{};
        std::size_t tail{};
    };

    class RingQueueFile {
    public:
        static constexpr std::size_t kMaxMessageLength = 20;

        static std::expected<void, std::string> initialize(std::string_view file, std::size_t capacity);

        explicit RingQueueFile(std::string_view file);

        [[nodiscard]] bool ok() const noexcept;
        [[nodiscard]] std::string error() const noexcept;

        [[nodiscard]] std::expected<void, std::string> try_push(std::string_view msg);
        [[nodiscard]] std::expected<std::string, std::string> try_pop();

        [[nodiscard]] bool is_full() const;
        [[nodiscard]] bool is_empty() const;

    private:
        std::string file_;
        bool ok_{ false };
        std::string last_error_;
    };

}