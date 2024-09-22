#include <chrono>
#include <iostream>
#include <limits>

#include "AvailableMovesGenerator.hpp"
#include "Game.hpp"
#include "Negamax.hpp"
#include "GameUtils.hpp"
#include "SearchUtils.hpp"

Negamax::Negamax(unsigned int thinkingTimeMs)
    : m_thinkingTimeMs(thinkingTimeMs), m_maxDepth(std::numeric_limits<unsigned int>::max()) {}

Negamax::Negamax(unsigned int thinkingTimeMs, unsigned int maxDepth)
    : m_thinkingTimeMs(thinkingTimeMs), m_maxDepth(maxDepth) {}

auto Negamax::getMove(const Game &game) -> Move {
    m_searchStart = Clock::now();
    EvaluatedMove bestEvaluatedMove{};
    for (unsigned int depth = 1; depth <= m_maxDepth; depth++) {
        auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
        if (duration.count() > m_thinkingTimeMs) {
            break;
        }
        Game localGame = game;
        const EvaluatedMove evaluatedMove = negamax(localGame, {FIELDS, FIELDS}, depth);
        bestEvaluatedMove = evaluatedMove;
    }
    return bestEvaluatedMove.move;
}

auto Negamax::negamax(Game &game, Move move, unsigned int depth) -> EvaluatedMove {
    if (move.from != FIELDS) {
        Winner winner = game.makeMove(move);
        if (winner == Winner::Attacker) {
            return {move, std::numeric_limits<int>::max() - 1};
        } 
        if (winner == Winner::Defender) {
            return {move, std::numeric_limits<int>::min() + 1};
        } 
        if (winner == Winner::Draw) {
            return {move, 0};
        } 
    }
    if (depth == 0) {
        return {move, evaluate(game)};
    };

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
    if (duration.count() > m_thinkingTimeMs) {
        EvaluatedMove evaluatedMove{};
        evaluatedMove.evaluation = std::numeric_limits<int>::min();
        return evaluatedMove; 
    }

    EvaluatedMove bestEvaluatedMove{};
    bestEvaluatedMove.evaluation = std::numeric_limits<int>::min();
    AvailableMovesGenerator availableMovesGenerator(game);
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) {
            break;
        }
        printMove(*moveOption);
        EvaluatedMove evaluatedMove = negamax(game, *moveOption, depth - 1);
        evaluatedMove.evaluation *= -1;
        std::cout << evaluatedMove.evaluation << '\n'; 
        game.unmakeMove();
        if (evaluatedMove.evaluation > bestEvaluatedMove.evaluation) {
            bestEvaluatedMove.move = *moveOption;
            bestEvaluatedMove.evaluation = evaluatedMove.evaluation;
        }
    }
    return bestEvaluatedMove;
}
