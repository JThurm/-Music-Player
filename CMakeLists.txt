cmake_minimum_required(VERSION 3.20)

project(Pulse
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Pulse
    src/main.cpp
)

target_include_directories(Pulse
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)