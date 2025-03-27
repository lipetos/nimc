#ifndef BYTECODE_H
#define BYTECODE_H

typedef enum {
    OP_CONSTANT,    // Push constant onto stack
    OP_ADD,         // Add top two values on stack
    OP_SUBTRACT,    // Subtract top two values on stack
    OP_MULTIPLY,    // Multiply top two values on stack
    OP_DIVIDE,      // Divide top two values on stack
    OP_NEGATE,      // Negate top value on stack
    OP_DEFINE_VAR,  // Define a variable
    OP_GET_VAR,     // Get variable value
    OP_SET_VAR,     // Set variable value
    OP_PRINT,       // Print top value from stack
    OP_RETURN,      // Return from current function
    OP_POP,         // Pop value from stack
} OpCode;

#endif 