#include <chrono>
#include <cstdlib>
#include <cstring>
#include <limits>

#include "AvailableMovesGenerator.hpp"
#include "EvaluatedMovePath.hpp"
#include "Game.hpp"
#include "GlobalConfig.hpp"
#include "Move.hpp"
#include "Negamax.hpp"
#include "SearchUtils.hpp"

constexpr int WINNING_VALUE = 10000;
constexpr int ALPHA_BETA_VALUE = 100000;

Negamax::Negamax(unsigned int thinkingTimeMs)
    : m_thinkingTimeMs(thinkingTimeMs), m_maxDepth(std::numeric_limits<unsigned int>::max()) {}

Negamax::Negamax(unsigned int thinkingTimeMs, unsigned int maxDepth)
    : m_thinkingTimeMs(thinkingTimeMs), m_maxDepth(maxDepth) {}

auto Negamax::getMove(const Game &game) -> Move {
    m_searchStart = Clock::now();
    EvaluatedMove bestEvaluatedMove{};
    EvaluatedMovePath lastPrincipalVariation{};
    for (unsigned int depth = 1; depth <= m_maxDepth; depth++) {
        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
        if (duration.count() > m_thinkingTimeMs) {
            break;
        }
        Game localGame = game;
        EvaluatedMovePath principalVariation{};
        const EvaluatedMove evaluatedMove =
            negamax(localGame, {FIELDS, FIELDS}, depth, -ALPHA_BETA_VALUE, ALPHA_BETA_VALUE,
                    principalVariation);
        const auto durationAfterSearch =
            std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
        if (durationAfterSearch.count() > m_thinkingTimeMs) {
            break;
        }
        lastPrincipalVariation = principalVariation;
        if (verbosity >= 4) {
            printPrincipalVariation(game, principalVariation);
        }
        if (abs(evaluatedMove.evaluation) == WINNING_VALUE) {
            return evaluatedMove.move;
        }
        bestEvaluatedMove = evaluatedMove;
    }
    if (verbosity >= 3) {
        printPrincipalVariation(game, lastPrincipalVariation);
    }
    return bestEvaluatedMove.move;
}

auto Negamax::negamax(Game &game, Move move, unsigned int depth, int alpha, int beta,
                      EvaluatedMovePath &principalVariation) -> EvaluatedMove {
    if (move.from != FIELDS) {
        const Winner winner = game.makeMove(move);
        const int sign = (game.areAttackersToMove()) ? 1 : -1;
        if (winner == Winner::Attacker) {
            principalVariation.moveCount = 0;
            principalVariation.evaluation = sign * WINNING_VALUE;
            return {move, sign * WINNING_VALUE};
        }
        if (winner == Winner::Defender) {
            principalVariation.moveCount = 0;
            principalVariation.evaluation = -sign * WINNING_VALUE;
            return {move, -sign * WINNING_VALUE};
        }
        if (winner == Winner::Draw) {
            principalVariation.moveCount = 0;
            principalVariation.evaluation = 0;
            return {move, 0};
        }
    }

    if (depth == 0) {
        const int evaluation = evaluate(game);
        principalVariation.moveCount = 0;
        principalVariation.evaluation = evaluation;
        return {move, evaluation};
    };

    const auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
    if (duration.count() > m_thinkingTimeMs) {
        const EvaluatedMove evaluatedMove{};
        return evaluatedMove;
    }

    EvaluatedMovePath evaluatedMovePath{};
    EvaluatedMove bestEvaluatedMove{};
    bestEvaluatedMove.evaluation = std::numeric_limits<int>::min();
    AvailableMovesGenerator availableMovesGenerator(game);
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) {
            break;
        }
        EvaluatedMove evaluatedMove =
            negamax(game, *moveOption, depth - 1, -beta, -alpha, evaluatedMovePath);
        evaluatedMove.evaluation *= -1;
        game.unmakeMove();
        if (evaluatedMove.evaluation > bestEvaluatedMove.evaluation) {
            bestEvaluatedMove.move = *moveOption;
            bestEvaluatedMove.evaluation = evaluatedMove.evaluation;
            principalVariation.moves[0] = *moveOption;
            memcpy(static_cast<Move *>(principalVariation.moves) + 1,
                   static_cast<Move *>(evaluatedMovePath.moves),
                   evaluatedMovePath.moveCount * sizeof(Move));
            principalVariation.moveCount = evaluatedMovePath.moveCount + 1;
            principalVariation.evaluation = -evaluatedMovePath.evaluation;
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
