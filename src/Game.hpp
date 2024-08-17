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

typedef std::array<std::array<Figur, FIELD_SIZE>, FIELD_SIZE> Field ;

class Game {
private:
    Field field;
    bool wikingsToMove;
    Vec2D kingPosition;

    void capture(Vec2D lastMovedTo, Vec2D direction);
    bool isBlocked(Vec2D startingPosition, Vec2D direction);
    void setFigurAt(Figur figur, Vec2D position);
public:
    Game();
    Game(Field field);
    bool areWikingsToMove();
    Figur getFigurAt(Vec2D position);
    const Field& getField();
    void move(Vec2D from, Vec2D to);
    void updateField(Vec2D lastMovedTo);
    bool isGameOver(Vec2D lastMovedTo);
    void moveDone();
    void printField();
};
