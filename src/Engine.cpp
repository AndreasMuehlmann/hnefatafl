#include <iostream>
#include <chrono>

#include "Engine.hpp"
#include "Game.hpp"


std::vector<Vec2D> getFigursToMove(const Field& field, bool wikingsToMove) {
    std::vector<Vec2D> startingPositions;
    startingPositions.reserve(20);

    for (int x = 0; x < FIELD_SIZE; x++) {
        for (int y = 0; y < FIELD_SIZE; y++) {
            if ((wikingsToMove && field[x][y] == Figur::Wiking) || (!wikingsToMove && (field[x][y] == Figur::Guard || field[x][y] == Figur::King))) {
                startingPositions.push_back({x, y});
            }
        }
    }
    return startingPositions;
}


void insertAvailableMovesFigurInDirection(std::vector<Move>& availableMoves, const Field& field, Vec2D from, Vec2D direction) {
    Vec2D position = {from.x + direction.x, from.y + direction.y};
    while (0 <= position.x && position.x < FIELD_SIZE && 0 <= position.y && position.y < FIELD_SIZE) {
        if (field[position.x][position.y] != Figur::None) {
            return;
        } else if (field[from.x][from.y] != Figur::King) {
            if ((position.x == 4 && position.y == 4)
                || ((position.x == 0 || position.x == 8) && (position.y == 0 || position.y == 8))) {
                return;
            }
        }
        availableMoves.push_back({from, position});
        position = {position.x + direction.x, position.y + direction.y};
    }
}


std::vector<Move> getAvailableMoves(const Field& field, std::vector<Vec2D>& figuresToMove) {
    std::vector<Move> availableMoves;
    availableMoves.reserve(100);

    for (Vec2D figurePosition : figuresToMove) {
        insertAvailableMovesFigurInDirection(availableMoves, field, figurePosition, {1, 0});
        insertAvailableMovesFigurInDirection(availableMoves, field, figurePosition, {-1, 0});
        insertAvailableMovesFigurInDirection(availableMoves, field, figurePosition, {0, 1});
        insertAvailableMovesFigurInDirection(availableMoves, field, figurePosition, {0, -1});
    }
    
    return availableMoves;
}


int staticEvaluation(const Field& field) {
    int score = 0;
    for (int x = 0; x < FIELD_SIZE; x++) {
        for (int y = 0; y < FIELD_SIZE; y++) {
            switch (field[x][y]) {
                case Figur::King:
                    score -= 6;
                    break;
                case Figur::Guard:
                    score -= 1;
                    break;
                case Figur::Wiking:
                    score += 1;
                    break;
                default:
                    break;
            }
        }
    }
    return score;
};


int minimaxHelper(Game game, Move move, unsigned int depth, int alpha, int beta) {
    game.move(move);
    game.updateField(move.to);
    Figur winner = game.whoWon();
    if (winner == Figur::Wiking) {
        return 1000;
    } else if (winner == Figur::King) {
        return -1000;
    }

    const Field& field = game.getField();

    if (depth == 0) {
        int eval = staticEvaluation(field);
        //std::cout << "eval: " << eval << std::endl;
        //game.printField();
        return eval;
    }

    if (game.areWikingsToMove()) {
        int max = -1000;
        std::vector<Vec2D> figuresToMove = getFigursToMove(field, game.areWikingsToMove());
        std::vector<Move> availableMoves = getAvailableMoves(field, figuresToMove);
        for (Move move : availableMoves) {
            int evaluation = minimaxHelper(game, move, depth - 1, alpha, beta);
            max = std::max(max, evaluation);
            alpha = std::max(alpha, evaluation);
            if (beta <= alpha) {
                break;
            }
        }
        return max;
    } else {
        int min = 1000;
        std::vector<Vec2D> figuresToMove = getFigursToMove(field, game.areWikingsToMove());
        std::vector<Move> availableMoves = getAvailableMoves(field, figuresToMove);
        for (Move move : availableMoves) {
            int evaluation = minimaxHelper(game, move, depth - 1, alpha, beta);
            min = std::min(min, evaluation);
            beta = std::min(beta, evaluation);
            if (beta <= alpha) {
                break;
            }
        }
        return min;
    }
}


Engine::Engine(Game& game, unsigned int maxDepth)
    : game(game), maxDepth(maxDepth) {}


Move Engine::getMove() {
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    // std::cout << duration.count() << std::endl;

    EvaluatedMove evaluatedMove = {{{0, 0}, {0, 0}}, 0};
    for (unsigned int depth = 1; depth < maxDepth + 1; depth++) {
        evaluatedMove = minimax(depth);
        if (evaluatedMove.evaluation == 1000 || evaluatedMove.evaluation == -1000) {
            std::cout << "evaluation of best move: " << evaluatedMove.evaluation << std::endl;
            return evaluatedMove.move;
        }
    }
    std::cout << "evaluation of best move: " << evaluatedMove.evaluation << std::endl;
    return evaluatedMove.move;
}


EvaluatedMove Engine::minimax(unsigned int depth) {
    int alpha = -10000;
    int beta = 10000;
    EvaluatedMove bestMove = {{{0, 0}, {0, 0}}, 0};
    const Field& field = game.getField();

    if (game.areWikingsToMove()) {
        int max = -100000;
        std::vector<Vec2D> figuresToMove = getFigursToMove(field, game.areWikingsToMove());
        std::vector<Move> availableMoves = getAvailableMoves(field, figuresToMove);
        for (Move move : availableMoves) {
            int evaluation = minimaxHelper(game, move, depth - 1, alpha, beta);
            // std::cout << "evaluation: " << evaluation << " move " << move.from.x << ", " << move.from.y << "; " << move.to.x << ", " << move.to.y << std::endl;
            if (evaluation > max) {
                max = evaluation;
                bestMove = {move, evaluation};
            }
            alpha = std::max(alpha, evaluation);
        }
    } else {
        int min = 100000;
        std::vector<Vec2D> figuresToMove = getFigursToMove(field, game.areWikingsToMove());
        std::vector<Move> availableMoves = getAvailableMoves(field, figuresToMove);
        for (Move move : availableMoves) {
            int evaluation = minimaxHelper(game, move, depth - 1, alpha, beta);
            if (evaluation < min) {
                min = evaluation;
                bestMove = {move, evaluation};
            }
            beta = std::min(beta, evaluation);
        }
    }
    return bestMove;
}
