#include <array>
#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "FieldDefinitionHelper.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"
#include "Move.hpp"

TEST_CASE("Check if coordinates (2D) are converted to position (1D) correctly",
          "[coordinatesToPosition]") {
    REQUIRE(coordinatesToPosition({4, 1}) == 13);
    REQUIRE(coordinatesToPosition({0, 3}) == 27);
}

TEST_CASE("Check if positions (1D) are converted to coordinates (2D) correctly",
          "[positionToCoordinates]") {
    {
        Coordinates coordinates = positionToCoordinates(13);
        REQUIRE(coordinates.x == 4);
        REQUIRE(coordinates.y == 1);
    }
    Coordinates coordinates = positionToCoordinates(27);
    REQUIRE(coordinates.x == 0);
    REQUIRE(coordinates.y == 3);
}

TEST_CASE("converting a 2D array of Figurs to the internal representation",
          "[fieldToInternalField]") {
    Field field = {
        std::array<Figur, FIELD_SIZE>{_, w, _, g, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{g, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, k},
    };
    InternalField internalField = fieldToInternalField(field);
    REQUIRE(internalField.count() == 7);
    REQUIRE(internalField.test(2));
    REQUIRE(internalField.test(7));
    REQUIRE(internalField.test(19));
    REQUIRE(internalField.test(19));
    REQUIRE(internalField.test(FIELD_SIZE * FIELD_SIZE * BITS_PER_FIELD + 4));
    REQUIRE(internalField.test(FIELD_SIZE * FIELD_SIZE * BITS_PER_FIELD + 6));
}

TEST_CASE("possibleCapture", "[fieldToInternalField]") {
    Field field = {
        std::array<Figur, FIELD_SIZE>{_, _, w, g, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{w, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{k, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, w},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, g, _, _, _, g},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{_, _, _, _, _, _, _, _, _},
        std::array<Figur, FIELD_SIZE>{g, g, _, _, _, _, _, _, _},
    };
    InternalField internalField = fieldToInternalField(field);
    REQUIRE(possibleCapture(internalField, coordinatesToPosition({3, 0}), false));
    REQUIRE(!possibleCapture(internalField, coordinatesToPosition({4, 4}), false));
    REQUIRE(possibleCapture(internalField, coordinatesToPosition({8, 3}), true));
    REQUIRE(possibleCapture(internalField, coordinatesToPosition({0, 1}), true));
    REQUIRE(!possibleCapture(internalField, coordinatesToPosition({0, 8}), false));
}
