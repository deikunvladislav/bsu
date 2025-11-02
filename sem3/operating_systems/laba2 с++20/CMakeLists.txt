cmake_minimum_required(VERSION 3.15)
project(laba2_cxx20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
  add_compile_options(/EHsc /D_CRT_SECURE_NO_WARNINGS /D_SCL_SECURE_NO_WARNINGS)
endif()

add_executable(lab2
    src/main.cpp
    src/thread_average.cpp
    src/thread_min_max.cpp
    common/utils.cpp
)

target_include_directories(lab2 PRIVATE src common)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(run_tests
    tests/test_average.cpp
    tests/test_input.cpp
    tests/test_min_max.cpp
    tests/test_replace.cpp
    common/utils.cpp
    src/thread_average.cpp
    src/thread_min_max.cpp
)

target_include_directories(run_tests PRIVATE src common)
target_link_libraries(run_tests PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)
