#pragma once

#include "Move.hpp"
#include "Game.hpp"

class GameManager {

public:
  virtual ~GameManager() = default;
  virtual auto run()-> void = 0;
};
