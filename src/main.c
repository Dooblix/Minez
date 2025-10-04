// main.c
// Copyright (c) 2025 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

// Standart libs
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
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
    bool no_pause = false;
    int num_of_regs = 100;
    size_t print_until = 100;
    bool print_until_used = false;
    vector_int print_intervalls = make_vector_int(2);
    bool docs = false;
    bool quiet = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--docs") == 0 || strcmp(argv[i], "-d") == 0) {
            docs = true;
        } else  if(strcmp(argv[i], "--quiet") == 0 || strcmp(argv[i], "-q") == 0) {
            quiet = true;
        } else if (strcmp(argv[i], "--num-of-regs") == 0) {
            if (i + 1 < argc) {
                num_of_regs = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Error: --num-of-regs expects a number\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--print-until") == 0) {
            if (i + 1 < argc) {
                print_until = atoi(argv[++i]);
                if (print_until > num_of_regs) {
                    fprintf(stderr, "Error: --print-until has to be smaller than num_of_regs.\n");
                    return 1;
                }
                print_until_used = true;
            } else {
                fprintf(stderr, "Error: --print-until expects a number\n");
                return 1;
            }
        } else if (strcmp(argv[i], "--print-intervalls") == 0) {
            while (i + 1 < argc && argv[i+1][0] != '-') {
                vector_int_push(&print_intervalls, atoi(argv[++i]));
            }
            if (print_intervalls.size % 2 != 0) {
                fprintf(stderr, "Error: --print-intervalls expects a list of integers with an even size!");
                return 1;
            }
        } else if (strcmp(argv[i], "--no-pause") == 0) {
            no_pause = true;
        } else if (argv[i][0] != '-') {
            file_name = argv[i];
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            return 1;
        }
    }

    if (!print_until_used) {
        print_until = num_of_regs;
    }

    if (docs) {
        print_docs();
        return 0;
    }

    if (file_name == NULL) {
        fprintf(stderr, "Fatal error: missing one required argument 'source_file' (required when not using '--docs').");
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
    char prev = 0;
    // Reads file but removes comments and whitespaces
    while ((curr_ch = fgetc(file)) != EOF) {
        if (prev == '/' && curr_ch == '/') {
            while ((curr_ch = fgetc(file)) != '\n' && curr_ch != EOF);
            prev = 0;
        } else {
            if (prev && !(prev == '\n' || prev == '\t' || prev == ' ')) vector_char_push(&code, prev);
            if (curr_ch != '/' && !(curr_ch == '\n' || curr_ch == '\t' || curr_ch == ' ')) vector_char_push(&code, (char)curr_ch);
            prev = curr_ch == '/' ? curr_ch : 0;
        }
    }
    if (prev) vector_char_push(&code, prev);
    fclose(file);
    
    if (code.size == 0 || code.data[code.size-1] != ';') {
        vector_char_push(&code, ';');
    }

    interpret(code, num_of_regs, print_until, print_intervalls, no_pause, quiet);

    free(code.data);
    free(print_intervalls.data);
    return 0;
}