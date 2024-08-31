#include <iostream>

#include "Engine.hpp"
#include "Game.hpp"
#include "Utils.hpp"

constexpr int evaluation_starting_value = 100000;
constexpr int winning_value = evaluation_starting_value - 1;
constexpr int alpha_beta_starting_value = evaluation_starting_value;
constexpr size_t max_amount_figurs = 25;
constexpr size_t reserverd_moves = 100;

auto getFigursToMove(const Game &game, bool wikingsToMove) -> std::vector<Vec2D> {
    std::vector<Vec2D> startingPositions;
    startingPositions.reserve(max_amount_figurs);

    for (size_t x = 0; x < FIELD_SIZE; x++) {
        for (size_t y = 0; y < FIELD_SIZE; y++) {
            Vec2D position = {static_cast<int>(x), static_cast<int>(y)};
            Figur figur = game.getFigurAt(position);
            if ((wikingsToMove && isKingAttacker(figur)) ||
                (!wikingsToMove && isKingDefender(figur))) {
                startingPositions.push_back(position);
            }
        }
    }
    return startingPositions;
}

void insertAvailableMovesFigurInDirection(std::vector<Move> &availableMoves, const Game &game,
                                          Vec2D from, Vec2D direction) {
    Vec2D position = {from.x + direction.x, from.y + direction.y};
    while (0 <= position.x && static_cast<size_t>(position.x) < FIELD_SIZE && 0 <= position.y &&
           static_cast<size_t>(position.y) < FIELD_SIZE) {
        if (game.getFigurAt(position) != Figur::None) {
            return;
        }
        if (game.getFigurAt(position) != Figur::King) {
            if ((position.x == (FIELD_SIZE - 1) / 2 && position.y == (FIELD_SIZE - 1) / 2) ||
                ((position.x == 0 || position.x == FIELD_SIZE - 1) &&
                 (position.y == 0 || position.y == FIELD_SIZE - 1))) {
                return;
            }
        }
        availableMoves.push_back({from, position});
        position = {position.x + direction.x, position.y + direction.y};
    }
}

auto getAvailableMoves(const Game &game, std::vector<Vec2D> &figursToMove) -> std::vector<Move> {
    std::vector<Move> availableMoves;
    availableMoves.reserve(reserverd_moves);

    for (Vec2D figurePosition : figursToMove) {
        insertAvailableMovesFigurInDirection(availableMoves, game, figurePosition, {1, 0});
        insertAvailableMovesFigurInDirection(availableMoves, game, figurePosition, {-1, 0});
        insertAvailableMovesFigurInDirection(availableMoves, game, figurePosition, {0, 1});
        insertAvailableMovesFigurInDirection(availableMoves, game, figurePosition, {0, -1});
    }

    return availableMoves;
}

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

    std::vector<Vec2D> figuresToMove = getFigursToMove(localGame, localGame.areWikingsToMove());
    std::vector<Move> availableMoves = getAvailableMoves(localGame, figuresToMove);
    EvaluatedMovePath evaluatedMovePath;
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
        if (evaluatedMovePath.evaluation == winning_value ||
            evaluatedMovePath.evaluation == winning_value * -1) {
            moveAlongPath(m_game, evaluatedMovePath);
            std::cout << "Evaluation: " << evaluatedMovePath.evaluation << std::endl;
            return evaluatedMovePath.movePath[evaluatedMovePath.movePath.size() - 1].move;
        }
    }
    moveAlongPath(m_game, evaluatedMovePath);
    std::cout << "Evaluation: " << evaluatedMovePath.evaluation << std::endl;
    return evaluatedMovePath.movePath[evaluatedMovePath.movePath.size() - 1].move;
}
