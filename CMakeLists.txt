cmake_minimum_required(VERSION 3.10)
project(ErrorGenerator)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. DIR_SRC)

add_executable(ErrorGenerator ${DIR_SRC})
