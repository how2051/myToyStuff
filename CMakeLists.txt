cmake_minimum_required(VERSION 3.0)
project(mycrypt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(mycrypt mycrypt.cpp)

target_include_directories(mycrypt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

