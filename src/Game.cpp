#include <iostream>

#include "Game.hpp"

Game::Game() {
    toMove = Player::KingAttacker;

    for (int x = 0; x < FIELD_SIZE; x++) {
        for (int y = 0; y < FIELD_SIZE; y++) {
            field[x][y] = Figur::None; 
        }
    }

    field[3][0] = Figur::Wiking;
    field[4][1] = Figur::Wiking;
    field[4][0] = Figur::Wiking;
    field[5][0] = Figur::Wiking;

    field[3][8] = Figur::Wiking;
    field[4][7] = Figur::Wiking;
    field[4][8] = Figur::Wiking;
    field[5][8] = Figur::Wiking;

    field[0][3] = Figur::Wiking;
    field[1][4] = Figur::Wiking;
    field[0][4] = Figur::Wiking;
    field[0][5] = Figur::Wiking;

    field[8][3] = Figur::Wiking;
    field[7][4] = Figur::Wiking;
    field[8][4] = Figur::Wiking;
    field[8][5] = Figur::Wiking;

    field[4][4] = Figur::King;

    field[4][2] = Figur::Guard;
    field[4][3] = Figur::Guard;

    field[4][5] = Figur::Guard;
    field[4][6] = Figur::Guard;

    field[2][4] = Figur::Guard;
    field[3][4] = Figur::Guard;
              
    field[5][4] = Figur::Guard;
    field[6][4] = Figur::Guard;
}

bool Game::move(Position from, Position to) {
    if (from.x < FIELD_SIZE && from.y < FIELD_SIZE) {
        throw std::invalid_argument("Position to move from out of field range.");
    }
    if (to.x < FIELD_SIZE && to.y < FIELD_SIZE) {
        throw std::invalid_argument("Position to move to out of field range.");
    }
    return true;
}


void Game::printField() {
    for (int y = 0; y < FIELD_SIZE; y++) {
        for (int x = 0; x < FIELD_SIZE; x++) {
            switch (field[x][y]) {
                case Figur::None:
                    std::cout << "_";
                    break;
                case Figur::Wiking:
                    std::cout << "W";
                    break;
                case Figur::Guard:
                    std::cout << "G";
                    break;
                case Figur::King:
                    std::cout << "K";
                    break;
            }
        }
        std::cout << std::endl;
    }
}

