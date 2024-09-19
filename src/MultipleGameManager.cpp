#include <iostream>
#include <string>

#include "MultipleGameManager.hpp"
#include "SingleGameManager.hpp"
#include "Utils.hpp"

MultipleGameManager::MultipleGameManager(std::string attackingPlayer, std::string defendingPlayer, size_t games)
    : m_attackingPlayer(std::move(attackingPlayer)), m_defendingPlayer(std::move(defendingPlayer)), m_games(games) {}

auto MultipleGameManager::run() -> void {
    size_t attackerWins = 0;
    size_t defenderWins = 0;
    size_t draws = 0;
    for (size_t i = 0; i < m_games; i++) {
        Game game;
        SingleGameManager singleGameManager(game, m_attackingPlayer, m_defendingPlayer, false);
        Winner winner = singleGameManager.run();
        if (winner == Winner::Attacker) {
            attackerWins++;
        } else if (winner == Winner::Defender) {
            defenderWins++;
        } else {
            draws++;
        }
    }

    std::cout << rightPad("Wins attacker", TABLE_WIDTH) << " " << rightPad("Draws", TABLE_WIDTH) << " " << rightPad("Wins defender", TABLE_WIDTH) << '\n';
    std::cout << rightPad(std::to_string(attackerWins), TABLE_WIDTH) << " " << rightPad(std::to_string(draws), TABLE_WIDTH) << " " << rightPad(std::to_string(defenderWins), TABLE_WIDTH) << '\n';
}
