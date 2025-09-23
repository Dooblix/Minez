// interpreter.c
// Copyright (c) 2025 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#include "../include/getch.h"
#include "../include/vectors.h"

#define BOLD    "\033[1m"
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
    size_t endloop = pc + 1;
    size_t loop_depth = 1;
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
    fprintf(stderr, RED BOLD "\nSyntaxError" RESET " at index " CYAN "%u" RESET ": Missing closing bracket ']'.", pc);
    exit(1);
}

void interprete(vector_char code, int num_of_regs) {

    size_t code_len = code.size;
    size_t pc = 0;
    int* memory = calloc(num_of_regs, sizeof(int));
    vector_int stack = make_vector_int(1);
    size_t ptr = 0;
    vector_size_t index_memory = make_vector_size_t(1);
    vector_size_t loop_stack = make_vector_size_t(1);

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
                        RED BOLD "\nIndexError" RESET ": pointer out of bounds\n"
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
                        RED BOLD "\nIndexError" RESET ": pointer out of bounds\n"
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
                } else if (code.data[pc + 1] == '>') {
                    if (index_memory.size > 0) {
                        if (code.data[pc + 2] == '(') {
                            pc += 2;
                            long num = extract_number(code.data, code_len, &pc);
                            if (code.data[pc + 1] != ')') {
                                fprintf(stderr, 
                                    RED BOLD "\nSyntaxError" RESET ": unmatched closing bracket ')'\n"
                                    "  At instruction index " CYAN "%zu\n" RESET,
                                    pc
                                );
                                exit(1);
                            } else {
                                pc++;
                            }
                            if (num < 0 || num >= index_memory.size) {
                                fprintf(stderr,
                                    RED BOLD "\nIndexError" RESET ": param for '->(y)' out of bounds\n"
                                    "  At instruction index" CYAN " %zu " RESET "\n"
                                    "  Value:" RED " %zu" RESET "\n"
                                    "  Valid range: " GREEN "0-%zu" RESET "\n",
                                    pc-2, num, index_memory.size-1
                                );
                                exit(1);
                            } else {
                                ptr = index_memory.data[num];
                            }
                        } else {
                            ptr = index_memory.data[index_memory.size - 1];
                            pc++;
                        }
                    } else {
                        fprintf(stderr,
                            RED BOLD "\nStackError" RESET ": attempted to access empty index_memory.\n"
                            "  At instruction index " CYAN "%zu" RESET " (command: '->')\n",
                            pc
                        );
                        exit(1);
                    }
                } else {
                    memory[ptr]--;
                }
                break;

            case 'x':
                memory[ptr] = 0;
                break;

            case '|':
                vector_size_t_push(&index_memory, ptr);
                break;

            case 'X':
                if (index_memory.size == 0) {
                     fprintf(stderr,
                        RED BOLD "\nStackError" RESET ": attempted to pop from empty index_memory.\n"
                        "  At instruction index " CYAN "%zu" RESET " (command: 'X')\n",
                        pc
                    );
                    exit(1);
                }
                vector_size_t_pop(&index_memory);
                break;

            case '^':
                if (code.data[pc + 1] == 's') {
                    pc = (size_t)vector_int_pop(&stack);
                    continue;
                }
                if (!(isdigit(code.data[pc + 1]))) {
                    fprintf(stderr,
                        RED BOLD "SyntaxError" RESET ": missing parameter for '^'.\n"
                        "  At instruction index" CYAN " %zu " RESET ":\n"
                        "  '^' has to be followed by an unsigned integer or 's'.\n"
                    );
                    exit(1);
                }
                long param = extract_number(code.data, code_len, &pc);
                vector_int_push(&stack, pc+1);
                pc = (size_t)param;
                if (pc >= code_len) {
                    fprintf(stderr,
                        RED BOLD "\nIndexError" RESET ": program counter out of bounds\n"
                        "  At instruction index" CYAN " %zu " RESET "(command '^')\n"
                        "  PC value:" RED " %zu" RESET "\n"
                        "  Largest possible '^'-parameter: " GREEN "%zu" RESET "\n",
                        pc, ptr-1, code_len-1
                    );
                    exit(1);
                }
                continue;

            case '#':
                if (code.data[pc+1] == '!') {
                    printf("%d", memory[ptr]);
                    pc++;
                    break;
                }
                if (memory[ptr] < 0 || memory[ptr] > 255) {
                    fprintf(stderr,
                        RED BOLD "\nValueError" RESET ": cannot print cell value\n"
                        "  At instruction index" CYAN " %zu " RESET "(command '.'):\n"
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

            case ':':
                int input;
                if (scanf("%d", &input) == 1) {
                    memory[ptr] = input;
                } else {
                    fprintf(stderr,
                            RED BOLD "\nInputError" RESET ": expected an integer.\n"
                            "  At instuction index " CYAN "%zu" RESET " (command ':'):\n"
                            "  Provided input is not a valid integer.\n",
                            pc
                    );
                    exit(1);
                }
                break;

            case '@':
                vector_int_push(&stack, memory[ptr]);
                break;

            case '_':
                if (stack.size > 0) {
                    memory[ptr] += vector_int_pop(&stack);
                    break;
                } else {
                    fprintf(stderr,
                        RED BOLD "\nStackError" RESET ": attempted to pop from empty stack.\n"
                        "  At instruction index " CYAN "%zu" RESET " (command: '_')\n",
                        pc
                    );
                    exit(1);
                }

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
                        RED BOLD "\nSyntaxError" RESET ": unmatched closing bracket ']'\n"
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

            case '~':
                 if (loop_stack.size == 0) {
                    fprintf(stderr, 
                        RED BOLD "\nSyntaxError" RESET ": '~' (continue) outside of loop\n"
                        "  At instruction index " CYAN "%zu\n" RESET
                        "  The loop stack is empty.\n",
                        pc
                    );
                    exit(1);
                }
                if (memory[ptr] != 0) {
                    pc = loop_stack.data[loop_stack.size - 1];
                } else {
                    vector_size_t_pop(&loop_stack);
                    pc = search_for_endloop(code.data, code_len, pc);
                }
                break;

            case ';':
                printf("\nProgram halted at index %zu.", pc);
                pc = code_len;
                break;

            default:
                fprintf(stderr, RED BOLD "\nError" RESET ": Unknown command '" RED "%c" RESET"' at index " CYAN "%d" RESET ".", code.data[pc], pc);
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
    printf("Stack: ");
    if (stack.size > 0) {
        for (size_t i = 0; i < stack.size - 1; i++) {
            printf("%d, ", stack.data[i]);
        }
        printf("%d\n", stack.data[stack.size - 1]);
    } else {
        printf("empty\n");
    }
    printf("Index memory: ");
    if (index_memory.size > 0) {
        for (size_t i = 0; i < index_memory.size - 1; i++) {
            printf("%d, ", index_memory.data[i]);
        }
        printf("%d\n", index_memory.data[index_memory.size - 1]);
    } else {
        printf("empty\n");
    }
    printf("Press any key to continue...");
    getch();
}