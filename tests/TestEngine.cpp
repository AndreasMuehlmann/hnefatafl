#include <catch2/catch_test_macros.hpp>

#include "Engine.hpp"
#include "Game.hpp"
#include "TestingUtils.hpp"

constexpr Figur w = Figur::Wiking;
constexpr Figur g = Figur::Guard;
constexpr Figur k = Figur::King;
constexpr Figur _ = Figur::None;


TEST_CASE("Test if king captures two wikings", "[minimax]") {
    Field field = {
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
    Game game = createGameFromField(field, false);
    Engine engine(game, 4);
    Move move = engine.getMove();
    REQUIRE(move.from.y == 6);
    REQUIRE(move.from.x == 1);
    REQUIRE(move.to.x == 0);
    REQUIRE(move.to.y == 6);
}

TEST_CASE("Test if king goes into the corner with one move", "[getMove]") {
    Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{k, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, false);
    Engine engine(game, 4);
    Move move = engine.getMove();
    REQUIRE(move.from.x == 0);
    REQUIRE(move.from.y == 2);
    REQUIRE(move.to.x == 0);
    REQUIRE(move.to.y == 0);
}

TEST_CASE("Test if guard goes to the side to let the king into the corner", "[getMove]") {
    Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, w, _, _, _, k, g, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, g, _, w, _, _},
        std::array<Figur, FIELD_SIZE>{_, w, w, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, g, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, w, _, _, w, _, _, _},
    };
    Game game = createGameFromField(field, false);
    Engine engine(game, 4);
    Move move = engine.getMove();
    REQUIRE(move.from.x == 7);
    REQUIRE(move.from.y == 0);
    REQUIRE(move.to.x == 7);
}

TEST_CASE("Test if guard goes to the side to let the king into the corner (other situation)",
          "[getMove]") {
    Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, w, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, w, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, w, _, w},
        std::array<Figur, FIELD_SIZE>{k, w, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{g, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, w, w, _, w, _},
    };
    Game game = createGameFromField(field, false);
    Engine engine(game, 4);
    Move move = engine.getMove();
    REQUIRE(move.from.x == 0);
    REQUIRE(move.from.y == 7);
    REQUIRE(move.to.x == 1);
    REQUIRE(move.to.y == 7);
}

TEST_CASE("Test if wiking blocks path for king to win", "[getMove]") {
    Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, k, w, _, _, _, w, _},
        std::array<Figur, FIELD_SIZE>{w, w, g, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, w, _, _, _, _, _, w, _},
    };
    Game game = createGameFromField(field, true);
    Engine engine(game, 4);
    Move move = engine.getMove();
    REQUIRE(move.from.x == 1);
    REQUIRE(move.from.y == 1);
    REQUIRE(move.to.x == 1);
    REQUIRE(move.to.y == 0);
}
