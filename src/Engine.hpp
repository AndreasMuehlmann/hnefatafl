#include <vector>

#include "Game.hpp"


struct Move {
    Vec2D from;
    Vec2D to;
};


std::vector<Vec2D> getFigursToMove(const Field& field, bool wikingsToMove);
void insertAvailableMovesFigurInDirection(std::vector<Move>& availableMoves, const Field& field, Vec2D from, Vec2D direction);
std::vector<Move> getAvailableMoves(const Field& field, bool wikingsToMove);


class Engine {
public:
    Engine(Game& game);
    Move getMove();

private:
    int minimax(Field field, unsigned int depth, int alpha, int beta, bool wikingsToMove);

    Game& game;
};
