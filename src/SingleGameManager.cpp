#include <iostream>
#include <memory>

#include "Game.hpp"
#include "SingleGameManager.hpp"

SingleGameManager::SingleGameManager(Game game, std::unique_ptr<Player> attackingPlayer,
                                     std::unique_ptr<Player> defendingPlayer,
                                     bool commandLineOuptut)
    : m_game(std::move(game)), m_attackingPlayer(std::move(attackingPlayer)),
      m_defendingPlayer(std::move(defendingPlayer)), m_commandLineOuptut(commandLineOuptut) {}

auto SingleGameManager::run() -> Winner {
    Winner winner = Winner::NoWinner;
    while (winner == Winner::NoWinner) {
        m_game.printField();
        Move move{};
        if (m_game.areAttackersToMove()) {
            move = m_attackingPlayer->getMove(m_game);
        } else {
            move = m_defendingPlayer->getMove(m_game);
        }
        std::string error = m_game.validMove(move);
        if (!error.empty()) {
            std::cout << error << '\n';
            continue;
        }
        winner = m_game.makeMove(move);
    }

    if (m_commandLineOuptut) {
        if (winner == Winner::Attacker) {
            std::cout << "The attackers won\n";
        } else if (winner == Winner::Defender) {
            std::cout << "The defenders won\n";
        } else {
            std::cout << "The Game ended in a draw\n";
        }
        m_game.printField();
    }
    return winner;
}
