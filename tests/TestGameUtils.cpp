#include <array>
#include <iostream>

#include <catch2/catch_test_macros.hpp>

#include "FieldDefinitionHelper.hpp"
#include "Game.hpp"
#include "GameUtils.hpp"

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
    std::cout << internalField << '\n';
    REQUIRE(internalField.count() == 7);
    REQUIRE(internalField.test(2));
    REQUIRE(internalField.test(7));
    REQUIRE(internalField.test(19));
    REQUIRE(internalField.test(19));
    REQUIRE(internalField.test(FIELD_SIZE * FIELD_SIZE * BITS_PER_FIELD + 4));
    REQUIRE(internalField.test(FIELD_SIZE * FIELD_SIZE * BITS_PER_FIELD + 6));
}
