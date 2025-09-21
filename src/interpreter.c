// interpreter.c
// Copyright (c) 2025 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/getch.h"
#include "../include/vectors.h"

#define CYAN "\033[36m"
#define RED  "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

long extract_number(char* code, size_t code_len, size_t* pc) {
    (*pc)++;
    vector_char result = make_vector_char(1);
    while (*pc < code_len && isdigit((unsigned char)code[*pc])) {
        vector_char_push(&result, code[*pc]);
        (*pc)++;
    }
    (*pc)--;
    vector_char_push(&result, '\0');
    return strtol(result.data, NULL, 10);
}

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
    fprintf(stderr, "SyntaxError at index %u: Missing closing brakcet ']'.", pc);
    exit(1);
}

void interprete(vector_char code, bool hide_input_prompts, int num_of_regs) {

    size_t code_len = code.size;
    size_t pc = 0;
    int* memory = calloc(num_of_regs, sizeof(int));
    size_t ptr = 0;
    vector_size_t loop_stack = make_vector_size_t(2);

    while (pc < code_len) {
        char cmd = code.data[pc];
        switch (cmd) {
            case '>':
                if (isdigit(code.data[pc + 1])) {
                    ptr = (size_t)extract_number(code.data, code_len, &pc);
                } else {
                    ptr++;
                }
                if (ptr >= num_of_regs) {
                    fprintf(stderr,
                        RED "IndexError" RESET ": pointer out of bounds\n"
                        "  At instruction index" CYAN " %zu " RESET "(command '>y')\n"
                        "  Pointer value:" RED " %zu " RESET "(Number of available memory cells: " CYAN "%d" RESET ")"
                        "\n"
                        "Hint: Increase the number of available memory cells:\n"
                        "  minez <file> --num-of-regs" GREEN " %zu " RESET "<-- HERE\n",
                        pc, ptr, num_of_regs,ptr+1
                    );
                    exit(1);
                }
                break;
            case '<':
                if (ptr > 0) {
                    ptr--;
                } else {
                    fprintf(stderr,
                        RED "IndexError" RESET ": pointer out of bounds\n"
                        "  At instruction index" CYAN " %zu " RESET "(command '<')\n"
                        "  Pointer value:" RED " %zu" RESET "\n(Pointer value underflowed to a very large number due to unsigned wraparound)\n",
                        pc, ptr-1
                    );
                    exit(1);
                }
                break;
            case '+':
                if (isdigit(code.data[pc + 1])) {
                    memory[ptr] += extract_number(code.data, code_len, &pc);
                } else {
                    memory[ptr]++;
                }
                break;
            case '-':
                if (isdigit(code.data[pc + 1])) {
                    memory[ptr] -= extract_number(code.data, code_len, &pc);
                } else {
                    memory[ptr]--;
                }
                break;
            case '#':
                if (memory[ptr] < 0 || memory[ptr] > 255) {
                    fprintf(stderr,
                        RED "ValueError" RESET ": cannot print cell value\n"
                        "  At instruction index" CYAN " %zu " RESET "(command '.'): \n"
                        "  Cell[" CYAN "%zu" RESET "] =" RED " %d " RESET"(valid range: " GREEN "0-255" RESET ")\n",
                        pc, ptr, memory[ptr]
                    );
                    exit(1);
                }
                printf("%c", (unsigned char)memory[ptr]);
                break;
            case '.':
                int inp = getch();
                printf("%c", inp);
                memory[ptr] = (unsigned char)inp;
                break;
            case '[':
                if (memory[ptr] == 0) {
                    pc = search_for_endloop(code.data, code_len, pc);
                } else {
                    vector_size_t_push(&loop_stack, pc);
                }
                break;
            case ']':
                if (loop_stack.size == 0) {
                    fprintf(stderr, 
                        RED "LoopError" RESET ": unmatched closing bracket ']'\n"
                        "  At instruction index " CYAN "%zu\n" RESET
                        "  The loop stack is empty; no matching opening '[' found.\n",
                        pc
                    );
                    exit(1);
                }
                if (memory[ptr] != 0) {
                    pc = loop_stack.data[loop_stack.size - 1];
                } else {
                    vector_size_t_pop(&loop_stack);
                }
                break;
            case ';':
                printf("\nProgram halted at index %zu.", pc);
                pc = code_len;
                break;
            default:
                fprintf(stderr, RED "\nError" RESET ": Unknown command '" RED "%c" RESET"' at index " CYAN "%d" RESET ".", code.data[pc], pc);
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