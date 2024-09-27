#include <chrono>
#include <cstdlib>
#include <iostream>
#include <limits>

#include "AvailableMovesGenerator.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"
#include "Negamax.hpp"
#include "SearchUtils.hpp"
#include "EvaluatedMovePath.hpp"

constexpr int WINNING_VALUE = 10000;
constexpr int ALPHA_BETA_VALUE = 100000;

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
        EvaluatedMovePath principalVariation{};
        const EvaluatedMove evaluatedMove =
            negamax(localGame, {FIELDS, FIELDS}, depth, -ALPHA_BETA_VALUE, ALPHA_BETA_VALUE, principalVariation);
        auto durationAfterSearch =
            std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
        if (durationAfterSearch.count() > m_thinkingTimeMs) {
            break;
        }
        //std::cout << "depth: " << depth << '\n';
        if (abs(evaluatedMove.evaluation) == WINNING_VALUE) {
            return evaluatedMove.move;
        }
        bestEvaluatedMove = evaluatedMove;
    }

    std::cout << "evaluation: " << bestEvaluatedMove.evaluation << '\n';
    return bestEvaluatedMove.move;
}

auto Negamax::negamax(Game &game, Move move, unsigned int depth, int alpha,
                      int beta, EvaluatedMovePath& principalVariation) -> EvaluatedMove {
    if (move.from != FIELDS) {
        Winner winner = game.makeMove(move);
        int sign = (game.areAttackersToMove()) ? 1 : -1;
        if (winner == Winner::Attacker) {
            return {move, sign * WINNING_VALUE};
        }
        if (winner == Winner::Defender) {
            return {move, -sign * WINNING_VALUE};
        }
        if (winner == Winner::Draw) {
            return {move, 0};
        }
    }

    EvaluatedMovePath evaluatedMovePath{};
    if (depth == 0) {
        return {move, evaluate(game)};
    };

    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
    if (duration.count() > m_thinkingTimeMs) {
        EvaluatedMove evaluatedMove{};
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
        EvaluatedMove evaluatedMove = negamax(game, *moveOption, depth - 1, -beta, -alpha, evaluatedMovePath);
        evaluatedMove.evaluation *= -1;
        game.unmakeMove();
        if (evaluatedMove.evaluation > bestEvaluatedMove.evaluation) {
            bestEvaluatedMove.move = *moveOption;
            bestEvaluatedMove.evaluation = evaluatedMove.evaluation;
            if (evaluatedMove.evaluation > alpha) {
                alpha = evaluatedMove.evaluation;
            }
        }
        if (evaluatedMove.evaluation >= beta) {
            return bestEvaluatedMove;
        }
    }
    return bestEvaluatedMove;
}
