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

    kingPosition = {4, 4};
}


Game::Game(Figur newFieldFirstArrayYAxis[FIELD_SIZE][FIELD_SIZE]) {
    wikingsToMove = true;

     for (unsigned int x = 0; x < FIELD_SIZE; ++x) {
        for (unsigned int y = 0; y < FIELD_SIZE; ++y) {
            setFigurAt(newFieldFirstArrayYAxis[y][x], {x, y});
        }
    }
}


bool Game::areWikingsToMove() {
    return wikingsToMove;
}


Figur Game::getFigurAt(Position position) {
    return field[position.x][position.y];
}


void Game::setFigurAt(Figur figur, Position position) {
    field[position.x][position.y] = figur;
}


bool Game::isBlockedXAxis(unsigned int fromX, unsigned int toX, unsigned int y) {
    unsigned int low, high;
    if (fromX < toX) {
        low = fromX + 1;
        high = toX;
    } else if (fromX > toX) {
        low = toX;
        high = fromX - 1;
    } else {
        return false;
    }
    for (unsigned int x = low; x <= high; x++) {
        if (field[x][y] != Figur::None) {
            return true;
        }
    }
    return false;
}


bool Game::isBlockedYAxis(unsigned int fromY, unsigned toY, unsigned int x) {
    unsigned int low, high;
    if (fromY < toY) {
        low = fromY + 1;
        high = toY;
    } else if (fromY > toY){
        low = toY;
        high = fromY - 1;
    } else {
        return false;
    }
    for (unsigned int y = low; y <= high; y++) {
        if (field[x][y] != Figur::None) {
            return true;
        }
    }
    return false;
}


void Game::move(Position from, Position to) {
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

    if (!isBlockedXAxis(from.x, to.x, from.y) && !isBlockedYAxis(from.y, to.y, from.x)) {
        setFigurAt(figur, to);
        setFigurAt(Figur::None, from);
        if (figur == Figur::King) {
            kingPosition = to;
        }
    } else {
        throw std::invalid_argument("Cannot move because the path is blocked.");
    }
}


void Game::captureXAxis(Position lastMovedTo, int direction) {
    for (unsigned int x = lastMovedTo.x + direction; x < FIELD_SIZE; x += direction) {
        Figur figur = getFigurAt({x, lastMovedTo.y});
        if (figur == Figur::None || (wikingsToMove && figur == Figur::King)) {
            break;
        } else if ((wikingsToMove && figur == Figur::Wiking) 
                || (!wikingsToMove && (figur == Figur::Guard || figur == Figur::King))) {
            for (unsigned int xToDelete = lastMovedTo.x + direction; xToDelete * direction < x * direction; xToDelete += direction) {
                setFigurAt(Figur::None, {xToDelete, lastMovedTo.y});
            }
        }             
    }
}


void Game::captureYAxis(Position lastMovedTo, int direction) {
    for (unsigned int y = lastMovedTo.y + direction; y < FIELD_SIZE; y += direction) {
        Figur figur = getFigurAt({lastMovedTo.x, y});
        if (figur == Figur::None || (wikingsToMove && figur == Figur::King)) {
            break;
        } else if ((wikingsToMove && figur == Figur::Wiking) 
                || (!wikingsToMove && (figur == Figur::Guard || figur == Figur::King))) {
            for (unsigned int yToDelete = lastMovedTo.y + direction; yToDelete * direction < y * direction; yToDelete += direction) {
                setFigurAt(Figur::None, {lastMovedTo.x, yToDelete});
            }
        }           
    }
}


void Game::updateField(Position lastMovedTo) {
    captureXAxis(lastMovedTo, 1);
    captureXAxis(lastMovedTo, -1);
    captureYAxis(lastMovedTo, 1);
    captureYAxis(lastMovedTo, -1);
}


bool Game::isGameOver(Position lastMovedTo) {
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
