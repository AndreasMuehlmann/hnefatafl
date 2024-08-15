#include <iostream>
#include <catch2/catch_test_macros.hpp>

#include "Game.hpp"


TEST_CASE( "Multiple possible moves are tested", "[move]" ) {
    Game game;
    struct Position from;
    struct Position to;

    from = { 4, 1 }; 
    to = { 5, 1 };
    game.move(from, to);
    game.moveDone();
    REQUIRE(game.getFigurAt(to) == Figur::Wiking);

    from = { 4, 5 };
    to = { 7, 5 };
    game.move(from, to);
    game.moveDone();
    REQUIRE(game.getFigurAt(to) == Figur::Guard);

    from = { 8, 5 };
    to = { 8, 7 };
    game.move(from, to);
    game.moveDone();
    REQUIRE(game.getFigurAt(to) == Figur::Wiking);
}


bool check_for_exception_in_move(Game &game, Position from, Position to, std::string expectedException) {
    try {
        game.move(from, to);
    } catch (std::invalid_argument& e) {
        if (expectedException == e.what()) {
            return true;
        } else {
            std::cout << "\"" <<e.what() << "\"" <<
                " not matching expected exception " << "\"" << expectedException << "\"" << std::endl;

            std::cout << "Move from " << from.x << ", " << from.y 
                << " to " << to.x << ", " << to.y << "." << std::endl;
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


void move_king_out_of_the_middle(Game &game) {
    game.moveDone();
    game.move({ 4, 5 }, { 5, 5 });
    game.move({ 4, 4 }, { 4, 5 });
}


TEST_CASE( "Test impossible moves", "[move]" ) {
    Game game;

    REQUIRE(check_for_exception_in_move(game, {10, 5}, {7, 10}, "Position to move away from out of field range."));
    REQUIRE(check_for_exception_in_move(game, {0, 5}, {0, 8}, "Cannot move into the corner unless the figur is the king."));
    REQUIRE(check_for_exception_in_move(game, {0, 4}, {0, 6}, "Cannot move because the path is blocked."));

    try {
        move_king_out_of_the_middle(game);
    } catch (std::invalid_argument& e) {
        std::cout << "Unexpected exception." << std::endl;
        game.printField();
        throw e;
    }

    REQUIRE(check_for_exception_in_move(game, {3, 4}, {4, 4}, "Cannot move into the center position unless the figur is the king."));
    REQUIRE(check_for_exception_in_move(game, {4, 2}, {4, 0}, "Cannot move because the path is blocked."));
}


TEST_CASE( "Test if figures are removed by a capture", "[captureXAxis, captureYAxis]" ) {
    {
        Figur field[FIELD_SIZE][FIELD_SIZE] = {
            {Figur::None, Figur::Wiking, Figur::Guard, Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        Game game(field);
        game.move({4, 1}, {4, 0});
        game.updateField({4, 0});
        REQUIRE(game.getFigurAt({2, 0}) == Figur::None);
        REQUIRE(game.getFigurAt({3, 0}) == Figur::None);
    }
    {
        Figur field[FIELD_SIZE][FIELD_SIZE] = {
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        Game game(field);
        game.moveDone();
        game.move({1, 1}, {0, 1});
        game.updateField({0, 1});
        REQUIRE(game.getFigurAt({0, 2}) == Figur::None);
        REQUIRE(game.getFigurAt({0, 3}) == Figur::None);
    }
    {
        Figur field[FIELD_SIZE][FIELD_SIZE] = {
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::Guard, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        Game game(field);
        game.move({1, 4}, {0, 4});
        game.updateField({0, 4});
        REQUIRE(game.getFigurAt({0, 2}) == Figur::King);
        REQUIRE(game.getFigurAt({0, 3}) == Figur::Guard);
    }
}

TEST_CASE( "Test if a winning condition is met", "[isGameOver]" ) {
    {
        Figur field[FIELD_SIZE][FIELD_SIZE] = {
            {Figur::King, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
        };
        Game game(field);
        REQUIRE(game.isGameOver({0, 0}));
    }
    {
        Figur field[FIELD_SIZE][FIELD_SIZE] = {
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::King, Figur::Wiking, Figur::None, Figur::None},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None},
            {Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::King, Figur::Wiking, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            {Figur::Wiking, Figur::None, Figur::Wiking, Figur::King, Figur::Wiking, Figur::None, Figur::None, Figur::Wiking, Figur::King},
            {Figur::None, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None, Figur::Wiking},
            {Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::Wiking, Figur::None, Figur::None, Figur::None, Figur::None},
            {Figur::None, Figur::King, Figur::Wiking, Figur::Wiking, Figur::King, Figur::Wiking, Figur::None, Figur::None, Figur::None},
        };
        Game game(field);
        REQUIRE(game.isGameOver({0, 3}));
        REQUIRE(game.isGameOver({4, 8}));
        REQUIRE(game.isGameOver({3, 4}));
        REQUIRE(game.isGameOver({5, 0}));
        REQUIRE(game.isGameOver({8, 4}));
        REQUIRE(!game.isGameOver({1, 7}));
    }
}
