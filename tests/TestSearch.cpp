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
        std::array<Figur, FIELD_SIZE>{_, w, _, _, _, _, _, w, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{g, _, _, _, _, w, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, k, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, w, _, _, _, _, _, w, _},
    };
    Game game(field, false);
    Negamax negamax(MAX_THINKING_TIME, 2);
    Move move = negamax.getMove(game);
    Coordinates from = positionToCoordinates(move.from);
    Coordinates to = positionToCoordinates(move.to);
    REQUIRE(from.x == 1);
    REQUIRE(from.y == 6);
    REQUIRE(to.x == 0);
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
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, false);
    Negamax negamax(MAX_THINKING_TIME, 4);
    Move move = negamax.getMove(game);
    std::cout << '\n';
    game.printField();
    Coordinates from = positionToCoordinates(move.from);
    Coordinates to = positionToCoordinates(move.to);
    REQUIRE(from.x == 2);
    REQUIRE(from.y == 3);
    REQUIRE(to.x == 2);
    REQUIRE(to.y == 0);
}
