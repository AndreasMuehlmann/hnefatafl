#include <chrono>
#include <limits>
#include <iostream>

#include "AvailableMovesGenerator.hpp"
#include "Game.hpp"
#include "Negamax.hpp"
#include "SearchUtils.hpp"

constexpr int winning_value = 10000;

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
        std::cout << "depth: " << depth << '\n';
        //std::cout << "eval: " << evaluatedMove.evaluation << '\n';
        if (evaluatedMove.evaluation == winning_value) { 
            return evaluatedMove.move;
        }
        bestEvaluatedMove = evaluatedMove;
    }
    return bestEvaluatedMove.move;
}

auto Negamax::negamax(Game &game, Move move, unsigned int depth) -> EvaluatedMove {
    if (move.from != FIELDS) {
        Winner winner = game.makeMove(move);
        if (winner == Winner::Attacker) {
            return {move, winning_value};
        } 
        if (winner == Winner::Defender) {
            return {move, -winning_value};
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
        EvaluatedMove evaluatedMove = negamax(game, *moveOption, depth - 1);
        evaluatedMove.evaluation *= -1;
        game.unmakeMove();
        if (evaluatedMove.evaluation > bestEvaluatedMove.evaluation) {
            bestEvaluatedMove.move = *moveOption;
            bestEvaluatedMove.evaluation = evaluatedMove.evaluation;
        }
    }
    return bestEvaluatedMove;
}
