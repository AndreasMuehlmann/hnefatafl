#include "Player.hpp"

class RandomMovesGenerator : public Player {

  public:
    RandomMovesGenerator();
    ~RandomMovesGenerator() override = default;
    auto getMove(const Game &game) -> Move override;
};
