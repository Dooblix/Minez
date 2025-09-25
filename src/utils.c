#include <stdio.h>
#include <ctype.h>
#include "../include/utils.h"

#define BOLD "\033[1m"
#define CYAN "\033[36m"
#define RED  "\033[31m"
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

size_t search_for_endif(char* code, size_t code_len, size_t pc) {
    size_t endif = pc + 1;
    size_t depth = 1;
    while (endif < code_len) {
        if (code[endif] == '(') {
            depth++;
        } else if (code[endif] == ')') {
            depth--;
            if (depth == 0) {
                return endif;
            }
        }
        endif++;
    }
    fprintf(stderr, RED BOLD "\nSyntaxError" RESET " at index " CYAN "%u" RESET ": Missing closing bracket ')'.", pc);
    exit(1);
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

int* closest_factors(int x) {
    int res[3];
    if (x == 0) {
        res[0] = 0;
        res[1] = 0;
        res[2] = 0;
        return res;
    } else if (x <= 10) {
        res[0] = x;
        res[1] = 1;
        res[2] = 0;
        return res;
    }
    return res;
}