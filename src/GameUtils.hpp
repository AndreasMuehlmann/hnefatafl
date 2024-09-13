#pragma once

#include "Game.hpp"
#include "Move.hpp"

auto positionToCoordinates(Position position) -> Coordinates;
auto coordinatesToPosition(Coordinates coordinates) -> Position;
auto fieldToInternalField(const Field& field) -> InternalField;
auto maskForPosition(Position position) -> InternalField;
auto numberToPosition(Position position, uint8_t number) -> InternalField;
auto maskedFieldMatchesPosition(const InternalField &field, const InternalField &mask,
                                const InternalField &position) -> bool;
auto printCoordinates(Coordinates coordinates) -> void;
auto printMove(Move m) -> void;
auto positionInBounds(Position position) -> bool;
auto isDefender(Figur figur) -> bool;
auto isAttacker(Figur figur) -> bool;
auto bitShiftLeft(const InternalField& field, uint8_t shift) -> InternalField;
auto bitShiftRight(const InternalField& field, uint8_t shift) -> InternalField;
auto possibleCapture(const InternalField& field, Position lastMovedTo, bool attackersToMove) -> bool;
