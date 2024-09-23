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

## Contribute

Run `clang-tidy src/* tests/* -p build/` to check for style improvements.
Run `git-clang-format` to format modified files before commiting them or run `clang-format -i src/*`.

## TODO

- In static evaluation it matters who was to move first initially and not at depth 0  
- To remove checks for who is to move, cache the value in game
- Don't use move when thinking time is over
- reimplement draw only with the history for the current move and check if no move is available


## Testcases

```
Cannot move because figur is in the way.
_ _ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ _ 
_ _ _ _ _ _ g g _ 
w _ _ _ _ _ _ _ _ 
_ _ _ _ g _ _ g _ 
_ _ _ k _ _ _ _ _ 
_ _ _ _ _ _ _ _ _ 
```


```
Guards to move
_ w w w w w _ _ _ 
_ _ _ _ _ _ _ _ _ 
_ _ _ _ _ _ _ _ g 
w _ _ _ g _ _ _ w 
_ _ g g k g g w w 
_ _ _ _ g _ _ _ w 
_ _ _ _ g _ _ _ _ 
w _ _ _ _ _ _ w _ 
w w _ _ w _ _ _ w 
```
