#include <iostream>
#include <string>

#include "Game.hpp"
#include "MultipleGameManager.hpp"
#include "SingleGameManager.hpp"
#include "Utils.hpp"

MultipleGameManager::MultipleGameManager(PlayerFactory playerFactory, size_t games)
    : m_playerFactory(std::move(playerFactory)), m_games(games) {}

auto MultipleGameManager::run() const -> void {
    size_t attackerWins = 0;
    size_t defenderWins = 0;
    size_t draws = 0;
    for (size_t i = 0; i < m_games; i++) {
        Game game;
        SingleGameManager singleGameManager(game, m_playerFactory.createAttacker(),
                                            m_playerFactory.createDefender(), false);
        Winner winner = singleGameManager.run();
        if (winner == Winner::Attacker) {
            attackerWins++;
        } else if (winner == Winner::Defender) {
            defenderWins++;
        } else {
            draws++;
        }
    }

    std::cout << rightPad("Wins attacker", TABLE_WIDTH) << " " << rightPad("Draws", TABLE_WIDTH)
              << " " << rightPad("Wins defender", TABLE_WIDTH) << '\n';
    std::cout << rightPad(std::to_string(attackerWins), TABLE_WIDTH) << " "
              << rightPad(std::to_string(draws), TABLE_WIDTH) << " "
              << rightPad(std::to_string(defenderWins), TABLE_WIDTH) << '\n';
}
