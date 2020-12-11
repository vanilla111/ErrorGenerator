cmake_minimum_required(VERSION 3.15)
project(ErrorGenerator)

set(CMAKE_CXX_STANDARD 14)

add_executable(ErrorGenerator
        Memory.cpp
        FileHelper.cpp
        main.cpp)