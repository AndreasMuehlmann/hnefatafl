#pragma once

#include "Game.hpp"
#include "Move.hpp"

auto positionToCoordinates(const Position &position) -> Coordinates;
auto coordinatesToPosition(const Coordinates &coordinates) -> Position;
auto fieldToInternalField(const Field &field) -> InternalField;
auto maskForPosition(const Position& position) -> InternalField;
auto numberToPosition(const Position& position, const uint8_t& number) -> InternalField;
auto maskedFieldMatchesPosition(const InternalField &field, const InternalField &mask,
                                const InternalField &position) -> bool;
auto printCoordinates(const Coordinates &coordinates) -> void;
auto printMove(const Move &m) -> void;
auto positionInBounds(const Position &position) -> bool;
auto isDefender(const Figur &figur) -> bool;
auto isAttacker(const Figur &figur) -> bool;
auto bitShiftLeft(const InternalField& field, const uint8_t& shift) -> InternalField;
auto bitShiftRight(const InternalField& field, const uint8_t& shift) -> InternalField;
auto possibleCapture(const InternalField& field, const Position &lastMovedTo, const bool& attackersToMove) -> bool;
