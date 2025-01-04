cmake_minimum_required(VERSION 3.28)
project(NumPP)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(headers/)

add_executable(usage_example
        usage_example.cpp
)
