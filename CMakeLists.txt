cmake_minimum_required(VERSION 3.16)
project(processes)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror --std=c++17")
add_executable(${PROJECT_NAME} main.cpp)
