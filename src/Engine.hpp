#pragma once

#include <vector>
#include <chrono>

#include "Game.hpp"


struct EvaluatedMove {
    Move move;
    int evaluation;
};


std::vector<Vec2D> getFigursToMove(const Field& field, bool wikingsToMove);
void insertAvailableMovesFigurInDirection(std::vector<Move>& availableMoves, const Field& field, Vec2D from, Vec2D direction);
std::vector<Move> getAvailableMoves(const Field& field, std::vector<Vec2D>& figursToMove);
int minimaxHelper(Game game, Move move, unsigned int depth, int alpha, int beta, bool wikingsToMove);


class Engine {
public:
    Engine(Game& game, unsigned int maxDepth);
    Move getMove();
    EvaluatedMove minimax(unsigned int depth);

private:
    Game& game;
    unsigned int maxDepth;
    unsigned int availableMiliseconds;
    std::chrono::steady_clock::time_point start;
};
