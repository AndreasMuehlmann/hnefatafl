#include <memory>

#include "Game.hpp"
#include "SingleGameManager.hpp"

SingleGameManager::SingleGameManager(Game game, std::unique_ptr<Player> attackingPlayer,
                                     std::unique_ptr<Player> defendingPlayer)
    : m_game(std::move(game)), m_attackingPlayer(std::move(attackingPlayer)),
      m_defendingPlayer(std::move(defendingPlayer)) {}

auto SingleGameManager::run() -> void {
    Winner winner = Winner::NoWinner;
    while (winner == Winner::NoWinner) {
        Move move{};
        if (m_game.areAttackersToMove()) {
            move = m_attackingPlayer->getMove(m_game);
        } else {
            move = m_defendingPlayer->getMove(m_game);
        }
        winner = m_game.makeMove(move);
    }
}
