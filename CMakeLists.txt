cmake_minimum_required(VERSION 3.0)
project(mycrypt)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(mycrypt mycrypt.cpp)
