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

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(factorial src/factorial.cpp)

add_library(dedup INTERFACE)
target_include_directories(dedup INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_library(list INTERFACE)
target_include_directories(list INTERFACE ${CMAKE_SOURCE_DIR}/include)

add_executable(cli src/cli_main.cpp)
target_link_libraries(cli PRIVATE factorial dedup list)