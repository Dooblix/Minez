#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <math.h>
#include "vectors.h"

size_t search_for_endloop(char* code, size_t code_len, size_t pc);
long extract_number(char* code, size_t code_len, size_t* pc);
size_t search_for_endif(char* code, size_t code_len, size_t pc);
double get_time_sec();
static inline int runtime_mz(double start) {
    return (int)round((get_time_sec() - start) * 1000);
}

#endif