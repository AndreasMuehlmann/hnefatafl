#include <vector>

#include "Game.hpp"


struct Move {
    Vec2D from;
    Vec2D to;
};


class Engine {
public:
    std::vector<Vec2D> getFigursToMove(Field field, bool wikingsToMove);
    std::vector<Move> getAvailableMoves(Field field, bool wikingsToMove);
    int minimax(Field field, unsigned int depth, int alpha, int beta, bool wikingsToMove);
};
