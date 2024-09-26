#include <iostream>
#include <string>

#include "Game.hpp"
#include "MultipleGameManager.hpp"
#include "SingleGameManager.hpp"
#include "GameManagerUtils.hpp"
#include "Utils.hpp"

MultipleGameManager::MultipleGameManager(PlayerFactory playerFactory, size_t games, size_t randomMoveDepthForStartState)
    : m_playerFactory(std::move(playerFactory)), m_games(games), m_randomMoveDepthForStartState(randomMoveDepthForStartState) {}

auto MultipleGameManager::run() const -> void {
    size_t attackerWins = 0;
    size_t defenderWins = 0;
    size_t draws = 0;
    for (size_t i = 0; i < m_games; i++) {
        Game game = createRandomGame(m_randomMoveDepthForStartState);
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
        if (i % (m_games / 4) == 0) {
            std::cout << i + 1 << " game(s) played...\n";
        }
    }

    std::cout << rightPad("Wins attacker", TABLE_WIDTH) << " " << rightPad("Draws", TABLE_WIDTH)
              << " " << rightPad("Wins defender", TABLE_WIDTH) << '\n';
    std::cout << rightPad(std::to_string(attackerWins), TABLE_WIDTH) << " "
              << rightPad(std::to_string(draws), TABLE_WIDTH) << " "
              << rightPad(std::to_string(defenderWins), TABLE_WIDTH) << '\n';
}
