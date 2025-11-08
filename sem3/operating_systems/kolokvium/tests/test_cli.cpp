#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <iostream>
#include "factorial.hpp"
#include "errors.hpp"

TEST(CLI, TooManyFactorialsMessage) {
    std::istringstream fakeInput(std::to_string(app::math::MAX_FACTORIALS + 1) + "\n");
    std::cin.rdbuf(fakeInput.rdbuf());

    std::ostringstream capturedErr;
    auto* oldCerrBuf = std::cerr.rdbuf(capturedErr.rdbuf());

    size_t n{};
    bool ok = static_cast<bool>(fakeInput >> n);
    ASSERT_TRUE(ok);

    if (n > app::math::MAX_FACTORIALS) {
        std::cerr << "Maximum allowed factorials is "
            << app::math::MAX_FACTORIALS << ".\n";
        EXPECT_THROW(
            throw app::InvalidArgumentError("Requested factorial count exceeds safe limit"),
            app::InvalidArgumentError
        );
    }

    std::cerr.rdbuf(oldCerrBuf);

    std::string errOutput = capturedErr.str();
    EXPECT_NE(errOutput.find("Maximum allowed factorials"), std::string::npos);
}