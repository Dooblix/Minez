// Standart libs
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
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

long extract_number(char* code, size_t code_len, size_t* pc) {
    (*pc)++;
    vector_char result = make_vector_char(1);
    while (*pc < code_len && isdigit((unsigned char)code[*pc])) {
        vector_char_push(&result, code[*pc]);
        (*pc)++;
    }
    vector_char_push(&result, '\0');
    return strtol(result.data, NULL, 10);
}

int main() {
    char* code = "++>1036;";
    size_t code_len = 5;
    size_t pc = 2;
    long number = extract_number(code, code_len, &pc);
    printf("%ld\n%zu", number, pc);
}