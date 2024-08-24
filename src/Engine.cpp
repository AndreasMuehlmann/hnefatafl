#include <iostream>

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


int staticEvaluation(const Game& game) {
    const int kingWorth = 6;
    return game.getWikingCount() - game.getGuardCount() - kingWorth;
}


EvaluatedMovePath Engine::minimax(Game game, Move move, unsigned int depth, int alpha, int beta) {
    ids[depth]++;
    if (move.from.x != -1) {
        game.move(move);
        game.updateField(move.to);
        Figur winner = game.whoWon();
        if (winner == Figur::Wiking) {
            /*
            std::cout << "eval: " << std::numeric_limits<int>::max() << std::endl;
            std::cout << "wikingsCount: " << game.getWikingCount() << ", guardsCount: " << game.getGuardCount() << std::endl;
            game.printField();
            */
            return {std::vector<MoveWithId>{}, std::numeric_limits<int>::max()};
        } else if (winner == Figur::King) {
            /*
            std::cout << "eval: " << std::numeric_limits<int>::min() << std::endl;
            std::cout << "wikingsCount: " << game.getWikingCount() << ", guardsCount: " << game.getGuardCount() << std::endl;
            game.printField();
            */
            return {std::vector<MoveWithId>{}, std::numeric_limits<int>::min()};
        }

        if (depth == 0) {
            int evaluation = staticEvaluation(game);
            /*
            std::cout << "static evaluation: " << evaluation << std::endl;
            std::cout << "wikingsCount: " << game.getWikingCount() << ", guardsCount: " << game.getGuardCount() << std::endl;
            game.printField();
            */
            return {std::vector<MoveWithId>{}, evaluation};
        }
    }

    const Field& field = game.getField();
    std::vector<Vec2D> figuresToMove = getFigursToMove(field, game.areWikingsToMove());
    std::vector<Move> availableMoves = getAvailableMoves(field, figuresToMove);
    EvaluatedMovePath evaluatedMovePath;
    EvaluatedMovePath bestEvaluatedMovePath = evaluatedMovePath;
    Move bestMove;
    if (game.areWikingsToMove()) {
        int max = std::numeric_limits<int>::min();
        for (Move move : availableMoves) {
            evaluatedMovePath = minimax(game, move, depth - 1, alpha, beta);
            if (evaluatedMovePath.evaluation > max) {
                max = evaluatedMovePath.evaluation;
                bestMove = move;
                bestEvaluatedMovePath = evaluatedMovePath;
            }
            alpha = std::max(alpha, evaluatedMovePath.evaluation);
            if (beta <= alpha) {
                break;
            }
        }
        if ((ids[depth] == 1 && depth == 5)
            || (ids[depth] == 73 && depth == 4)
            || (ids[depth] == 199 && depth == 3)
            || (ids[depth] == 5464 && depth == 2)
            || (ids[depth] == 15511 && depth == 1)
            ) 
        {
            std::cout << "The move" << std::endl;
        }
        bestEvaluatedMovePath.movePath.push_back({bestMove, ids[depth], depth});
        return bestEvaluatedMovePath;
    } else {
        int min = std::numeric_limits<int>::max();
        for (Move move : availableMoves) {
            evaluatedMovePath = minimax(game, move, depth - 1, alpha, beta);
            if (evaluatedMovePath.evaluation < min) {
                min = evaluatedMovePath.evaluation;
                bestMove = move;
                bestEvaluatedMovePath = evaluatedMovePath;
            }
            beta = std::min(beta, evaluatedMovePath.evaluation);
            if (beta <= alpha) {
                break;
            }
        }
        if ((ids[depth] == 1 && depth == 5)
            || (ids[depth] == 73 && depth == 4)
            || (ids[depth] == 199 && depth == 3)
            || (ids[depth] == 5464 && depth == 2)
            || (ids[depth] == 15511 && depth == 1)
            ) 
        {
            std::cout << "The move" << std::endl;
        }
        bestEvaluatedMovePath.movePath.push_back({bestMove, ids[depth], depth});
        return bestEvaluatedMovePath;
    }
}


