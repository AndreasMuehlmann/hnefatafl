# hnefatafl

This is a program for playing hnefatafl. Also a algorithm will developed to play hnefatafl.

## Quickstart

- Clone the repository with `https://github.com/AndreasMuehlmann/hnefatafl.git`.
- Then get all the submodules with `git submodule update --init --recursive`.
- Then create the `build` directory and `cd` into it.
- Run `cmake -DCMAKE_BUILD_TYPE=Debug ..` for a Debug build configuration or run `cmake -DCMAKE_BUILD_TYPE=Release ..`
    for a Release build configuration.
- Build the Project with `cmake --build build` from the project directory
- Execute the tests executable in the build directory to run the unit tests
- The program (`hnefatafl`) is also in the build directory
- Install the program and libraries with `sudo make install` in the `build` directory

 ## TODO



- Bug with field in Hnefatafl.cpp
- Rule that not all corner can be blocked
- Remis
- Search path extension
- bessere statische bewertung
- gute züge zuerst => besseres alpha beta pruning
- check if no figures are left to move
=> irgendwie höhere tiefe erreichen
- Die engine gibt einfach auf wenn sie weiß, dass sie verloren hat
