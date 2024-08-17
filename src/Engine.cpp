#include "Engine.hpp"
#include "Game.hpp"

std::vector<Vec2D> getFigursToMove(const Field& field, bool wikingsToMove) {
    std::vector<Vec2D> startingPositions;
    startingPositions.reserve(20);

    for (int x = 0; x < FIELD_SIZE; x++) {
        for (int y = 0; y < FIELD_SIZE; y++) {
            if ((wikingsToMove && field[x][y] == Figur::Wiking) || (!wikingsToMove && (field[x][y] == Figur::Guard || field[x][y] == Figur::King))) {
                startingPositions.push_back({x, y});
            }
        }
    }
    return startingPositions;
}


void insertAvailableMovesFigurInDirection(std::vector<Move>& availableMoves, const Field& field, Vec2D from, Vec2D direction) {
    Vec2D position = {from.x + direction.x, from.y + direction.y};
    while (0 <= position.x && position.x < FIELD_SIZE && 0 <= position.y && position.y < FIELD_SIZE) {
        if (field[position.x][position.y] != Figur::None) {
            return;
        }
        availableMoves.push_back({from, position});
        position = {position.x + direction.x, position.y + direction.y};
    }
}


std::vector<Move> getAvailableMoves(const Field& field, bool wikingsToMove) {
    std::vector<Move> availableMoves;
    availableMoves.reserve(120);

    std::vector<Vec2D> FiguresToMove = getFigursToMove(field, wikingsToMove);
    for (Vec2D figurePosition : FiguresToMove) {
        insertAvailableMovesFigurInDirection(availableMoves, field, figurePosition, {1, 0});
        insertAvailableMovesFigurInDirection(availableMoves, field, figurePosition, {-1, 0});
        insertAvailableMovesFigurInDirection(availableMoves, field, figurePosition, {0, 1});
        insertAvailableMovesFigurInDirection(availableMoves, field, figurePosition, {0, -1});
    }
    
    return availableMoves;
}


Engine::Engine(Game& game)
    : game(game) {}


Move Engine::getMove() {
    return {{0, 0}, {0, 0}};
}


int Engine::minimax(Field field, unsigned int depth, int alpha, int beta, bool wikingsToMove) {
    (void)depth;
    (void)alpha;
    (void)beta;
    std::vector<Move> vec = getAvailableMoves(field, wikingsToMove);
    return 0;
}
