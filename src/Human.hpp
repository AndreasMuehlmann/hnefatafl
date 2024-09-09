#include "Player.hpp"

class Human : public Player {

  public:
    ~Human() override = default;
    auto getMove(const Game &game) -> Move override;
};
