cmake_minimum_required(VERSION 3.15)
project(Lab2Threads)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main executable
add_executable(lab2
    src/main.cpp
    src/thread_min_max.cpp
    src/thread_average.cpp
    common/utils.cpp
)

target_include_directories(lab2 PRIVATE src common)

# Google Test setup
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(run_tests
    tests/test_average.cpp
    tests/test_min_max.cpp
    tests/test_replace.cpp
    tests/test_input.cpp
    common/utils.cpp
    src/thread_min_max.cpp
    src/thread_average.cpp
)

target_include_directories(run_tests PRIVATE src common)
target_link_libraries(run_tests gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)