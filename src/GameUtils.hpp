#pragma once

#include "Game.hpp"
#include "Move.hpp"

auto positionToCoordinates(const Position &position) -> Coordinates;
auto coordinatesToPosition(const Coordinates &coordinates) -> Position;
auto fieldToInternalField(const Field &field) -> InternalField;
auto maskForPosition(Position position) -> InternalField;
auto printMove(const Move& m) -> void;
