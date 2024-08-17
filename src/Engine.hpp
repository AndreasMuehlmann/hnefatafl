#include <vector>

#include "Game.hpp"


std::vector<Vec2D> getFigursToMove(const Field& field, bool wikingsToMove);
void insertAvailableMovesFigurInDirection(std::vector<Move>& availableMoves, const Field& field, Vec2D from, Vec2D direction);
std::vector<Move> getAvailableMoves(const Field& field, std::vector<Vec2D>& figursToMove);
int minimax(Game game, Move move, unsigned int depth, int alpha, int beta, bool wikingsToMove);


class Engine {
public:
    Engine(Game& game, unsigned int depth);
    Move getMove();

private:
    Game& game;
    unsigned int depth;
};
