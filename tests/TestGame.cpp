#include <array>
#include <catch2/catch_test_macros.hpp>
#include <iostream>

#include "Game.hpp"
#include "TestingUtils.hpp"

constexpr Figur w = Figur::Wiking;
constexpr Figur g = Figur::Guard;
constexpr Figur k = Figur::King;
constexpr Figur _ = Figur::None;

using Row = std::array<Figur, FIELD_SIZE>;

TEST_CASE("Multiple possible moves are tested", "[move]") {
    Game game;
    Vec2D from{};
    Vec2D to{};

    from = {4, 1};
    to = {5, 1};
    game.move({{4, 1}, {5, 1}});
    REQUIRE(game.getFigurAt(to) == Figur::Wiking);

    from = {4, 5};
    to = {7, 5};
    game.move({from, to});
    REQUIRE(game.getFigurAt(to) == Figur::Guard);

    from = {8, 5};
    to = {8, 7};
    game.move({from, to});
    REQUIRE(game.getFigurAt(to) == Figur::Wiking);
}

auto check_for_exception_in_move(Game &game, Move move,
                                 const std::string &expectedException) -> bool {
    try {
        game.move({move.from, move.to});
    } catch (std::invalid_argument &e) {
        if (expectedException == e.what()) {
            return true;
        }
        std::cout << "\"" << e.what() << "\"" << " not matching expected exception " << "\""
                  << expectedException << "\"" << std::endl;

        std::cout << "Move from " << move.from.x << ", " << move.from.y << " to " << move.to.x
                  << ", " << move.to.y << "." << std::endl;
        std::cout << "Field before move:" << std::endl;
        game.printField();
        return false;
    }
    std::cout << "No Exception thrown." << std::endl;
    std::cout << "Field after move:" << std::endl;
    game.printField();
    return false;
}

void moveKingOutOfMiddle(Game &game) {
    {
        constexpr Vec2D from = {4, 5};
        constexpr Vec2D to = {5, 5};
        game.moveUnchecked({from, to});
    }
    {
        constexpr Vec2D from = {4, 4};
        constexpr Vec2D to = {4, 5};
        game.moveUnchecked({from, to});
    }
    {
        constexpr Vec2D from = {3, 0};
        constexpr Vec2D to = {1, 0};
        game.moveUnchecked({from, to});
    }
}

TEST_CASE("Test impossible moves", "[move]") {
    Game game;

    REQUIRE(check_for_exception_in_move(game, {{10, 5}, {7, 10}},
                                        "Position to move away from out of field range."));
    REQUIRE(check_for_exception_in_move(
        game, {{0, 5}, {0, 8}}, "Cannot move into the corner unless the figur is the king."));
    REQUIRE(check_for_exception_in_move(game, {{0, 4}, {0, 6}},
                                        "Cannot move because the path is blocked."));

    try {
        moveKingOutOfMiddle(game);
    } catch (std::invalid_argument &e) {
        std::cout << "Unexpected exception." << std::endl;
        game.printField();
        throw e;
    }

    REQUIRE(check_for_exception_in_move(
        game, {{3, 4}, {4, 4}},
        "Cannot move into the center position unless the figur is the king."));
    REQUIRE(check_for_exception_in_move(game, {{4, 2}, {4, 0}},
                                        "Cannot move because the path is blocked."));
}

TEST_CASE("Test if figures are removed by a capture from wiking", "[updateField]") {
    Field field = {
        Row{_, w, g, g, _, _, _, _, _}, Row{_, _, _, _, w, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, true);
    game.move({{4, 1}, {4, 0}});
    game.updateField({4, 0});
    REQUIRE(game.getFigurAt({2, 0}) == Figur::None);
    REQUIRE(game.getFigurAt({3, 0}) == Figur::None);
}

TEST_CASE("Test if figures are removed by a capture from king", "[updateField]") {
    Field field = {
        Row{_, _, _, _, _, _, _, _, _}, Row{_, k, _, _, _, _, _, _, _},
        Row{w, _, _, _, _, _, _, _, _}, Row{w, _, _, _, _, _, _, _, _},
        Row{g, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, false);
    game.move({{1, 1}, {0, 1}});
    game.updateField({0, 1});
    REQUIRE(game.getFigurAt({0, 2}) == Figur::None);
    REQUIRE(game.getFigurAt({0, 3}) == Figur::None);
}

TEST_CASE("Test if figures are not removed when king in the middle", "[updateField]") {
    Field field = {
        Row{_, _, _, _, _, _, _, _, _}, Row{w, _, _, _, _, _, _, _, _},
        Row{k, _, _, _, _, _, _, _, _}, Row{g, _, _, _, _, _, _, _, _},
        Row{_, w, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, w, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, true);
    game.move({{1, 4}, {0, 4}});
    game.updateField({0, 4});
    REQUIRE(game.getFigurAt({0, 2}) == Figur::King);
    REQUIRE(game.getFigurAt({0, 3}) == Figur::Guard);
}

TEST_CASE("Test if figures are removed by a capture from guard and king not captured",
          "[updateField]") {
    Field field = {
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, g, k, w, _},
        Row{_, _, _, _, _, _, _, _, g}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, false);
    game.move({{8, 2}, {8, 1}});
    game.updateField({8, 1});
    REQUIRE(game.getFigurAt({7, 1}) == Figur::None);
    REQUIRE(game.getFigurAt({6, 1}) == Figur::King);
}

TEST_CASE("Test if a winning condition is met when king is in the corner", "[whoWon]") {
    Field field = {
        Row{k, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, false);
    REQUIRE(game.whoWon() == Figur::King);
}

TEST_CASE("Test if a winning condition is met when is surrounded by wikings and border",
          "[whoWon]") {
    Field field = {
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{w, _, _, _, _, _, _, _, _}, Row{k, w, _, _, _, _, _, _, _},
        Row{w, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, false);
    REQUIRE(game.whoWon() == Figur::Wiking);
}

TEST_CASE("Test that winning condition is not met when king is nearly surrounded", "[whoWon]") {
    Field field = {
        Row{_, w, k, _, _, _, _, _, _}, Row{_, _, w, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _}, Row{_, _, _, _, _, _, _, _, _},
        Row{_, _, _, _, _, _, _, _, _},
    };
    Game game = createGameFromField(field, false);
    REQUIRE(game.whoWon() == Figur::None);
}
