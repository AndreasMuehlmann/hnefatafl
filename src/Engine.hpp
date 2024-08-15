#include <vector>

#include "Game.hpp"


struct Move {
    Position from;
    Position to;
};


class Engine {
public:
    std::vector<Position> getFigursToMove(Field field, bool wikingsToMove);
    std::vector<Move> getAvailableMoves(Field field, bool wikingsToMove);
    int minimax(Field field, unsigned int depth, int alpha, int beta, bool wikingsToMove);
};
