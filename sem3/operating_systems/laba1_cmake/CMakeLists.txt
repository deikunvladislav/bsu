cmake_minimum_required(VERSION 3.15)
project(EmployeeProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(common STATIC
    common/employee.h
    common/utils.cpp
    common/utils.h
)
target_include_directories(common PUBLIC common)

add_executable(Creator src/Creator.cpp)
target_link_libraries(Creator PRIVATE common)

add_executable(Reporter src/Reporter.cpp)
target_link_libraries(Reporter PRIVATE common)

add_executable(Main src/Main.cpp)
target_link_libraries(Main PRIVATE common)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(Tests tests/Tests.cpp)
target_link_libraries(Tests PRIVATE common gtest_main)

include(GoogleTest)
gtest_discover_tests(Tests)