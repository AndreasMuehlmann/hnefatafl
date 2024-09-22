#include <random>

#include "Player.hpp"

class RandomMovesGenerator : public Player {

  public:
    RandomMovesGenerator();
    ~RandomMovesGenerator() override = default;
    auto getMove(const Game &game) -> Move override;

  private:
    std::mt19937 m_rng;
    std::uniform_int_distribution<std::mt19937::result_type> m_dist;
};
