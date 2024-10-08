#pragma once

#include <memory>

#include "Game.hpp"
#include "Player.hpp"

class SingleGameManager {

  public:
    SingleGameManager(Game game, std::unique_ptr<Player> attackingPlayer,
                      std::unique_ptr<Player> defendingPlayer, bool commandLineOuptut);
    auto run() -> Winner;

  private:
    Game m_game;
    std::unique_ptr<Player> m_attackingPlayer;
    std::unique_ptr<Player> m_defendingPlayer;
    bool m_commandLineOuptut;
};
