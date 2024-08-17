#include "Engine.hpp"
#include "Game.hpp"


std::vector<Vec2D> Engine::getFigursToMove(Field field, bool wikingsToMove) {
    std::vector<Vec2D> startingPositions;
    startingPositions.reserve(100);

    for (int x = 0; x < FIELD_SIZE; x++) {
        for (int y = 0; y < FIELD_SIZE; y++) {
            if ((wikingsToMove && field[x][y] == Figur::Wiking) || (!wikingsToMove && (field[x][y] == Figur::Guard || field[x][y] == Figur::King))) {
                startingPositions.push_back({x, y});
            }
        }
    }
    return startingPositions;
}

std::vector<Move> Engine::getAvailableMoves(Field field, bool wikingsToMove) {
    (void)field;
    (void)wikingsToMove;
    return std::vector<Move>{};
}

int Engine::minimax(Field field, unsigned int depth, int alpha, int beta, bool wikingsToMove) {
    (void)depth;
    (void)alpha;
    (void)beta;
    getAvailableMoves(field, wikingsToMove);
    return 0;
}
