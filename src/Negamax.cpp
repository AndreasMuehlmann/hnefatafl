#include <chrono>
#include <cstdlib>
#include <cstring>
#include <limits>

#include "AvailableMovesGenerator.hpp"
#include "MovePath.hpp"
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
    MovePath lastPrincipalVariation{};
    int lastEvaluation = 0;
    for (unsigned int depth = 1; depth <= m_maxDepth; depth++) {
        const auto duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
        if (duration.count() > m_thinkingTimeMs) {
            break;
        }
        Game localGame = game;
        MovePath principalVariation{};
        int evaluation = negamax(localGame, depth, -ALPHA_BETA_VALUE, ALPHA_BETA_VALUE, principalVariation);
        evaluation *= game.areAttackersToMove() ? 1 : -1;
        const auto durationAfterSearch =
            std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
        if (durationAfterSearch.count() > m_thinkingTimeMs) {
            break;
        }
        lastPrincipalVariation = principalVariation;
        lastEvaluation = evaluation;
        if (verbosity >= 4) {
            printPrincipalVariation(game, principalVariation, evaluation);
        }
        if (abs(evaluation) == WINNING_VALUE) {
            return principalVariation.moves[0];
        }
    }
    if (verbosity >= 3) {
        printPrincipalVariation(game, lastPrincipalVariation, lastEvaluation);
    }
    return lastPrincipalVariation.moves[0];
}

auto Negamax::negamax(Game &game, unsigned int depth, int alpha, int beta,
                      MovePath &principalVariation) -> int {
    if (depth == 0) {
        principalVariation.moveCount = 0;
        return evaluate(game);
    };

    const auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(Clock::now() - m_searchStart);
    if (duration.count() > m_thinkingTimeMs) {
        return 0;
    }

    MovePath movePath{};
    int bestEvaluation = std::numeric_limits<int>::min();
    AvailableMovesGenerator availableMovesGenerator(game);
    while (true) {
        auto moveOption = availableMovesGenerator.next();
        if (moveOption == std::nullopt) {
            break;
        }
        int evaluation = 0;
        const Winner winner = game.makeMove(*moveOption);
        const int sign = game.areAttackersToMove() ? 1 : -1;
        if (winner == Winner::Attacker) {
            principalVariation.moveCount = 0;
            evaluation = -sign * WINNING_VALUE;
        }
        else if (winner == Winner::Defender) {
            principalVariation.moveCount = 0;
            evaluation = sign * WINNING_VALUE;
        }
        else if (winner == Winner::Draw) {
            principalVariation.moveCount = 0;
            evaluation = 0;
        }
        else {
            evaluation = -negamax(game, depth - 1, -beta, -alpha, movePath);
        }
        game.unmakeMove();
        if (evaluation > bestEvaluation) {
            bestEvaluation = evaluation;
            principalVariation.moves[0] = *moveOption;
            memcpy(static_cast<Move *>(principalVariation.moves) + 1,
                   static_cast<Move *>(movePath.moves),
                   movePath.moveCount * sizeof(Move));
            principalVariation.moveCount = movePath.moveCount + 1;
            if (evaluation > alpha) {
                alpha = evaluation;
            }
        }
        if (evaluation >= beta) {
            return bestEvaluation;
        }
    }
    return bestEvaluation;
}
