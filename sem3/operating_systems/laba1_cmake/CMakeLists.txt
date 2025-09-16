cmake_minimum_required(VERSION 3.10)
project(LabProject)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Creator Creator.cpp)
add_executable(Reporter Reporter.cpp)
add_executable(Main Main.cpp)
