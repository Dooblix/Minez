// minez_help.c
// Copyright (c) 2026 Dooblix
// Licensed under the MIT license. See LICENSE file for details.

#include <stdio.h>
#include <string.h>
#include "../include/minez_help.h"

#define RESET "\033[0m"
#define BOLD "\033[1m"
#define YELLOW "\033[33m"

void print_help() {
    printf("%sMinez - Help%s\n", BOLD, RESET);

    printf("%sUsage:%s\n", YELLOW, RESET);
    printf("  ./minez <path-to-program> [options]\n\n");

    printf("%sOptions:%s\n", YELLOW, RESET);
    printf("  --num-of-regs N        Number of registers to allocate (default: 100)\n");
    printf("  --print-until N        Number of memory cells to display after program ends\n");
    printf("                         and when using 'd' (default: num_of_regs)\n");
    printf("  --print-intervals A B ...\n");
    printf("                         Print memory contents in specified intervals.\n");
    printf("                         Each interval is a pair A B. Must be even count.\n");
    printf("  --pre-input \"\"         Provide program input before execution.\n");
    printf("                         Delimiter-separated integers, '\\n' recommended.\n");
    printf("  --no-pause             Do not wait for keypress after program ends\n");
    printf("  --quiet, -q            Disable all metadata and debugger output\n");
    printf("  --help,  -h            Show this help and exit\n\n");

    printf("%sMore details:%s\n", YELLOW, RESET);
    printf("  https://github.com/Dooblix/Minez\n\n");

    printf("%sCopyright:%s\n", YELLOW, RESET);
    printf("  Copyright (c) 2026 Dooblix\n");
    printf("  Licensed under the MIT License\n");
}
