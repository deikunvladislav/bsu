cmake_minimum_required(VERSION 3.20)
project(lab3_sync_win LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

add_executable(lab3
    src/main.cpp
    src/marker.cpp
)

target_include_directories(lab3 PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/boost
)

if (MSVC)
    target_compile_definitions(lab3 PRIVATE WIN32_LEAN_AND_MEAN _CRT_SECURE_NO_WARNINGS)
endif()

enable_testing()

add_executable(test_array tests/test_array.cpp src/marker.cpp)
target_include_directories(test_array PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/boost
)
add_test(NAME test_array COMMAND $<TARGET_FILE:test_array>)

add_executable(test_sync tests/test_sync.cpp)
target_include_directories(test_sync PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_test(NAME test_sync COMMAND $<TARGET_FILE:test_sync>)

add_executable(test_marker tests/test_marker.cpp src/marker.cpp)
target_include_directories(test_marker PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/boost
)
add_test(NAME test_marker COMMAND $<TARGET_FILE:test_marker>)

set(TEST_TARGETS test_array test_sync test_marker)

set(GENERATED_TESTS_HEADER "${CMAKE_BINARY_DIR}/generated_tests_$<CONFIG>.h")

set(GENERATED_CONTENT "static const char* kGeneratedTests[] = {\n")
foreach(t IN LISTS TEST_TARGETS)
    list(APPEND GENERATED_ENTRIES "    \"$<TARGET_FILE:${t}>\",\n")
endforeach()
foreach(entry IN LISTS GENERATED_ENTRIES)
    set(GENERATED_CONTENT "${GENERATED_CONTENT}${entry}")
endforeach()
set(GENERATED_CONTENT "${GENERATED_CONTENT}};\nstatic const int kGeneratedTestsCount = sizeof(kGeneratedTests)/sizeof(kGeneratedTests[0]);\n")

file(GENERATE OUTPUT "${GENERATED_TESTS_HEADER}" CONTENT "${GENERATED_CONTENT}")

add_executable(test_runner tests/runner.cpp)
target_include_directories(test_runner PRIVATE ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/include)
if (MSVC)
    target_compile_definitions(test_runner PRIVATE NOMINMAX)
endif()

add_dependencies(test_runner ${TEST_TARGETS})

set(GENERATED_TEST_COMMON "${CMAKE_BINARY_DIR}/generated_tests.h")
add_custom_command(TARGET test_runner PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_BINARY_DIR}/generated_tests_$<CONFIG>.h"
        "${GENERATED_TEST_COMMON}"
)

add_test(NAME run_all_tests COMMAND $<TARGET_FILE:test_runner>)