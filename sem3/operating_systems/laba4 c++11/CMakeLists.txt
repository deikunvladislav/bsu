cmake_minimum_required(VERSION 3.16)
project(lab4_sync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
  add_compile_options(/W4 /permissive-)
else()
  add_compile_options(-Wall -Wextra -pedantic)
endif()

add_library(core_lib
  src/core/ring_queue_file.cpp
  src/core/byte_io.cpp
  src/ipc/win_sync.cpp
  src/ipc/process_launcher.cpp
)

target_include_directories(core_lib PUBLIC include)

add_executable(receiver src/receiver.cpp)
target_link_libraries(receiver PRIVATE core_lib)

add_executable(sender src/sender.cpp)
target_link_libraries(sender PRIVATE core_lib)

enable_testing()