cmake_minimum_required(VERSION 3.10)

project(ubench)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(ubench benchmarks.cpp)

target_include_directories(ubench PUBLIC
    "${PROJECT_SOURCE_DIR}/../include"
)
