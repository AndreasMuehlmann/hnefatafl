#include <chrono>
#include <iostream>
#include <optional>

#include "Perft.hpp"
#include "GameUtils.hpp"

Perft::Perft(unsigned int maxDepth) : m_maxDepth(maxDepth) {}

auto Perft::perft(Game game) const -> void {
    auto start = std::chrono::high_resolution_clock::now();
    auto& availableMovesGenerator = game.getAvailableMovesGenerator();
    unsigned long sumMoveCounts = 0;
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) { break; }
        game.makeMove(*moveOption);
        const auto moveCount = perftHelper(game, m_maxDepth - 1);
        game.unmakeMove();
        printMove(*moveOption);
        sumMoveCounts += moveCount;
    }
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "SUM: " << sumMoveCounts << '\n';
    std::cout << "Elapsed time: " << duration.count() << " µ" << '\n';
}

auto Perft::perftHelper(Game& game, unsigned int depth) -> unsigned long {
    if (depth == 0) {
        return 1;
    }

    auto& availableMovesGenerator = game.getAvailableMovesGenerator();
    unsigned long sumMoveCounts = 0;
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) { break; }
        game.makeMove(*moveOption);
        sumMoveCounts += perftHelper(game, depth - 1);
        game.unmakeMove();
    }
    return sumMoveCounts;
}
