#include "helper.hpp"


void transform(Field& field) {
    Field oldField;
    for (int i = 0; i < FIELD_SIZE; ++i) {
        for (int j = 0; j < FIELD_SIZE; ++j) {
            oldField[i][j] = field[i][j];
        }
    }

    for (int i = 0; i < FIELD_SIZE; i++) {
        for (int j = 0; j < FIELD_SIZE; j++) {
            field[i][j] = oldField[j][i];
        }
    }
}
