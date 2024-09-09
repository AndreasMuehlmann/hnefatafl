cmake_minimum_required(VERSION 3.30.3)

project(hnefatafl VERSION 0.0.1
    DESCRIPTION "A program for playing hnefatafl"
    LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -ggdb -O0 -fsanitize=address,undefined")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(hnefatafl 
    src/main.cpp
    src/Human.cpp
    src/SingleGameManager.cpp
    src/GameUtils.cpp
)

target_include_directories(hnefatafl PRIVATE lib/argparse/include)

#   find_package(Catch2 3 REQUIRED)

#   add_executable(tests
#       tests/TestGame.cpp
#
#       src/Game.cpp
#   )
#
#   target_include_directories(tests PRIVATE src)
#   target_include_directories(tests PRIVATE tests)
#   target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)


install(
    TARGETS hnefatafl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
