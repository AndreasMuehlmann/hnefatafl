#include <iostream>

#include "Game.hpp"


bool isKingAttacker(Figur figur) {
    return figur == Figur::Wiking;
}


bool isKingDefender(Figur figur) {
    return figur == Figur::Guard || figur == Figur::King;
}


Game::Game() {
    wikingsToMove = true;

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


bool Game::isBlockedXRange(unsigned int fromX, unsigned int toX, unsigned int y) {
    for (int x = fromX; fromX <= toX; x++) {
        if (field[x][y] != Figur::None) {
            return false;
        }
    }
    return true;
}


bool Game::isBlockedYRange(unsigned int fromY, unsigned toY, unsigned int x) {
    for (int y = fromY; fromY <= toY; y++) {
        if (field[x][y] != Figur::None) {
            return false;
        }
    }
    return true;
}


void Game::updateField(Position lastMovedTo) {
    
}

void Game::move(Position from, Position to) {
    if (from.x >= FIELD_SIZE && from.y >= FIELD_SIZE) {
        throw std::invalid_argument("Position to move away from out of field range.");
    } else if (to.x >= FIELD_SIZE && to.y >= FIELD_SIZE) {
        throw std::invalid_argument("Position to move to out of field range.");
    } else if (from.x == to.x && from.y == to.y) {
        throw std::invalid_argument("Position to move to and position to move from are equal.");
    } else if (field[from.x][from.y] == Figur::None) {
        throw std::invalid_argument("On the position to move away from is no figur.");
    } else if ((wikingsToMove && isKingDefender(field[from.x][from.y])) 
            || (!wikingsToMove && isKingAttacker(field[from.x][from.y]))) {
        throw std::invalid_argument("The figur belongs to the other player.");
    } else if (from.x != to.x && from.y != to.y) {
        throw std::invalid_argument("Diagonal movement is not allowed.");
    }

    if (!isBlockedXRange(from.x, to.x, from.y) && !isBlockedYRange(from.y, to.y, from.x)) {
        field[to.x][to.y] = field[from.x][from.y];
        field[from.x][from.y] = Figur::None;
    } else {
        throw std::invalid_argument("Cannot move because the path is blocked.");
    }

    updateField(to);

    wikingsToMove = !wikingsToMove;
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

