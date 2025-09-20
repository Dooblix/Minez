// interpreter.c
// Copyright (c) 2025 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#include "../include/helper_functions.h"

size_t search_for_endloop(char* code, size_t code_len, size_t pc) {
    size_t endloop = pc;
    size_t loop_depth = 1;
    while (endloop < code_len) {
        if (code[endloop] == '[') {
            loop_depth++;
        } else if (code[endloop] == ']') {
            if (loop_depth == 0) {
                return endloop;
            } else {
                loop_depth--;
            }
        }
        endloop++;
    }
    fprintf(stderr, "Error at index %u: Missing closing ']'.", pc);
    exit(1);
}

void interprete(vector_char code, bool hide_input_prompts, int num_of_regs) {

    size_t code_len = code.size;
    size_t pc = 0;
    unsigned char* memory = calloc(num_of_regs, sizeof(unsigned char));
    size_t ptr = 0;
    vector_size_t loop_stack = make_vector_size_t(2);

    while (pc < code_len) {
        char cmd = code.data[pc];
        switch (cmd) {
            case '>':
                ptr++;
                break;
            case '<':
                ptr--;
                break;
            case '+':
                memory[ptr]++;
                break;
            case '-':
                memory[ptr]--;
                break;
            case '.':
                printf("%c", memory[ptr]);
                break;
            case ',':
                char inp = getch();
                printf("%c", inp);
                memory[ptr] = inp;
                break;
            case '[':
                if (memory[ptr] == 0) {
                    pc = search_for_endloop(code.data, code_len, pc) - 1;
                } else {
                    vector_size_t_push(&loop_stack, pc);
                }
                break;
            case ']':
                if (memory[ptr] != 0) {
                    pc = loop_stack.data[loop_stack.size - 1] - 1;
                } else {
                    vector_size_t_pop(&loop_stack);
                }
                break;
            default:
                fprintf(stderr, "Error: Unknown Brainfuck-Command at index %d", pc);
                exit(1);
        }
        pc++;
    }
}