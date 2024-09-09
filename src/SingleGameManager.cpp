#include <memory>

#include "SingleGameManager.hpp"

SingleGameManager::SingleGameManager(Game game, std::unique_ptr<Player> attackingPlayer, std::unique_ptr<Player> defendingPlayer) : 
    m_game(game), m_defendingPlayer(std::move(defendingPlayer)), m_attackingPlayer(std::move(attackingPlayer)) {}

auto SingleGameManager::run() -> void {}
