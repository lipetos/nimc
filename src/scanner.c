#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "scanner.h"
#include "token_utils.h"

void init_scanner(Scanner* scanner, const char* source) {
    scanner->start = source;
    scanner->current = source;
    scanner->line = 1;
}

static bool is_alpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

static bool is_at_end(Scanner* scanner) {
    return *scanner->current == '\0';
}

static char advance(Scanner* scanner) {
    return *scanner->current++;
}

static char peek(Scanner* scanner) {
    return *scanner->current;
}

static char peek_next(Scanner* scanner) {
    if (is_at_end(scanner)) return '\0';
    return scanner->current[1];
}

static bool match(Scanner* scanner, char expected) {
    if (is_at_end(scanner)) return false;
    if (*scanner->current != expected) return false;
    scanner->current++;
    return true;
}

static void skip_whitespace(Scanner* scanner) {
    for (;;) {
        char c = peek(scanner);
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(scanner);
                break;
            case '\n':
                scanner->line++;
                advance(scanner);
                break;
            case '*':
                // Skip comments until end of line or end of file
                advance(scanner); // Skip the *
                while (peek(scanner) != '\n' && !is_at_end(scanner)) {
                    advance(scanner);
                }
                break;
            default:
                return;
        }
    }
}

static Token make_token(Scanner* scanner, TOKEN_TYPE type) {
    Token token;
    token.type = type;
    token.start = scanner->start;
    token.length = (int)(scanner->current - scanner->start);
    token.line = scanner->line;
    return token;
}

static Token error_token(Scanner* scanner, const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int)strlen(message);
    token.line = scanner->line;
    return token;
}

static TOKEN_TYPE check_keyword(Scanner* scanner, int start, int length, 
                           const char* rest, TOKEN_TYPE type) {
    if (scanner->current - scanner->start == start + length &&
        memcmp(scanner->start + start, rest, length) == 0) {
        return type;
    }
    return TOKEN_IDENTIFIER;
}

static TOKEN_TYPE identifier_type(Scanner* scanner) {
    switch (scanner->start[0]) {
        case 'd': return check_keyword(scanner, 1, 3, "efv", TOKEN_DEFV);
        case 'p': return check_keyword(scanner, 1, 4, "rint", TOKEN_PRINT);
    }
    return TOKEN_IDENTIFIER;
}

static Token identifier(Scanner* scanner) {
    while (is_alpha(peek(scanner)) || is_digit(peek(scanner))) {
        advance(scanner);
    }
    return make_token(scanner, identifier_type(scanner));
}

static Token number(Scanner* scanner) {
    while (is_digit(peek(scanner))) {
        advance(scanner);
    }

    // Look for a decimal part
    if (peek(scanner) == '.' && is_digit(peek_next(scanner))) {
        // Consume the "."
        advance(scanner);

        while (is_digit(peek(scanner))) {
            advance(scanner);
        }
    }

    return make_token(scanner, TOKEN_NUMBER);
}

static Token string(Scanner* scanner) {
    while (peek(scanner) != '"' && !is_at_end(scanner)) {
        if (peek(scanner) == '\n') scanner->line++;
        advance(scanner);
    }

    if (is_at_end(scanner)) {
        return error_token(scanner, "Unterminated string.");
    }

    // The closing quote
    advance(scanner);
    return make_token(scanner, TOKEN_STRING);
}

Token scan_token(Scanner* scanner) {
    skip_whitespace(scanner);
    scanner->start = scanner->current;

    if (is_at_end(scanner)) return make_token(scanner, TOKEN_EOF);

    char c = advance(scanner);

    if (is_alpha(c)) return identifier(scanner);
    if (is_digit(c)) return number(scanner);

    switch (c) {
        case '(': return make_token(scanner, TOKEN_LEFT_PAREN);
        case ')': return make_token(scanner, TOKEN_RIGHT_PAREN);
        case '%': return make_token(scanner, TOKEN_PERCENT);
        case '+': return make_token(scanner, TOKEN_PLUS);
        case '-': return make_token(scanner, TOKEN_MINUS);
        case '*': return make_token(scanner, TOKEN_TIMES);
        case '=': return make_token(scanner, TOKEN_EQUAL);
        case '"': return string(scanner);
    }

    return error_token(scanner, "Unexpected character.");
} 