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

TEST_CASE("Check if defender wins when king on the edge", "[makeMove]") {
    {
        constexpr Field field = {
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, k, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        };
        Game game(field, false);
        Winner winner =
            game.makeMove({coordinatesToPosition({3, 5}), coordinatesToPosition({8, 5})});
        REQUIRE(winner == Winner::Defender);
    }
    {
        constexpr Field field = {
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, k, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        };
        Game game(field, false);
        Winner winner =
            game.makeMove({coordinatesToPosition({3, 5}), coordinatesToPosition({3, 0})});
        REQUIRE(winner == Winner::Defender);
    }
}

TEST_CASE("Check if attacker wins when king surrounded in castle", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, k, _, w, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, true);
    Winner winner = game.makeMove({coordinatesToPosition({6, 4}), coordinatesToPosition({5, 4})});
    REQUIRE(winner == Winner::Attacker);
}

TEST_CASE("Check if attacker wins when king surrounded next to castle", "[makeMove]") {
    {
        constexpr Field field = {
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, w, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, k, _, w, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, w, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        };
        Game game(field, true);
        Winner winner =
            game.makeMove({coordinatesToPosition({7, 4}), coordinatesToPosition({6, 4})});
        REQUIRE(winner == Winner::Attacker);
    }
    {
        constexpr Field field = {
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, w, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, w, _, k, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, w, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        };
        Game game(field, true);
        Winner winner =
            game.makeMove({coordinatesToPosition({1, 4}), coordinatesToPosition({2, 4})});
        REQUIRE(winner == Winner::Attacker);
    }
    {
        constexpr Field field = {
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, w, k, w, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        };
        Game game(field, true);
        Winner winner =
            game.makeMove({coordinatesToPosition({4, 1}), coordinatesToPosition({4, 2})});
        REQUIRE(winner == Winner::Attacker);
    }
    {
        constexpr Field field = {
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, w, k, w, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
            std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        };
        Game game(field, true);
        Winner winner =
            game.makeMove({coordinatesToPosition({4, 7}), coordinatesToPosition({4, 6})});
        REQUIRE(winner == Winner::Attacker);
    }
}

TEST_CASE("Check for draw after three move repetions", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, k, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, w, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, true);

    Position fromKing = coordinatesToPosition({6, 4});
    Position toKing = coordinatesToPosition({7, 4});
    Position fromWiking = coordinatesToPosition({6, 4});
    Position toWiking = coordinatesToPosition({7, 4});
    Move moveKing = {fromKing, toKing};
    Move reversedMoveKing = {toKing, fromKing};
    Move moveWiking = {fromWiking, toWiking};
    Move reversedMoveWiking = {toWiking, fromWiking};

    REQUIRE(game.makeMove(moveWiking) == Winner::NoWinner);
    REQUIRE(game.makeMove(moveKing) == Winner::NoWinner);
    REQUIRE(game.makeMove(reversedMoveWiking) == Winner::NoWinner);
    REQUIRE(game.makeMove(reversedMoveKing) == Winner::NoWinner);

    REQUIRE(game.makeMove(moveWiking) == Winner::NoWinner);
    REQUIRE(game.makeMove(moveKing) == Winner::NoWinner);
    REQUIRE(game.makeMove(reversedMoveWiking) == Winner::Draw);
}

TEST_CASE("Check if figurs are captured correctly", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, w, g, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, g, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, k, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, false);

    REQUIRE(game.makeMove({coordinatesToPosition({2, 3}), coordinatesToPosition({2, 2})}) == Winner::NoWinner);
    REQUIRE(game.getFigurAt(coordinatesToPosition({3, 2})) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(coordinatesToPosition({4, 2})) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(coordinatesToPosition({5, 2})) == Figur::Guard);
}

TEST_CASE("Check if king is captured", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, g, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, g, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, k, g, w, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, w, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, true);

    REQUIRE(game.makeMove({coordinatesToPosition({2, 3}), coordinatesToPosition({2, 2})}) == Winner::Attacker);
    REQUIRE(game.getFigurAt(coordinatesToPosition({3, 2})) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(coordinatesToPosition({4, 2})) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(coordinatesToPosition({5, 2})) == Figur::Wiking);
    REQUIRE(game.getFigurAt(coordinatesToPosition({2, 1})) == Figur::Guard);
    REQUIRE(game.getFigurAt(coordinatesToPosition({2, 0})) == Figur::Guard);
}

TEST_CASE("Check if king can take part in a capture", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, k, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, g, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, false);

    REQUIRE(game.makeMove({coordinatesToPosition({2, 1}), coordinatesToPosition({3, 1})}) == Winner::NoWinner);
    REQUIRE(game.getFigurAt(coordinatesToPosition({3, 2})) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(coordinatesToPosition({3, 3})) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(coordinatesToPosition({3, 4})) == Figur::NoFigur);
    REQUIRE(game.getFigurAt(coordinatesToPosition({3, 5})) == Figur::Guard);
}

TEST_CASE("Check if king is not captured in the castle", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, w, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, k, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, true);

    REQUIRE(game.makeMove({coordinatesToPosition({5, 3}), coordinatesToPosition({5, 4})}) == Winner::NoWinner);
    REQUIRE(game.getFigurAt(coordinatesToPosition({4, 4})) == Figur::King);
}

TEST_CASE("Check if king is not captured next to the castle", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, w, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, k, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, w, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, true);

    REQUIRE(game.makeMove({coordinatesToPosition({4, 5}), coordinatesToPosition({5, 5})}) == Winner::NoWinner);
    REQUIRE(game.getFigurAt(coordinatesToPosition({5, 4})) == Figur::King);
}

TEST_CASE("Check if figurs are captured next to castle", "[makeMove]") {
    constexpr Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, g, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, w, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, g, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, w, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, k, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
    };
    Game game(field, true);

    REQUIRE(game.makeMove({coordinatesToPosition({3, 6}), coordinatesToPosition({4, 6})}) == Winner::NoWinner);
    REQUIRE(game.getFigurAt(coordinatesToPosition({4, 5})) == Figur::NoFigur);

    REQUIRE(game.makeMove({coordinatesToPosition({6, 3}), coordinatesToPosition({6, 4})}) == Winner::NoWinner);
    REQUIRE(game.getFigurAt(coordinatesToPosition({5, 4})) == Figur::NoFigur);
}
