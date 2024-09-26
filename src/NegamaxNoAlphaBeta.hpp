#pragma once

#include <chrono>

#include "EvaluatedMove.hpp"
#include "Player.hpp"

using Clock = std::chrono::high_resolution_clock;

class NegamaxNoAlphaBeta : public Player {

  public:
    NegamaxNoAlphaBeta(unsigned int thinkingTimeMs);
    NegamaxNoAlphaBeta(unsigned int thinkingTimeMs, unsigned int maxDepth);
    ~NegamaxNoAlphaBeta() override = default;
    auto getMove(const Game &game) -> Move override;
    auto negamax(Game &game, Move move, unsigned int depth) -> EvaluatedMove;

  private:
    unsigned int m_thinkingTimeMs;
    unsigned int m_maxDepth;
    std::chrono::time_point<Clock> m_searchStart;
};
