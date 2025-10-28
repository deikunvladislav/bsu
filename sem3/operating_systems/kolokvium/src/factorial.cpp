#include "factorial.hpp"
#include <sstream>
#include <iomanip>

namespace app::math {

    BigInt::BigInt() : digits_{ 0 } {}

    BigInt::BigInt(uint64_t v) {
        if (v == 0) { digits_ = { 0 }; return; }
        while (v) { digits_.push_back(v % base); v /= base; }
    }

    BigInt& BigInt::mul(uint32_t m) {
        uint64_t carry = 0;
        for (auto& d : digits_) {
            uint64_t cur = static_cast<uint64_t>(d) * m + carry;
            d = static_cast<uint32_t>(cur % base);
            carry = cur / base;
        }
        while (carry) {
            digits_.push_back(static_cast<uint32_t>(carry % base));
            carry /= base;
        }
        if (digits_.empty()) digits_.push_back(0);
        return *this;
    }

    std::string BigInt::to_string() const {
        std::ostringstream oss;
        oss << digits_.back();
        for (int i = static_cast<int>(digits_.size()) - 2; i >= 0; --i) {
            oss << std::setw(limb_width) << std::setfill(limb_fill_char) << digits_[i];
        }
        return oss.str();
    }

    std::vector<BigInt> first_n_factorials(std::size_t n) {
        if (n == 0) throw InvalidArgumentError("Argument n must be greater than 0");
        std::vector<BigInt> out;
        out.reserve(n);
        BigInt cur(1);
        out.push_back(cur);
        for (std::size_t i = 2; i <= n; ++i) {
            cur.mul(static_cast<uint32_t>(i));
            out.push_back(cur);
        }
        return out;
    }
}