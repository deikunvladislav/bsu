cmake_minimum_required(VERSION 3.20)
project(laba4 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(core_lib
    src/core/byte_io.cpp
    src/core/ring_queue_file.cpp
    src/util/string_trim.cpp
    src/util/console.cpp
    src/ipc/win_sync.cpp
    src/ipc/process_launcher.cpp
)

target_include_directories(core_lib PUBLIC include)

add_executable(receiver src/receiver.cpp)
target_link_libraries(receiver PRIVATE core_lib)

add_executable(sender src/sender.cpp)
target_link_libraries(sender PRIVATE core_lib)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
    tests/test_byte_io.cpp
    tests/test_ring_queue_file.cpp
    tests/test_util.cpp
    tests/test_sync.cpp
    tests/test_process_launcher.cpp
    
)

target_link_libraries(unit_tests
    PRIVATE core_lib
    PRIVATE gtest
    PRIVATE gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)