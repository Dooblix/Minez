// interpreter.c
// Copyright (c) 2025 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <conio.h>

#include "../include/vectors.h"

size_t search_for_endloop(char* code, size_t code_len, size_t pc) {
    size_t endloop = pc;
    size_t loop_depth = 0;
    while (endloop < code_len) {
        if (code[endloop] == '[') {
            loop_depth++;
        } else if (code[endloop] == ']') {
            loop_depth--;
            if (loop_depth == 0) {
                return endloop;
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
                    pc = search_for_endloop(code.data, code_len, pc);
                } else {
                    vector_size_t_push(&loop_stack, pc);
                }
                break;
            case ']':
                if (memory[ptr] != 0) {
                    pc = loop_stack.data[loop_stack.size - 1];
                } else {
                    vector_size_t_pop(&loop_stack);
                }
                break;
            default:
                fprintf(stderr, "\nError: Unknown Brainfuck-Command '%c' at index %d.", code.data[pc], pc);
                exit(1);
        }
        pc++;
    }
    free(loop_stack.data);
    printf("\n\nProgram ended successfully!\n");
    printf("Memory: ");
    for (size_t i = 0; i < num_of_regs - 1; i++) {
        printf("%d, ", memory[i]);
    }
    printf("%d\n", memory[num_of_regs - 1]);
    printf("Pointer: %zu\n", ptr);
    printf("Press any key to continue...");
    getch();
}