void moveAlongPath(Game game, EvaluatedMovePath evaluatedMovePath) {
    std::cout << std::endl << std::endl;
    std::cout << "HOT PATH" << std::endl;
    std::cout << "Evaluation: " << evaluatedMovePath.evaluation << std::endl;
    std::cout << "Size: " << evaluatedMovePath.movePath.size() << std::endl;
    game.printField();
    for (int i = evaluatedMovePath.movePath.size() - 1; i >= 0; i--) {
        MoveWithId move = evaluatedMovePath.movePath[i];
        std::cout << "Move: "<< move.move.from.x << ", " << move.move.from.y << "; " << move.move.to.x << ", " << move.move.to.y << " | id " << move.id <<  " depth " << move.depth << std::endl;
        game.move(move.move);
        game.updateField(move.move.to);
        game.printField();
        if (game.whoWon() == Figur::King) {
            std::cout << "King won" << std::endl;
        } else if (game.whoWon() == Figur::Wiking) {
            std::cout << "Wiking won" << std::endl;
        }
    }
    std::cout << std::endl << std::endl;
}

Engine::Engine(Game& game, unsigned int maxDepth)
    : game(game), maxDepth(maxDepth) {
        for (unsigned int i = 0; i  < maxDepth; i++) {
            ids.push_back(0);
        }
}


Move Engine::getMove() {
    Move move = {{-1, -1}, {-1, -1}};
    EvaluatedMovePath evaluatedMovePath;
    for (unsigned int depth = 1; depth < maxDepth + 1; depth++) {
        evaluatedMovePath = minimax(game, move, depth, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max() - 1);
        if (evaluatedMovePath.evaluation ==  std::numeric_limits<int>::max() 
            || evaluatedMovePath.evaluation == std::numeric_limits<int>::min()) {
            moveAlongPath(game, evaluatedMovePath);
            std::cout << "Evaluation: " << evaluatedMovePath.evaluation << std::endl;
            return evaluatedMovePath.movePath[evaluatedMovePath.movePath.size() - 1].move;
        }
    }
    moveAlongPath(game, evaluatedMovePath);
    std::cout << "Evaluation: " << evaluatedMovePath.evaluation << std::endl;
    return evaluatedMovePath.movePath[evaluatedMovePath.movePath.size() - 1].move;
}

/*
int minimaxHelper(Game game, Move move, unsigned int depth, int alpha, int beta) {
    game.move(move);
    game.updateField(move.to);
    Figur winner = game.whoWon();
    if (winner == Figur::Wiking) {
        game.printField();
        return 1000;
    } else if (winner == Figur::King) {
        game.printField();
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




EvaluatedMove Engine::minimax(unsigned int depth) {
    int alpha = -10000;
    int beta = 10000;
    const Field& field = game.getField();

    std::vector<EvaluatedMove> evaluatedMoves;
    if (game.areWikingsToMove()) {
        std::vector<Vec2D> figuresToMove = getFigursToMove(field, game.areWikingsToMove());
        std::vector<Move> availableMoves = getAvailableMoves(field, figuresToMove);
        evaluatedMoves.reserve(availableMoves.size());

        #pragma omp parallel for num_threads(1)
        for (Move move : availableMoves) {
            int evaluation = minimaxHelper(game, move, depth - 1, alpha, beta);
            // std::cout << "evaluation: " << evaluation << " move " << move.from.x << ", " << move.from.y << "; " << move.to.x << ", " << move.to.y << std::endl;
            #pragma omp critical
            {
                evaluatedMoves.push_back({move, evaluation});
                alpha = std::max(alpha, evaluation);
            }
        }
        EvaluatedMove bestMove = evaluatedMoves[0];
        for (EvaluatedMove evaluatedMove : evaluatedMoves) {
            if (evaluatedMove.evaluation > bestMove.evaluation) {
                bestMove = evaluatedMove;
            }
        }
        return bestMove;
    } else {
        std::vector<Vec2D> figuresToMove = getFigursToMove(field, game.areWikingsToMove());
        std::vector<Move> availableMoves = getAvailableMoves(field, figuresToMove);
        evaluatedMoves.reserve(availableMoves.size());

        #pragma omp parallel for num_threads(1)
        for (Move move : availableMoves) {
            int evaluation = minimaxHelper(game, move, depth - 1, alpha, beta);
            #pragma omp critical
            {
                evaluatedMoves.push_back({move, evaluation});
                beta = std::min(beta, evaluation);
            }
        }
        EvaluatedMove bestMove = evaluatedMoves[0];
        for (EvaluatedMove evaluatedMove : evaluatedMoves) {
            if (evaluatedMove.evaluation < bestMove.evaluation) {
                bestMove = evaluatedMove;
            }
        }
        return bestMove;
    }
}
*/
