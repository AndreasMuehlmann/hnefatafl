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
        Move move{};
        if (m_game.areAttackersToMove()) {
            //std::cout << "Attackers to move.\n";
            move = m_attackingPlayer->getMove(m_game);
        } else {
            //std::cout << "Defenders to move.\n";
            move = m_defendingPlayer->getMove(m_game);
        }
        std::string error = m_game.validMove(move);
        if (!error.empty()) {
            std::cout << error << '\n';
            continue;
        }
        winner = m_game.makeMove(move);
        //m_game.printField();
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
