// minez_docs.c
// Copyright (c) 2025 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

#include <stdio.h>
#include <string.h>
#include "../include/minez_docs.h"

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define CYAN "\033[36m"
#define WHITE "\033[97m"
#define YELLOW "\033[33m"

void section(const char* title) {
    printf("\n%s%s%s\n", BOLD, title, RESET);
    for (int i = 0; i < (int)strlen(title); i++) printf("-");
    printf("\n");
}

void table(const char* commands[][2], int n) {
    int col1_width = 0;
    for (int i = 0; i < n; i++) {
        int len1 = strlen(commands[i][0]);
        if (len1 > col1_width) col1_width = len1;
    }
    col1_width += 2;

    printf("%s%-*s | %s%s\n", BOLD, col1_width, "Command", "Description", RESET);
    for (int i = 0; i < col1_width; i++) printf("-");
    printf("-+-");
    for (int i = 0; i < 50; i++) printf("-");
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%s%-*s%s | %s%s%s\n",
               CYAN, col1_width, commands[i][0], RESET,
               WHITE, commands[i][1], RESET);
    }
}

void print_docs() {
    printf("%sMinez (2025) - Esoteric Language inspired by Brainfuck%s\n", BOLD, RESET);
    printf("%sQuick Usage:%s ./minez.exe <program.minez> [options]\n", YELLOW, RESET);
    printf("%sOptions:%s --num-of-regs N, --print-until N, --print-intervalls A B ..., --quiet/-q, --docs/-d, --no-pause\n\n", YELLOW, RESET);

    // Pointer
    const char* pointer_ops[][2] = {
        {">y", "Move pointer to absolute index y"},
        {">",  "Increment pointer by 1"},
        {"<",  "Decrement pointer by 1"}
    };
    section("Pointer Manipulation");
    table(pointer_ops, 3);

    // Flow Control
    const char* flow_control[][2] = {
        {"^y", "Jump to index y, save next pointer on stack"},
        {"^s", "Return to last saved pointer"},
        {"[",  "Begin loop (while current register != 0)"},
        {"~",  "Skip to next loop iteration"},
        {"]",  "End loop (repeat if current register != 0)"},
        {"{}", "Conditional: {reg>reg}(...), {reg<reg}(...), {reg=reg}(...), i=current register"},
        {";",  "End program"}
    };
    section("Flow Control");
    table(flow_control, 7);

    // Stack
    const char* stack_ops[][2] = {
        {"@", "Push current register onto stack"},
        {"@R","Push runtime (ms) onto stack"},
        {"_", "Pop last value from stack into current register (additive)"}
    };
    section("Stack Operations");
    table(stack_ops, 3);

    // Register Arithmetic
    const char* reg_arith[][2] = {
        {"+y", "Add y to current register"},
        {"+R", "Add runtime (ms) to current register"},
        {"+",  "Increment register by 1"},
        {"-y", "Subtract y from current register"},
        {"-R", "Subtract runtime (ms) from current register"},
        {"-",  "Decrement register by 1"},
        {"x",  "Reset current register to 0"}
    };
    section("Register Arithmetic");
    table(reg_arith, 7);

    // Input/Output
    const char* io_ops[][2] = {
        {"#",  "Output ASCII character of current register"},
        {"#!", "Output numeric value"},
        {".",  "Read single input character (ASCII)"},
        {":",  "Read integer (any size, incl. negatives)"}
    };
    section("Input/Output");
    table(io_ops, 4);

    // Index Memory
    const char* index_mem[][2] = {
        {"|",     "Save current pointer index"},
        {"->",    "Jump to last saved index"},
        {"->(y)", "Jump to y-th saved index"},
        {"X",     "Delete last saved index"}
    };
    section("Index Memory");
    table(index_mem, 4);

    // Debugging
    const char* debug_ops[][2] = {
        {"d", "Print debugging info: Memory, Pointer, Stack, Index memory, Runtime"}
    };
    section("Debugging");
    table(debug_ops, 1);

    printf("\n%sDetailed documentation:%s https://github.com/Dooblix/Minez\n", BOLD, RESET);
    printf("%sAuthor:%s Dooblix © 2025\n", BOLD, RESET);
    printf("\n%s----------------------------------------%s\n", YELLOW, RESET);
}