#pragma once

#include "Move.hpp"

class Game;

class Player {

  public:
    virtual ~Player() = default;
    virtual auto getMove(const Game &game) -> Move = 0;
};
