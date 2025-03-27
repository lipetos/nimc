#ifndef PARSER
#define PARSER

#include "token_utils.h"
#include "chunk.h"
#include "scanner.h"

typedef struct {
  int line;
  int current_c;
  const char *source;
  Chunk *chunk;
  Scanner *scanner;
} Parser;

void init_parser(Parser *p, const char *source);
void start_parsing(Parser *p);

#endif