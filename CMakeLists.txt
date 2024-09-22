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
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O1 -fsanitize=address,undefined")
elseif(CMAKE_BUILD_TYPE STREQUAL "Gprof")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -O3")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "DebugSan")
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
        tests/TestGame.cpp
        tests/TestGameUtils.cpp
        tests/TestSearch.cpp

        src/PlayerFactory.cpp
        src/Human.cpp
        src/RandomMovesGenerator.cpp
        src/Negamax.cpp
        src/SearchUtils.cpp
        src/SingleGameManager.cpp
        src/MultipleGameManager.cpp
        src/GameUtils.cpp
        src/Game.cpp
        src/AvailableMovesGenerator.cpp
        src/Perft.cpp
        src/Utils.cpp
    )

    target_include_directories(tests PRIVATE src)
    target_include_directories(tests PRIVATE tests)
    target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
endif()

add_executable(hnefatafl 
    src/main.cpp
    src/PlayerFactory.cpp
    src/Human.cpp
    src/RandomMovesGenerator.cpp
    src/Negamax.cpp
    src/SearchUtils.cpp
    src/SingleGameManager.cpp
    src/MultipleGameManager.cpp
    src/GameUtils.cpp
    src/Game.cpp
    src/AvailableMovesGenerator.cpp
    src/Perft.cpp
    src/Utils.cpp
)

target_include_directories(hnefatafl PRIVATE lib/argparse/include)


install(
    TARGETS hnefatafl
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
)
