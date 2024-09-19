#pragma once

#include "Game.hpp"
#include "Player.hpp"

constexpr size_t TABLE_WIDTH = 20;

class MultipleGameManager {

  public:
    MultipleGameManager(std::string attackingPlayer, std::string defendingPlayer, size_t games);
    auto run() -> void;

  private:
    Game m_game;
    std::string m_attackingPlayer;
    std::string m_defendingPlayer;
    size_t m_games;
};
