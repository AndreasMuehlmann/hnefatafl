#include <cstdlib>
#include <ctime>
#include <limits>
#include <vector>

#include "Game.hpp"
#include "AvailableMovesGenerator.hpp"
#include "Move.hpp"
#include "RandomMovesGenerator.hpp"

RandomMovesGenerator::RandomMovesGenerator() {
    std::random_device dev;
    std::mt19937 rng(dev());
    m_rng = rng;
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, std::numeric_limits<unsigned int>::max());
    m_dist = dist;
}

auto RandomMovesGenerator::getMove(const Game &game) -> Move {
    std::vector<Move> moves;
    AvailableMovesGenerator availableMovesGenerator(game);
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) { break; }
        moves.push_back(*moveOption);
    }
    size_t index = static_cast<size_t>(m_dist(m_rng)) % moves.size();
    return moves.at(index);
}
