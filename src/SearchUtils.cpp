#include <iostream>

#include "SearchUtils.hpp"
#include "BitMasks.hpp"
#include "EvaluatedMovePath.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"

constexpr int KING_VALUE = 6;

auto evaluate(Game &game) -> int {
    InternalField field = game.getInternalField();
    const size_t wikingCount = (field & MASK_GET_ALL_WIKINGS_AND_KING).count() - 1;
    const size_t guardCount = (field & MASK_GET_ALL_GUARDS_AND_KING).count() - 1;
    int sign = (game.areAttackersToMove()) ? 1 : -1;
    return (static_cast<int>(wikingCount) - static_cast<int>(guardCount) - KING_VALUE) * sign;
}

auto printPrincipalVariation(Game game, const EvaluatedMovePath& evaluatedMovePath) -> void {
    std::cout << "Principal Variation (depth: " << evaluatedMovePath.moveCount << ")\n";
    const int sign = (game.areAttackersToMove()) ? 1 : -1;
    for (size_t i = 0; i < evaluatedMovePath.moveCount; i++) {
        printMove(evaluatedMovePath.moves[i]);
        game.makeMove(evaluatedMovePath.moves[i]);
        game.printField();
    }
    std::cout << "evaluation: " << sign * evaluatedMovePath.evaluation << "\n\n";
}
