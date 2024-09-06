#include <iostream>

#include "Engine.hpp"
#include "Game.hpp"
#include "Utils.hpp"

constexpr int evaluation_starting_value = 100000;
constexpr int winning_value = evaluation_starting_value - 1;
constexpr int alpha_beta_starting_value = winning_value;

auto staticEvaluation(const Game &game) -> int {
    constexpr int kingWorth = 6;
    return static_cast<int>(game.getWikingCount()) - static_cast<int>(game.getGuardCount()) -
           kingWorth;
}

auto Engine::minimax(Game localGame, Move move, unsigned int depth, int alpha,
                     int beta) -> EvaluatedMovePath {
    if (move.from.x != -1) {
        localGame.moveUnchecked(move);
        localGame.updateField(move.to);
        Figur winner = localGame.whoWon();
        if (winner == Figur::Wiking) {
            return {std::vector<MoveWithId>{}, winning_value};
        }
        if (winner == Figur::King) {
            return {std::vector<MoveWithId>{}, winning_value * -1};
        }

        if (depth == 0) {
            int evaluation = staticEvaluation(localGame);
            return {std::vector<MoveWithId>{}, evaluation};
        }
    }

    std::vector<Move> availableMoves = localGame.getAvailableMoves();
    EvaluatedMovePath evaluatedMovePath{};
    EvaluatedMovePath bestEvaluatedMovePath = evaluatedMovePath;
    MoveWithId bestMove{};
    if (localGame.areWikingsToMove()) {
        int max = evaluation_starting_value * -1;
        for (Move nextMove : availableMoves) {
            ids[depth]++;
            /*
            if ((ids[depth] == 1 && depth == 3) || (ids[depth] == 22 && depth ==
            2))
            {
                std::cout << "The move" << std::endl;
            }
            */
            evaluatedMovePath = minimax(localGame, nextMove, depth - 1, alpha, beta);
            if (evaluatedMovePath.evaluation > max) {
                max = evaluatedMovePath.evaluation;
                bestMove = {nextMove, ids[depth], depth};
                bestEvaluatedMovePath = evaluatedMovePath;
            }
            alpha = std::max(alpha, evaluatedMovePath.evaluation);
            if (beta <= alpha) {
                break;
            }
        }
        bestEvaluatedMovePath.movePath.push_back(bestMove);
    } else {
        int min = evaluation_starting_value;
        for (Move nextMove : availableMoves) {
            /*
            if ((ids[depth] == 1 && depth == 3) || (ids[depth] == 22 && depth ==
            2))
            {
                std::cout << "The move" << std::endl;
            }
            */
            evaluatedMovePath = minimax(localGame, nextMove, depth - 1, alpha, beta);
            if (evaluatedMovePath.evaluation < min) {
                min = evaluatedMovePath.evaluation;
                bestMove = {nextMove, ids[depth], depth};
                bestEvaluatedMovePath = evaluatedMovePath;
            }
            beta = std::min(beta, evaluatedMovePath.evaluation);
            if (beta <= alpha) {
                break;
            }
        }
        bestEvaluatedMovePath.movePath.push_back(bestMove);
    }
    return bestEvaluatedMovePath;
}

Engine::Engine(Game &game, unsigned int maxDepth) : m_game(game), m_maxDepth(maxDepth) {
    for (unsigned int i = 0; i < maxDepth + 1; i++) {
        ids.push_back(0);
    }
}

auto Engine::getMove() -> Move {
    Move move = {{-1, -1}, {-1, -1}};
    EvaluatedMovePath evaluatedMovePath = {std::vector<MoveWithId>{{move, 0, 0}}, 0};
    for (unsigned int depth = 1; depth < m_maxDepth + 1; depth++) {
        evaluatedMovePath =
            minimax(m_game, move, depth, alpha_beta_starting_value * -1, alpha_beta_starting_value);
        if (std::abs(evaluatedMovePath.evaluation) == winning_value) {
            moveAlongPath(m_game, evaluatedMovePath);
            std::cout << "Evaluation: " << evaluatedMovePath.evaluation << std::endl;
            return evaluatedMovePath.movePath[evaluatedMovePath.movePath.size() - 1].move;
        }
    }
    moveAlongPath(m_game, evaluatedMovePath);
    std::cout << "Evaluation: " << evaluatedMovePath.evaluation << std::endl;
    return evaluatedMovePath.movePath[evaluatedMovePath.movePath.size() - 1].move;
}
