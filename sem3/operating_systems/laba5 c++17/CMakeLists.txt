cmake_minimum_required(VERSION 3.16)
project(os_lab5_named_pipes LANGUAGES CXX)
add_compile_definitions(NOMINMAX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(
  "$<$<CXX_COMPILER_ID:MSVC>:/W4;/permissive->"
  "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic>"
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(Server
  server/server_main.cpp
  server/server_app.cpp
)

add_executable(Client
  client/client_main.cpp
  client/client_app.cpp
)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(Tests
  tests/test_employee.cpp
  tests/test_file_storage.cpp
  tests/test_rw_lock.cpp
  tests/test_protocol.cpp
)

target_include_directories(Tests PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(Tests PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(Tests)