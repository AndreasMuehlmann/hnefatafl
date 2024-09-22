#include <catch2/catch_test_macros.hpp>

#include "FieldDefinitionHelper.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"
#include "Negamax.hpp"

constexpr unsigned int MAX_THINKING_TIME = 5000;

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
    Negamax negamax(MAX_THINKING_TIME, 1);
    Move move = negamax.getMove(game);
    Coordinates from = positionToCoordinates(move.from);
    Coordinates to = positionToCoordinates(move.to);
    REQUIRE(from.y == 6);
    REQUIRE(from.x == 1);
    REQUIRE(to.x == 0);
    REQUIRE(to.y == 6);
}
