#include <iostream>
#include <vector>

#include "Perft.hpp"

Perft::Perft(unsigned int maxDepth) : m_maxDepth(maxDepth) {}

auto Perft::perft(Game game) const -> void {
    perftHelper(game, m_maxDepth);
}

auto Perft::perftHelper(Game& game, unsigned int depth) -> unsigned long {
    if (depth == 0) {
        return 0;
    }
    return perftHelper(game, depth - 1);
}

auto Perft::perftCopyUnmake(Game game) const -> void {
    const std::vector<Move> availableMoves = game.getAvailableMoves();
    unsigned long sumMoveCounts = 0;
    for (const auto& move : availableMoves) {
        Game gameCopy = game;
        gameCopy.moveUnchecked(move);
        gameCopy.updateField(move.to);
        const auto moveCount = perftCopyUnmakeHelper(gameCopy, m_maxDepth - 1);
        std::cout << move.from.x << ", " << move.from.y << "; " << move.to.x << ", " << move.to.y << ": "<< moveCount << '\n';
        sumMoveCounts += moveCount;
    }
    std::cout << "SUM: " << sumMoveCounts << '\n';
}

auto Perft::perftCopyUnmakeHelper(Game& game, unsigned int depth) -> unsigned long {
    const std::vector<Move> availableMoves = game.getAvailableMoves();
    if (depth == 1) {
        return availableMoves.size();
    }

    unsigned long moveCount = 0;
    for (const auto& move : availableMoves) {
        Game gameCopy = game;
        gameCopy.moveUnchecked(move);
        gameCopy.updateField(move.to);
        moveCount += perftCopyUnmakeHelper(gameCopy, depth - 1);
    }
    return moveCount;
}
