#pragma once

#include <array>

#define FIELD_SIZE 9


enum Figur {
    None,
    King, 
    Wiking,
    Guard
}; 

struct Position {
    unsigned int x;
    unsigned int y;
};

typedef std::array<std::array<Figur, FIELD_SIZE>, FIELD_SIZE> Field ;

class Game {
private:
    Field field;
    bool wikingsToMove;
    Position kingPosition;

    void captureXAxis(Position lastMovedTo, int direction);
    void captureYAxis(Position lastMovedTo, int direction);
    bool isBlockedXAxis(unsigned int fromX, unsigned int toX, unsigned int y);
    bool isBlockedYAxis(unsigned int fromY, unsigned toY, unsigned int x);
public:
    Game();
    Game(Field field);
    bool areWikingsToMove();
    Figur getFigurAt(Position position);
    void setFigurAt(Figur figur, Position position);
    void getFieldCopy(Field fieldToCopyInto);
    void move(Position from, Position to);
    void updateField(Position lastMovedTo);
    bool isGameOver(Position lastMovedTo);
    void moveDone();
    void printField();
};
