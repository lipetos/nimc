#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "token_utils.h"
#include "parser.h"
#include "scanner.h"
#include "compiler.h"
#include "chunk.h"

void init_parser(Parser *p, const char *source) {
    p->source = source;
    p->line = 1;
    p->current_c = 0;
    
    p->scanner = malloc(sizeof(Scanner));
    init_scanner(p->scanner, source);

    p->chunk = malloc(sizeof(Chunk));
    init_chunk(p->chunk);
}

void start_parsing(Parser *p) {
    printf("Compiling source...\n");
    
    if (!compile(p->source, p->chunk)) {
        printf("Compilation failed.\n");
        return;
    }
    VM vm;
    init_vm(&vm);
    InterpretResult result = interpret(&vm, p->chunk);
    
    if (result == INTERPRET_OK) {
        printf("Program executed successfully.\n");
    } else {
        printf("Runtime error.\n");
    }
    
    // Clean up
    free_vm(&vm);
}