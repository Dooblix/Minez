// Standart libs
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// Project headers
#include "../include/vectors.h"

size_t search_for_endloop(char* code, size_t code_len, size_t pc) {
    size_t endloop = pc;
    size_t loop_depth = 0;
    while (endloop < code_len) {
        printf("Current Char: %c\nLoop depth: %d", code[endloop], loop_depth);
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

int main() {
    char* code = "++++[>++++<-]>.";
    size_t code_len = 15;
    size_t endloop = search_for_endloop(code, code_len, 4);
    printf("%zu", endloop);
}