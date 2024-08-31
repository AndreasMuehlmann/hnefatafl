#include <catch2/catch_test_macros.hpp>
#include <vector>

#include "Engine.hpp"
#include "Game.hpp"
#include "TestingUtils.hpp"

constexpr Figur w = Figur::Wiking;
constexpr Figur g = Figur::Guard;
constexpr Figur k = Figur::King;
constexpr Figur _ = Figur::None;

using Row = std::array<Figur, FIELD_SIZE>;

TEST_CASE("Test if all wikings to move are collected", "[getAllFigursToMove]") {
    Field field = {
        Row{_, w, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{w, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, true);

    std::vector<Vec2D> figursToMove = getFigursToMove(game, true);
    REQUIRE(figursToMove[0].x == 0);
    REQUIRE(figursToMove[0].y == 5);
    REQUIRE(figursToMove[1].x == 1);
    REQUIRE(figursToMove[1].y == 0);
}

TEST_CASE("Test if all guards to move are collected", "[getAllFigursToMove]") {
    Field field = {
        Row{_, g, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{g, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, false);

    std::vector<Vec2D> figursToMove = getFigursToMove(game, false);
    REQUIRE(figursToMove[0].x == 0);
    REQUIRE(figursToMove[0].y == 5);
    REQUIRE(figursToMove[1].x == 1);
    REQUIRE(figursToMove[1].y == 0);
}

TEST_CASE("Test getting available moves for a single figur in one direction",
          "[insertAvailableMovesFigurInDirection]") {
    Field field = {
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{w, _, g, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, true);

    constexpr Vec2D wikingPosition = {0, 5};

    std::vector<Move> availableMoves;
    insertAvailableMovesFigurInDirection(availableMoves, game, wikingPosition, {1, 0});
    REQUIRE(availableMoves.size() == 1);
    REQUIRE(availableMoves[0].from.x == 0);
    REQUIRE(availableMoves[0].from.y == 5);
    REQUIRE(availableMoves[0].to.x == 1);
    REQUIRE(availableMoves[0].to.y == 5);

    insertAvailableMovesFigurInDirection(availableMoves, game, wikingPosition, {-1, 0});
    REQUIRE(availableMoves.size() == 1);
    REQUIRE(availableMoves[0].to.x == 1);

    insertAvailableMovesFigurInDirection(availableMoves, game, wikingPosition, {0, 1});
    REQUIRE(availableMoves.size() == 3);
    REQUIRE(availableMoves[2].to.y == 7);

    insertAvailableMovesFigurInDirection(availableMoves, game, wikingPosition, {0, -1});
    REQUIRE(availableMoves.size() == 7);
    REQUIRE(availableMoves[6].to.y == 1);
}

TEST_CASE("Test getting all available moves", "[getAvailableMoves]") {
    Field field = {
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{g, _, _, _, _, _, _, _, _},
        Row{w, k, _, _, _, _, _, _, _}, Row{w, _, g, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, true);
    std::vector<Vec2D> figursToMove = getFigursToMove(game, true);
    std::vector<Move> availableMoves = getAvailableMoves(game, figursToMove);
    REQUIRE(availableMoves[0].from.x == 0);
    REQUIRE(availableMoves[0].from.y == 5);
    REQUIRE(availableMoves[0].to.x == 1);
    REQUIRE(availableMoves[0].to.y == 5);
    REQUIRE(availableMoves.size() == 3);
    REQUIRE(availableMoves[2].to.y == 7);
}

TEST_CASE("Test if king captures two wikings", "[minimax]") {
    Field field = {
        Row{_, w, _, _, _, _, _, w, _}, Row{w, _, _, _, _, _, _, _, w},
        Row{_, _, _, _, _, _, _, _, _}, Row{g, _, _, _, _, w, _, _, _},
        Row{w, _, _, _, _, _, _, _, _}, Row{w, _, _, _, _, _, _, _, _},
        Row{_, k, _, _, _, _, _, _, _}, Row{w, _, _, _, _, _, _, _, w},
        Row{_, w, _, _, _, _, _, w, _},
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
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{k, _, _, _, _, _, _, _, _}, Row{w, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
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
        Row{_, _, w, _, _, _, k, g, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, w, g, _, w, _, _}, Row{_, w, w, _, _, _, _, _, w},
        Row{_, _, _, _, _, _, _, _, w}, Row{_, _, _, _, _, _, _, _, w},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, w, g, _, _, w},
        Row{_, _, w, _, _, w, _, _, _},
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
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, w, _, _, _, _, w},
        Row{_, _, _, w, _, _, _, _, _}, Row{_, _, w, _, _, _, _, _, _},
        Row{w, _, _, _, _, _, _, w, _}, Row{_, _, _, _, _, _, w, _, w},
        Row{k, w, _, _, _, _, _, _, _}, Row{g, _, _, _, _, _, _, _, _},
        Row{_, _, _, w, w, w, _, w, _},
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
        Row{_, _, k, w, _, _, _, w, _}, Row{w, w, g, _, _, _, _, _, w},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{w, _, _, _, _, _, _, _, w},
        Row{_, w, _, _, _, _, _, w, _},
    };
    Game game = createGameFromField(field, true);
    Engine engine(game, 4);
    Move move = engine.getMove();
    REQUIRE(move.from.x == 1);
    REQUIRE(move.from.y == 1);
    REQUIRE(move.to.x == 1);
    REQUIRE(move.to.y == 0);
}
