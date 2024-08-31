#pragma once

#include <vector>

#include "Game.hpp"

struct MoveWithId {
    Move move;
    unsigned int id;
    unsigned int depth;
};

struct EvaluatedMovePath {
    std::vector<MoveWithId> movePath;
    int evaluation;
};

auto getFigursToMove(const Game &game, bool wikingsToMove) -> std::vector<Vec2D>;
auto insertAvailableMovesFigurInDirection(std::vector<Move> &availableMoves, const Game &game,
                                          Vec2D from, Vec2D direction) -> void;
auto getAvailableMoves(const Game &game, std::vector<Vec2D> &figursToMove) -> std::vector<Move>;

class Engine {
  public:
    Engine(Game &game, unsigned int maxDepth);
    auto minimax(Game localGame, Move move, unsigned int depth, int alpha,
                 int beta) -> EvaluatedMovePath;
    auto getMove() -> Move;

  private:
    Game &m_game;
    unsigned int m_maxDepth;
    std::vector<unsigned int> ids;
};
