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

    void capture(Vec2D lastMovedTo, Vec2D direction);
    bool isBlocked(Move move);
    void setFigurAt(Figur figur, Vec2D position);
public:
    Game();
    Game(Field field);
    bool areWikingsToMove();
    Figur getFigurAt(Vec2D position);
    const Field& getField();
    void moveUnchecked(Move move);
    void move(Move move);
    void updateField(Vec2D lastMovedTo);
    Figur whoWon();
    void printField();
};
