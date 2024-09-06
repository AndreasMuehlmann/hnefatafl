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
