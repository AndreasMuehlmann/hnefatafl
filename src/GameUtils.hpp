#pragma once

#include "Game.hpp"
#include "Move.hpp"

auto positionToCoordinates(const Position &position) -> Coordinates;
auto coordinatesToPosition(const Coordinates &coordinates) -> Position;
auto fieldToInternalField(const Field &field) -> InternalField;
auto maskForPosition(Position position) -> InternalField;
auto maskedFieldMatchesPosition(const InternalField &field, const InternalField &mask,
                                const InternalField &position) -> bool;
auto printCoordinates(const Coordinates &coordinates) -> void;
auto printMove(const Move &m) -> void;
