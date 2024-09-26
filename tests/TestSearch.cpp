#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <limits>

#include "FieldDefinitionHelper.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"
#include "Negamax.hpp"

constexpr unsigned int MAX_THINKING_TIME = std::numeric_limits<unsigned int>::max();

TEST_CASE("Test if king captures two wikings", "[getMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, w, w, w, w, w, w, w, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, _, g, _, _, w, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, _, w, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, _, w, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, k, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, w, w, w, w, w, w, w, _},
    };
    Game game(field, false);
    Negamax negamax(MAX_THINKING_TIME, 2);
    Move move = negamax.getMove(game);
    Coordinates from = positionToCoordinates(move.from);
    Coordinates to = positionToCoordinates(move.to);
    REQUIRE(from.x == 1);
    REQUIRE(from.y == 6);
    REQUIRE(to.x == 2);
    REQUIRE(to.y == 6);
}

TEST_CASE("Test if king makes winning move", "[getMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, g, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, g, k, w, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, w, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, g, _, g, _, _},
        std::array<Figur, FIELD_SIZE>{_, w, _, _, _, _, _, w, _},
    };
    Game game(field, false);
    Negamax negamax(MAX_THINKING_TIME, 1);
    Move move = negamax.getMove(game);
    Coordinates from = positionToCoordinates(move.from);
    Coordinates to = positionToCoordinates(move.to);
    REQUIRE(from.x == 2);
    REQUIRE(from.y == 3);
    REQUIRE(to.x == 2);
    REQUIRE(to.y == 0);
}

TEST_CASE("Test if wiking makes winning move", "[getMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, g, g, g, g, g, g, g, _},
        std::array<Figur, FIELD_SIZE>{g, _, _, _, _, _, _, _, g},
        std::array<Figur, FIELD_SIZE>{g, _, _, _, w, _, w, _, g},
        std::array<Figur, FIELD_SIZE>{g, _, k, w, _, w, _, _, g},
        std::array<Figur, FIELD_SIZE>{g, w, _, _, _, _, w, _, g},
        std::array<Figur, FIELD_SIZE>{g, _, g, g, g, w, _, _, g},
        std::array<Figur, FIELD_SIZE>{g, _, _, _, _, _, _, _, g},
        std::array<Figur, FIELD_SIZE>{g, _, _, _, _, _, _, _, g},
        std::array<Figur, FIELD_SIZE>{_, g, g, g, g, g, g, g, _},
    };
    Game game(field, true);
    Negamax negamax(MAX_THINKING_TIME, 3);
    Move move = negamax.getMove(game);
    Coordinates from = positionToCoordinates(move.from);
    Coordinates to = positionToCoordinates(move.to);
    REQUIRE(from.x == 1);
    REQUIRE(from.y == 4);
    REQUIRE(to.x == 1);
    REQUIRE(to.y == 3);
}
