#pragma once

#include <chrono>

#include "MovePath.hpp"
#include "Player.hpp"

using Clock = std::chrono::high_resolution_clock;

class Negamax : public Player {

  public:
    Negamax(unsigned int thinkingTimeMs);
    Negamax(unsigned int thinkingTimeMs, unsigned int maxDepth);
    ~Negamax() override = default;
    auto getMove(const Game &game) -> Move override;
    auto negamax(Game &game, Move move, unsigned int depth, int alpha, int beta,
                 MovePath &principalVariation) -> int;

  private:
    unsigned int m_thinkingTimeMs;
    unsigned int m_maxDepth;
    std::chrono::time_point<Clock> m_searchStart;
};
