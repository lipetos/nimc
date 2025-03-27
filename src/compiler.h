#ifndef COMPILER_H
#define COMPILER_H

#include "vm.h"
#include "scanner.h"

bool compile(const char* source, Chunk* chunk);

#endif 