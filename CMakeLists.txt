cmake_minimum_required(VERSION 3.10.2)

project(hnefatafl VERSION 0.0.1
    DESCRIPTION "A program for playing hnefatafl"
    LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(hnefatafl 
    src/hnefatafl.cpp
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb -fsanitize=address")
endif()



add_subdirectory(lib/Catch2)
add_executable(tests tests/test.cpp src/helper.cpp)
target_include_directories(tests PRIVATE src)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
