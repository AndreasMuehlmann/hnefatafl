#include <chrono>
#include <limits>
#include <iostream>

#include "Negamax.hpp"
#include "Game.hpp"

Negamax::Negamax(unsigned int thinkingTimeMs)
    : m_thinkingTimeMs(thinkingTimeMs), m_maxDepth(std::numeric_limits<unsigned int>::max()) {}

Negamax::Negamax(unsigned int thinkingTimeMs, unsigned int maxDepth) 
    : m_thinkingTimeMs(thinkingTimeMs), m_maxDepth(maxDepth) {}

auto Negamax::getMove(const Game& game) -> Move {
    m_searchStart = Clock::now();
    EvaluatedMove bestEvaluatedMove{};
    for (unsigned int depth = 1; depth < m_maxDepth; depth++) {
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
        if (duration.count() > m_thinkingTimeMs) {
            break;
        }
        Game localGame = game;
        const EvaluatedMove evaluatedMove = negamax(localGame, depth);
        bestEvaluatedMove = evaluatedMove;
    }
    return bestEvaluatedMove.move;
}

auto Negamax::negamax(Game& game, unsigned int depth) -> EvaluatedMove {
    (void)game;
    (void)depth;
    return EvaluatedMove{};
}
