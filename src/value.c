#include <stdio.h>
#include <stdlib.h>
#include "value.h"

void print_value(Value value) {
    switch (value.type) {
        case VAL_NUMBER:
            printf("%g", value.as.number);
            break;
        case VAL_BOOL:
            printf(value.as.boolean ? "true" : "false");
            break;
        case VAL_STRING:
            printf("%s", value.as.string);
            break;
    }
}

void init_value_array(ValueArray* array) {
    array->values = NULL;
    array->capacity = 0;
    array->count = 0;
}

void write_value_array(ValueArray* array, Value value) {
    if (array->capacity < array->count + 1) {
        int old_capacity = array->capacity;
        array->capacity = old_capacity == 0 ? 8 : old_capacity * 2;
        array->values = realloc(array->values, sizeof(Value) * array->capacity);
    }

    array->values[array->count] = value;
    array->count++;
}

void free_value_array(ValueArray* array) {
    free(array->values);
    init_value_array(array);
} 