#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <stdbool.h>
#include <string.h>

#include "../include/utils.h"
#include "../include/vectors.h"

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
    long result_num = strtol(result.data, NULL, 10);
    free(result.data);
    return result_num;
}

long extract_number_pre_input(char* text, size_t length, size_t* idx) {
    bool negative = false;
    if (text[*idx] == '-') {
        negative = true;
        (*idx)++;
    }
    vector_char result = make_vector_char(1);
    while (*idx < length && isdigit((unsigned char)text[*idx])) {
        vector_char_push(&result, text[*idx]);
        (*idx)++;
    }
    vector_char_push(&result, '\0');
    long result_num = strtol(result.data, NULL, 10);
    free(result.data);
    return negative ? -result_num : result_num;
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
    fprintf(stderr, RED BOLD "\nSyntaxError" RESET " at index " CYAN "%zu" RESET ": Missing closing bracket ')'.", pc);
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
    fprintf(stderr, RED BOLD "\nSyntaxError" RESET " at index " CYAN "%zu" RESET ": Missing closing bracket ']'.", pc);
    exit(1);
}

double get_time_sec() {
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_sec + t.tv_usec / 1e6;
}

char* parse_pre_input(char* pre_input) {
    if (!pre_input) return NULL;
    size_t pre_input_len = strlen(pre_input);
    size_t result_idx = 0;
    char* result = malloc((pre_input_len + 1) * sizeof(char));

    for (size_t i = 0; i < pre_input_len; i++) {
        if (pre_input[i] == '\\' && i + 1 < pre_input_len) {
            i++;
            if (pre_input[i] == 'n') {
                result[result_idx++] = '\n';
            } else if (pre_input[i] == 't') {
                result[result_idx++] = '\t';
            } else if (pre_input[i] == 'r') {
                result[result_idx++] = '\r';
            } else {
                result[result_idx++] = pre_input[i];
            }
        } else {
            result[result_idx++] = pre_input[i];
        }
    }
    result[result_idx] = '\0';
    return result;
}
