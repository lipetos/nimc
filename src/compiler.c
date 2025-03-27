#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"
#include "scanner.h"
#include "chunk.h"
#include "bytecode.h"

typedef struct {
    Token current;
    Token previous;
    Scanner scanner;
    Chunk* chunk;
    bool hadError;
} Parser;

static Parser parser;

// Forward declarations
static void expression();
static void statement();
static void declaration();

// Error handling
static void errorAt(Token* token, const char* message) {
    fprintf(stderr, "[line %d] Error", token->line);
    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

static void error(const char* message) {
    errorAt(&parser.previous, message);
}

static void errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

static void advance() {
    parser.previous = parser.current;
    
    for (;;) {
        parser.current = scan_token(&parser.scanner);
        if (parser.current.type != TOKEN_ERROR) break;
        
        errorAtCurrent(parser.current.start);
    }
}

static void consume(TOKEN_TYPE type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }
    
    errorAtCurrent(message);
}

// Bytecode emission
static void emitByte(uint8_t byte) {
    write_chunk(parser.chunk, byte, parser.previous.line);
}

static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

static int makeConstant(Value value) {
    int constant = add_constant(parser.chunk, value);
    return constant;
}

static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}

// Variable handling
static int identifierConstant(Token* name) {
    // Store the variable name as a string constant
    Value value;
    value.type = VAL_STRING;
    value.as.string = strndup(name->start, name->length);
    return makeConstant(value);
}

static void defineVariable(int global) {
    emitBytes(OP_DEFINE_VAR, global);
}

// Parsing
static void binary() {
    TOKEN_TYPE operatorType = parser.previous.type;
    
    // Compile the right operand
    expression();
    
    switch (operatorType) {
        case TOKEN_PLUS:  emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_TIMES: emitByte(OP_MULTIPLY); break;
        default: return; // Unreachable
    }
}

static void literal() {
    switch (parser.previous.type) {
        case TOKEN_NUMBER: {
            double value = strtod(parser.previous.start, NULL);
            Value val;
            val.type = VAL_NUMBER;
            val.as.number = value;
            emitConstant(val);
            break;
        }
        case TOKEN_STRING: {
            int length = parser.previous.length - 2;  // Remove quotes
            char* string = malloc(length + 1);
            memcpy(string, parser.previous.start + 1, length);
            string[length] = '\0';
            
            Value val;
            val.type = VAL_STRING;
            val.as.string = string;
            emitConstant(val);
            break;
        }
        default:
            return;
    }
}

static void variable() {
    // Get the variable name
    Token name = parser.previous;
    
    // Emit bytecode to load the variable
    int arg = identifierConstant(&name);
    emitBytes(OP_GET_VAR, arg);
}

static void parsePrecedence() {
    advance();
    
    // Parse prefix
    switch (parser.previous.type) {
        case TOKEN_NUMBER: literal(); break;
        case TOKEN_STRING: literal(); break;
        case TOKEN_IDENTIFIER: variable(); break;
        default: error("Expect expression."); return;
    }
    
    // Parse infix
    while (parser.current.type == TOKEN_PLUS || 
           parser.current.type == TOKEN_MINUS || 
           parser.current.type == TOKEN_TIMES) {
        advance();
        binary();
    }
}

static void expression() {
    parsePrecedence();
}

static void printStatement() {
    advance(); 
    expression();
    emitByte(OP_PRINT);
}

static void variableDeclaration() {
    advance();
    
    bool isImmutable = false;
    if (parser.current.type == TOKEN_PERCENT) {
        isImmutable = true;
        advance();
    }
    
    consume(TOKEN_IDENTIFIER, "Expect variable name.");
    Token variableName = parser.previous;
    
    consume(TOKEN_EQUAL, "Expect '=' after variable name.");
    
    // Parse the initializer
    expression();
    
    // Define the variable
    int global = identifierConstant(&variableName);
    defineVariable(global);
}

static void statement() {
    if (parser.current.type == TOKEN_PRINT) {
        printStatement();
    } else {
        expression();
        // Discard value if not used
        emitByte(OP_POP);
    }
}

static void declaration() {
    if (parser.current.type == TOKEN_DEFV) {
        variableDeclaration();
    } else {
        statement();
    }
}

bool compile(const char* source, Chunk* chunk) {
    // Initialize the global parser
    init_scanner(&parser.scanner, source);
    parser.chunk = chunk;
    parser.hadError = false;
    
    // Start parsing
    advance();
    
    // Parse declarations until EOF
    while (parser.current.type != TOKEN_EOF) {
        declaration();
    }
    
    emitByte(OP_RETURN);
    return !parser.hadError;
} 