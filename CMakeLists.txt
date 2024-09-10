cmake_minimum_required(VERSION 3.22.1)

project(hnefatafl VERSION 0.0.1
    DESCRIPTION "A program for playing hnefatafl"
    LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Wshadow -Wformat=2 -Wcast-align -Wconversion -Wsign-conversion -Wnull-dereference")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -ggdb -O0")
elseif(CMAKE_BUILD_TYPE STREQUAL "DebugSan")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -ggdb -O1")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(hnefatafl 
    src/main.cpp
    src/Human.cpp
    src/SingleGameManager.cpp
    src/GameUtils.cpp
    src/Game.cpp
)

target_include_directories(hnefatafl PRIVATE lib/argparse/include)

find_package(Catch2 3)

if(NOT Catch2_FOUND)
    Include(FetchContent)

    FetchContent_Declare(
      Catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.4.0
    )

    FetchContent_MakeAvailable(Catch2)
endif()

add_executable(tests
    tests/TestGameUtils.cpp

    src/GameUtils.cpp
    src/Game.cpp
    src/SingleGameManager.cpp
    src/Human.cpp
)

target_include_directories(tests PRIVATE src)
target_include_directories(tests PRIVATE tests)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)


install(
    TARGETS hnefatafl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
