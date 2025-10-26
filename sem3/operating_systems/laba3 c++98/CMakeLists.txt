cmake_minimum_required(VERSION 3.20)
project(lab3_sync_win LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

add_executable(lab3
    src/main.cpp
    src/marker.cpp
)

target_include_directories(lab3 PRIVATE ${CMAKE_SOURCE_DIR}/include)

if (MSVC)
    target_compile_definitions(lab3 PRIVATE WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
endif()

enable_testing()

add_executable(test_array tests/test_array.cpp src/marker.cpp)
target_include_directories(test_array PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME test_array COMMAND $<TARGET_FILE:test_array>)

add_executable(test_sync tests/test_sync.cpp)
target_include_directories(test_sync PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME test_sync COMMAND $<TARGET_FILE:test_sync>)

add_executable(test_marker tests/test_marker.cpp src/marker.cpp)
target_include_directories(test_marker PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME test_marker COMMAND $<TARGET_FILE:test_marker>)