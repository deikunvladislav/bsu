cmake_minimum_required(VERSION 3.16)
project(os_lab5_named_pipes LANGUAGES CXX)
add_compile_definitions(NOMINMAX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

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