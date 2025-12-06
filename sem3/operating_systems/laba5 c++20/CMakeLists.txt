cmake_minimum_required(VERSION 3.20)
project(os_lab5_named_pipes LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

add_compile_options(
  "$<$<CXX_COMPILER_ID:MSVC>:/W4;/permissive->"
  "$<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall;-Wextra;-Wpedantic>"
)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(core INTERFACE)
target_include_directories(core INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_library(serverlib
  server/server_app.cpp
  server/server_session.cpp
)
target_link_libraries(serverlib PRIVATE core)

add_executable(Server
  server/server_main.cpp
)
target_link_libraries(Server PRIVATE serverlib)

add_executable(Client
  client/client_main.cpp
  client/client_app.cpp
)
target_link_libraries(Client PRIVATE core)

enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(UnitTests
  tests/test_employee.cpp
  tests/test_file_storage.cpp
  tests/test_rw_lock.cpp
  tests/test_console_io.cpp
  tests/test_pipe.cpp
  tests/test_write_commit.cpp
)
target_link_libraries(UnitTests PRIVATE core gtest_main serverlib)
target_include_directories(UnitTests PRIVATE ${CMAKE_SOURCE_DIR}/include)

include(GoogleTest)
gtest_discover_tests(UnitTests)