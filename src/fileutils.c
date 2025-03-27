#include "fileutils.h"
#include <stdio.h>
long fsize(FILE *f) {
  fseek(f, 0, SEEK_END);
  long file_size = ftell(f);
  rewind(f);
  return file_size;
}