#ifndef VALUE_H
#define VALUE_H

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

typedef enum {
    VAL_NUMBER,
    VAL_BOOL,
    VAL_STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        double number;
        bool boolean;
        char* string;
    } as;
} Value;

typedef struct {
    int capacity;
    int count;
    Value* values;
} ValueArray;

void init_value_array(ValueArray* array);
void write_value_array(ValueArray* array, Value value);
void free_value_array(ValueArray* array);
void print_value(Value value);

#endif 