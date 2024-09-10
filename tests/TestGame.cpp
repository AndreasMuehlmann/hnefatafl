#include <catch2/catch_test_macros.hpp>

#include "FieldDefinitionHelper.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"

TEST_CASE("Check if correct figures are returned from getFigurAt", "[getFigurAt]") {
    Game game;
    REQUIRE(game.getFigurAt(0) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(FIELD_SIZE / 2) == Figur::Wiking);
    REQUIRE(game.getFigurAt(FIELD_SIZE / 2 + FIELD_SIZE * 4) == Figur::King);
    REQUIRE(game.getFigurAt(FIELD_SIZE / 2 + FIELD_SIZE * 2) == Figur::Guard);
}

TEST_CASE("Check if figurs are moved correctly", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, w, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{w, w, g, g, k, g, g, w, w},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, g, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, g, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, w, w, _, _, _},
    };
    Game game(field, true);
    game.makeMove({5, 6});
    REQUIRE(game.getFigurAt(6) == Figur::Wiking);
    REQUIRE(game.getFigurAt(5) == Figur::NoFigur);

    game.makeMove({FIELD_SIZE * 3, FIELD_SIZE * 2});
    REQUIRE(game.getFigurAt(18) == Figur::Wiking);
    REQUIRE(game.getFigurAt(27) == Figur::NoFigur);

    const Position from = coordinatesToPosition({4, 4});
    const Position to = coordinatesToPosition({4, 5});
    REQUIRE(game.getKingPosition() == from);
    game.makeMove({from, to});
    REQUIRE(game.getKingPosition() == to);
}
