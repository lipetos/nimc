#ifndef SCANNER_H
#define SCANNER_H

#include "token_utils.h"
#include <stdbool.h>

typedef struct {
    const char* start;
    const char* current;
    int line;
} Scanner;

void init_scanner(Scanner* scanner, const char* source);
Token scan_token(Scanner* scanner);

#endif 