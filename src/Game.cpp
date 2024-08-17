#include <cstring>
#include <iostream>

#include "Game.hpp"


int sign(int x) {
    return (x > 0) - (x < 0);
}


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

    kingPosition = {4, 4};
}


Game::Game(Field newField) {
    wikingsToMove = true;
    field = newField;
}


bool Game::areWikingsToMove() {
    return wikingsToMove;
}


Figur Game::getFigurAt(Vec2D position) {
    return field[position.x][position.y];
}


void Game::setFigurAt(Figur figur, Vec2D position) {
    field[position.x][position.y] = figur;
}


void Game::getFieldCopy(Field fieldToCopyInto) {
    for (int y = 0; y < FIELD_SIZE; y++) {
        for (int x = 0; x < FIELD_SIZE; x++) {
            fieldToCopyInto[x][y] = field[x][y];
        }
    }
}


bool Game::isBlocked(Vec2D from, Vec2D to) {
    Vec2D direction = {sign(to.x - from.x), sign(to.y - from.y)};
    Vec2D positionToCheck = from;
    do {
        positionToCheck = {positionToCheck.x + direction.x, positionToCheck.y + direction.y};
        if (getFigurAt(positionToCheck) != Figur::None) {
            return true;
        }
    } while (!(positionToCheck.x == to.x && positionToCheck.y == to.y)
            && 0 <= positionToCheck.x && positionToCheck.x < FIELD_SIZE
            && 0 <= positionToCheck.y && positionToCheck.y < FIELD_SIZE);
    return false;
}


void Game::move(Vec2D from, Vec2D to) {
    if (from.x >= FIELD_SIZE || from.y >= FIELD_SIZE) {
        throw std::invalid_argument("Position to move away from out of field range.");
    } else if (to.x >= FIELD_SIZE || to.y >= FIELD_SIZE) {
        throw std::invalid_argument("Position to move to out of field range.");
    } else if (from.x == to.x && from.y == to.y) {
        throw std::invalid_argument("Position to move to and position to move from are equal.");
    } 

    Figur figur = getFigurAt(from);
    if (figur == Figur::None) {
        throw std::invalid_argument("On the position to move away from is no figur.");
    } else if ((wikingsToMove && isKingDefender(getFigurAt(from))) 
            || (!wikingsToMove && isKingAttacker(getFigurAt(from)))) {
        throw std::invalid_argument("The figur belongs to the other player.");
    } else if (from.x != to.x && from.y != to.y) {
        throw std::invalid_argument("Diagonal movement is not allowed.");
    } else if ((to.x == 0 || to.x == 8) && (to.y == 0 || to.y == 8) && figur != Figur::King) {
        throw std::invalid_argument("Cannot move into the corner unless the figur is the king.");
    } else if ((to.x == 4 && to.y == 4) && figur != Figur::King) {
        throw std::invalid_argument("Cannot move into the center position unless the figur is the king.");
    }

    if (!isBlocked(from, to)) {
        setFigurAt(figur, to);
        setFigurAt(Figur::None, from);
        if (figur == Figur::King) {
            kingPosition = to;
        }
    } else {
        throw std::invalid_argument("Cannot move because the path is blocked.");
    }
}


void Game::capture(Vec2D lastMovedTo, Vec2D direction) {
    Vec2D positionToCheck = {lastMovedTo.x + direction.x, lastMovedTo.y + direction.y};
    while (0 <= positionToCheck.x && positionToCheck.x < FIELD_SIZE && 0 <= positionToCheck.y && positionToCheck.y < FIELD_SIZE) {
        Figur figur = getFigurAt(positionToCheck);
        if (figur == Figur::None || (wikingsToMove && figur == Figur::King)) {
            break;
        } else if ((wikingsToMove && figur == Figur::Wiking) 
                || (!wikingsToMove && (figur == Figur::Guard || figur == Figur::King))) {
                Vec2D positionToDelete = {lastMovedTo.x + direction.x, lastMovedTo.y + direction.y};
                while (!(positionToDelete.x == positionToCheck.x && positionToDelete.y == positionToCheck.y)) {
                    setFigurAt(Figur::None, positionToDelete);
                    positionToDelete = {positionToDelete.x + direction.x, positionToDelete.y + direction.y};
                }
        }           
        positionToCheck = {positionToCheck.x + direction.x, positionToCheck.y + direction.y};
    }
}


void Game::updateField(Vec2D lastMovedTo) {
    capture(lastMovedTo, {1, 0});
    capture(lastMovedTo, {-1, 0});
    capture(lastMovedTo, {0, 1});
    capture(lastMovedTo, {0, -1});
}


bool Game::isGameOver(Vec2D lastMovedTo) {
    if (getFigurAt(lastMovedTo) == Figur::King) {
        return (kingPosition.x == 0 || kingPosition.x == FIELD_SIZE - 1) 
            && (kingPosition.y == 0 || kingPosition.y == FIELD_SIZE - 1);
    } else if (wikingsToMove) {
        return (kingPosition.x == FIELD_SIZE - 1 || getFigurAt({kingPosition.x + 1, kingPosition.y}) == Figur::Wiking)
            && (kingPosition.x == 0 || getFigurAt({kingPosition.x - 1, kingPosition.y}) == Figur::Wiking)
            && (kingPosition.y == FIELD_SIZE - 1 || getFigurAt({kingPosition.x, kingPosition.y + 1}) == Figur::Wiking)
            && (kingPosition.y == 0 || getFigurAt({kingPosition.x, kingPosition.y - 1}) == Figur::Wiking);
    }
    return false;
}


void Game::moveDone() {
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
