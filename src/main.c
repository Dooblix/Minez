// main.c
// Copyright (c) 2026 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

// Standart libs
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
// Project headers
#include "../include/minez_help.h"
#include "../include/interpreter.h"
#include "../include/vectors.h"
#include "../include/utils.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Fatal error: no arguments provided. (use '-h' or '--help' for usage documentation)\n");
        return 1;
    }

    int status = 0;

    vector_char code = make_vector_char(50);
    char* file_name = NULL;
    bool no_pause = false;
    int num_of_regs = 100;
    size_t print_until = 100;
    bool print_until_used = false;
    vector_int print_intervals = make_vector_int(2);
    char* parsed = NULL;
    char* pre_input = NULL;
    bool help = false;
    bool quiet = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            help = true;
        } else  if(strcmp(argv[i], "--quiet") == 0 || strcmp(argv[i], "-q") == 0) {
            quiet = true;
        } else if (strcmp(argv[i], "--num-of-regs") == 0) {
            if (i + 1 < argc) {
                num_of_regs = atoi(argv[++i]);
            } else {
                fprintf(stderr, "Error: --num-of-regs expects a number\n");
                status = 1;
                goto cleanup;
            }
        } else if (strcmp(argv[i], "--print-until") == 0) {
            if (i + 1 < argc) {
                print_until = atoi(argv[++i]);
                if (print_until > num_of_regs) {
                    fprintf(stderr, "Error: --print-until has to be smaller than num_of_regs.\n");
                    status = 1;
                    goto cleanup;
                }
                print_until_used = true;
            } else {
                fprintf(stderr, "Error: --print-until expects a number\n");
                status = 1;
                goto cleanup;
            }
        } else if (strcmp(argv[i], "--print-intervals") == 0) {
            while (i + 1 < argc && argv[i+1][0] != '-') {
                vector_int_push(&print_intervals, atoi(argv[++i]));
            }
            if (print_intervals.size % 2 != 0) {
                fprintf(stderr, "Error: --print-intervals expects a list of integers with an even size!");
                status = 1;
                goto cleanup;
            }
        } else if (strcmp(argv[i], "--no-pause") == 0) {
            no_pause = true;
        } else if (strcmp(argv[i], "--pre-input") == 0) {
            if (i + 1 < argc) {
                pre_input = argv[++i];
            } else {
                fprintf(stderr, "Error: --pre-input expects a string!");
                status = 1;
                goto cleanup;
            }
        } else if (argv[i][0] != '-') {
            file_name = argv[i];
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            status = 1;
            goto cleanup;
        }
    }

    if (!print_until_used) {
        print_until = num_of_regs;
    }

    if (help) {
        print_help();
        goto cleanup;
    }

    if (file_name == NULL) {
        fprintf(stderr, "Fatal error: missing one required argument 'source_file' (required when not using '--help').");
        status = 1;
        goto cleanup;
    }

    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Error: File '%s' not found.\n", file_name);
        status = 1;
        goto cleanup;
    }

    int curr_ch;
    size_t code_len = 0;
    char prev = 0;
    // Reads file; removes comments and whitespaces
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
    
    // Makes sure the program halts and prevents some index errors
    if (code.size == 0 || code.data[code.size-1] != ';') {
        vector_char_push(&code, ';');
    }

    if (pre_input) {
        parsed = parse_pre_input(pre_input);
        if (!parsed) {
            status = 1;
            goto cleanup;
        }
    }
    
    status = interpret(code, num_of_regs, print_until, print_intervals, parsed, no_pause, quiet);

cleanup:
    free(parsed);
    free(code.data);
    free(print_intervals.data);
    return status;
}