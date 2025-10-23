#pragma once

#include <chrono>

static constexpr auto kPreWriteSleep = std::chrono::milliseconds(5);
static constexpr auto kPostWriteSleep = std::chrono::milliseconds(5);

static constexpr auto kLoopBackoff = std::chrono::milliseconds(1);
