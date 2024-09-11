#include <iostream>
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
            std::cout << "Attackers to move\n";
            move = m_attackingPlayer->getMove(m_game);
        } else {
            std::cout << "Defenders to move\n";
            move = m_defendingPlayer->getMove(m_game);
        }
        std::string error = m_game.validMove(move);
        if (!error.empty()) {
            std::cout << error << '\n';
            continue;
        }
        winner = m_game.makeMove(move);
    }
}
