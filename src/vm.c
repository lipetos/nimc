#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vm.h"
#include "compiler.h"
#include "debug.h"
#include "bytecode.h"

void init_vm(VM* vm) {
    vm->stack_top = vm->stack;
    vm->variable_count = 0;
}

void free_vm(VM* vm) {
}

static InterpretResult run(VM* vm) {
    #ifdef DEBUG_TRACE_EXECUTION
        printf("== VM Execution ==\n");
        disassemble_chunk(vm->chunk, "code");
    #endif
    
    #define READ_BYTE() (*vm->ip++)
    #define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])
    
    for (;;) {
        #ifdef DEBUG_TRACE_EXECUTION
            printf("          ");
            for (Value* slot = vm->stack; slot < vm->stack_top; slot++) {
                printf("[ ");
                print_value(*slot);
                printf(" ]");
            }
            printf("\n");
            disassemble_instruction(vm->chunk, (int)(vm->ip - vm->chunk->code));
        #endif
        
        uint8_t instruction = READ_BYTE();
        
        switch (instruction) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(vm, constant);
                break;
            }
            case OP_ADD: {
                Value b = pop(vm);
                Value a = pop(vm);

                if (a.type == VAL_NUMBER && b.type == VAL_NUMBER) {
                    push(vm, (Value){.type = VAL_NUMBER, .as.number = a.as.number + b.as.number});
                } else {
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            
            case OP_PRINT: {
                Value value = pop(vm);
                print_value(value);
                printf("\n");
                break;
            }
            
            case OP_DEFINE_VAR: {
                uint8_t index = READ_BYTE();
                char* name = (char*)READ_CONSTANT().as.string;
                Value value = pop(vm);
                
                printf("Defining variable: %.*s\n", (int)strlen(name), name);
                
                vm->variables[vm->variable_count].name = strdup(name);
                vm->variables[vm->variable_count].value = value;
                vm->variable_count++;
                
                printf("Setting var %s = ", name);
                print_value(value);
                printf("\n");
                
                break;
            }
            
            case OP_RETURN:
                return INTERPRET_OK;
            
            case OP_GET_VAR: {
                uint8_t index = READ_BYTE();
                Value nameValue = vm->chunk->constants.values[index];
                char* name = nameValue.as.string;
                
                printf("Getting variable: %.*s\n", (int)strlen(name), name);
                
                // Find variable
                bool found = false;
                Value value;
                for (int i = 0; i < vm->variable_count; i++) {
                    if (strcmp(vm->variables[i].name, name) == 0) {
                        value = vm->variables[i].value;
                        found = true;
                        break;
                    }
                }
                
                if (!found) {
                    printf("Error: Undefined variable '%s'\n", name);
                    return INTERPRET_RUNTIME_ERROR;
                }
                
                push(vm, value);
                break;
            }
            
            case OP_POP: {
                pop(vm);
                break;
            }
        }
    }
    
    #undef READ_BYTE
    #undef READ_CONSTANT
}

void push(VM* vm, Value value) {
    *vm->stack_top = value;
    vm->stack_top++;
}

Value pop(VM* vm) {
    vm->stack_top--;
    return *vm->stack_top;
}

InterpretResult interpret(VM* vm, Chunk* chunk) {
    vm->chunk = chunk;
    vm->ip = chunk->code;
    return run(vm);
} 