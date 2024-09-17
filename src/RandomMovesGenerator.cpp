#include <cstdlib>
#include <ctime>
#include <vector>

#include "Move.hpp"
#include "AvailableMovesGenerator.hpp"
#include "RandomMovesGenerator.hpp"

RandomMovesGenerator::RandomMovesGenerator() {
    srand(static_cast<unsigned int>(time(nullptr)));
}

auto RandomMovesGenerator::getMove(const Game &game) -> Move {
    std::vector<Move> moves;
    AvailableMovesGenerator availableMovesGenerator(game);
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) { break; }
        moves.push_back(*moveOption);
    }
    size_t index = static_cast<size_t>(rand()) % moves.size();
    return moves.at(index);
}
