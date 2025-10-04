#ifndef INTERPRETER
#define INTERPRETER
#include <stdlib.h>
#include "vectors.h"

void interpret(vector_char code, int num_of_regs, size_t print_until, vector_int print_intervalls, bool no_pause, bool quiet);

#endif