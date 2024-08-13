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
    REQUIRE(game.figurAt(to) == Figur::Wiking);

    from = { 4, 5 };
    to = { 7, 5 };
    game.move(from, to);
    REQUIRE(game.figurAt(to) == Figur::Guard);

    from = { 8, 5 };
    to = { 8, 7 };
    game.move(from, to);
    REQUIRE(game.figurAt(to) == Figur::Wiking);
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

TEST_CASE( "Test impossible moves", "[move]" ) {
    Game game;

    REQUIRE(check_for_exception_in_move(game, {10, 5}, {7, 10}, "Position to move away from out of field range."));
    REQUIRE(check_for_exception_in_move(game, {0, 5}, {0, 8}, "Cannot move into the corner unless the figur is the king."));
    REQUIRE(check_for_exception_in_move(game, {0, 4}, {0, 6}, "Cannot move because the path is blocked."));

    try {
        game.move({ 4, 5 }, { 5, 5 });
        game.move({ 7, 4 }, { 7, 5 });
        game.move({ 4, 4 }, { 4, 5 });
        game.move({ 7, 5 }, { 7, 4 });
    } catch (std::invalid_argument& e) {

        std::cout << "Unexpected exception." << std::endl;
        game.printField();
        throw e;
    }

    REQUIRE(check_for_exception_in_move(game, {3, 4}, {4, 4}, "Cannot move into the center position unless the figur is the king."));
    REQUIRE(check_for_exception_in_move(game, {4, 4}, {4, 5}, "Cannot move because the path is blocked."));
}
