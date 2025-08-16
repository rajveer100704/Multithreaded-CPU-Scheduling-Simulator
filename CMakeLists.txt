cmake_minimum_required(VERSION 3.10)
project(cpu_scheduler_simulator CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(scheduler src/main.cpp)
