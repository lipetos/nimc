#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "fileutils.h"
#include "parser.h"

int main(int argc, char *argv[]) {
    // Check for command-line argument
    if (argc < 2) {
        printf("Usage: %s <script>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *file_path = argv[1];
    FILE *file_pointer;
    file_pointer = fopen(file_path, "r");

    if (file_pointer == NULL) {
        printf("Could not find the following file: %s", file_path);
        exit(EXIT_FAILURE);
    }

    const long int sc_size = fsize(file_pointer);
    char *buffer = malloc(sc_size + 1);

    if (buffer == NULL) {
        printf("Memory allocation for buffer failed.");
        fclose(file_pointer);
        exit(EXIT_FAILURE);
    }

    fread(buffer, 1, sc_size, file_pointer);
    buffer[sc_size] = '\0';

    Parser *parser = malloc(sizeof(Parser));
    if (parser == NULL) {
        printf("Memory allocation for parser failed.");
        fclose(file_pointer);
        free(buffer);
        exit(EXIT_FAILURE);
    }
    init_parser(parser, buffer);
    start_parsing(parser);

    free(parser->chunk);
    free(parser->scanner);
    free(parser);
    free(buffer);
    fclose(file_pointer);

    return 0;
}
