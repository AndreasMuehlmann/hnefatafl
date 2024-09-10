#include <catch2/catch_test_macros.hpp>

#include "FieldDefinitionHelper.hpp"
#include "Game.hpp"

TEST_CASE("Check if correct figures are returned from getFigurAt", "[getFigurAt]") {
    Game game;
    REQUIRE(game.getFigurAt(0) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(FIELD_SIZE / 2) == Figur::Wiking);
    REQUIRE(game.getFigurAt(FIELD_SIZE / 2 + FIELD_SIZE * 4) == Figur::King);
    REQUIRE(game.getFigurAt(FIELD_SIZE / 2 + FIELD_SIZE * 2) == Figur::Guard);
}

TEST_CASE("Check if figurs are moved correctly", "[makeMove]") {
    Game game;
    game.makeMove({5, 6});
    REQUIRE(game.getFigurAt(6) == Figur::Wiking);
    REQUIRE(game.getFigurAt(5) == Figur::NoFigur);

    game.makeMove({FIELD_SIZE * 3, FIELD_SIZE * 2});
    REQUIRE(game.getFigurAt(18) == Figur::Wiking);
    REQUIRE(game.getFigurAt(27) == Figur::NoFigur);
}
