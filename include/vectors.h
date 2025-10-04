#ifndef VECTORS
#define VECTORS
#include <stdlib.h>

typedef struct {
    size_t* data;
    size_t size;
    size_t capacity;
} vector_size_t;

typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} vector_int;

typedef struct {
    unsigned char* data;
    size_t size;
    size_t capacity;
} vector_char;

vector_size_t make_vector_size_t(size_t initial_capacity);
void vector_size_t_push(vector_size_t* v, size_t val);
vector_int make_vector_int(size_t initial_capacity);
void vector_int_push(vector_int* v, int val);
vector_char make_vector_char(size_t initial_capacity);
void vector_char_push(vector_char* v, unsigned char val);
size_t vector_size_t_pop(vector_size_t* v);
int vector_int_pop(vector_int* v);
unsigned char vector_char_pop(vector_char* v);

#endif