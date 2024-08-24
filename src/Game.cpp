#include <cstring>
#include <iostream>
#include <stdexcept>

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
    wikingCount = 16;
    guardCount = 8;
}


Game::Game(Field newField) {
    wikingsToMove = true;
    field = newField;
    unsigned int kingCount = 0;
    for (int x = 0; x < FIELD_SIZE; x++) {
        for (int y = 0; y < FIELD_SIZE; y++) {
            if (getFigurAt({x, y}) == Figur::Wiking) {
                wikingCount += 1;
            } else if (getFigurAt({x, y}) == Figur::Guard) {
                guardCount += 1;
            } else if (getFigurAt({x, y}) == Figur::King) {
                kingCount += 1;
                if (kingCount >= 2) {
                    throw std::invalid_argument("Two kings in field.");
                }
                kingPosition = {x, y};
            }
        }
    }
}


bool Game::areWikingsToMove() const {
    return wikingsToMove;
}


Figur Game::getFigurAt(Vec2D position) const {
    return field[position.x][position.y];
}


void Game::setFigurAt(Figur figur, Vec2D position) {
    field[position.x][position.y] = figur;
}


const Field& Game::getField() const{
    return field;
}


unsigned int Game::getWikingCount() const {
    return wikingCount;
}


unsigned int Game::getGuardCount() const {
    return guardCount;
}


void Game::moveUnchecked(Move move) {
    Figur figur = getFigurAt(move.from);
    setFigurAt(figur, move.to);
    setFigurAt(Figur::None, move.from);
    if (figur == Figur::King) {
        kingPosition = move.to;
    }

    wikingsToMove = !wikingsToMove;
}


bool Game::isBlocked(Move move) const {
    Vec2D direction = {sign(move.to.x - move.from.x), sign(move.to.y - move.from.y)};
    Vec2D positionToCheck = move.from;
    do {
        positionToCheck = {positionToCheck.x + direction.x, positionToCheck.y + direction.y};
        if (getFigurAt(positionToCheck) != Figur::None) {
            return true;
        }
    } while (!(positionToCheck.x == move.to.x && positionToCheck.y == move.to.y)
            && 0 <= positionToCheck.x && positionToCheck.x < FIELD_SIZE
            && 0 <= positionToCheck.y && positionToCheck.y < FIELD_SIZE);
    return false;
}


void Game::move(Move move) {
    if (move.from.x >= FIELD_SIZE || move.from.y >= FIELD_SIZE) {
        throw std::invalid_argument("Position to move away from out of field range.");
    } else if (move.to.x >= FIELD_SIZE || move.to.y >= FIELD_SIZE) {
        throw std::invalid_argument("Position to move to out of field range.");
    } else if (move.from.x == move.to.x && move.from.y == move.to.y) {
        throw std::invalid_argument("Position to move to and position to move from are equal.");
    } 

    Figur figur = getFigurAt(move.from);
    if (figur == Figur::None) {
        throw std::invalid_argument("On the position to move away from is no figur.");
    } else if ((wikingsToMove && isKingDefender(getFigurAt(move.from))) 
            || (!wikingsToMove && isKingAttacker(getFigurAt(move.from)))) {
        throw std::invalid_argument("The figur belongs to the other player.");
    } else if (move.from.x != move.to.x && move.from.y != move.to.y) {
        throw std::invalid_argument("Diagonal movement is not allowed.");
    } else if ((move.to.x == 0 || move.to.x == 8) && (move.to.y == 0 || move.to.y == 8) && figur != Figur::King) {
        throw std::invalid_argument("Cannot move into the corner unless the figur is the king.");
    } else if ((move.to.x == 4 && move.to.y == 4) && figur != Figur::King) {
        throw std::invalid_argument("Cannot move into the center position unless the figur is the king.");
    }

    if (!isBlocked(move)) {
        setFigurAt(figur, move.to);
        setFigurAt(Figur::None, move.from);
        if (figur == Figur::King) {
            kingPosition = move.to;
        }
    } else {
        throw std::invalid_argument("Cannot move because the path is blocked.");
    }

    wikingsToMove = !wikingsToMove;
}


void Game::capture(Vec2D lastMovedTo, Vec2D direction) {
    Vec2D positionToCheck = {lastMovedTo.x + direction.x, lastMovedTo.y + direction.y};
    Figur movedFigur = getFigurAt(lastMovedTo);
    if (!(0 <= positionToCheck.x && positionToCheck.x < FIELD_SIZE && 0 <= positionToCheck.y && positionToCheck.y < FIELD_SIZE)) {
        return;
    }
    if (movedFigur == getFigurAt(positionToCheck) || (movedFigur == Figur::Guard && getFigurAt(positionToCheck) == Figur::King)) {
        return;
    }
    while (0 <= positionToCheck.x && positionToCheck.x < FIELD_SIZE && 0 <= positionToCheck.y && positionToCheck.y < FIELD_SIZE) {
        Figur figur = getFigurAt(positionToCheck);
        if (figur == Figur::None || (movedFigur == Figur::Wiking && figur == Figur::King)) {
            break;
        } else if ((movedFigur == Figur::Wiking && figur == Figur::Wiking) 
                || (movedFigur != Figur::Wiking && (figur == Figur::Guard || figur == Figur::King))) {
                Vec2D positionToDelete = {lastMovedTo.x + direction.x, lastMovedTo.y + direction.y};
                while (!(positionToDelete.x == positionToCheck.x && positionToDelete.y == positionToCheck.y)) {
                    Figur figurToDelete = getFigurAt(positionToDelete);
                    if (figurToDelete == Figur::Guard) {
                        guardCount -= 1;
                    } else if (figurToDelete == Figur::Wiking) {
                        wikingCount -= 1;
                    }
                    setFigurAt(Figur::None, positionToDelete);
                    positionToDelete = {positionToDelete.x + direction.x, positionToDelete.y + direction.y};
                }
                break;
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


Figur Game::whoWon() const {
    if ((kingPosition.x == 0 || kingPosition.x == FIELD_SIZE - 1) 
            && (kingPosition.y == 0 || kingPosition.y == FIELD_SIZE - 1))  {
        return Figur::King;
    } else if ((kingPosition.x == FIELD_SIZE - 1 || getFigurAt({kingPosition.x + 1, kingPosition.y}) == Figur::Wiking)
            && (kingPosition.x == 0 || getFigurAt({kingPosition.x - 1, kingPosition.y}) == Figur::Wiking)
            && (kingPosition.y == FIELD_SIZE - 1 || getFigurAt({kingPosition.x, kingPosition.y + 1}) == Figur::Wiking)
            && (kingPosition.y == 0 || getFigurAt({kingPosition.x, kingPosition.y - 1}) == Figur::Wiking)) {
        return Figur::Wiking;
    }
    return Figur::None;
}


void Game::printField() const {
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
