#pragma once

#include <array>

#define FIELD_SIZE 9


enum Figur {
    None,
    King, 
    Wiking,
    Guard
}; 


struct Vec2D {
    int x;
    int y;
};



struct Move {
    Vec2D from;
    Vec2D to;
};

typedef std::array<std::array<Figur, FIELD_SIZE>, FIELD_SIZE> Field ;

class Game {

private:
    Field field;
    bool wikingsToMove;
    Vec2D kingPosition;
    unsigned int guardCount;
    unsigned int wikingCount;

    void capture(Vec2D lastMovedTo, Vec2D direction);
    bool isBlocked(Move move) const;
    void setFigurAt(Figur figur, Vec2D position);
public:
    Game();
    Game(Field field);
    bool areWikingsToMove() const;
    Figur getFigurAt(Vec2D position) const;
    const Field& getField() const;
    unsigned int getWikingCount() const;
    unsigned int getGuardCount() const;
    Vec2D getKingPosition() const;
    void moveUnchecked(Move move);
    void move(Move move);
    void updateField(Vec2D lastMovedTo);
    Figur whoWon() const;
    void printField() const;
};
