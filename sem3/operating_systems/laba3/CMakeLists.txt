cmake_minimum_required(VERSION 3.16)
project(lab3_sync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к заголовкам
include_directories(${CMAKE_SOURCE_DIR}/include)

# --- Основное приложение ---
add_executable(lab3
    src/main.cpp
    src/threads/marker.cpp
)

if (WIN32)
    target_link_libraries(lab3 PRIVATE ws2_32)
endif()

# --- Тесты ---
include(FetchContent)

option(BUILD_TESTS "Build unit tests" ON)
if (BUILD_TESTS)
    enable_testing()

    # Подтягиваем GoogleTest
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
        src/threads/marker.cpp
    )

    target_link_libraries(unit_tests
        PRIVATE
            GTest::gtest
            GTest::gtest_main
    )

    if (WIN32)
        target_link_libraries(unit_tests PRIVATE ws2_32)
    endif()

    include(GoogleTest)
    gtest_discover_tests(unit_tests)
endif()