#pragma once
#include <string>
#include <string_view>

namespace util {

	[[nodiscard]] std::string trim_copy(std::string_view s);

} 