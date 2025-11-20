#pragma once
#include <string>
#include <string_view>
#include <expected>
#include <filesystem>
#include <cstddef>

namespace core {

    class ByteIO {
    public:
        explicit ByteIO(std::filesystem::path path, bool create_if_missing = false);

        [[nodiscard]] bool good() const noexcept;
        [[nodiscard]] std::string error() const noexcept;

        [[nodiscard]] std::expected<void, std::string> write_at(std::size_t offset, std::string_view data);
        [[nodiscard]] std::expected<std::string, std::string> read_at(std::size_t offset, std::size_t size);

        [[nodiscard]] std::expected<void, std::string> resize(std::size_t new_size);
        [[nodiscard]] std::size_t file_size() const noexcept;

    private:
        std::filesystem::path file_path_;
        bool ok_{ false };
        std::string last_error_;
    };

} 