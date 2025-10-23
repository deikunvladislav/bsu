cmake_minimum_required(VERSION 3.16)
project(lab3_sync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(lab3
    src/main.cpp
    src/marker.cpp
)

include(FetchContent)
option(BUILD_TESTS "Build unit tests" ON)
if (BUILD_TESTS)
    enable_testing()

    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(unit_tests
        tests/test_array.cpp
        tests/test_marker.cpp
        tests/test_sync.cpp
        src/marker.cpp
    )

    target_link_libraries(unit_tests
        PRIVATE
            GTest::gtest
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()