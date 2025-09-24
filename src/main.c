// main.c
// Copyright (c) 2025 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

// Standart libs
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Project headers
#include "../include/minez_docs.h"
#include "../include/interpreter.h"
#include "../include/vectors.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Fatal error: no arguments provided.\n");
        return 1;
    }

    char* file_name = NULL;
    bool hide_input_prompts = false;
    int num_of_regs = 100;
    bool docs = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--docs") == 0) {
            docs = true;
        } else if (strcmp(argv[i], "--num-of-regs") == 0) {
            if (i + 1 < argc) {
                num_of_regs = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Error: --num-of-regs expects a number\n");
                return 1;
            }
        } else if (argv[i][0] != '-') {
            file_name = argv[i];
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            return 1;
        }
    }

    if (docs) {
        print_docs();
        return 0;
    }

    if (file_name == NULL) {
        fprintf(stderr, "Fatal error: missing one required argument 'file_name' (required when not using '--docs').");
        return 1;
    }

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", file_name);
        return 1;
    }

    int curr_ch;
    vector_char code = make_vector_char(50);
    size_t code_len = 0;
    while ((curr_ch = fgetc(file)) != EOF) {
        // only add valid minez chars
        if (curr_ch == '>' || curr_ch == '<' || curr_ch == '+' || curr_ch == '-' || curr_ch == '[' || curr_ch == ']' || curr_ch == '~' || curr_ch == '.' || curr_ch == ':' || curr_ch == '#' || curr_ch == '!' || curr_ch == '0' || curr_ch == '1' || curr_ch == '2' || curr_ch == '3' || curr_ch == '4' || curr_ch == '5' || curr_ch == '6' || curr_ch == '7' || curr_ch == '8' || curr_ch == '9' || curr_ch == '{' || curr_ch == '}' || curr_ch == '(' || curr_ch == ')' || curr_ch == 'd' || curr_ch == 's' || curr_ch == 'x' || curr_ch == 'X' || curr_ch == '^' || curr_ch == '@' || curr_ch == '_' || curr_ch == '=' || curr_ch == 'i' || curr_ch == '|' || curr_ch == ';') {
            vector_char_push(&code, (char)curr_ch);
        }
    }
    fclose(file);
    
    if (code.data[code.size-1] != ';') {
        vector_char_push(&code, ';');
    }

    interprete(code, num_of_regs);

    free(code.data);
    return 0;
}