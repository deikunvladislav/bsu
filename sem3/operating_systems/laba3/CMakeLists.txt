cmake_minimum_required(VERSION 3.16)
project(lab3_sync LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к заголовкам
include_directories(${CMAKE_SOURCE_DIR}/include)

# Основное приложение
add_executable(laba3
    src/main.cpp
    src/threads/marker.cpp
)

# Для Windows: подключаем WinAPI
if (WIN32)
    target_link_libraries(laba3 PRIVATE ws2_32)
endif()
