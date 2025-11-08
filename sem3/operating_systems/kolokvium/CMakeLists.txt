cmake_minimum_required(VERSION 3.20)
project(IndustrialLab CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

include(CTest)
include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(dedup INTERFACE)
target_include_directories(dedup INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_sources(dedup INTERFACE ${CMAKE_SOURCE_DIR}/include/dedup.hpp)

add_library(list INTERFACE)
target_include_directories(list INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_sources(list INTERFACE ${CMAKE_SOURCE_DIR}/include/singly_list.hpp)

add_library(factorial src/factorial.cpp)
target_include_directories(factorial PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(cli src/cli_main.cpp)
target_link_libraries(cli PRIVATE factorial dedup list)

find_package(GTest CONFIG QUIET)
if (NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(unit_tests
    tests/test_factorial.cpp
    tests/test_dedup.cpp
    tests/test_list.cpp
    tests/test_cli.cpp 
)
target_link_libraries(unit_tests PRIVATE
    factorial dedup list
    GTest::gtest GTest::gtest_main
)
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(unit_tests)

option(ENABLE_COVERAGE "Enable coverage flags" OFF)
if (ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(--coverage -O0 -g)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        link_libraries(gcov)
    endif()
endif()