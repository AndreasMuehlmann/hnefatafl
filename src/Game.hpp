#pragma once

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

class Game {
private:
    Figur field[FIELD_SIZE][FIELD_SIZE];
    bool wikingsToMove;
    void updateField(Position lastMovedTo);
public:
    Game();
    Figur figurAt(Position position);
    bool isBlockedXRange(unsigned int fromX, unsigned int toX, unsigned int y);
    bool isBlockedYRange(unsigned int fromY, unsigned toY, unsigned int x);
    void move(Position from, Position to);
    void printField();
};
