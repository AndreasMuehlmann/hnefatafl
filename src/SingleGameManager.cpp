#include <memory>

#include "SingleGameManager.hpp"
#include "Game.hpp"

SingleGameManager::SingleGameManager(Game game, std::unique_ptr<Player> attackingPlayer, std::unique_ptr<Player> defendingPlayer) : 
    m_game(game), m_defendingPlayer(std::move(defendingPlayer)), m_attackingPlayer(std::move(attackingPlayer)) {}

auto SingleGameManager::run() -> void {
    Faction factionToMove = Faction::Attacker;
    Faction winner = Faction::None;
    while (winner == Faction::None) {
        Move move{};
        if (factionToMove == Faction::Attacker) {
            move = m_attackingPlayer->getMove(m_game);
            factionToMove = Faction::Defender;
        } else {
            move = m_defendingPlayer->getMove(m_game);
            factionToMove = Faction::Attacker;
        }
        winner = m_game.makeMove(move);
    }
}
