#include <stdio.h>
#include <string.h>
#include "../include/minez_docs.h"

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define ITALIC "\033[3m"
#define CYAN "\033[36m"
#define WHITE "\033[97m"

void section(const char* title) {
    printf("\n%s%s%s\n", BOLD, title, RESET);
}

void table(const char* commands[][2], int n) {
    int col1_width = 0;
    int col2_width = 0;

    for (int i = 0; i < n; i++) {
        int len1 = strlen(commands[i][0]);
        int len2 = strlen(commands[i][1]);
        if (len1 > col1_width) col1_width = len1;
        if (len2 > col2_width) col2_width = len2;
    }
    col1_width += 2;
    col2_width += 2;

    printf("%s%-*s| Description%s\n", BOLD, col1_width, "Command", RESET);
    for (int i = 0; i < col1_width; i++) printf("-");
    printf("+");
    for (int i = 0; i < col2_width; i++) printf("-");
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%s%-*s%s| %s%s%s\n",
               CYAN, col1_width, commands[i][0], RESET,
               WHITE, commands[i][1], RESET);
    }
    printf("\n");
}

void print_docs() {
    printf("\n%sMinez Documentation%s\n\n", BOLD, RESET);
    printf("%sMinez is an esoteric programming language created by %sDooblix%s%s "
           "in 2024 (current version: 2025), designed to be a more accessible and "
           "powerful version of %sBrainfuck%s.%s\n",
           WHITE, ITALIC, RESET, WHITE, ITALIC, RESET, RESET);

    printf("%s\n", "--------------------------------------------------------------------------------");

    section("Usage");
    printf("%s1. Installation%s\n", BOLD, RESET);
    printf("Download the Minez folder and place it in your desired location. Then run:\n\n");
    printf("    add_to_path.bat\n");
    printf("This will enable the `minez` command in your terminal.\n\n");

    printf("%s2. Basic Usage%s\n", BOLD, RESET);
    printf("    minez <file>\n");
    printf("    * `<file>`: Name of the `.minez` file to execute.\n\n");

    printf("%s3. Optional Arguments%s\n", BOLD, RESET);
    printf("    * `--hide-input-prompts`\n");
    printf("      Disables all default input prompts (`Int:` and `Char:`).\n");
    printf("    * `--num-of-regs <x>`\n");
    printf("      Sets the number of available memory cells to `x`.\n");
    printf("    * `--docs`\n");
    printf("      Prints this documentation file.\n\n");

    const char* pointer_ops[][2] = {
        {">y", "Move pointer to absolute index y."},
        {">",  "Increment pointer by 1."},
        {"<",  "Decrement pointer by 1."}
    };
    section("Pointer Manipulation");
    table(pointer_ops, 3);

    const char* flow_control[][2] = {
        {"^y", "Jump to program index `y`, saving pointer of the next command on the stack."},
        {"^s", "Jump back to the last saved value on the stack."},
        {"[",  "Begin loop (executes while current register != 0). Skipped if register = 0."},
        {"~",  "Skip to next loop iteration (like 'continue')."},
        {"]",  "End loop (repeats if register != 0)."},
        {"{}", "Conditional block: {reg>reg}(...), {reg<reg}(...), {reg=reg}(...). Use i for current register."},
        {";",  "End program."}
    };
    section("Flow Control");
    table(flow_control, 7);

    const char* stack_ops[][2] = {
        {"@", "Push value of current register onto the stack."},
        {"_", "Pop last value from the stack into current register (additively)."}
    };
    section("Stack Operations");
    table(stack_ops, 2);

    const char* reg_arith[][2] = {
        {"+y", "Increment current register by y."},
        {"+",  "Increment current register by 1."},
        {"-y", "Decrement current register by y."},
        {"-",  "Decrement current register by 1."},
        {"x",  "Reset current register to 0."}
    };
    section("Register Arithmetic");
    table(reg_arith, 5);

    const char* io_ops[][2] = {
        {"#",  "Output ASCII character of current register."},
        {"#!", "Output numeric value of current register."},
        {".",  "Read a single input character into current register (ASCII)."},
        {":",  "Read an integer into current register (any size, incl. negatives)."}
    };
    section("Input/Output");
    table(io_ops, 4);

    const char* index_mem[][2] = {
        {"|",     "Save current pointer index (`index_memory`)."},
        {"->",    "Jump to last saved index."},
        {"->(y)", "Jump to the y-th saved index."},
        {"X",     "Delete last saved index."}
    };
    section("Index Memory");
    table(index_mem, 4);

    const char* debug_ops[][2] = {
        {"d", "Print debugging information (Memory, Pointer, Stack, Index memory, Usage)."}
    };
    section("Debugging");
    table(debug_ops, 1);

    printf("%s\n", "--------------------------------------------------------------------------------");
}