#include <chrono>
#include <iostream>
#include <limits>

#include "AvailableMovesGenerator.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Negamax.hpp"
#include "SearchUtils.hpp"

Negamax::Negamax(unsigned int thinkingTimeMs)
    : m_thinkingTimeMs(thinkingTimeMs), m_maxDepth(std::numeric_limits<unsigned int>::max()) {}

Negamax::Negamax(unsigned int thinkingTimeMs, unsigned int maxDepth)
    : m_thinkingTimeMs(thinkingTimeMs), m_maxDepth(maxDepth) {}

auto Negamax::getMove(const Game &game) -> Move {
    m_searchStart = Clock::now();
    EvaluatedMove bestEvaluatedMove{};
    for (unsigned int depth = 1; depth < m_maxDepth; depth++) {
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
        if (duration.count() > m_thinkingTimeMs) {
            break;
        }
        Game localGame = game;
        std::cout << "depth: " << depth << '\n';
        const EvaluatedMove evaluatedMove = negamax(localGame, {FIELDS, FIELDS}, depth);
        bestEvaluatedMove = evaluatedMove;
    }
    printMove(bestEvaluatedMove.move);
    return bestEvaluatedMove.move;
}

auto Negamax::negamax(Game &game, Move move, unsigned int depth) -> EvaluatedMove {
    if (move.from != FIELDS) {
        game.makeMove(move);
    }
    if (depth == 0) {
        return {move, evaluate(game)};
    };
    int max = std::numeric_limits<int>::min();
    EvaluatedMove bestEvaluatedMove{};
    AvailableMovesGenerator availableMovesGenerator(game);
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) {
            break;
        }
        EvaluatedMove evaluatedMove = negamax(game, *moveOption, depth - 1);
        if (evaluatedMove.evaluation > max) {
            bestEvaluatedMove = evaluatedMove;
        }
    }
    bestEvaluatedMove.evaluation *= -1;
    return bestEvaluatedMove;
}
