#ifndef INTERPRETER
#define INTERPRETER
#include <stdlib.h>
#include "vectors.h"

size_t search_for_endloop(char* code, size_t code_len, size_t pc);
void interprete(vector_char code, int num_of_regs);

#endif