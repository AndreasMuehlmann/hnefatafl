#pragma once

#include <vector>
#include <chrono>

#include "Game.hpp"

struct MoveWithId {
    Move move;
    unsigned int id;
    unsigned int depth;
};

struct EvaluatedMovePath {
    std::vector<MoveWithId> movePath;
    double evaluation;
};


std::vector<Vec2D> getFigursToMove(const Field& field, bool wikingsToMove);
void insertAvailableMovesFigurInDirection(std::vector<Move>& availableMoves, const Field& field, Vec2D from, Vec2D direction);
std::vector<Move> getAvailableMoves(const Field& field, std::vector<Vec2D>& figursToMove);


class Engine {
public:
    Engine(Game& game, unsigned int maxDepth);
    EvaluatedMovePath minimax(Game game, Move move, unsigned int depth, double alpha, double beta);
    Move getMove();

private:
    Game& game;
    unsigned int maxDepth;
    unsigned int availableMiliseconds;
    std::chrono::steady_clock::time_point start;
    std::vector<unsigned int> ids;
};
