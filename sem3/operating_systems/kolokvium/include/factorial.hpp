#pragma once
#include <vector>
#include <string>
#include "errors.hpp"

namespace app::math {

    class BigInt {
    public:
        BigInt();
        explicit BigInt(uint64_t v);
        BigInt& mul(uint32_t m);
        std::string to_string() const;
    private:
        static constexpr uint32_t base = 1'000'000'000; 
        static constexpr int limb_width = 9;           
        static constexpr char limb_fill_char = '0';    
        std::vector<uint32_t> digits_;
    };

    std::vector<BigInt> first_n_factorials(std::size_t n);

}