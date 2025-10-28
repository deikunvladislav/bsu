#pragma once
#include <vector>
#include <unordered_set>
#include <functional>

namespace app::algo {
    template <typename T, typename Hasher = std::hash<T>, typename Eq = std::equal_to<T>>
    std::vector<T> stable_unique(const std::vector<T>& input) {
        std::unordered_set<T, Hasher, Eq> seen;
        std::vector<T> result;
        result.reserve(input.size());
        for (const auto& v : input) {
            if (seen.insert(v).second) result.push_back(v);
        }
        return result;
    }
}