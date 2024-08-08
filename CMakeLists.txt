cmake_minimum_required(VERSION 3.10.2)

project(hnefatafl VERSION 0.0.1
    DESCRIPTION "A program for playing hnefatafl"
    LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(hnefatafl 
    src/hnefatafl.cpp
)
