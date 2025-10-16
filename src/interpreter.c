// interpreter.c
// Copyright (c) 2025 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "../include/getch.h"
#include "../include/vectors.h"
#include "../include/utils.h"

#define BOLD    "\033[1m"
#define CYAN "\033[36m"
#define RED  "\033[31m"
#define GREEN "\033[32m"
#define RESET "\033[0m"

void interpret(vector_char code, int num_of_regs, size_t print_until, vector_int print_intervalls, char* pre_input, bool no_pause, bool quiet) {

    size_t pre_input_idx = 0;
    size_t pre_input_len = pre_input ? strlen(pre_input) : 0;
    size_t code_len = code.size;
    size_t pc = 0;
    int* memory = calloc(num_of_regs, sizeof(int));
    vector_int stack = make_vector_int(1);
    size_t ptr = 0;
    vector_size_t index_memory = make_vector_size_t(1);
    vector_size_t loop_stack = make_vector_size_t(1);

    double start = get_time_sec();
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
                } else if (code.data[pc + 1] == 'R') {
                    memory[ptr] += runtime_mz(start);
                    pc++;
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
                            if (num >= index_memory.size) {
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
                } else if (code.data[pc + 1] == 'R') {
                    memory[ptr] -= runtime_mz(start);
                    pc++;
                
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
                        "  '^' has to be followed by an unsigned integer or 's'.\n",
                        pc
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
                        "  At instruction index" CYAN " %zu " RESET "(command '#'):\n"
                        "  Cell[" CYAN "%zu" RESET "] =" RED " %d " RESET"(valid range: " GREEN "0-255" RESET ")\n",
                        pc, ptr, memory[ptr]
                    );
                    exit(1);
                }
                printf("%c", (unsigned char)memory[ptr]);
                break;

            case '.':
                int inp;
                if (pre_input) {
                    if (pre_input_idx >= pre_input_len) {
                        fprintf(stderr,
                            RED BOLD "\nIndexError" RESET ": pre-input index out of bounds\n"
                            "  At instruction index" CYAN " %zu " RESET "(command '.'):\n"
                            "  Provided pre-input string length: %zu\n",
                            pc, pre_input_len
                        );
                        exit(1);
                    }
                    inp = pre_input[pre_input_idx++];
                } else {
                    inp = getch();
                }
                printf("%c", inp);
                memory[ptr] = (unsigned char)inp;
                break;

            case ':':
                int input;
                if (pre_input) {
                    if (pre_input_idx >= pre_input_len) {
                        fprintf(stderr,
                            RED BOLD "\nIndexError" RESET ": pre-input index out of bounds\n"
                            "  At instruction index" CYAN " %zu " RESET "(command ':'):\n"
                            "  Provided pre-input string length: %zu\n",
                            pc, pre_input_len
                        );
                        exit(1);
                    }
                    input = extract_number_pre_input(pre_input, pre_input_len, &pre_input_idx);
                    printf("%d\n", input);
                    pre_input_idx++;
                    memory[ptr] = input;
                } else {
                    if (scanf("%d", &input) == 1) {
                        memory[ptr] = input;
                        int leftover = getchar();
                    } else {
                        fprintf(stderr,
                                RED BOLD "\nInputError" RESET ": expected an integer.\n"
                                "  At instuction index " CYAN "%zu" RESET " (command ':'):\n"
                                "  Provided input is not a valid integer.\n",
                                pc
                        );
                        exit(1);
                    }
                }
                break;

            case '@':
                if (code.data[pc + 1] == 'R') {
                    vector_int_push(&stack, runtime_mz(start));
                    pc++;
                } else {
                    vector_int_push(&stack, memory[ptr]);
                }
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

            case '{':
                
                size_t reg1;
                if (isdigit(code.data[pc + 1])) {
                    reg1 = (size_t)extract_number(code.data, code_len, &pc);
                    if (reg1 >= num_of_regs) {
                        fprintf(stderr,
                            RED BOLD "\nIndexError" RESET ": first parameter for if command out of bounds\n"
                            "  At instruction index" CYAN " %zu " RESET "(command '{}')\n"
                            "  Value:" RED " %zu " RESET "(Number of available memory cells: " CYAN "%d" RESET ")"
                            "\n"
                            "Hint: Increase the number of available memory cells:\n"
                            "  minez <file> --num-of-regs" GREEN " %zu " RESET "<-- HERE\n",
                            pc, reg1, num_of_regs, reg1+1
                        );
                        exit(1);
                    }
                } else if (code.data[pc + 1] == 'i') {
                    reg1 = ptr;
                    pc++;
                } else {
                    fprintf(stderr,
                        RED BOLD "\nSyntaxError" RESET ": invalid first parameter for 'if' command\n"
                        "  At instruction index" CYAN " %zu " RESET "(command '{}')\n"
                        "  Found character:" RED " '%c' " RESET "(expected uint or 'i')\n",
                        pc, code.data[pc + 1]
                    );
                    exit(1);
                }
                unsigned char op = code.data[++pc];
                size_t reg2;
                if (isdigit(code.data[pc + 1])) {
                    reg2 = (size_t)extract_number(code.data, code_len, &pc);
                    if (reg2 >= num_of_regs) {
                        fprintf(stderr,
                            RED BOLD "\nIndexError" RESET ": second parameter for if command out of bounds\n"
                            "  At instruction index" CYAN " %zu " RESET "(command '{}')\n"
                            "  Value:" RED " %zu " RESET "(Number of available memory cells: " CYAN "%d" RESET ")"
                            "\n"
                            "Hint: Increase the number of available memory cells:\n"
                            "  minez <file> --num-of-regs" GREEN " %zu " RESET "<-- HERE\n",
                            pc, reg2, num_of_regs, reg2+1
                        );
                        exit(1);
                    }
                } else if (code.data[pc + 1] == 'i') {
                    reg2 = ptr;
                    pc++;
                } else {
                    fprintf(stderr,
                        RED BOLD "\nSyntaxError" RESET ": invalid second parameter for 'if' command\n"
                        "  At instruction index" CYAN " %zu " RESET "(command '{}')\n"
                        "  Found character:" RED " '%c' " RESET "(expected uint or 'i')\n",
                        pc, code.data[pc + 1]
                    );
                    exit(1);
                }
                if (code.data[pc + 2] != '(') {
                    fprintf(stderr, 
                        RED BOLD "\nSyntaxError" RESET ": missing opening bracket '(' after 'if' command\n"
                        "  At instruction index " CYAN "%zu" RESET " (command '{}')\n",
                        pc
                    );
                    exit(1);
                }
                if (op == '=') {
                    if (memory[reg1] == memory[reg2]) {
                        pc += 2;
                    } else {
                        pc = search_for_endif(code.data, code_len, pc + 2);
                    }
                } else if (op == '<') {
                    if (memory[reg1] < memory[reg2]) {
                        pc += 2;
                    } else {
                        pc = search_for_endif(code.data, code_len, pc + 2);
                    }
                } else if (op == '>') {
                    if (memory[reg1] > memory[reg2]) {
                        pc += 2;
                    } else {
                        pc = search_for_endif(code.data, code_len, pc + 2);
                    }
                } else {
                    fprintf(stderr,
                        RED BOLD "\nSyntaxError" RESET ": invalid operator for 'if' command\n"
                        "  At instruction index" CYAN " %zu " RESET "(command '{}')\n"
                        "  Found character:" RED " '%c' " RESET "(expected '=', '<' or '>')\n",
                        pc, op
                    );
                    exit(1);
                }
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

            case ')':
                break;

            case ';':
                if (!quiet) {
                    printf("\n\n-------------------------------------\n");
                    printf("\nProgram halted at index %zu.", pc);
                }
                pc = code_len;
                break;

            case 'd':
                if (quiet) break;
                printf("\n\nDebugging info at instruction index %zu\n", pc);
                printf("Memory: ");
                for (size_t i = 0; i < print_until - 1; i++) {
                    printf("%d, ", memory[i]);
                }
                printf("%d", memory[print_until - 1]);
                if (print_until < num_of_regs) {
                    printf(" . . .");
                }
                printf("\n");
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
                        printf("%zu, ", index_memory.data[i]);
                    }
                    printf("%zu\n", index_memory.data[index_memory.size - 1]);
                } else {
                    printf("empty\n");
                }
                printf("Loop stack: ");
                if (loop_stack.size > 0) {
                    for (size_t i = 0; i < loop_stack.size - 1; i++) {
                        printf("%zu, ", loop_stack.data[i]);
                    }
                    printf("%zu\n", loop_stack.data[loop_stack.size - 1]);
                } else {
                    printf("empty\n");
                }
                break;

            default:
                fprintf(stderr, RED BOLD "\nError" RESET ": Unknown command '" RED "%c" RESET"' at instruction index " CYAN "%zu" RESET ".", code.data[pc], pc);
                exit(1);
        }
        pc++;
    }
    double end = get_time_sec();
    double runtime = end - start;
    if (!quiet) {
        printf("\n\nProgram ended successfully!\n");
        printf("Memory: ");
        for (size_t i = 0; i < print_until - 1; i++) {
            printf("%d, ", memory[i]);
        }
        printf("%d", memory[print_until - 1]);
        if (print_until < num_of_regs) {
            printf(" . . .");
        }
        printf("\n");
        if (print_intervalls.size > 0) {
            for (size_t i = 0; i < print_intervalls.size; i += 2) {
                size_t start = print_intervalls.data[i];
                size_t end = print_intervalls.data[i + 1];
                if (start > end) {
                    printf("Invalid interval %zu, %zu\n", start, end);
                    continue;
                }
                printf("Interval %zu, %zu: ", start, end);
                for (size_t j = start; j <= end; j++) {
                    printf("%d", memory[j]);
                    if (j != end) printf(", ");
                }
                printf("\n");
            }
        }
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
                printf("%zu, ", index_memory.data[i]);
            }
            printf("%zu\n", index_memory.data[index_memory.size - 1]);
        } else {
            printf("empty\n");
        }
        printf("Runtime: %.6f sec\n", runtime);
        if (!no_pause) {
            printf("Press any key to continue...");
            getch();
            printf("\n");
        }
    }
    free(loop_stack.data);
    free(memory);
    free(stack.data);
    free(index_memory.data);
}