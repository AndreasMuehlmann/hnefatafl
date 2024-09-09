#pragma once

#include <memory>

#include "Game.hpp"
#include "GameManager.hpp"
#include "Player.hpp"

class SingleGameManager : GameManager {

public:
  SingleGameManager(Game game, std::unique_ptr<Player> attackingPlayer, std::unique_ptr<Player> defendingPlayer); 
  auto run() -> void override;
  
private:
  Game m_game;
  std::unique_ptr<Player> m_defendingPlayer;
  std::unique_ptr<Player> m_attackingPlayer;
};
