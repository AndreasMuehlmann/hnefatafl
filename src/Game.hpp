#pragma once

#define FIELD_SIZE 9

enum Figur {
    None,
    King, 
    Wiking,
    Guard
}; 

enum Player {
    KingAttacker,
    KingDefender
};

struct Position {
    unsigned int x;
    unsigned int y;
};

class Game {
private:
    Figur field[FIELD_SIZE][FIELD_SIZE];
    Player toMove;
public:
    Game();
    bool move(Position from, Position to);
    void printField();
};
