#ifndef INTERPRETER
#define INTERPRETER
#include <stdlib.h>
#include "vectors.h"

int interpret(vector_char code, int num_of_regs, size_t print_until, vector_int print_intervalls, char* pre_input, bool no_pause, bool quiet);

#endif