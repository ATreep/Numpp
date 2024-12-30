cmake_minimum_required(VERSION 3.28)
project(NumPP)

set(CMAKE_CXX_STANDARD 23)

include_directories(headers/)

add_executable(usage_example
        usage_example.cpp
)
