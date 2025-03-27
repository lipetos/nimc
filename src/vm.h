#ifndef VM_H
#define VM_H

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    Chunk* chunk;
    uint8_t* ip;
    Value stack[STACK_MAX];
    Value* stack_top;
    struct {
        char* name;
        Value value;
    } variables[UINT8_MAX];
    int variable_count;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

void init_vm(VM* vm);
void free_vm(VM* vm);
InterpretResult interpret(VM* vm, Chunk* chunk);
void push(VM* vm, Value value);
Value pop(VM* vm);

#endif 