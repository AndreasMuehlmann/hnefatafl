#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "Game.hpp"
#include "helper.hpp"


TEST_CASE( "Multiple possible moves are tested", "[move]" ) {
    Game game;
    struct Vec2D from;
    struct Vec2D to;

    from = { 4, 1 }; 
    to = { 5, 1 };
    game.move({from, to});
    REQUIRE(game.getFigurAt(to) == Figur::Wiking);

    from = { 4, 5 };
    to = { 7, 5 };
    game.move({from, to});
    REQUIRE(game.getFigurAt(to) == Figur::Guard);

    from = { 8, 5 };
    to = { 8, 7 };
    game.move({from, to});
    REQUIRE(game.getFigurAt(to) == Figur::Wiking);
}


bool check_for_exception_in_move(Game& game, Move move, std::string expectedException) {
    try {
        game.move({move.from, move.to});
    } catch (std::invalid_argument& e) {
        if (expectedException == e.what()) {
            return true;
        } else {
            std::cout << "\"" <<e.what() << "\"" <<
                " not matching expected exception " << "\"" << expectedException << "\"" << std::endl;

            std::cout << "Move from " << move.from.x << ", " << move.from.y 
                << " to " << move.to.x << ", " << move.to.y << "." << std::endl;
            std::cout << "Field before move:" << std::endl;
            game.printField();
            return false;
        }
    }
    std::cout << "No Exception thrown." << std::endl;
    std::cout << "Field after move:" << std::endl;
    game.printField();
    return false;
}


void moveKingOutOfMiddle(Game& game) {
    game.moveUnchecked({{ 4, 5 }, { 5, 5 }});
    game.moveUnchecked({{ 4, 4 }, { 4, 5 }});
    game.moveUnchecked({{ 3, 0 }, { 1, 0 }});
}


TEST_CASE( "Test impossible moves", "[move]" ) {
    Game game;

    REQUIRE(check_for_exception_in_move(game, {{10, 5}, {7, 10}}, "Position to move away from out of field range."));
    REQUIRE(check_for_exception_in_move(game, {{0, 5}, {0, 8}}, "Cannot move into the corner unless the figur is the king."));
    REQUIRE(check_for_exception_in_move(game, {{0, 4}, {0, 6}}, "Cannot move because the path is blocked."));

    try {
        moveKingOutOfMiddle(game);
    } catch (std::invalid_argument& e) {
        std::cout << "Unexpected exception." << std::endl;
        game.printField();
        throw e;
    }

    REQUIRE(check_for_exception_in_move(game, {{3, 4}, {4, 4}}, "Cannot move into the center position unless the figur is the king."));
    REQUIRE(check_for_exception_in_move(game, {{4, 2}, {4, 0}}, "Cannot move because the path is blocked."));
}


TEST_CASE( "Test if figures are removed by a capture", "[captureXAxis, captureYAxis]" ) {
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::Guard, Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        Game game(field);
        game.move({{4, 1}, {4, 0}});
        game.updateField({4, 0});
        REQUIRE(game.getFigurAt({2, 0}) == Figur::None);
        REQUIRE(game.getFigurAt({3, 0}) == Figur::None);
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        Game game(field);
        game.move({{1, 8}, {2, 8}});
        game.move({{1, 1}, {0, 1}});
        game.updateField({0, 1});
        REQUIRE(game.getFigurAt({0, 2}) == Figur::None);
        REQUIRE(game.getFigurAt({0, 3}) == Figur::None);
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        Game game(field);
        game.move({{1, 4}, {0, 4}});
        game.updateField({0, 4});
        REQUIRE(game.getFigurAt({0, 2}) == Figur::King);
        REQUIRE(game.getFigurAt({0, 3}) == Figur::Guard);
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Guard, Figur::King, Figur::Wiking, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Guard},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        Game game(field);
        game.move({{0, 7}, {0, 6}});
        game.move({{8, 2}, {8, 1}});
        game.updateField({8, 1});
        REQUIRE(game.getFigurAt({7, 1}) == Figur::None);
        REQUIRE(game.getFigurAt({6, 1}) == Figur::King);
    }
}

TEST_CASE( "Test if a winning condition is met", "[isGameOver]" ) {
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        Game game(field);
        REQUIRE(game.whoWon() == Figur::King);
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::King, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        Game game(field);
        REQUIRE(game.whoWon() == Figur::Wiking);
    }
    {
        Field field = {
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::Wiking, Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            std::array<Figur, FIELD_SIZE> {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        transform(field);
        Game game(field);
        REQUIRE(game.whoWon() == Figur::None);
    }
}
