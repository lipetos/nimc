#ifndef TOKEN_UTILS
#define TOKEN_UTILS

typedef enum {
  // single char tokens
  TOKEN_PLUS, TOKEN_MINUS, TOKEN_TIMES, TOKEN_EQUAL,
  TOKEN_PERCENT, TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
  
  // literals
  TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
  
  // keywords
  TOKEN_DEFV, TOKEN_PRINT,
  
  // other
  TOKEN_EOF, TOKEN_ERROR
} TOKEN_TYPE;

// we just store the start of the char + the length to discover where the token is at.
typedef struct {
  TOKEN_TYPE type;
  int length;
  int line;
  const char *start;
} Token;

#endif