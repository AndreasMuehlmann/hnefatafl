#pragma once

#include <memory>

#include "Game.hpp"
#include "GameManager.hpp"
#include "Player.hpp"

class SingleGameManager : public GameManager {

  public:
    SingleGameManager(Game game, std::unique_ptr<Player> attackingPlayer,
                      std::unique_ptr<Player> defendingPlayer);
    ~SingleGameManager() override = default;
    auto run() -> void override;

  private:
    Game m_game;
    std::unique_ptr<Player> m_attackingPlayer;
    std::unique_ptr<Player> m_defendingPlayer;
};
